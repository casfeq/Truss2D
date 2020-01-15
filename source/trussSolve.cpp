#include "customPrinter.hpp"
#include "stiffnessMatrixAssembly.hpp"
#include "dirichletApplication.hpp"
#include "forceVectorAssembly.hpp"
#include "linearSystemSolver.hpp"
#include "dataProcessing.hpp"

int main(int argc, char** args)
{	
	string connecFile="connectivity";
	string coordFile="coordinates";
	string propFile="properties";
	string dirichletBCFile="dirichletBC";
	string neumannBCFile="neumannBC";
	vector<vector<double>> stiffnessMatrix;
	vector<double> forceVector;
	vector<double> displacement;

/*		STIFFNESS MATRIX ASSEMBLY
	----------------------------------------------------------------*/
	
	stiffnessMatrixAssembly myStiffnessMatrixAssembly(connecFile,coordFile,propFile);
	stiffnessMatrix=myStiffnessMatrixAssembly.stiffnessMatrix;

/*		DIRICHLET BOUNDARY CONDITIONS APPLICATION
	----------------------------------------------------------------*/

	dirichletApplication myDirichletApplication(dirichletBCFile,stiffnessMatrix);
	stiffnessMatrix=myDirichletApplication.stiffnessMatrix;

/*		FORCE VECTOR ASSEMBLY
	----------------------------------------------------------------*/

	forceVectorAssembly myForceVectorAssembly(dirichletBCFile,neumannBCFile,stiffnessMatrix.size());
	forceVector=myForceVectorAssembly.forceVector;

/*		SOLVE LINEAR SYSTEM
	----------------------------------------------------------------*/

	PetscErrorCode ierr;

	ierr=PetscInitialize(&argc,&args,(char*)0,NULL);CHKERRQ(ierr);

	linearSystemSolver myLinearSystemSolver(stiffnessMatrix,stiffnessMatrix.size());
	ierr=myLinearSystemSolver.coefficientsMatrixLUFactorization();CHKERRQ(ierr);
	ierr=myLinearSystemSolver.createPETScArrays();CHKERRQ(ierr);
	ierr=myLinearSystemSolver.zeroPETScArrays();CHKERRQ(ierr);
	ierr=myLinearSystemSolver.setRHSValue(forceVector);CHKERRQ(ierr);
	ierr=myLinearSystemSolver.solveLinearSystem();CHKERRQ(ierr);
	ierr=myLinearSystemSolver.setFieldValue();CHKERRQ(ierr);
	displacement=myLinearSystemSolver.solution;

	cout << "Displacement field determined.";

/*		REACTION FORCES DETERMINATION
 	----------------------------------------------------------------*/

 	myStiffnessMatrixAssembly.assemblyStiffnessMatrix();
	stiffnessMatrix=myStiffnessMatrixAssembly.stiffnessMatrix;
	myLinearSystemSolver.importData(stiffnessMatrix,stiffnessMatrix.size());
	ierr=myLinearSystemSolver.reactionForcesCalc();CHKERRQ(ierr);
	
	ierr=PetscFinalize();CHKERRQ(ierr);

/*		EXPORT SOLUTION
	----------------------------------------------------------------*/

	dataProcessing myDataProcessing(displacement);

	return ierr;
};