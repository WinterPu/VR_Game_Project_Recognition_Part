#include "Classifier.h"

//int file_count2 = 0;
void Classifier::CreatePatternSet(std::string folder_path)
{
	/*std::string pattern_folder = "C:/Users/Winter Pu/Desktop/pattern";*/
	pattern_name = GetAllFilesNamesWithinFolder(folder_path, true);
	for (int i = 0; i < pattern_name.size(); i++)
	{
		
		pattern_feature.push_back(std::vector<Feature>());

		std::vector<std::vector<Point3D>> points = ReadPatternsFromFiles(folder_path,pattern_name[i]);
		for (int j = 0; j < points.size(); j++)
		{
			Feature tmp_feature;
			tmp_feature.ExtractPatternFeature(points[j]);
			pattern_feature[i].push_back(tmp_feature);
		}
	}
}

std::vector<std::vector<Point3D>>  Classifier::ReadPatternsFromFiles(std::string folder_path,std::string pattern_name)
{
	std::vector<std::vector<Point3D>> patterns;

#if DEBUG_MODE_OUTPUT_MESSAGE
		std::cout << pattern_name << std::endl;
#endif
		folder_path += ("/" + pattern_name);
		std::vector<std::string> file_names = GetAllFilesNamesWithinFolder(folder_path, false);
		for (int j = 0; j < file_names.size(); j++)
		{
			std::vector<Point3D> tmp_pattern_points;
			std::string path = folder_path +"/"+file_names[j];
			std::fstream file(path);
			try {
				if (!file.is_open())
					throw FileOpenError();

				while (!file.eof())
				{
					Point3D point;
					file >> point.x >> point.y >> point.z;
					tmp_pattern_points.push_back(point);
				}
				file.close();
				patterns.push_back(tmp_pattern_points);
			}
			catch (ExceptionBase & e) {
				e.Handle();
			}
		}
		
	return patterns;
}

std::string Classifier::GetPatternName(int index)
{
	if (index < 0 || index >= pattern_name.size())
		return "Index Error";
	else
		return pattern_name[index];
}


std::vector<std::string> GetAllFilesNamesWithinFolder(std::string folder, bool flag_pattern_name_mode)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (flag_pattern_name_mode == true)
			{
				//remove . & .. folder
				if (fd.cFileName[0] == '.')
					continue;
				else 
					names.push_back(fd.cFileName);
			}
			else {

				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					names.push_back(fd.cFileName);
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}





//use distance

PatternType Classifier::RecognizeDTW(std::vector<Point3D> unknown_points)
{
	MathType min_dtw_distance = MAX_VALUE;
	int recognized_pattern_index = 0;

	Feature unknown_feature;
	unknown_feature.ExtractPatternFeature(unknown_points);
	std::vector<Point2D> unknown_feature_points = unknown_feature.GetProjectedPoints();
	for (int i = 0; i < pattern_feature.size(); i++)
	{
		for (int n = 0; n < pattern_feature[i].size(); n++)
		{
			std::vector<Point2D> pattern_points = pattern_feature[i][n].GetProjectedPoints();
			int n_pattern = pattern_points.size();
			int n_unknown = unknown_feature_points.size();
			std::vector<std::vector<MathType>> distance(n_pattern + 1, std::vector<MathType>(n_unknown + 1));
			std::vector<std::vector<MathType>> dp_dtw_output(n_pattern + 1, std::vector<MathType>(n_unknown + 1));

			//initialize
			//........

			//j => jth element
			//k => kth element
			for (int j = 1; j <= n_pattern; j++)
			{
				for (int k = 1; k <= n_unknown; k++)
				{
					distance[j][k] = CalcDistanceEuclid2D(pattern_points[j - 1], unknown_feature_points[k - 1]);
				}
			}

			for (int j = 1; j <= n_pattern; j++)
			{
				for (int k = 1; k <= n_unknown; k++)
				{
					dp_dtw_output[j][k] = min(min(dp_dtw_output[j - 1][k], dp_dtw_output[j][k - 1]), dp_dtw_output[j - 1][k - 1]) + distance[j][k];
				}
			}

			// find pattern type
			// ** Noted: so the default pattern type is 1st pattern 
			if (min_dtw_distance > dp_dtw_output[n_pattern][n_unknown])
			{
				min_dtw_distance = dp_dtw_output[n_pattern][n_unknown];
				recognized_pattern_index = i;
			}
		}
	}
	return (PatternType)(recognized_pattern_index);
}




//use Distance
// Smith Waterman
int GetScore(Point2D point1,Point2D point2) {
	int score = 3;
	MathType evaluated_distance = 0.1;
	MathType distance = CalcDistanceEuclid2D(point1,point2);
	if (distance <= evaluated_distance)
		return score;
	else
		return -score;
}


