#ifndef TEMPERATURE_HS
#define TEMPERATURE_HS

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

//namespaces and type defs
using bgeot::base_small_vector; /* special class for small (dim<16) vectors */
using bgeot::base_node; /* geometrical nodes (derived from base_small_vector)*/
using bgeot::scalar_type; /* = double */
using bgeot::size_type;   /* = unsigned long */
using bgeot::dim_type; 

typedef gmm::rsvector<scalar_type> sparse_vector_type;
typedef gmm::row_matrix<sparse_vector_type> sparse_matrix_type;
typedef gmm::col_matrix<sparse_vector_type> col_sparse_matrix_type;
typedef std::vector<scalar_type> plain_vector;

class TEMPERATURE
{
public:
    TEMPERATURE(const GetPot& );
    void assembly_mat();
    void assembly_rhs();
    void solve();
    void print(int iter=0);

    inline int ndof(){return mf_.nb_dof(); } // get dof number;

    inline sparse_matrix_type& get_iter_mat() {return K_;}
    inline std::vector<scalar_type>& get_iter_rhs() {return rhs_;}
    inline getfem::mesh_fem& get_fem() {return mf_;}
    void   setsol(std::vector<scalar_type>& sol); // copy the solution
private:
   getfem::mesh mesh_;
   getfem::mesh_im mim_;           /// the integration methods
   getfem::mesh_fem mf_;         /// the main mesh_fem, for the pressure solution
   getfem::mesh_fem mf_coef_;      /// the mesh_fem to represent pde coefficients 
   getfem::ga_workspace workspace_; /// generic workspace for the solution
   enum { BOTTOM = 1, TOP = 2, LEFT = 3, RIGHT =4, LEFTX = 5, RIGHTX =6}; // descriptor for zones
   
   double Temp_ref_,time_ref_,uref_,lref_;

   int N_;                     /// problem dimension
   // material properties
   std::vector<scalar_type> tau_, th_cond_, penalty_, rhocp_c_,uref_wp_;
   
   scalar_type rho_c_,k_c_,cp_c_;

   std::vector<scalar_type> sol_, rhs_, sol_old_;    /// diplacement,  pressure, pressure_old
   sparse_matrix_type K_;                         /// iteration matrix 	

   void gen_bc(void);    /// create zones for boundary conditions
   void configure_wp(const GetPot& df);	

};
#endif
