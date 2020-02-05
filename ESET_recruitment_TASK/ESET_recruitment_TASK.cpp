#include "pch.h"
#include <iostream>
#include <string>
#include <filesystem> //Library necessery to use find. method 
#include <queue> //Necessery to implement queue
#include <fstream> // Use to open file end get all text

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
		//Get initial path
		cout << "Insert path to file or directory" << endl;
		cin >> this->InputPath;


		//Validate if path lead to txt file or directory
		if (fs::exists(this->InputPath))
		{
			string rPath = InputPath.substr(InputPath.length() - 4, InputPath.length());
			if (rPath == ".txt")
			{
				return 0; //if path lead to txt file
			}
			else
			{
				return 1; //if path lead to directory
			}
		}
		else
		{	
			cout << "Path not found";
			return -100; //if path not exists
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
		cin >> this->TextToFind; //Get text to be found


		//Chceck if text is lower then 128 charcters
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
	//Return text to be found
	string RetTxT()
	{
		return this->TextToFind;
	}
};
//----------------------------------------------
//----------------End of Path Class------------

//Class for one directory
class directory
{
public:
	queue <string> FilesQueue; //Store paths to txt files
	queue <string> DirQueue; //Store paths to directorys

	//Method to explore path and search for txt files
	void FindTxtFiles(string Path)
	{
		//Looking for txt files
		for (auto& entry : fs::directory_iterator(Path))
		{	
			string rPath = entry.path().u8string();
			if (rPath.substr(rPath.length() - 4, rPath.length()) == ".txt")
			{	
				this->FilesQueue.push(rPath);
			}		
		};


	}

	//Method to expolore path and looking for directories
	void FindDir(string Path)
	{
		//Looking for Dir
		for (auto& entry : fs::directory_iterator(Path))
		{	
			if (entry.is_directory())
			{
				this->DirQueue.push(entry.path().u8string());
			}
		}

	}


};
//----------------------------------------------
//------------End of Directory Class------------



//Class for one file
class File
{
private:
	string name; //Name of file
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

