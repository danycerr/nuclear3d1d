#include "problem3d1d.hpp"

PROBLEM_3d1d::PROBLEM_3d1d(const GetPot& df):bulk_problem_(df),
                                             oned_problem_(df),
                                             op_3d1d_(df,bulk_problem_.get_fem(),
                                                         oned_problem_.get_fem(),oned_problem_.get_im(), oned_problem_.get_radius())
{
   std::cout<<"PROBLEM_3d1d::PROBLEM_3d1d init vectors"<<std::endl;
   nb_dof_bulk_ = bulk_problem_.ndof();
   nb_dof_oned_ = oned_problem_.ndof();
   std::cout<<"nb dof biot "<< nb_dof_bulk_<< " nb dof 1d "<< nb_dof_oned_<<std::endl;
   gmm::resize(rhs_,nb_dof_bulk_ + nb_dof_oned_);
   gmm::resize(sol_, nb_dof_bulk_ + nb_dof_oned_); gmm::clear(sol_);
   gmm::resize(sol_old_, nb_dof_bulk_ + nb_dof_oned_); gmm::clear(sol_old_);
   std::cout<<"PROBLEM_3d1d:: init matricies"<<std::endl;
   gmm::resize(K_, nb_dof_bulk_ + nb_dof_oned_, nb_dof_bulk_ + nb_dof_oned_ ); gmm::clear(K_);
   
   std::cout<<"PROBLEM_3d1d:: build interpolation operators"<<std::endl;
   op_3d1d_.build_operator();

  
   coupling_ =  df("3d1d/coupling",1);
   coupling_order_ = df("3d1d/couplingOrder",2);
   dt_= df("time/dt", 1.0 );
   // std::cout << coupling_order_<<std::endl;
   // std::cout << coupling_<<std::endl;
   // std::cin.ignore();
}

//=====================================
void PROBLEM_3d1d::assembly_exchange_mat(){
   std::cout << "PROBLEM_3d1d::assembly_exchange_mat()  alternative form " <<std::endl;
   sparse_matrix_type B1d1d(nb_dof_oned_,nb_dof_oned_);   gmm::clear(B1d1d);
   sparse_matrix_type B1d3d(nb_dof_oned_,nb_dof_bulk_);   gmm::clear(B1d3d);
   sparse_matrix_type B3d1d(nb_dof_bulk_,nb_dof_oned_);   gmm::clear(B3d1d);
   sparse_matrix_type B3d3d(nb_dof_bulk_,nb_dof_bulk_);   gmm::clear(B3d3d);
   getfem::asm_mass_matrix_param(B1d1d,  oned_problem_.get_im(), 
                                         oned_problem_.get_fem(), 
                                         oned_problem_.get_fem_coef(),
                                         oned_problem_.get_Y());
   gmm::scale(B1d1d,dt_);
   if(coupling_order_==1){
       std::cout <<"Using linear interpolation"<<std::endl;
       gmm::mult(B1d1d,op_3d1d_.get_Mlin(), B1d3d);
       gmm::mult(gmm::transposed(op_3d1d_.get_Mlin()), B1d3d, B3d3d);
   }
   if(coupling_order_==2){
       std::cout <<"Using quadratic interpolation"<<std::endl;
       gmm::mult(B1d1d,op_3d1d_.get_Mbar(), B1d3d);
       gmm::mult(gmm::transposed(op_3d1d_.get_Mbar()), B1d3d, B3d3d);
   }
   gmm::copy(gmm::transposed(B1d3d), B3d1d);
   std::cout << "PROBLEM_3d1d::assembly_exchange_mat()  B3d3d " <<std::endl;
   if(coupling_){
   std::cout<<"Adding coupling operator to global matrix"<<std::endl;
   double pi=3.1415; double r0=(oned_problem_.get_radius())[0];
//    std::cout<<r0<<std::endl;std::cin.ignore();
      gmm::add(gmm::scaled(B3d3d, 2.0*pi*r0),			 
            gmm::sub_matrix(K_,gmm::sub_interval(0, nb_dof_bulk_), 
                               gmm::sub_interval(0, nb_dof_bulk_))); 

// 		gmm::MatrixMarket_IO::write("Btt_fede.mm",gmm::scaled(Btt, 2.0*pi*param.R(0)));
      gmm::add(gmm::scaled(B3d1d, -2.0*pi*r0),
                     gmm::sub_matrix(K_,gmm::sub_interval(0, nb_dof_bulk_),
                                        gmm::sub_interval(0 +  nb_dof_bulk_, nb_dof_oned_)));
      
      gmm::add(gmm::scaled(B1d3d, -2.0/r0),
                     gmm::sub_matrix(K_,gmm::sub_interval(0 +  nb_dof_bulk_, nb_dof_oned_),
                                        gmm::sub_interval(0, nb_dof_bulk_)));
      gmm::add(gmm::scaled(B1d1d, 2.0/r0),								
                     gmm::sub_matrix(K_,gmm::sub_interval(0 +  nb_dof_bulk_, nb_dof_oned_), 
					gmm::sub_interval(0 +  nb_dof_bulk_, nb_dof_oned_))); 

	}
}

