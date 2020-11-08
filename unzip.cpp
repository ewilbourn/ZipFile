//Emily Wilbourn
//Project 3 - File Compression : Unzipped File Generator
//10/29/2020

#include <iostream>
#include <sstream> /*used to turn a string into a integer*/
#include <string>
#include <exception> /*used to override the exception class and write my own exception*/
#include <fstream> /*used for file input and output*/
#include <map>/*for mapping object*/
#include <algorithm>
#include <stdio.h> /*for removing the file we unzipped from our directoy*/
using namespace std;

bool isZipFile(string fileName);
string fillMap(string fileName, map <string, char> &m);
string decodeMessage(map<string, char> m, string message);
void fillUnzippedFile(string message, string fileName);

//overwriting the exception class to create my own exception
//found this idea from: https://www.tutorialspoint.com/cplusplus/cpp_exceptions_handling.htm
//and http://www.cplusplus.com/doc/tutorial/exceptions/
struct BadFilename : public exception 
{
   //this is defining what will be printed when the .what() method is called
   const char * what () const throw () 
   {
      return "Bad Filename entered on command line -- Now Aborting.";
   }
} myex;

int main(int argc, char *argv[])
{
	//receive the fileName via command line argument	
	string fileName = argv[1];
	map <string, char> myMap;
	try
	{
		//ensure that our file is a zip file
		bool isZip = isZipFile(fileName);

		//if the extension is not .zip, then throw an error and exit program
		if(!isZip)
			throw myex;
		string zipped_message = fillMap(fileName, myMap);						
		string unzipped_message = decodeMessage(myMap, zipped_message);
		
		fillUnzippedFile(unzipped_message, fileName);
	}

	//Catch a bad filetype exception
	catch(BadFilename &e)
	{
		cout << e.what() << endl;
	}
	return 0;
}

//method to determine if a given fileName is has .zip as the extension
//precondition: pass in a string
//postcondition: return a boolean; true is .zip is the file extension 
//(the file being the string argument being passed in), false if it has
//a different file extension
bool isZipFile(string f)
{
	//store the substring from the period to the end of the string 
	string fileExtension = f.substr(f.find("."),(f.length()-f.find(".")));

	//returns true if the extension is ".zip", false if it isn't	 
	return (fileExtension.compare(".zip") == 0 ? true : false);
}

//method to read in the file
//precondition: pass in the filename (string) and the map object we're filling, which is a mapping of a char 
//(the letter in the file determined by the ascii code) to a string (the string representation of the unique 
//code created when zipping the file)
//postcondition: return the string, which is at the end of the file (the zipped message) and
//at the end of the method, the mapping object will be filled
string fillMap(string fileName, map <string, char> &m)
{
	ifstream infile;
	infile.open(fileName);		
	
	int numUniqueChars;
	string line;

	string zipped_message = "";

	//counter to keep track of how many lines we have looked at in the file
	int counter = 0;

	//variable to hold the position we're mapping to in our mapping object
	int map_pos = 0;
	if(infile.is_open())
	{
		while( getline(infile, line) )
		{
			counter += 1;
			if(counter == 1)
			{
				string firstLine = line;
				stringstream s(firstLine);

				s >> numUniqueChars;
				continue;
			}
			//if statement to map items when the counter is <= numUniqueChars+1,
			//since we increment counter at the beginning of the while loop
			if(counter <= numUniqueChars+1)
			{
				string ascii = line.substr(0, line.find(" "));
				string code = line.substr(line.find(" ")+1, (line.length()-line.find(" ")));
				//turn the string representation of the ascii code to an integer		
				stringstream l (ascii);
				int ascii_code = 0;
				l >> ascii_code;

				//turn the ascii code to a letter to be added to our mapping object
				char character = char(ascii_code);		
				m[code] = character;	
			}
			//this fills up a string that has the zipped message	
			else
			{
				//loop to delete any new lines from the line we're reading in 	
				for (int i = 0; i < line.size(); i++)
				{
					if(line[i] == '\n')
						line.erase(remove(line.begin(), line.end(), line[i]), line.end());
				}
				zipped_message += line;			
			}	
		}
	}
	infile.close();
	return zipped_message;
}

//method to decode the zipped message being passed in
//precondition: pass in the map object and the encoded message (string)
//postcondition: return the final decoded string
string decodeMessage(map<string, char> m, string message)
{
	string final_string = "";
	string sub_str = "";
	map<string, char>::iterator j;
	for (int i = 0; i < message.size(); i++)
	{
		sub_str += message[i];
		for (j = m.begin(); j != m.end(); ++j)
		{
			if(j->first == sub_str)
			{
				final_string += j->second;
				sub_str = "";
				break;	
			}
		}
	}
	return final_string;
}

//method to fill a file with the unzipped message
//precondition: pass in the zipped message (string) and the name of the zipped file (string) respectively
//postcondition: return nothing, but the file we're reading out to is filled
void fillUnzippedFile(string message, string fileName)
{
	string new_name = fileName.substr(0, fileName.find("."));
	ofstream outfile;
	outfile.open(new_name);

	outfile << message;
	outfile.close();
	cout << "File Successfully Inflated Back To Original" << endl;
	int i = remove(fileName.c_str());
}

