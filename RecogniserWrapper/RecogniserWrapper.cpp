// This is the main DLL file.

#include "stdafx.h"
#include "C:\Users\Vladyslav\Documents\Visual Studio 2013\Projects\RecogniserWrapper\RecogniserWrapper.h"

using namespace Wrapper;

RecogniserWrapper::RecogniserWrapper()
{
	converter = gcnew Wrapper::Converter();
	recogniser = new Recogniser();
}

//Конструктор
RecogniserWrapper::RecogniserWrapper(String^ resultsPath, String^ templatesPath)
{
	Wrapper::Converter ^converter = gcnew Wrapper::Converter();
	//Якщо заданої папки немає
	if (!System::IO::Directory::Exists(resultsPath))
		System::IO::Directory::CreateDirectory(resultsPath);
	if (!System::IO::Directory::Exists(templatesPath))
		throw gcnew Exception("Папка з шаблонами не знайдена");

	char* unmanaged_resultsPath = converter->stringToChar(resultsPath + "\\");
	char* unmanaged_templatesPath = converter->stringToChar(templatesPath);
	recogniser = new Recogniser(unmanaged_resultsPath, unmanaged_templatesPath);
}

int RecogniserWrapper::findCircle(int argc, array<String^>^ argv)
{
	char** strs = new char*[argc];
	for (int i = 0; i < argc; i++)
		strs[i] = converter->stringToChar(argv[i]);
	int result = recogniser->findCircle(argc, strs);

	// Add the line below will cause runtime assertion error (sth about heap error) in C#
	//free(str);   

	return result;
}

double RecogniserWrapper::percentageDiff(String^ file1, String^ file2)
{
	if (String::IsNullOrWhiteSpace(file1) || String::IsNullOrWhiteSpace(file2))
		return -1;

	char* unmanaged_file1 = converter->stringToChar(file1);
	char* unmanaged_file2 = converter->stringToChar(file2);
	return recogniser->percentageDiff(unmanaged_file1, unmanaged_file2);
}

long long RecogniserWrapper::pictureDiffHash(String^ file1, String^ file2)
{
	if (String::IsNullOrWhiteSpace(file1) || String::IsNullOrWhiteSpace(file2) )
		return -1;

	char* unmanaged_file1 = converter->stringToChar(file1);
	char* unmanaged_file2 = converter->stringToChar(file2);
	return recogniser->pictureDiffHash(unmanaged_file1, unmanaged_file2);
}

// ++++++++++++ Conversion Class ++++++++++++
// Переведення некерованої пам'яті с++ в керовану й навпаки

char* Wrapper::Converter::stringToChar(String^ str)
{
	IntPtr ptr = Marshal::StringToHGlobalAnsi(str);
	return static_cast<char*>(ptr.ToPointer());
}

String^ Wrapper::Converter::charToString(char *ptr)
{
	return gcnew String(ptr);
}