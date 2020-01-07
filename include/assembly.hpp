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

	// Class functions
	double angleCalc(int);
	double sizeCalc(int);

	// Constructor
	assembly(string,string);

	// Destructor
	~assembly();
};

assembly::assembly(string connecFile, string coordFile)
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
}

assembly::~assembly(){}

double assembly::angleCalc(int element)
{
	double angle;

	return angle;
}

double assembly::sizeCalc(int element)
{
	double size;

	return size;
}