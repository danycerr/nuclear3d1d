#include<temperature.hpp>

TEMPERATURE::TEMPERATURE(const GetPot& df): mim_(mesh_),
                              mf_(mesh_),
                              mf_coef_(mesh_),
                              tau_(1), penalty_(1),th_cond_(1),rhocp_c_(1),uref_wp_(1)
{
    std::cout<<"TEMPERATURE::TEMPERATURE build biot class"<<std::endl;
    bgeot::pgeometric_trans pgt = 
		bgeot::geometric_trans_descriptor(df("temperature/mesh/mesh_type", "GT_PK(3,1)"));
    // Mesh generation
    N_ = pgt->dim();
    if(df("temperature/mesh/read_mesh", 0)==0)
    {
        std::vector<size_type> nsubdiv(N_);
        std::fill(nsubdiv.begin(),nsubdiv.end(),df("temperature/mesh/ndiv", 10));
        getfem::regular_unit_mesh(mesh_, nsubdiv, pgt, 0);
    }
    else{
        std::string read_command("gmsh:" + df("temperature/mesh/mesh_name", "mesh not found"));
        std::cout <<read_command<<std::endl;
        getfem::import_mesh(read_command,mesh_);
        for(int iref=0; iref<df("temperature/mesh/refine", 0);iref++)
        {
            mesh_.Bank_refine(mesh_.convex_index());
            std::cout << "refining "<< iref<< std::endl;
        }
        
    }
    {
    getfem::vtk_export exp("mesh_temp.vtk");
    exp.exporting(mesh_); // will save the geometrical structure of the slice
    exp.write_mesh();
    }
    std::cout<<"end mesh generation"<<std::endl;

    // set  integration methods  
    getfem::pfem pf = getfem::fem_descriptor(df("temperature/fem/fem_u", "FEM_PK(3,1)"));
    getfem::pintegration_method ppi = getfem::int_method_descriptor(df("temperature/fem/integr_method", "IM_TETRAHEDRON(6)"));
    //getfem::pintegration_method simp_ppi = getfem::int_method_descriptor(p_des.SIMPLEX_INTEGRATION);
    mim_.set_integration_method(mesh_.convex_index(), ppi);
    mf_.set_finite_element(mesh_.convex_index(), pf); // finite element for displacement
    mf_coef_.set_finite_element(mesh_.convex_index(),getfem::classical_fem(pgt,0));// p0 for coefficient
    // ===boundary condition labelling
    gen_bc();
    rho_c_=df("temperature/material/LBE/rho",1.);
    k_c_=df("temperature/material/LBE/k",1.);
    cp_c_=df("temperature/material/LBE/cp",1.);
    // non dimensional quantities
    lref_=1.;
    Temp_ref_=df("oned/source",1.)*lref_*lref_/k_c_;
    uref_=k_c_/lref_;
    time_ref_=lref_*lref_/k_c_;
    Temp_ref_=1.;
    uref_=1.;
    time_ref_=1.;
    // init matricies
    std::cout<<"init vectors"<<std::endl;
    getfem::size_type nb_dof = mf_.nb_dof();
    gmm::resize(rhs_,nb_dof);
    gmm::resize(sol_, nb_dof); gmm::clear(sol_);
    gmm::resize(sol_old_, nb_dof); gmm::clear(sol_old_);
    std::fill(sol_.begin(), sol_.end(), 0); gmm::copy(sol_,sol_old_);
    std::cout<<"init matricies"<<std::endl;
    gmm::resize(K_,nb_dof,nb_dof); gmm::clear(K_);
    std::cout<<"number of dof "<< nb_dof<<std::endl;
    std::cout <<"add variables to workspace"<<std::endl;
    workspace_.add_fem_variable("u", mf_, gmm::sub_interval(0, nb_dof), sol_);
    workspace_.add_fem_variable("u_old", mf_, gmm::sub_interval(0, nb_dof), sol_old_);
    configure_wp(df);
    
}/// end constructor


// ===========================================
// method for generation of bcs zones
// ===========================================
void TEMPERATURE::gen_bc()
{
   std::cout << "TEMPERATURE::gen_bc()"<< std::endl;
   getfem::mesh_region border_faces;
   getfem::outer_faces_of_mesh(mesh_, border_faces);
   for (getfem::mr_visitor i(border_faces); !i.finished(); ++i) {
      assert(i.is_face());
      base_node un = mesh_.normal_of_face_of_convex(i.cv(), i.f());
		un /= gmm::vect_norm2(un);
      if ((un[N_-1] ) > 9.0E-1) { 
  	mesh_.region(TOP).add(i.cv(), i.f()); //all top iced
      } else if ((un[N_-1] ) < -9.0E-1) {  //the bottom surface is the most sharp
	mesh_.region(BOTTOM).add(i.cv(), i.f());
      } else if ((un[N_-2] ) < -1.0E-1) {
	mesh_.region(LEFT).add(i.cv(), i.f());
      } else if ((un[N_-2] ) > 1.0E-1) {
	mesh_.region(RIGHT).add(i.cv(), i.f());
      }	else if(N_=3){if ((un[N_-3] ) < -1.0E-1) {
			  mesh_.region(LEFTX).add(i.cv(), i.f());
			} else if ((un[N_-3] ) > 1.0E-1) {
			  mesh_.region(RIGHTX).add(i.cv(), i.f());
			}
		      }// end if n=3
    } // end for
}
// end bc generations
// =======================================================

