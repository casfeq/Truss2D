#include <iostream>
#include <petscksp.h>
#include <string>
#include <vector>

using namespace std;

class linearSystemSolver
{
public:
	// Class variables
	vector<vector<double>> coefficientsMatrix;
	vector<double> sparseCoefficientsRow;
	vector<double> sparseCoefficientsColumn;
	vector<double> sparseCoefficientsValue;
	vector<double> independentTermsArray;
	vector<double> solution;
	vector<double> reactions;
	int size;
	PetscErrorCode ierr;
	Mat coefficientsMatrixPETSc;
	Vec independentTermsArrayPETSc;
	Vec linearSystemSolutionPETSc;
	Vec reactionsPETSc;
	IS perm, iperm;
	MatFactorInfo info;

	// Class functions
	void importData(vector<vector<double>>,int);
	int coefficientsMatrixLUFactorization();
	int createPETScArrays();
	int zeroPETScArrays();
	int setRHSValue(vector<double>);
	int solveLinearSystem();
	int setFieldValue();
	int reactionForcesCalc();

	// Constructor
	linearSystemSolver(vector<vector<double>>,int);

	// Destructor
	~linearSystemSolver();
};

linearSystemSolver::linearSystemSolver(vector<vector<double>> myCoefficientsMatrix, int mySize)
{
	importData(myCoefficientsMatrix,mySize);

	return;
}

linearSystemSolver::~linearSystemSolver(){}

void linearSystemSolver::importData(vector<vector<double>> myCoefficientsMatrix, int mySize)
{
	coefficientsMatrix=myCoefficientsMatrix;
	size=mySize;
	sparseCoefficientsRow.clear();
	sparseCoefficientsColumn.clear();
	sparseCoefficientsValue.clear();

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			double value=coefficientsMatrix[i][j];
			if(value!=0)
			{
				sparseCoefficientsRow.push_back(i);
				sparseCoefficientsColumn.push_back(j);
				sparseCoefficientsValue.push_back(value);
			}
		}
	}

	return;
}

int linearSystemSolver::coefficientsMatrixLUFactorization()
{
	PetscInt n=size;
	PetscInt nonZeroEntries=sparseCoefficientsValue.size();
	PetscInt rowNo, colNo;
	PetscScalar value;

	ierr=MatCreate(PETSC_COMM_WORLD,&coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatSetSizes(coefficientsMatrixPETSc,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr=MatSetFromOptions(coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatSetUp(coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatZeroEntries(coefficientsMatrixPETSc);CHKERRQ(ierr);

	for(int i=0; i<nonZeroEntries; i++)
	{	
		rowNo=sparseCoefficientsRow[i];
		colNo=sparseCoefficientsColumn[i];
		value=sparseCoefficientsValue[i];
		ierr=MatSetValue(coefficientsMatrixPETSc,rowNo,colNo,value,ADD_VALUES);CHKERRQ(ierr);
	}

	ierr=MatAssemblyBegin(coefficientsMatrixPETSc,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr=MatAssemblyEnd(coefficientsMatrixPETSc,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	ierr=MatGetOrdering(coefficientsMatrixPETSc,MATORDERINGRCM,&perm,&iperm);CHKERRQ(ierr);

	ierr=MatFactorInfoInitialize(&info);CHKERRQ(ierr);
	info.fill=1.0;
	info.dt=0;
	info.dtcol=0;
	info.zeropivot=0;
	info.pivotinblocks=0;

	ierr=MatLUFactor(coefficientsMatrixPETSc,perm,iperm,&info);CHKERRQ(ierr);

	return ierr;
}

int linearSystemSolver::createPETScArrays()
{
	PetscInt n=size;

	ierr=VecCreateSeq(PETSC_COMM_SELF,n,&independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecSet(independentTermsArrayPETSc,0.0);CHKERRQ(ierr);
	ierr=VecAssemblyBegin(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyEnd(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecDuplicate(independentTermsArrayPETSc,&linearSystemSolutionPETSc);CHKERRQ(ierr);

	return ierr;
}

int linearSystemSolver::zeroPETScArrays()
{
	ierr=VecZeroEntries(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyBegin(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyEnd(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecZeroEntries(linearSystemSolutionPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyBegin(linearSystemSolutionPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyEnd(linearSystemSolutionPETSc);CHKERRQ(ierr);

	return ierr;
}

int linearSystemSolver::setRHSValue(vector<double> independentTermsArray)
{
	PetscInt n=size;
	PetscScalar value;

	for(int i=0; i<n; i++)
	{
		value=independentTermsArray[i];
		ierr=VecSetValue(independentTermsArrayPETSc,i,value,ADD_VALUES);CHKERRQ(ierr);
	}

	ierr=VecAssemblyBegin(independentTermsArrayPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyEnd(independentTermsArrayPETSc);CHKERRQ(ierr);

	return ierr;
}

int linearSystemSolver::solveLinearSystem()
{	
	ierr=MatSolve(coefficientsMatrixPETSc,independentTermsArrayPETSc,linearSystemSolutionPETSc);
		CHKERRQ(ierr);
	ierr=VecAssemblyBegin(linearSystemSolutionPETSc);CHKERRQ(ierr);
	ierr=VecAssemblyEnd(linearSystemSolutionPETSc);CHKERRQ(ierr);

	return ierr;
}

int linearSystemSolver::setFieldValue()
{
	PetscScalar value;

	solution.clear();
	for(int i=0; i<size; i++)
	{
		ierr=VecGetValues(linearSystemSolutionPETSc,1,&i,&value);CHKERRQ(ierr);
		solution.push_back(value);
	}

	return ierr;
}

int linearSystemSolver::reactionForcesCalc()
{
	PetscInt n=size;
	PetscInt nonZeroEntries=sparseCoefficientsValue.size();
	PetscInt rowNo, colNo;
	PetscScalar value;
	
	ierr=MatCreate(PETSC_COMM_WORLD,&coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatSetSizes(coefficientsMatrixPETSc,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr=MatSetFromOptions(coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatSetUp(coefficientsMatrixPETSc);CHKERRQ(ierr);
	ierr=MatZeroEntries(coefficientsMatrixPETSc);CHKERRQ(ierr);

	for(int i=0; i<nonZeroEntries; i++)
	{	
		rowNo=sparseCoefficientsRow[i];
		colNo=sparseCoefficientsColumn[i];
		value=sparseCoefficientsValue[i];
		ierr=MatSetValue(coefficientsMatrixPETSc,rowNo,colNo,value,ADD_VALUES);CHKERRQ(ierr);
	}

	ierr=MatAssemblyBegin(coefficientsMatrixPETSc,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr=MatAssemblyEnd(coefficientsMatrixPETSc,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	// ierr=MatMultAdd()

	return ierr;
}