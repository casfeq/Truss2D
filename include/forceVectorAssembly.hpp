#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class forceVectorAssembly
{
public:
	// Class variables
	vector<double> forceVector;
	vector<double> displacementValues;
	vector<double> forceValues;
	vector<vector<int>> dirichletBC;
	vector<vector<int>> neumannBC;

	// Class functions
	void importData(string,string);
	void assemblyForceVector();

	// Constructor
	forceVectorAssembly(string,string,int);

	// Destructor
	~forceVectorAssembly();
};

forceVectorAssembly::forceVectorAssembly(string dirichletBCFile, string neumannBCFile, int size)
{
	forceVector.resize(size);
	importData(dirichletBCFile,neumannBCFile);
	assemblyForceVector();
}

forceVectorAssembly::~forceVectorAssembly(){}

void forceVectorAssembly::importData(string dirichletBCFile, string neumannBCFile)
{
	ifstream inFile;
	int node, direction;
	double value;
	vector<int> thisPair;
	
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
		thisPair.push_back(node);
		thisPair.push_back(direction);
		dirichletBC.push_back(thisPair);
		displacementValues.push_back(value);
		thisPair.clear();
	}
	inFile.close();
	inFile.open("../input/"+neumannBCFile+".txt");
	if(!inFile)
	{
		cout << "Unable to open Neumann BC file.";
		exit(1);
	}
	while(inFile >> node)
	{
		inFile >> direction;
		inFile >> value;
		thisPair.push_back(node);
		thisPair.push_back(direction);
		neumannBC.push_back(thisPair);
		forceValues.push_back(value);
		thisPair.clear();
	}
	inFile.close();

	return;
}

void forceVectorAssembly::assemblyForceVector()
{
	for(int i=0; i<forceVector.size(); i++) forceVector[i]=0;

	for(int i=0; i<dirichletBC.size(); i++)
	{
		int index=2*dirichletBC[i][0]-(3-dirichletBC[i][1]);
		forceVector[index]+=displacementValues[i];
	}

	for(int i=0; i<neumannBC.size(); i++)
	{
		int index=2*neumannBC[i][0]-(3-neumannBC[i][1]);
		forceVector[index]+=forceValues[i];
	}

	return;
}