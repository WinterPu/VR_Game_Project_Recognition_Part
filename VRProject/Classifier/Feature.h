#pragma once
#ifndef FEATURE_H
#define FEATURE_H
#include<vector>
#include "MyMath.h"
#include "DebugTools.h"
#include "Exception.h"

struct  PlaneFactors
{
	MathType a,b,c,d;
};

struct PatternFrame {
	MathType x_left;
	MathType x_right;
	MathType x_length;
	MathType y_top;
	MathType y_bottom;
	MathType y_length;
};

class Feature {
public:
	void ExtractPatternFeature(std::vector<Point3D> pattern_points);
	std::vector<Point2D> GetProjectedPoints();
private:
	// ax+by+cz+d=0
	// www.9math.com/book/projection-point-plane
	PlaneFactors plane;
	std::vector<Point2D> projected_points;
	std::vector<int> direction_feature;
};

#endif
