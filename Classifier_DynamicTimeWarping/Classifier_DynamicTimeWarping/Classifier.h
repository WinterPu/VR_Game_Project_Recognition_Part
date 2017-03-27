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

class Classifier {

public :
	void CreatePatternSet(std::string folder_path);
	std::vector<std::vector<Point3D>> ReadPatternsFromFiles(std::string folder_path);
	PatternType RecognizeDTW(std::vector<Point3D> unknown_points); //Dynamic Time Warping
	PatternType RecognizeSW(std::vector<Point3D> unknown_points); // Smith Waterman
private:
	std::vector<Feature> pattern_feature;
};

#endif