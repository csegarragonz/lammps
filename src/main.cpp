/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "lammps.h"
#include "input.h"

#include <mpi.h>
#include <cstdlib>
#include <iostream>

#if defined(LAMMPS_TRAP_FPE) && defined(_GNU_SOURCE)
#include <fenv.h>
#endif

#if defined(LAMMPS_EXCEPTIONS)
#include "exceptions.h"
#endif

using namespace LAMMPS_NS;

/* ----------------------------------------------------------------------
   main program to drive LAMMPS
------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
// Faasm instrumentation
// https://www.open-mpi.org/faq/?category=running#mpi-environmental-variables
// OMPI_COMM_WORLD_SIZE - the number of processes in this process' MPI_COMM_WORLD
// OMPI_UNIVERSE_SIZE - the number of process slots allocated to this job.
  const char *ompi_env = std::getenv("OMPI_COMM_WORLD_SIZE");
  if (ompi_env)
  {
    std::cout << "OMPI_COMM_WORLD_SIZE: " << ompi_env << "\n";
  }
  else
  {
    std::cout << "OMPI_COMM_WORLD_SIZE: 0\n";
  }
  
  MPI_Init(&argc,&argv);

// enable trapping selected floating point exceptions.
// this uses GNU extensions and is only tested on Linux
// therefore we make it depend on -D_GNU_SOURCE, too.

#if defined(LAMMPS_TRAP_FPE) && defined(_GNU_SOURCE)
  fesetenv(FE_NOMASK_ENV);
  fedisableexcept(FE_ALL_EXCEPT);
  feenableexcept(FE_DIVBYZERO);
  feenableexcept(FE_INVALID);
  feenableexcept(FE_OVERFLOW);
#endif

#ifdef LAMMPS_EXCEPTIONS
  try {
    LAMMPS *lammps = new LAMMPS(argc,argv,MPI_COMM_WORLD);
    lammps->input->file();
    delete lammps;
  } catch(LAMMPSAbortException &ae) {
    MPI_Abort(ae.universe, 1);
  } catch(LAMMPSException &e) {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(1);
  }
#else
  LAMMPS *lammps = new LAMMPS(argc,argv,MPI_COMM_WORLD);
  lammps->input->file();
  delete lammps;
#endif
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
