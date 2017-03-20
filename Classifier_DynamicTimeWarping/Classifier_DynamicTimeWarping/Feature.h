#pragma once
#ifndef FEATURE_H
#define FEATURE_H
#include<vector>
#include "MyMath.h"

struct  PlaneFactors
{
	MathType a,b,c,d;
};


class Feature {
public:
	void ExtractPatternFeature(std::vector<Point3D> pattern_points);
	std::vector<Point3D> GetProjectedPoints();
private:
	// ax+by+cz+d=0
	// www.9math.com/book/projection-point-plane
	PlaneFactors plane;
	std::vector<Point3D> projected_points;
};
#endif
