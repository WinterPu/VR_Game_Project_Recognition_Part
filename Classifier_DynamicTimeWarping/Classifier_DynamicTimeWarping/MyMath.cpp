#include "MyMath.h"

MathType Square(MathType number)
{
	return number * number;
}

MathType CalcDistanceEuclid3D(Point3D point1, Point3D point2)
{
	return sqrt(Square(point1.x - point2.x) + Square(point1.y - point2.y) + Square(point1.z - point2.z));
}

// n*3 matrix
std::vector<std::vector<MathType>> ConvertToMatrix(std::vector<Point3D> points)
{
	std::vector<std::vector<MathType>> matrix(points.size(), std::vector<MathType>(3));
	for (int i = 0; i < points.size(); i++)
	{
		matrix[i][0] = points[i].x;
		matrix[i][1] = points[i].y;
		matrix[i][2] = points[i].z;
	}
	return matrix;
}


std::vector<std::vector<MathType>> TransposeMatrix(std::vector<std::vector<MathType>> matrix)
{
	std::vector<std::vector<MathType>> matrix_transpose(matrix[0].size(), std::vector<MathType>(matrix.size()));
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix_transpose[0][i] = matrix[i][0];
		matrix_transpose[1][i] = matrix[i][1];
		matrix_transpose[2][i] = matrix[i][2];
	}
	return matrix_transpose;
}


//Method 1
std::vector<std::vector<MathType>> InverseMatrix(std::vector<std::vector<MathType>> matrix)
{
	int n = matrix.size();
	std::vector<std::vector<MathType>> inverse_matrix(n, std::vector<MathType>(n));
	MathType deltaA = GetA(matrix,matrix.size());
	if (deltaA == 0)
	{
		//Error! deltaA = 0
	}
	else
	{
		std::vector<std::vector<MathType>> a_star = GetAStar(matrix);
		for (int i = 0; i<n; i++)
		{
			for (int j = 0; j<n; j++)
			{
				inverse_matrix[i][j] = a_star[i][j] / deltaA;
			}
		}
	}
	return inverse_matrix;
}

MathType GetA(std::vector<std::vector<MathType>> matrix,int n)
{
	if (n == 1)
	{
		return matrix[0][0];
	}
	MathType answer = 0;
	std::vector<std::vector<MathType>> temp(n, std::vector<MathType>(n));
	int i, j, k;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n - 1; j++)
		{
			for (k = 0; k<n - 1; k++)
			{
				temp[j][k] = matrix[j + 1][(k >= i) ? k + 1 : k];

			}
		}
		double t = GetA(temp, n - 1);
		if (i % 2 == 0)
		{
			answer += matrix[0][i] * t;
		}
		else
		{
			answer -= matrix[0][i] * t;
		}
	}
	return answer;
}


std::vector<std::vector<MathType>>  GetAStar(std::vector<std::vector<MathType>> matrix)
{
	int n = matrix.size();
	std::vector<std::vector<MathType>> answer(n, std::vector<MathType>(n));
	if (n == 1)
	{
		answer[0][0] = matrix[0][0];
		return answer;
	}
	int i, j, k, t;
	std::vector<std::vector<MathType>> temp(n, std::vector<MathType>(n));
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			for (k = 0; k<n - 1; k++)
			{
				for (t = 0; t<n - 1; t++)
				{
					temp[k][t] = matrix[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
				}
			}
			answer[j][i] = GetA(temp, n - 1);
			if ((i + j) % 2 == 1)
			{
				answer[j][i] = -answer[j][i];
			}
		}
	}
	return answer;
}



//End Mathod 1
std::vector<std::vector<MathType>> MultipleMatrices(std::vector<std::vector<MathType>> matrix_a, std::vector<std::vector<MathType>> matrix_b)
{
	int n1 = matrix_a.size();
	int m1 = matrix_a[0].size();

	int n2 = matrix_b.size();
	int m2 = matrix_b[0].size();

	if (m1 != n2)
	{
		//Error!
	}

	std::vector<std::vector<MathType>> result(n1, std::vector<MathType>(m2));
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j <= m2; j++)
		{
			result[i][j] = 0;
			for (int k = 0; k < m1; k++)
				result[i][j] += matrix_a[i][k] * matrix_b[k][j];
		}
	}
	return result;

}