PatternType Classifier::RecognizeSW(std::vector<Point3D> unknown_points)
{
	MathType max_sw_distance = MIN_VALUE;
	int recognized_pattern_index = 0;

	Feature unknown_feature;
	unknown_feature.ExtractPatternFeature(unknown_points);
	std::vector<Point2D> unknown_feature_points = unknown_feature.GetProjectedPoints();
	
	for (int i = 0; i < pattern_feature.size(); i++)
	{
		for (int n = 0; n < pattern_feature[i].size(); n++)
		{
			std::vector<Point2D> pattern_points = pattern_feature[i][n].GetProjectedPoints();
			int n_pattern = pattern_points.size();
			int n_unknown = unknown_feature_points.size();
			//std::vector<std::vector<MathType>> distance(n_pattern + 1, std::vector<MathType>(n_unknown + 1));
			std::vector<std::vector<MathType>> dp_sw_score(n_pattern + 1, std::vector<MathType>(n_unknown + 1));


			int penalty = 2;

			if (n_pattern < 2 || n_unknown < 2)
			{
				//GG
				//Error
			}
			for (int i = 0; i <= n_pattern; i++)
				dp_sw_score[i][0] = 0;
			for (int j = 0; j <= n_unknown; j++)
				dp_sw_score[0][j] = 0;

			for (int i = 1; i <= n_pattern; i++)
			{
				for (int j = 1; j <= n_unknown; j++)
				{
					int match_score = GetScore(pattern_points[i - 1], unknown_feature_points[j - 1]);
					dp_sw_score[i][j] = max(max(dp_sw_score[i - 1][j - 1] + match_score, max(dp_sw_score[i][j - 1], dp_sw_score[i - 1][j]) - penalty), 0);
				}
			}

			MathType max_match_value = MIN_VALUE;
			for (int i = 1; i <= n_pattern; i++)
			{
				for (int j = 1; j <= n_unknown; j++)
				{
					if (max_match_value < dp_sw_score[i][j])
						max_match_value = dp_sw_score[i][j];
				}
			}

			// find pattern type
			// ** Noted: so the default pattern type is 1st pattern 
			if (max_sw_distance < max_match_value)
			{
				max_sw_distance = max_match_value;
				recognized_pattern_index = i;
			}
		}
	}
	return (PatternType)(recognized_pattern_index);
	
}

