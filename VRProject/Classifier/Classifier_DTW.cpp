#include "Classifier_DTW.h"

void ClassifierDTW::CreatePatternSet(std::string folder_path)
{
	/*std::string pattern_folder = "C:/Users/Winter Pu/Desktop/pattern";*/
	std::vector<std::vector<Point3D>> patterns = ReadPatternsFromFiles(folder_path);
	for (int i = 0; i < patterns.size(); i++)
	{
		Feature tmp_feature;
		tmp_feature.ExtractPatternFeature(patterns[i]);
		pattern_feature.push_back(tmp_feature);
	}
}

std::vector<std::vector<Point3D>>  ClassifierDTW::ReadPatternsFromFiles(std::string folder_path)
{
	std::vector<std::string> pattern_names = GetAllFilesNamesWithinFolder(folder_path);
	std::vector<std::vector<Point3D>> patterns;
	for (int i = 0; i < pattern_names.size(); i++)
	{
		std::vector<Point3D> tmp_pattern_points;

		std::string path = folder_path + "/" + pattern_names[i];
		std::fstream file(path);
		if (!file.is_open())
		{
			//File Open Error!
			//return;
		}
		else {
			while (!file.eof())
			{
				Point3D point;
				file >> point.x >> point.y >> point.z;
				tmp_pattern_points.push_back(point);
			}
			file.close();
			patterns.push_back(tmp_pattern_points);
		}
	}
	return patterns;
}

std::vector<std::string> GetAllFilesNamesWithinFolder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


PatternType ClassifierDTW::Recognize(std::vector<Point3D> unknown_points)
{
	MathType min_dtw_distance = MAX_VALUE;
	int recognized_pattern_index = 0;
	
	Feature unknown_feature;
	unknown_feature.ExtractPatternFeature(unknown_points);
	std::vector<Point3D> unknown_feature_points = unknown_feature.GetProjectedPoints();
	for (int i = 0; i < pattern_feature.size(); i++)
	{
		std::vector<Point3D> pattern_points = pattern_feature[i].GetProjectedPoints();
		int n_pattern = pattern_feature.size();
		int n_unknown = unknown_feature_points.size();
		std::vector<std::vector<MathType>> distance(n_pattern+1, std::vector<MathType>(n_unknown+1));
		std::vector<std::vector<MathType>> dp_dtw_output(n_pattern+1, std::vector<MathType>(n_unknown+1));

		//initialize
		//........

		//j => jth element
		//k => kth element
		for (int j = 1; j <= n_pattern; j++)
		{
			for (int k = 1; k <=n_unknown; k++)
			{
				distance[j][k] = CalcDistanceEuclid3D(pattern_points[j-1], unknown_feature_points[k-1]);
			}
		}

		for (int j = 1; j <= n_pattern; j++)
		{
			for (int k = 1; k <= n_unknown; k++)
			{
				dp_dtw_output[j][k] = min(min(dp_dtw_output[j-1][k], dp_dtw_output[j][k-1]), dp_dtw_output[j-1][k-1]) + distance[j][k];
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
	return (PatternType)(recognized_pattern_index);
}
