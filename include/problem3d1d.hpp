#ifndef PROBLEM3D1D_HS
#define PROBLEM3D1D_HS


#include"temperature.hpp"
#include"network.hpp"
#include"interpolator.hpp"

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


class PROBLEM_3d1d{
public:
   PROBLEM_3d1d(const GetPot& df);
   void assembly_mat();
   void assembly_rhs();
   void assembly_exchange_mat();
   void solve();
   void print(int iter=0);
   inline void set_iter(int it){iter_=it;}
   inline void set_coupling(bool flag){coupling_=flag;}
private:

   TEMPERATURE bulk_problem_;
   
   NETWORK oned_problem_;

   INTERPOLATOR op_3d1d_; // 3d1d operator

   bool coupling_=true;
   int coupling_order_=2;
   int iter_ =0;
   std::vector<scalar_type> sol_, sol_old_, rhs_;    /// solution and old solution
   sparse_matrix_type K_;                         /// iteration matrix 	
   double dt_;
   
   getfem::size_type nb_dof_bulk_; // nb dof biot problem
   getfem::size_type nb_dof_oned_; // nb dof 1d problem
};//end problem 3d1d


#endif
