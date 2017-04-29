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

//Direction Code
enum DirCode{
	Top,			//0
	TopRight,		//1
	Right,			//2
	BottomRight,	//3
	Bottom,			//4
	BottomLeft,		//5
	Left,			//6
	TopLeft,		//7
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
	std::vector<Point3D> GetProjectedPoints();
	DirCode GenerateDirCode(Point3D base, Point3D vector);
	DirCode GetDirCodeByAngle(MathType angle);
	std::vector<int> GetDirectionFeature();
private:
	// ax+by+cz+d=0
	// www.9math.com/book/projection-point-plane
	PlaneFactors plane;
	std::vector<Point3D> projected_points;
	std::vector<int> direction_feature;
};


int CalcDirectionDistance(DirCode a, DirCode b);
#endif
