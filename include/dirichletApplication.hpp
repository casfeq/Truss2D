#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class dirichletApplication
{
public:
	// Class variables
	vector<vector<double>> stiffnessMatrix;
	vector<vector<int>> dirichletBC;

	// Class functions
	void importData(string);
	void applyDirichletBC();

	// Constructor
	dirichletApplication(string,vector<vector<double>>);

	// Destructor
	~dirichletApplication();
};

dirichletApplication::dirichletApplication(string dirichletBCFile,
	vector<vector<double>> myStiffnessMatrix)
{
	stiffnessMatrix=myStiffnessMatrix;
	importData(dirichletBCFile);
	applyDirichletBC();
}

dirichletApplication::~dirichletApplication(){}

void dirichletApplication::importData(string dirichletBCFile)
{
	ifstream inFile;
	int node, direction;
	double value;
	vector<int> dirichletPair;
	
	inFile.open("../input/"+dirichletBCFile+".txt");
	if(!inFile)
	{
		cout << "Unable to open Dirichlet BC file.";
		exit(1);
	}
	while(inFile >> node)
	{
		inFile >> direction;
		inFile >> value;
		dirichletPair.push_back(node);
		dirichletPair.push_back(direction);
		dirichletBC.push_back(dirichletPair);
		dirichletPair.clear();
	}
	inFile.close();

	return;
}

void dirichletApplication::applyDirichletBC()
{
	for(int i=0; i<dirichletBC.size(); i++)
	{
		int index=2*dirichletBC[i][0]-(3-dirichletBC[i][1]);
		stiffnessMatrix[index].assign(stiffnessMatrix.size(),0);
		stiffnessMatrix[index][index]+=1;
	}

	return;
}