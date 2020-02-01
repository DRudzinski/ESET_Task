#include "pch.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

class directory
{
private:
	string path;

	bool PathExists;

	int NumOfFiles;
	int NumOftextFiles;
	int NumOfFolders;
public:
	void InsertPath()
	{
		cin >> this->path;
		if (fs::exists(this->path))
		{	
			this->PathExists = true;
			int i = 0;
			for (auto& entry : fs::directory_iterator(path))
				(

					cout << entry.is_directory() << endl

					);
		}
		else
		{	
			this->PathExists = false;
			cout << "Path did not found";
		}
	};
};

class File
{
private:
	string name;
	int characters;
	bool txtFile;
	string FindText;
public:
	void InsertText()
	{
		cin >> this->FindText;
	}
};


int main()
{	
	directory D1;
	File f1;
    cout << "Insert path to file or folder\n"; 
	cout << "Path:";
	D1.InsertPath();

	cout << "\nInsert text to be found\n";
	cout << "Text:";
	f1.InsertText();





}

