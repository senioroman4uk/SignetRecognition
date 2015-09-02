#pragma once
#include <opencv2/opencv.hpp>

class Recogniser 
{

public:
	Recogniser();
	Recogniser(char* new_results_path, char* new_templates_path);
	~Recogniser();
	char *results_path;
	char *templates_path;
	int findCircle(int argc, char** argv);
	int matchTemplate(int argc, char** argv);
	double percentageDiff(char* file1, char* file2);
	__int64 pictureDiffHash(char* file1, char* file2);
private:
	__int64 calcHammingDistance(__int64 x, __int64 y);
	__int64 calcImageHash(IplImage* src, bool show_results);
};