#include "Feature.h"
#include <fstream>
int file_count = 1;
void Feature::ExtractPatternFeature(std::vector<Point3D> pattern_points)
{
	// Get Plane
	// Ax=B
	// ax+by+cz = d
	// (a/d)x+(b/d)y+(c/d)z=1
	// (a/d)x+(b/d)y+(c/d)z -1 = 0


	//**OpenCV Version
//	
//#ifdef DEBUG_MODE
//	clock_t start_time = clock();
//#endif // 
	try {
		cv::Mat matrix = ConvertToMatrixCV(pattern_points);
		cv::Mat matrix_t = matrix.t();
		cv::Mat matrix_tmp = matrix_t * matrix;
		cv::Mat inverse_matrix_tmp = matrix_tmp.inv(cv::DECOMP_CHOLESKY);

		if (cv::countNonZero(inverse_matrix_tmp) < 1)
			throw DetermintZeroError();

		cv::Mat pseudoinverse_matrix = inverse_matrix_tmp * matrix_t;
		//cv::Mat pseudoinverse_matrix = matrix.inv(cv::DECOMP_SVD);
		int n = pattern_points.size();
		plane.a = plane.b = plane.c = 0;
		plane.d = -1;
		for (int i = 0; i < n; i++)
		{
			plane.a += pseudoinverse_matrix.at<MathType>(0, i);
			plane.b += pseudoinverse_matrix.at<MathType>(1, i);
			plane.c += pseudoinverse_matrix.at<MathType>(2, i);
		}
	}
	catch (ExceptionBase & e) {
		e.Handle();
		return;
	}

	//** Other Version


	//std::vector<std::vector<MathType>> matrix = ConvertToMatrix(pattern_points);
	//std::vector<std::vector<MathType>> matrix_t = TransposeMatrix(matrix);
	//std::vector<std::vector<MathType>> matrix_tmp = MultipleMatrices(matrix_t, matrix);

	//std::vector<std::vector<MathType>> inverse_matrix_tmp = InverseMatrix(matrix_tmp);
	//std::vector<std::vector<MathType>> pseudoinverse_matrix = MultipleMatrices(inverse_matrix_tmp, matrix_t);

	//int n = pattern_points.size();
	//plane.a = plane.b = plane.c = 0;
	//plane.d = -1;
	//for (int i = 0; i < n; i++)
	//{
	//	plane.a += pseudoinverse_matrix[0][i];
	//	plane.b += pseudoinverse_matrix[1][i];
	//	plane.c += pseudoinverse_matrix[2][i];
	//}

//#if DEBUG_MODE
//	clock_t end_time = clock();
//	std::cout << "Running time is: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
//#endif // DEBUG_MODE

	

	//** Rotate the plane to x-y plane
	//plane factors
	MathType a = plane.a;
	MathType b = plane.b;
	MathType c = plane.c;
	MathType d = plane.d;
	MathType denominator1 = sqrt(b*b + c*c);

	/*
	1,         0,              0
	0, c/sqrt(b^2+c^2),    b/sqrt(b^2+c^2)
	0, - b/sqrt(b^2+c^2),  c/sqrt(b^2+c^2)
	*/
	cv::Mat Rotate_Matrix1 = (cv::Mat_<MathType>(3, 3) << 1, 0, 0, 0, c / denominator1, b / denominator1, 0, -b / denominator1, c / denominator1);

	/*
	sqrt(b^2+c^2)/sqrt(a^2+b^2+c^2), 0 ,a/sqrt(a^2+b^2+c^2)
	0,                               1,          0
	-a/sqrt(a^2+b^2+c^2),            0, sqrt(b^2+c^2)/sqrt(a^2+b^2+c^2)
	*/
	MathType denominator2 = sqrt(a*a+b*b + c*c);
	cv::Mat Rotate_Matrix2=(cv::Mat_<MathType>(3, 3) <<denominator1/ denominator2,0,a/ denominator2,0,1,0,-a/ denominator2,0, denominator1/ denominator2);
	//**End preparation for rotating the plane to x-y plane



	// Get Projected Points
	//(u,v,w) => ( ax+by+cz+d= 0 ) =>(x0,y0,z0)
	for (int i = 0; i < pattern_points.size(); i++)
	{
		//original point (u,v,w)
		MathType u = pattern_points[i].x;
		MathType v = pattern_points[i].y;
		MathType w = pattern_points[i].z;

		Point3D projected_point;
		projected_point.x = u - a*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));
		projected_point.y = v - b*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));
		projected_point.z = w - c*((a*u + b*v + c*w + d) / (a*a + b*b + c*c));

		cv::Mat point_matrix = (cv::Mat_<MathType>(1, 3) << projected_point.x, projected_point.y, projected_point.z);
		point_matrix = point_matrix*Rotate_Matrix1*Rotate_Matrix2;
		
		projected_point.x = point_matrix.at<MathType>(0,0);
		projected_point.y = point_matrix.at<MathType>(0, 1);
		projected_point.z = point_matrix.at<MathType>(0, 2);
		projected_points.push_back(projected_point);
	}

#if DEBUG_MODE_OUTPUT_MESSAGE
	std::string path = "C:/Users/Winter Pu/Desktop/projected/";
	std::string file_name = path + "pattern" + std::to_string(file_count++) + ".txt";
	std::fstream file1(file_name, std::ios::out);
	if (!file1.is_open())
	{
		std::cout << "Error" << std::endl;
	}
	for (int i = 0; i < projected_points.size(); i++)
	{
		file1 << projected_points[i].x << " " << projected_points[i].y << " " << projected_points[i].z << std::endl;
	}
	file1.close();
#endif

	//Use the mean point as the center

	Point3D center_point;
	center_point.x = center_point.y = center_point.z = 0;
	int num_points = projected_points.size();
	for (int i = 0; i < projected_points.size(); i++)
	{
		center_point.x += projected_points[i].x;
		center_point.y += projected_points[i].y;
	}
	center_point.x /= (MathType)num_points;
	center_point.y /= (MathType)num_points;

	for (int i = 0; i < projected_points.size(); i++)
	{
		projected_points[i].x -= center_point.x;
		projected_points[i].y -= center_point.y;
	}
#if DEBUG_MODE_OUTPUT_MESSAGE
	std::string path2= "C:/Users/Winter Pu/Desktop/moved/";
	std::string file_name2 = path2 + "pattern" + std::to_string(file_count++) + ".txt";
	std::fstream file2(file_name2, std::ios::out);
	if (!file2.is_open())
	{
		std::cout << "Error" << std::endl;
	}
	for (int i = 0; i < projected_points.size(); i++)
	{
		file2 << projected_points[i].x << " " << projected_points[i].y << " " << projected_points[i].z << std::endl;
	}
	file2.close();
#endif
}

std::vector<Point3D> Feature::GetProjectedPoints()
{
	return projected_points;
}
