#include "customPrinter.hpp"
#include "assembly.hpp"
#include "linearSystemSolver.hpp"

int main(int argc, char** args)
{	
	string connecFile="connectivity";
	string coordFile="coordinates";

/*		STIFFNESS MATRIX ASSEMBLY
	----------------------------------------------------------------*/
	
	assembly myAssembly(connecFile,coordFile);
	
/*		PETSC INITIALIZE
	----------------------------------------------------------------*/

	PetscErrorCode ierr;
	ierr=PetscInitialize(&argc,&args,(char*)0,NULL);CHKERRQ(ierr);
	
/*		PETSC FINALIZE
	----------------------------------------------------------------*/
	
	ierr=PetscFinalize();CHKERRQ(ierr);

	return ierr;
};