#pragma once
#ifndef CLASSIFIER_H
#define CLASSIFIER_H
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
#include "Exception.h"

// true == read pattern names       false == read pattern files
std::vector<std::string> GetAllFilesNamesWithinFolder(std::string folder, bool flag_pattern_name_mode);

enum PatternType
{
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4,
	PATTERN5,
	PATTERN6
};

class Classifier {

public :
	void CreatePatternSet(std::string folder_path);
	std::vector<std::vector<Point3D>> ReadPatternsFromFiles(std::string folder_path, std::string pattern_name);
	PatternType RecognizeDTW(std::vector<Point3D> unknown_points); //Dynamic Time Warping
	PatternType RecognizeSW(std::vector<Point3D> unknown_points); // Smith Waterman
	
	
	//PatternType Recognize(std::vector<Point3D> unknown_points);
	std::string GetPatternName(int index);

private:
	std::vector<std::string> pattern_name;
	std::vector<std::vector<Feature>> pattern_feature;
	MathType recognition_rate;
};

//int EvaluateDistance(int a, int b);
#endif