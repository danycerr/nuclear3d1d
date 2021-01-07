#ifndef NETWORKHS
#define NETWORKHS

#include<iostream>
#include<cstddef>
#include<GetPot>

// getfem includes
#include "getfem/getfem_generic_assembly.h"
#include "getfem/getfem_export.h"
#include "getfem/getfem_regular_meshes.h"
#include "getfem/getfem_derivatives.h"
#include "getfem/getfem_superlu.h"
#include "getfem/getfem_models.h"
#include "getfem/getfem_model_solvers.h" // for preconditioners
#include "getfem/getfem_import.h"


#include "gmm/gmm.h"
#include "gmm/gmm_inoutput.h"

#include "defines.hpp"
#include "node.hpp"

class NETWORK
{
public:
    vector_size_type nb_vertices_;

    NETWORK(const GetPot& );
    void assembly_mat();
    void assembly_rhs();
    void solve();
    void print(int iter=0);
    
    inline int ndof(){return mf_.nb_dof();} // get dof number;

    inline sparse_matrix_type& get_iter_mat() {return K_;}
    
    inline getfem::mesh_fem& get_fem() {return mf_;}
    inline getfem::mesh_fem& get_fem_coef() {return mf_coef_;}
    inline getfem::mesh_im&  get_im() {return mim_;}
    
    inline std::vector<scalar_type>& get_iter_rhs() {return rhs_;}
    inline std::vector<scalar_type>& get_radius() {return radius_;}
    inline std::vector<scalar_type>& get_Y() {return Y_;}
   
    
    inline void   setsol(std::vector<scalar_type>& sol){gmm::copy(sol,sol_); gmm::copy(sol_, sol_old_);} // copy the solution

private:
   //! Mesh for the vessel network  (1D)
   getfem::mesh mesh_;   
   getfem::mesh_im mim_;           /// the integration methods
   getfem::mesh_fem mf_;         /// the main mesh_fem, for the pressure solution
   getfem::mesh_fem mf_coef_;      /// the mesh_fem to represent pde coefficients 
   std::vector<getfem::node> BCList_;
   std::vector<scalar_type> BC_value_,th_cond_f_,rhocp_f_,tau_,source_,pirr_;
      double Temp_ref_,time_ref_,uref_,lref_;
   std::vector<scalar_type> Y_;
   std::vector<int> local_idx_bc_;
   std::vector<scalar_type> radius_; // branch radius
   scalar_type delta_clad_;
   size_type nb_branches_; // number branches in the network
   
   scalar_type rho_f_,k_f_,cp_f_;
   scalar_type rho_cl_,k_cl_,cp_cl_;
   

   
   getfem::ga_workspace workspace_; /// generic workspace for the solution
   
   std::vector<scalar_type> sol_, sol_old_, rhs_;    /// solution and old solution
   sparse_matrix_type K_;                         /// iteration matrix 	
   
   
   void configure_wp(const GetPot& df);	
   void genBC();

   void build_mesh(getfem::mesh& mesh, const GetPot& df);
}; // end network class
#endif
