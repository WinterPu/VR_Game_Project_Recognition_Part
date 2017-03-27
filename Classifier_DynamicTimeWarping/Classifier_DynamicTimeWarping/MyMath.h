#pragma once
#ifndef  MYMATH_H
#define	 MYMATH_H
#include <cmath>
#include <vector>
#include "core/core.hpp"

#define MAX_VALUE 99999999
#define MIN_VALUE -99999
typedef double MathType;
#define MatrixType CV_64F 

struct Point3D
{
	MathType x, y, z;
};
cv::Mat ConvertToMatrixCV(std::vector<Point3D>);
std::vector<std::vector<MathType>> ConvertToVector(cv::Mat);



MathType Square(MathType number);
MathType CalcDistanceEuclid3D(Point3D point1, Point3D point2);



std::vector<std::vector<MathType>> ConvertToMatrix(std::vector<Point3D>);
std::vector<std::vector<MathType>> TransposeMatrix(std::vector<std::vector<MathType>> matrix);
std::vector<std::vector<MathType>> InverseMatrix(std::vector<std::vector<MathType>> matrix);
MathType GetA(std::vector<std::vector<MathType>> matrix,int n);
std::vector<std::vector<MathType>>  GetAStar(std::vector<std::vector<MathType>> matrix);
std::vector<std::vector<MathType>> MultipleMatrices(std::vector<std::vector<MathType>>, std::vector<std::vector<MathType>>);
#endif // ! MYMATH_H
