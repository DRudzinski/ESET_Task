#include "pch.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;


class Path
{
private:
	string InputPath;
	string TextToFind;
public:
	int InPath()
	{
		cout << "Insert path to file or directory" << endl;
		cin >> this->InputPath;
		if (fs::exists(this->InputPath))
		{
			string rPath = InputPath.substr(InputPath.length() - 4, InputPath.length());
			if (rPath == ".txt")
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{	
			cout << "Path nit found";
			return -100;
		}
	}

	string RetPath()
	{
		return this->InputPath;
	}

	void InputText()
	{	
		cout << "Insert text to be found" << endl;
		cin >> TextToFind;
	}
};


class directory: public Path
{
private:
	int NumOfFiles;
	int NumOftextFiles;
	int NumOfFolders;
public:
	void GetPath()
	{
		int i = 0;
		for (auto& entry : fs::directory_iterator(Path::RetPath()))
			(

				cout << entry.is_directory() << endl

			);
	}
};

class File: public Path
{
private:
	string name;
	int NumOfOcc;
public:
	string FileName(string InputPath)
	{	
		int i = 0;
		string sep="\\";
		size_t occur = InputPath.find(sep);
		while (occur != string::npos)
		{	
			occur = InputPath.find(sep, occur + 1);
			if (occur != string::npos)
			{	
				i = occur;
			}
			this->name = InputPath.substr(i+1, InputPath.length());

		}

		return this->name;
	}


	
};


int main()
{	
	Path InitPath;
	InitPath.InputText();

	int PathValid = InitPath.InPath();

	if (PathValid == 1)
	{
		directory Dir1;
		Dir1.GetPath();
	}
	else if (PathValid == 0)
	{
		File f1;
		cout<<f1.FileName(InitPath.RetPath());
	}
	else
	{
		cout << "Path not found";
	}





}

