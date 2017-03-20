#include <iostream>
#include "Classifier_DTW.h"
using namespace std;

int main()
{
	ClassifierDTW classifier;
	classifier.CreatePatternSet("C:/Users/Winter Pu/Desktop/pattern");
	vector<vector<Point3D>> patterns = classifier.ReadPatternsFromFiles("C:/Users/Winter Pu/Desktop/sample");
	for (int i = 0; i < patterns.size(); i++)
	{
		cout << (PatternType)classifier.Recognize(patterns[i]) << endl;
	}

	return 0;
}