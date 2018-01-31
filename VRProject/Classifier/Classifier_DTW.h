#pragma once
#ifndef CLASSIFIER_DTW_H
#define CLASSIFIER_DTW_H
#include "Feature.h"
#include "MyMath.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> GetAllFilesNamesWithinFolder(std::string folder);

enum PatternType
{
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4,
	PATTERN5,
	PATTERN6
};

class ClassifierDTW {

public :
	void CreatePatternSet(std::string folder_path);
	std::vector<std::vector<Point3D>> ReadPatternsFromFiles(std::string folder_path);
	PatternType Recognize(std::vector<Point3D> unknown_points);
private:
	std::vector<Feature> pattern_feature;
};

#endif