void TEMPERATURE::configure_wp(const GetPot& df){
   std::cout << "TEMPERATURE::configure_wp Configuring workspace " << std::endl;
   tau_[0] = df("time/dt", 1.0 )/time_ref_; // dt into  the workspace
   workspace_.add_fixed_size_constant("tau", tau_);
   //---------------------------------------------------------
//    diffusivity_[0] =  df("temperature/material/diffusivity",1);
//    workspace_.add_fixed_size_constant("alpha", diffusivity_);
   //---------------------------------------------------------
   penalty_[0] = 1.e+12; // 1---10
   workspace_.add_fixed_size_constant("penalty", penalty_);
   std::cout<<"TEMPERATURE::configure_wp end of workspace configuration"<<std::endl;
   
   th_cond_[0] = k_c_;
   rhocp_c_[0] = rho_c_*cp_c_;
   uref_wp_[0] = uref_;
   workspace_.add_fixed_size_constant("K", th_cond_);
   workspace_.add_fixed_size_constant("rhocp", rhocp_c_);
   workspace_.add_fixed_size_constant("uref", uref_wp_);
}


void TEMPERATURE::assembly_mat(){
   std::cout<<"TEMPERATURE::assembly_mat start assembling K"<<std::endl;
// ------------------ expressions --------------------------
  //  workspace_.add_expression( "rhocp*u.Test_u + K*Grad_u.Grad_Test_u*tau + rhocp*[0,0,0.77]*(1/uref).Grad_u*Test_u*tau", mim_);
   workspace_.add_expression( "rhocp*u.Test_u + K*Grad_u.Grad_Test_u*tau + rhocp*[0,0,0.3]*(1/uref).Grad_u*Test_u*tau", mim_);
   workspace_.assembly(2);
   gmm::copy(workspace_.assembled_matrix(), K_);
   workspace_.clear_expressions();
// bcs
//    workspace_.add_expression("penalty/element_size*u*Test_u", mim_, TOP);
//    workspace_.add_expression("-permeability*Grad_p.Normal*Test_p - permeability*Grad_Test_p.Normal*p ", mim_, TOP); 	
   // workspace_.add_expression("penalty/element_size*p*Test_p", mim_, LEFT);
   // workspace_.add_expression("penalty/element_size*p*Test_p", mim_, RIGHT);// 1 is the region	
   // workspace_.add_expression("-permeability*Grad_p.Normal*Test_p - permeability*Grad_Test_p.Normal*p ", mim_, LEFT); 	
   // workspace_.add_expression("-permeability*Grad_p.Normal*Test_p - permeability*Grad_Test_p.Normal*p", mim_, RIGHT); 
   // workspace_.add_expression("penalty/element_size*p*Test_p", mim_, LEFTX);
   // workspace_.add_expression("penalty/element_size*p*Test_p", mim_, RIGHTX);// 1 is the region	
   if(N_==3){	 	
   //   workspace_.add_expression("-permeability*Grad_p.Normal*Test_p - permeability*Grad_Test_p.Normal*p ", mim_, LEFTX); 	
   //   workspace_.add_expression("-permeability*Grad_p.Normal*Test_p - permeability*Grad_Test_p.Normal*p", mim_, RIGHTX);
     }	
   workspace_.add_expression("penalty*u.Test_u" , mim_, BOTTOM); //neumann disp
   workspace_.assembly(2);
   gmm::add(workspace_.assembled_matrix(), K_);
   workspace_.clear_expressions();
   std::cout<<"TEMPERATURE::assembly_mat End assembling K"<<std::endl;
}



void TEMPERATURE::assembly_rhs(){
   std::cout<<"TEMPERATURE::assembly_mat start assembling rhs"<<std::endl;
   gmm::clear(rhs_);
   //======= RHS =====================
   // if(N_==2) workspace_.add_expression("0.*0.001*(2200*0.8 + 1000*0.2 -1000 )*[0,-1].Test_u", mim_);
   // if(N_==3) workspace_.add_expression("0.*0.001*(2200*0.8 + 1000*0.2 -1000 )*[0,0,-1].Test_u", mim_);
   workspace_.add_expression("+[+0.*2.76e+6].Test_u*tau + rhocp*u_old.Test_u", mim_);
   workspace_.set_assembled_vector(rhs_);
   workspace_.assembly(1);
   workspace_.clear_expressions();
   std::cout<<"TEMPERATURE::assembly_mat end assembling rhs"<<std::endl;

   // all bcs are homogeneus no rhs terms
}


//=====================================
void TEMPERATURE::solve(){
   size_type restart = 50;
   gmm::identity_matrix PM; // no precond
   gmm::diagonal_precond<sparse_matrix_type> PR(K_);
   // gmm::mr_approx_inverse_precond<sparse_matrix_type> P(K, 10, 10E-17);
   // gmm::iteration iter(1.e-8);  // iteration object with the max residu
   //	iter.set_noisy(1);               // output of iterations (2: sub-iteration)
   //	iter.set_maxiter(1000); // maximum number of iterations
   //	gmm::gmres(K, UP, B, PR, restart, iter);
   scalar_type cond;
   gmm::SuperLU_solve(K_, sol_ , rhs_, cond);
   std::cout<<"condition number "<< cond<< std::endl;
   getfem::size_type nb_dof = mf_.nb_dof();
   gmm::copy(sol_,sol_old_);
}
//====================================
void TEMPERATURE::setsol(std::vector<scalar_type>& sol){
   gmm::copy(sol,sol_);
   gmm::copy(sol_,sol_old_);
}

//=====================================
void TEMPERATURE::print(int iter){
std::cout<< "TEMPERATURE::print() "<< "temperature.vtk"<<std::endl;

getfem::vtk_export exp("temperature." + std::to_string(iter) +".vtk");
exp.exporting(mf_);  	exp.write_point_data(mf_, sol_, "u");  
}