//
////use direction code
//PatternType Classifier::RecognizeDTW(std::vector<Point3D> unknown_points)
//{
//	MathType min_dtw_distance = MAX_VALUE;
//	int recognized_pattern_index = 0;
//
//	Feature unknown_feature;
//	unknown_feature.ExtractPatternFeature(unknown_points);
//	std::vector<int> unknown_feature_points = unknown_feature.GetDirectionFeature();
//
//
//
//#if DEBUG_MODE_OUTPUT_MESSAGE
//	std::string path = "C:/Users/Winter Pu/Desktop/Sample_DirCode/";
//	std::string file_name = path + "sample" + std::to_string(file_count2++) + ".txt";
//	std::fstream file1(file_name, std::ios::out);
//	std::vector<int> output = unknown_feature.GetDirectionFeature();
//	if (!file1.is_open())
//	{
//		std::cout << "Error" << std::endl;
//	}
//	for (int i = 0; i < output.size(); i++)
//	{
//		file1 << output[i] << std::endl;
//	}
//	file1.close();
//#endif
//
//	for (int i = 0; i < pattern_feature.size(); i++)
//	{
//		std::vector<int> pattern_points = pattern_feature[i].GetDirectionFeature();
//		int n_pattern = pattern_points.size();
//		int n_unknown = unknown_feature_points.size();
//		std::vector<std::vector<MathType>> distance(n_pattern + 1, std::vector<MathType>(n_unknown + 1));
//		std::vector<std::vector<MathType>> dp_dtw_output(n_pattern + 1, std::vector<MathType>(n_unknown + 1));
//
//		//initialize
//		//........
//
//		//j => jth element
//		//k => kth element
//		for (int j = 1; j <= n_pattern; j++)
//		{
//			for (int k = 1; k <= n_unknown; k++)
//			{
//				distance[j][k] = EvaluateDistance(pattern_points[j - 1], unknown_feature_points[k - 1]);
//			}
//		}
//
//		for (int j = 1; j <= n_pattern; j++)
//		{
//			for (int k = 1; k <= n_unknown; k++)
//			{
//				dp_dtw_output[j][k] = min(min(dp_dtw_output[j - 1][k], dp_dtw_output[j][k - 1]), dp_dtw_output[j - 1][k - 1]) + distance[j][k];
//			}
//		}
//
//		// find pattern type
//		// ** Noted: so the default pattern type is 1st pattern 
//		if (min_dtw_distance > dp_dtw_output[n_pattern][n_unknown])
//		{
//			min_dtw_distance = dp_dtw_output[n_pattern][n_unknown];
//			recognized_pattern_index = i;
//		}
//	}
//	return (PatternType)(recognized_pattern_index);
//}
//
//int EvaluateDistance(int a, int b)
//{
//	int tmp = a > b ? a - b : b - a;
//	return tmp >= 4 ? 8 - tmp : tmp;
//	//return a-b;
//}
//
//// Smith Waterman
//int GetScore(int point1, int point2) {
//	int score = 6;
//
//	//first version better: evaluated_distance = 1
//	////better for waterman
//	//direction_feature.push_back((int)(dircode_list[i]-dircode_list[i-1]));
//
//	MathType evaluated_distance = 1;
//	MathType distance = EvaluateDistance((DirCode)point1, (DirCode)point2);
//	return -distance;
//	//MathType distance = point1 - point2;
//	//if (distance <= evaluated_distance)
//	//	return score;
//	//else
//	//	return -score;
//}
//// Smith Waterman
////int GetScore(int point1, int point2) {
////	int score = 6;
////	int low_score = 3;
////	MathType evaluated_distance = 0;
////	MathType distance = EvaluateDistance((DirCode)point1, (DirCode)point2);
////	if (distance <= evaluated_distance)
////		return score;
////	else if (distance == 1)
////		return low_score;
////	else if (distance == 2)
////		return -low_score;
////	else if (distance >2)
////		return -score;
////}
//
//
//PatternType Classifier::RecognizeSW(std::vector<Point3D> unknown_points)
//{
//	MathType max_sw_distance = MIN_VALUE;
//	int recognized_pattern_index = 0;
//
//	Feature unknown_feature;
//	unknown_feature.ExtractPatternFeature(unknown_points);
//	std::vector<int> unknown_feature_points = unknown_feature.GetDirectionFeature();
//	
//	for (int i = 0; i < pattern_feature.size(); i++)
//	{
//		std::vector<int> pattern_points = pattern_feature[i].GetDirectionFeature();
//		int n_pattern = pattern_points.size();
//		int n_unknown = unknown_feature_points.size();
//		//std::vector<std::vector<MathType>> distance(n_pattern + 1, std::vector<MathType>(n_unknown + 1));
//		std::vector<std::vector<MathType>> dp_sw_score(n_pattern+1, std::vector<MathType>(n_unknown+1));
//
//
//		int penalty = 2;
//
//		if (n_pattern < 2 || n_unknown < 2)
//		{	
//			//GG
//			//Error
//		}
//		for (int i = 0; i <= n_pattern; i++)
//			dp_sw_score[i][0] = 0;
//		for (int j = 0; j <= n_unknown; j++)
//			dp_sw_score[0][j] = 0;
//
//		for (int i = 1; i <= n_pattern; i++)
//		{
//			for (int j = 1; j <= n_unknown; j++)
//			{
//				int match_score = GetScore(pattern_points[i-1],unknown_feature_points[j-1]);
//				dp_sw_score[i][j] = max(max(dp_sw_score[i - 1][j - 1] + match_score, max(dp_sw_score[i][j - 1], dp_sw_score[i - 1][j]) - penalty), 0);
//			}
//		}
//
//		MathType max_match_value = MIN_VALUE;
//		for (int i = 1; i <= n_pattern; i++)
//		{
//			for (int j = 1; j <= n_unknown; j++)
//			{
//				if (max_match_value < dp_sw_score[i][j])
//					max_match_value = dp_sw_score[i][j];
//			}
//		}
//
//		// find pattern type
//		// ** Noted: so the default pattern type is 1st pattern 
//		if (max_sw_distance < max_match_value)
//		{
//			max_sw_distance = max_match_value;
//			recognized_pattern_index = i;
//		}
//	}
//	return (PatternType)(recognized_pattern_index);
//	
//}
//
//PatternType Classifier::Recognize(std::vector<Point3D> unknown_points)
//{
//	//Feature unknown_feature;
//	//unknown_feature.ExtractPatternFeature(unknown_points);
//	//std::vector<int> unknown_feature_points = unknown_feature.GetDirectionFeature();
//
//	//std::vector<Feature> pattern_feature;
//
//	//Feature circle;
//
//	
//	return (PatternType)1;
//
//}
