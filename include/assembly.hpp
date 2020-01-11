#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

class assembly
{
public:
	// Class variables
	vector<vector<int>> connectivity;
	vector<vector<double>> coordinates;
	double youngModulus;
	double area;
	vector<vector<double>> elementStiffnessMatrix;
	vector<vector<double>> stiffnessMatrix;

	// Class functions
	void importData(string,string,string);
	void resizeMatrices();
	double angleCalc(int);
	double sizeCalc(int);
	void assemblyElementStiffnessMatrix(int);
	void stiffnessMatrixOverlap(int);
	void assemblyStiffnessMatrix();

	// Constructor
	assembly(string,string,string);

	// Destructor
	~assembly();
};

assembly::assembly(string connecFile, string coordFile, string propFile)
{
	importData(connecFile,coordFile,propFile);
	resizeMatrices();
	assemblyStiffnessMatrix();
}

assembly::~assembly(){}

void assembly::importData(string connecFile, string coordFile, string propFile)
{
	ifstream inFile;
	int I, J;
	vector<int> connecPair;
	double x, y;
	vector<double> coordPair;
	inFile.open("../input/"+connecFile+".txt");
	if(!inFile)
	{
		cout << "Unable to open connectivity file.";
		exit(1);
	}
	while(inFile >> I)
	{
		inFile >> J;
		connecPair.push_back(I);
		connecPair.push_back(J);
		connectivity.push_back(connecPair);
		connecPair.clear();
	}
	inFile.close();	
	inFile.open("../input/"+coordFile+".txt");
	if(!inFile)
	{
		cout << "Unable to open coordinates file.";
		exit(1);
	}
	while(inFile >> x)
	{
		inFile >> y;
		coordPair.push_back(x);
		coordPair.push_back(y);
		coordinates.push_back(coordPair);
		coordPair.clear();
	}
	inFile.close();
	inFile.open("../input/"+propFile+".txt");
	if(!inFile)
	{
		cout << "Unable to open coordinates file.";
		exit(1);
	}
	inFile >> youngModulus;
	inFile >> area;
	inFile.close();

	return;
}

void assembly::resizeMatrices()
{
	elementStiffnessMatrix.resize(4);
	for(int i=0; i<4; i++) elementStiffnessMatrix[i].resize(4);
	stiffnessMatrix.resize(2*coordinates.size());
	for(int i=0; i<2*coordinates.size(); i++) stiffnessMatrix[i].resize(2*coordinates.size());

	return;	
}

double assembly::angleCalc(int element)
{
	double angle;
	int I=connectivity[element][0]-1;
	int J=connectivity[element][1]-1;
	double xI=coordinates[I][0];
	double yI=coordinates[I][1];
	double xJ=coordinates[J][0];
	double yJ=coordinates[J][1];

	angle=atan((yJ-yI)/(xJ-xI));

	return angle;
}

double assembly::sizeCalc(int element)
{
	double size;
	int I=connectivity[element][0]-1;
	int J=connectivity[element][1]-1;
	double xI=coordinates[I][0];
	double yI=coordinates[I][1];
	double xJ=coordinates[J][0];
	double yJ=coordinates[J][1];

	size=sqrt((yJ-yI)*(yJ-yI)+(xJ-xI)*(xJ-xI));

	return size;
}

void assembly::assemblyElementStiffnessMatrix(int element)
{
	double elementStiffness=youngModulus*area/sizeCalc(element);
	double angle=angleCalc(element);

	elementStiffnessMatrix[0][0]=elementStiffness*cos(angle)*cos(angle);
	elementStiffnessMatrix[0][1]=elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[0][2]=-elementStiffness*cos(angle)*cos(angle);
	elementStiffnessMatrix[0][3]=-elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[1][0]=elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[1][1]=elementStiffness*sin(angle)*sin(angle);
	elementStiffnessMatrix[1][2]=-elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[1][3]=-elementStiffness*sin(angle)*sin(angle);
	elementStiffnessMatrix[2][0]=-elementStiffness*cos(angle)*cos(angle);
	elementStiffnessMatrix[2][1]=-elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[2][2]=elementStiffness*cos(angle)*cos(angle);
	elementStiffnessMatrix[2][3]=elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[3][0]=-elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[3][1]=-elementStiffness*sin(angle)*sin(angle);
	elementStiffnessMatrix[3][2]=elementStiffness*cos(angle)*sin(angle);
	elementStiffnessMatrix[3][3]=elementStiffness*sin(angle)*sin(angle);

	return;
}

void assembly::stiffnessMatrixOverlap(int element)
{
	int I=2*connectivity[element][0]-2;
	int J=2*connectivity[element][1]-2;

	for(int i=0; i<2; i++)
		for(int j=0; j<2; j++)
		{
			stiffnessMatrix[I+i][I+j]+=elementStiffnessMatrix[i][j];
			stiffnessMatrix[I+i][J+j]+=elementStiffnessMatrix[i][j+2];
			stiffnessMatrix[J+i][I+j]+=elementStiffnessMatrix[i+2][j];
			stiffnessMatrix[J+i][J+j]+=elementStiffnessMatrix[i+2][j+2];
		}

	return;
}

void assembly::assemblyStiffnessMatrix()
{
	for(int i=0; i<connectivity.size(); i++)
	{
		assemblyElementStiffnessMatrix(i);
		stiffnessMatrixOverlap(i);
	}

	return;
}