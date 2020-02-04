#include "pch.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <queue>
#include <fstream>

namespace fs = std::filesystem;
using namespace std;

//Path class to store imput atributes
class Path
{
private:
	string InputPath; //Path imputed by user
	string TextToFind; // Text imputed by user
	bool ValidTextToFind; //Flag inform if text has less then 128 characters
public:
	//Method to type initial path and check if path exists and if refers to directory or txt file
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
	//public method to reuurn full string with path
	string RetPath()
	{
		return this->InputPath;
	}
	//Method to type text which should be found
	bool InputText()
	{	
		cout << "Insert text to be found" << endl;
		cin >> this->TextToFind;

		if (this->TextToFind.length() > 128)
		{
			cout << "Inserted text is too long, max availabe length: 128 characters " << endl;
			this->ValidTextToFind = false;
			return this->ValidTextToFind;

		}
		else
		{
			this->ValidTextToFind = true;
			return this->ValidTextToFind;
		}
	}
	string RetTxT()
	{
		return this->TextToFind;
	}
};

//Class for one directory
class directory: public Path
{
private:
	int NumOfFiles; //Numbers of files in directory
	int NumOftextFiles; // Numbers of txt files in directory
	int NumOfFolders; // Numbers of other directory

public:
	queue <string> FilesQueue; //Store queue in directory with path to txt files


	//Method to check initial path
	void FindTxtFiles(string Path)
	{

		for (auto& entry : fs::directory_iterator(Path))
		{	
			string rPath = entry.path().u8string();
			if (rPath.substr(rPath.length() - 4, rPath.length()) == ".txt")
			{	
				this->FilesQueue.push(rPath);
			}		
		};


	}
};

//Class for one file
class File: public Path
{
private:
	string name; //Name of file
	int NumOfOcc; //Number of occurence of text typed in Path class
public:
	//Method set file name base on path
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
	void ExplorFile(string Inpath,string TxTtoFind)
	{
		fstream file;

		file.open(Inpath, ios::in);
		cout << Path::RetPath();
		if (file.good())
		{
			string line; //Store one line from txt file
			string Prevline; //Store Previous line

			int charNum=0; // Stor sum of character in txt file

			size_t occur; //Position where match has been found

			int TxTtoFind_Length = TxTtoFind.length(); //length of the match

			string preffix;
			string suffix;
			bool NextLineCout = false; //Refers if suffix require characters from next line
			int NextLineToCout = 0; //refers how many characters from next line should be display

			while (getline(file, line))
			{	
				line = line + "\\n";

				if (NextLineCout == true)
				{	
					cout << line.substr(0, NextLineToCout) << endl;
					NextLineCout = false;
				}


				occur = line.find(TxTtoFind);
				if (occur != string::npos)
				{	

					//Set Preffix start here
					if (occur > 2)
					{
						preffix = line.substr(occur - 3, 3);
					}
					else
					{	
						switch(occur)
						{
							case 0:
								preffix = Prevline.substr(Prevline.length() - 3, 3);
								break;
							case 1:
								preffix = Prevline.substr(Prevline.length() - 2, 2) + line.substr(0, 1);
								break;
							case 2:
								preffix = Prevline.substr(Prevline.length() - 1, 1) + line.substr(0, 2);
								break;
							default:
								break;
						}

					}
					// Setting preffix end here
					
					//Set Suffix start here
					int CharAfterOccur = line.length() - (occur + TxTtoFind_Length);

					if (CharAfterOccur > 2)
					{
						suffix = line.substr((occur + TxTtoFind_Length), 3);
					}
					else
					{
						switch(CharAfterOccur)
						{
						case 2:
							suffix = line.substr((occur + TxTtoFind_Length), 2);
							NextLineCout = true;
							NextLineToCout = 1;
							break;
						case 1:
							suffix = line.substr((occur + TxTtoFind_Length), 1);
							NextLineCout = true;
							NextLineToCout = 2;
							break;
						case 0:
							suffix = "";
							NextLineCout = true;
							NextLineToCout = 3;
							break;
						default:
							break;
						}
					}

					if (NextLineCout == false)
					{
						cout << this->name << "(" << charNum + occur << ")" << preffix << "..." << suffix << endl;
					}
					else
					{
						cout << this->name << "(" << charNum + occur << ")" << preffix << "..." << suffix;
					}
					

					charNum += line.length();
				}
				else
				{
					charNum += line.length();
				}
				Prevline = line;
			}


		}
		else
		{
			cout << "unexpected error in: " << this->name;
		}

		file.close();


	}




	
};


int main()
{	
	Path InitPath;
	if (InitPath.InputText() == true)
	{
		int PathValid = InitPath.InPath(); //check what type of path has been typed: 0 - txt file , 1 - directory 100 - path doesnt exists

		if (PathValid == 1)
		{
			directory Dir1;
			Dir1.FindTxtFiles(InitPath.RetPath());
			int QueueSize = Dir1.FilesQueue.size();
			for (int i = 0; i < QueueSize; i++)
			{
				File f;
				f.FileName(Dir1.FilesQueue.front());
				f.ExplorFile(Dir1.FilesQueue.front(), InitPath.RetTxT());

				Dir1.FilesQueue.pop();
			}
		}
		else if (PathValid == 0)
		{
			File f1;
			f1.FileName(InitPath.RetPath());
			f1.ExplorFile(InitPath.RetPath(), InitPath.RetTxT());
		}
		else
		{
			cout << "Path not found";
		}
	}
	




}

