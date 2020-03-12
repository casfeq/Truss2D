# Truss2D

This program implements a Finite Element Method for discretization and solution of two-dimensional truss problems. The governing equations are discretized within the FEM and the resulting linear system of equations is solved with a LU Factorization found in PETSc suite [1].

PETsc is also required. To run this program, first edit the "CMakeLists.txt" file with the location of PETsc installation. Put the coordinates and connectivity of the nodal points in input folder. Then call the "truss.sh" file.

Written by FERREIRA, C. A. S.

PETSc:
https://www.mcs.anl.gov/petsc/

[1] BALAY et al. PETSc User Manual. Technical Report, Argonne National Laboratory, 2017.
