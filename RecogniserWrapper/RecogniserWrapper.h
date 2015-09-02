// RecogniserWrapper.h

#pragma once

#include "C:\Users\Vladyslav\Documents\Visual Studio 2013\Projects\Recognizer\Recogniser.h"
#include "C:\Users\Vladyslav\Documents\Visual Studio 2013\Projects\Recognizer\Recogniser.cpp"


using namespace System;
using namespace System::Runtime::InteropServices;

namespace Wrapper {
	public ref class Converter
	{
	public:
		char* stringToChar(String^ str);
		String^ charToString(char*);
	};

	public ref class RecogniserWrapper
	{
		// TODO: Add your methods for this class here.
	public:
		property String^ ResultsPath
		{
			String^ get()
			{
				return converter->charToString(recogniser->results_path);
			}
		};

		property String^ TemplatesPath
		{
			String^ get()
			{
				return converter->charToString(recogniser->templates_path);
			}
		};

		RecogniserWrapper::RecogniserWrapper(String^ resultsPath, String^ templatesPath);
		RecogniserWrapper();
		int findCircle(int argc, array<String^>^ argv);
		double percentageDiff(String^ file1, String^ file2);
		long long RecogniserWrapper::pictureDiffHash(String^ file1, String^ file2);
	private:
		Wrapper::Converter ^converter;
		Recogniser *recogniser;
	};
}