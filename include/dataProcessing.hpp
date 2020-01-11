#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class dataProcessing
{
public:
	// Class functions
	void exportDisplacementToTxt(vector<double>);

	// Constructor
	dataProcessing(vector<double> my1DField);

	// Destructor
	~dataProcessing();
};

dataProcessing::dataProcessing(vector<double> my1DField)
{
	exportDisplacementToTxt(my1DField);
}

dataProcessing::~dataProcessing(){}

void dataProcessing::exportDisplacementToTxt(vector<double> my1DField)
{
	int rowNo=my1DField.size();
	string fileName="../export/trussSolution.txt";

	ofstream myFile(fileName);
	if(myFile.is_open())
	{
		for(int i=0; i<rowNo; i++)
		{
			myFile << my1DField[i];
			myFile << "\n";
		}

		myFile.close();
	}

	return;
}