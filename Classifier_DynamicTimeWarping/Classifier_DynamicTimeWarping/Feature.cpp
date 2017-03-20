#include "Feature.h"

void Feature::ExtractPatternFeature(std::vector<Point3D> pattern_points)
{
	// Get Plane
	// Ax=B
	// ax+by+cz = d
	// (a/d)x+(b/d)y+(c/d)z=1
	// (a/d)x+(b/d)y+(c/d)z -1 = 0


	std::vector<std::vector<MathType>> matrix = ConvertToMatrix(pattern_points);
	std::vector<std::vector<MathType>> matrix_t = TransposeMatrix(matrix);
	std::vector<std::vector<MathType>> matrix_tmp = MultipleMatrices(matrix, matrix_t);
	
	std::vector<std::vector<MathType>> inverse_matrix_tmp= InverseMatrix(matrix_tmp);
	std::vector<std::vector<MathType>> pseudoinverse_matrix = MultipleMatrices(inverse_matrix_tmp, matrix_t);

	int n = pattern_points.size();
	plane.a = plane.b = plane.c= 0;
	plane.d = -1;
	for (int i = 0; i < n; i++)
	{
		plane.a += pseudoinverse_matrix[0][i];
		plane.b += pseudoinverse_matrix[1][i];
		plane.c += pseudoinverse_matrix[2][i];
	}

	// Get Projected Points
	//(u,v,w) => ( ax+by+cz+d= 0 ) =>(x0,y0,z0)
	for (int i = 0; i < pattern_points.size(); i++)
	{
		//original point (u,v,w)
		MathType u = pattern_points[i].x;
		MathType v = pattern_points[i].y;
		MathType w = pattern_points[i].z;

		//plane factors
		MathType a = plane.a;
		MathType b = plane.b;
		MathType c = plane.c;
		MathType d = plane.d;

		Point3D projected_point;
		projected_point.x = u - a*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));
		projected_point.y = v - b*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));
		projected_point.z = w - c*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));

		projected_points.push_back(projected_point);
	}
}

std::vector<Point3D> Feature::GetProjectedPoints()
{
	return projected_points;
}
