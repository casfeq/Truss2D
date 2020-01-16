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
	void exportSolutionsToTxt(vector<double>,vector<double>);

	// Constructor
	dataProcessing(vector<double>,vector<double>);

	// Destructor
	~dataProcessing();
};

dataProcessing::dataProcessing(vector<double> solution, vector<double> reactions)
{
	exportSolutionsToTxt(solution,reactions);
}

dataProcessing::~dataProcessing(){}

void dataProcessing::exportSolutionsToTxt(vector<double> solution, vector<double> reactions)
{
	int rowNo=solution.size();
	string fileName="../export/trussSolution.txt";

	ofstream myFile(fileName);
	if(myFile.is_open())
	{
		for(int i=0; i<rowNo; i++)
		{
			myFile << solution[i];
			myFile << "\n";
		}

		myFile.close();
	}

	fileName="../export/trussReactions.txt";

	ofstream myFile2(fileName);
	if(myFile2.is_open())
	{
		for(int i=0; i<rowNo; i++)
		{
			myFile2 << reactions[i];
			myFile2 << "\n";
		}

		myFile2.close();
	}

	return;
}