//====================================
void PROBLEM_3d1d::assembly_mat(){
   bulk_problem_.assembly_mat();
   gmm::copy(bulk_problem_.get_iter_mat(),gmm::sub_matrix(K_, 
                                             gmm::sub_interval(0, nb_dof_bulk_), 
                                             gmm::sub_interval(0, nb_dof_bulk_)));
   oned_problem_.assembly_mat();
   gmm::copy(oned_problem_.get_iter_mat(),gmm::sub_matrix(K_, 
                                             gmm::sub_interval(nb_dof_bulk_, nb_dof_oned_), 
                                             gmm::sub_interval(nb_dof_bulk_, nb_dof_oned_)));
   
}

//====================================
void PROBLEM_3d1d::assembly_rhs(){
    gmm::clear(rhs_);
   bulk_problem_.assembly_rhs();
   gmm::copy(bulk_problem_.get_iter_rhs(), gmm::sub_vector(rhs_,gmm::sub_interval(0, nb_dof_bulk_)));
   oned_problem_.assembly_rhs();
   gmm::add(oned_problem_.get_iter_rhs(), gmm::sub_vector(rhs_,gmm::sub_interval(nb_dof_bulk_,nb_dof_oned_)));
}

//=====================================
void PROBLEM_3d1d::solve(){
   size_type restart = 50;
   gmm::identity_matrix PM; // no precond
   gmm::diagonal_precond<sparse_matrix_type> PR(K_);
   // gmm::mr_approx_inverse_precond<sparse_matrix_type> P(K, 10, 10E-17);
   // gmm::iteration iter(1.e-8);  // iteration object with the max residu
   //iter.set_noisy(1);               // output of iterations (2: sub-iteration)
   // iter.set_maxiter(2000); // maximum number of iterations
   // gmm::gmres(K_, sol_, rhs_, PR, restart, iter);
    scalar_type cond;
    gmm::SuperLU_solve(K_, sol_ , rhs_, cond);
    std::cout<<"condition number "<< cond<< std::endl;
   std::vector<scalar_type> buf(nb_dof_bulk_);
   gmm::copy(gmm::sub_vector(sol_,gmm::sub_interval(0 , nb_dof_bulk_)), buf);
   bulk_problem_.setsol(buf);
   buf.resize(nb_dof_oned_);
   gmm::copy(gmm::sub_vector(sol_,gmm::sub_interval(nb_dof_bulk_ , nb_dof_oned_)), buf);
   oned_problem_.setsol(buf);
}
//====================================
void PROBLEM_3d1d::print(int iter){
   bulk_problem_.print(iter);
   oned_problem_.print(iter);
}