	//Method to expore txt file and looking for matches
	void ExplorFile(string Inpath,string TxTtoFind)
	{
		fstream file;

		file.open(Inpath, ios::in);

		if (file.good())
		{
			//--------------------------------------------
			//--------------Set variables-----------------
			string line; //Store one line from txt file
			string Prevline=""; //Store Previous line

			int charNum=0; // Stor sum of character in txt file

			size_t occur; //Position where match has been found

			int TxTtoFind_Length = TxTtoFind.length(); //length of the match

			string preffix;
			string suffix;
			bool NextLineCout = false; //Refers if suffix require characters from next line
			int NextLineToCout = 0; //refers how many characters from next line should be display

			size_t offset = 0; //Offset for Find. method

			//--------------------------------------------
			//-------------MAIN LOOP----------------------
			//--------------------------------------------
			while (getline(file, line))
			{	
				//--------------------------------------------
				//Display missing suffix characters when match is at the end of the line
				if (NextLineCout == true)
				{	
					string tempSuffix = line.substr(0, NextLineToCout);
					//Check if there are tabulators
					offset = 0;
					//Looking for tabulator in suffix
					while ((tempSuffix.find("\t", offset) != string::npos))
					{
						size_t tab_poz = tempSuffix.find("\t", offset);

						switch (tab_poz)
						{
						case 0: tempSuffix = "\\t" + tempSuffix.substr(1, tempSuffix.length()); //if tabulator appear as 1 char of suffix eg: \ta
							break;
						case 1: tempSuffix = tempSuffix.substr(0, tab_poz) + "\\t" + tempSuffix.substr(tab_poz + 1, tempSuffix.length()); //if tabulator appear as 2 char of suffix eg: a\t
							break;
						case 2: tempSuffix = tempSuffix.substr(0, tab_poz) + "\\t" + tempSuffix.substr(tab_poz + 1, tempSuffix.length()); //if tabulator appear as 3 char of suffix eg: \t\t
							break;
						default:
							break;

						}
						offset = tab_poz +2;
					}

					cout << tempSuffix;
					NextLineCout = false;
				}//Display missing characters end
				//--------------------------------------------


				occur = line.find(TxTtoFind); //Looking for a match in current line

				//If match has been found
				if (occur != string::npos)
				{	
					//--------------------------------------------
					//--------------------------------------------
					//Set Preffix start here
					if (occur > 2)
					{
						preffix = line.substr(occur - 3, 3); //If there is enough characters in current line
					}
					else if (Prevline!="")
					{	
						switch(occur)
						{
							case 0:
								preffix = Prevline.substr(Prevline.length() - 2, 2)+"\\n"; //if match starts the line aa\n
								break;
							case 1:
								preffix = Prevline.substr(Prevline.length() - 1, 1) + "\\n"+ line.substr(0, 1); //If before match and line start there is 1 character eg a\na
								break;
							case 2:
								preffix = "\\n" + line.substr(0, 2);//If before match and line start there are 2 characters eg \naa
								break;
							default:
								break;
						}

					}

					offset = 0;
					//Looking for tabulator in preffix
					while (preffix.find("\t",offset) != string::npos)
					{
						size_t tab_poz = preffix.find("\t");
						switch (tab_poz)
						{
						case 0: preffix = "\\t" + preffix.substr(1, preffix.length()); //if tabulator appear as 1 char of prefix eg: \taa
							break;
						case 1: preffix = preffix.substr(0, tab_poz) + "\\t" + preffix.substr(tab_poz + 1, preffix.length());//if tabulator appear as 2 char of prefix eg: a\ta
							break;
						case 2: preffix = preffix.substr(0, tab_poz) + "\\t" + preffix.substr(tab_poz + 1, preffix.length());//if tabulator appear as 3 char of prefix aa\t
							break;
						case 3: preffix = preffix.substr(0, tab_poz) + "\\t" + preffix.substr(tab_poz + 1, preffix.length());//if tabulator appear as 3 char of prefix \ta\t
							break;
						case 4: preffix = preffix.substr(0, tab_poz) + "\\t" + preffix.substr(tab_poz + 1, preffix.length());//if tabulator appear as 3 char of prefix \t\t\t
							break;
						default:
							break;								
						}
						offset = tab_poz+2;
					}
					// Setting preffix end here
					//--------------------------------------------
					//--------------------------------------------

					//Set Suffix start here
					int CharAfterOccur = line.length() - (occur + TxTtoFind_Length); //Chceck how many characters there are after match

					if (CharAfterOccur > 2)
					{
						suffix = line.substr((occur + TxTtoFind_Length), 3); //If there are enough character after match
						NextLineToCout = 0;
					}
					else
					{
						switch(CharAfterOccur)
						{
						case 2:
							suffix = line.substr((occur + TxTtoFind_Length), 2)+"\\n"; //If there are 2 character after match
							break;
						case 1:
							suffix = line.substr((occur + TxTtoFind_Length), 1)+"\\n"; //If there are 1 character after match
							NextLineCout = true;
							NextLineToCout = 1;
							break;
						case 0:
							suffix = "\\n"; //If the match is in the end of the line
							NextLineCout = true;
							NextLineToCout = 2;
							break;
						default:
							break;
						}
					}

					offset = 0;
					//Looking for tabulator in suffix
					while ((suffix.find("\t",offset) != string::npos) && (NextLineToCout!=2))
					{	
						
						size_t tab_poz = suffix.find("\t", offset);

						switch (tab_poz)
						{
						case 0: suffix = "\\t" + suffix.substr(1, suffix.length()); //if tabulator appear as 1 char of suffix eg: \taa
							break;
						case 1: suffix = suffix.substr(0, tab_poz) + "\\t" + suffix.substr(tab_poz+1, suffix.length()); //if tabulator appear as 2 char of suffix eg: a\ta
							break;
						case 2: suffix = suffix.substr(0, tab_poz) + "\\t" + suffix.substr(tab_poz + 1, suffix.length()); //if tabulator appear as 3 char of suffix eg: aa\t or \t\ta
							break;
						case 3: suffix = suffix.substr(0, tab_poz) + "\\t"; //if tabulator appear as 4 char of suffix eg: \ta\t
							break;
						case 4: suffix = suffix.substr(0, tab_poz) + "\\t"; //if tabulator appear as 5 char of suffix eg: \t\t\t
							break;
						default:
							break;

						}
						offset = tab_poz+2;
					}//Set suffux end here
					//--------------------------------------------
					//--------------------------------------------


					//Final display
					if (NextLineCout == false)
					{
						cout << endl << this->name << "(" << charNum + occur << ")" << preffix << "..." << suffix; //If all preffix and suffix has been set 
					}
					else
					{
						cout<<endl << this->name << "(" << charNum + occur << ")" << preffix << "..." << suffix; //If suffix require haracters from next line
					}
					

					charNum += line.length(); //Caclulate total numbers of charaters
				}

				//If match not found
				else
				{
					charNum += line.length();
				}

				//Store previous line
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
//----------------------------------------------
//----------------End of File Class------------





int main()
{	
	Path InitPath;
	if (InitPath.InputText() == true)//If path is valid
	{
		int PathValid = InitPath.InPath(); //check what type of path has been inserted: 0 - txt file , 1 - directory 100 - path doesnt exists

		if (PathValid == 1)//if path leads to Directory
		{
			directory Dir1; //Initialize Directory object
			string CurrentPath = InitPath.RetPath();
			
			//Loop searching for all txt files
			do
			{
				Dir1.FindDir(CurrentPath); // searching directory in current path
				Dir1.FindTxtFiles(CurrentPath); //searching txt files in current path
				
				Dir1.DirQueue.pop();
				if (Dir1.DirQueue.empty() == false)
				{
					CurrentPath = Dir1.DirQueue.front();
				}


			} while (Dir1.DirQueue.empty()==false);

			//Loop to find match in files which path exists in Dir.FilesQueue
			int QueueSize = Dir1.FilesQueue.size();
			for (int i = 0; i < QueueSize; i++)
			{
				File f;
				f.FileName(Dir1.FilesQueue.front());
				f.ExplorFile(Dir1.FilesQueue.front(), InitPath.RetTxT());

				Dir1.FilesQueue.pop();
			}
		}
		else if (PathValid == 0)//if path leads to txt file
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
	cout << endl << endl;
	system("pause");
}

/*
------------------------
Multithread solution
------------------------

#include <thread>
#include <deque>


//Loop to explore all txt files from the front of the list
void ExploreLoopHead(int QueueSize,File f,directory Dir1, string txtFind)
{
	for (int i = 0; i < QueueSize; i++)
	{
		cout << f.FileName(Dir1.FilesDeQue.front()) << endl;;
		f.ExplorFile(Dir1.FilesDeQue.front(), txtFind);

		Dir1.FilesQueue.pop_front();

	}
}
//Loop to explore all txt files from the back of the list
void ExploreLoopBack(int QueueSize, File f, directory Dir1, string txtFind)
{
	for (int i = QueueSize; i > 0; i--)
	{
		cout << f.FileName(Dir1.FilesDeQue.back()) << endl;;
		f.ExplorFile(Dir1.FilesDeQue.back(), txtFind);

		Dir1.FilesQueue.pop_back();

	}


//IN MAIN:
int QueueSize = Dir1.FilesQueue.size();
if (QueueSize % 2 == 0)
{
	thread t1(ExploreLoopHead, QueueSize/2, f, Dir1, InitPath.RetTxT());
	thread t2(ExploreLoopBack, QueueSize/2, f, Dir1, InitPath.RetTxT());
	t1.join();
	t2.join();
}
else
{
	thread t1(ExploreLoopHead, ((QueueSize / 2)+1), f, Dir1, InitPath.RetTxT());
	thread t2(ExploreLoopBack, QueueSize / 2, f, Dir1, InitPath.RetTxT());
	t1.join();
	t2.join();
}
------------------------
		END
------------------------
*/



