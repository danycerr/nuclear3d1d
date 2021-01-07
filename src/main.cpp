#include <iostream>
#include"biot.hpp"
#include"temperature.hpp"
#include"network.hpp"

#include"problem3d1d.hpp"


int main(int argc, char** argv)
{
   GetPot   cl(argc, argv);
   GETFEM_MPI_INIT(argc, argv);

   const std::string data_file_name = cl.follow("data", 2, "-f",
            "--file");

   GetPot dataFile(data_file_name.data());
   
   std::cout<<data_file_name.data()<<std::endl;
   int n_step=dataFile("time/nstep",1);
   int printstep=dataFile("time/pstep",1);
  if(1) { 
   PROBLEM_3d1d bd(dataFile);
   bd.assembly_mat();
   bd.assembly_rhs();
   bd.assembly_exchange_mat();
//    bd.set_coupling(false);
   for (int iter=0; iter<n_step; iter++){
//        if(iter==10) {
//            bd.set_coupling(true);
//        }
       bd.set_iter(iter);
       bd.solve();
       bd.assembly_rhs();
       if(iter%printstep==0)
        bd.print(iter);      
   }
  }
  if(0) { 
   NETWORK oned_problem(dataFile);
   oned_problem.assembly_mat();
   oned_problem.assembly_rhs();
   oned_problem.solve();
   oned_problem.print();
}

if(0) {
   BIOT biot_problem(dataFile);
   biot_problem.assembly_mat();
   biot_problem.assembly_rhs();
   biot_problem.solve();
   biot_problem.print();
      }   

    
if(0) {
   TEMPERATURE temp_problem(dataFile);
   temp_problem.assembly_mat();
   temp_problem.assembly_rhs();
   temp_problem.solve();
   temp_problem.print();
      }
      
  GETFEM_MPI_FINALIZE;
}
