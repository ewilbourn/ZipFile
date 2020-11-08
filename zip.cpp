//Emily Wilbourn
//zip file
//11/2/2020
#include <iostream> /*used for console input and output*/
#include <fstream> /*used for file input and output*/
#include <map>/*for mapping object*/
#include <vector> /*used for determining how many unique values we have in the file*/
#include <algorithm> /*used to sort a vector*/
#include "huffman.h" /*used to create the Huffman Tree*/
#include <cstring> /*used for comparing the arguments given on command line to a string*/
#include <stdio.h> /*used to rename the file to a zip file*/
using namespace std;
void PrintLetters (map<int, int> m);
void countLetters (int Letters[],vector <int> uniqueLetters, int LettersSize, map<int, int> &m);
string readFile(string fileName);
void convertStringtoIntArray(int i [], string s);
void  removeDuplicateValues(string s, vector<int> &letters);
void insertLetters(map<int, int> m, HuffmanTree &h);
void writeZipFile(map<int, int> m, string fileName, HuffmanTree h, string long_string);
void printHelpMenu();
double getCompressionRatio(int numBits, int numChars);
int main(int argc, char *argv[])
{
	string fileName;
	//if argc == 1, it means only one thing was given on cmd line (just the call
	//to the executable file); we want argc to be either 2 or 3
	if( argc > 3 || argc < 2)
	{
		cout << "ZIP: Invalid format" << endl;
		cout << "Try `myZip --help' for more information." << endl;
		return -1;
	}
	
	if(strcmp(argv[1], "--help") == 0)
	{
		printHelpMenu();
		//end program by exiting with error code
		return -1;
	}			
	//receive the fileName via command line argument
	//strcmp compares a string against the command line argument 
	//that was received, which is a cstring
	if(strcmp(argv[1], "--t") == 0)
        {
		fileName = argv[2];
	}
	else
	{
		fileName = argv[1];
	}
	//test to see if the fileName is a valid file
 	ifstream test(fileName);
	if (!test.is_open())
	{
		cout << "ZIP: file " << fileName << " does not exist." << endl;
		return -1;
	}
	
	//create a string that will store all the contents of the file
	string long_string = readFile(fileName);
	//create an array of integers that will hold the ascii representation
	//of all the characters in the file
	int intArray [long_string.size()];

	//put the ASCII value for each character in long_string into intArray
	convertStringtoIntArray(intArray, long_string);	

	//create a map to map the ASCII value of a character to its frequency in the file
	map <int, int> myMap;

	//create a vector that will store the ASCII values of the unique characters
	vector <int> uniqueChars;

	//create a vector of just the unique characters that are found within long_string
	removeDuplicateValues(long_string, uniqueChars);

	//get the frequency of each character within long_string and map it to the ASCII value
	countLetters(intArray, uniqueChars, long_string.size(), myMap);

	//print out the frequency of each unique character in the file
	//PrintLetters(myMap);	
	
	HuffmanTree h;
	insertLetters (myMap, h);
	h.build();
	if(strcmp(argv[1],"--t") == 0)	
	{
		h.PrintTable();	
	}
	writeZipFile(myMap, fileName, h, long_string);
	return 0;
}

//method to read in the file
//precondition: pass in the filename (string) 
//postcondition: return the string, which is the whole file stored in a string
string readFile(string fileName)
{
        ifstream file;
        file.open(fileName);

	char c;
        string line;

	//long string that will hold all the contents of the file
	string long_string;
        if(file.is_open())
        {
		//keep looping while the file has characters
                while(file.get(c) )
                {
			//concatenate the current character to a string
			long_string+=c;
		}
	}
	file.close();
	return long_string;
}

//method to convert a string to an array of integers (ascii values that represent characters)
//precondition: pass in an integer array that will hold the integer representation (ASCII values)
//for all the characters in the file as well as the string that contains all the characters from
//the file
//postcondition: returns nothing, but the integer array will be filled with ASCII values
void convertStringtoIntArray(int letters [], string long_string)
{
	for (int j = 0; j < long_string.size(); j++)
	{
		//make the index of j in the int i array equal to the integer 
		//representation of the character found in the string at index j
		//
		//aka, the ascii value for the character
		letters[j] = int(long_string[j]);		
	}
}

//method to fill a vector of integers with only the ASCII representation
//of the unique values in the file
//precondition: pass in the string that contains all the characters in the file and the 
//vector we are filling with unique characters in the file
//postcondition: return nothing, but the letters vector is filled with unique values
void  removeDuplicateValues(string s, vector<int> &letters)
{
	for (int i = 0; i < s.size(); i++)
	{
		//add the ASCII representation of a character to the vector
		letters.push_back(int(s[i]));
	}
	sort(letters.begin(), letters.end());
	letters.erase(unique(letters.begin(), letters.end()), letters.end());
}

//method to determine the frequency of characters in a file
//
//precondition: pass in the integer array of letters (which has the ASCII values for each char
//in the file), the vector of integers of unique letters (which has the ASCII values for the 
//unique characters in the file), the size of the letters array, and the mapping object (which
//maps ASCII values for each unique character to their frequencies within the file)
//
//postcondition: return nothing, but the mapping object is filled with the frequencies of each
//ASCII representation of unique characters within the file
void countLetters (int letters[], vector <int> uniqueLetters, int lettersSize, map<int, int> &m)
{
        char ch;
	int frequencies [uniqueLetters.size()];
	//instantiate all the values in the frequency array to 0
	for (int i = 0; i < uniqueLetters.size(); i++)
		frequencies[i] = 0;

	//iterate through the characters in the file and increment the 
	//value in the frequency array that corresponds to the unique letter
	//as a letter in letters[] is determined to be a unique letter (which, 
	//every letter will be)
	for (int i = 0; i < lettersSize; i++)
	{
		for(int j = 0; j < uniqueLetters.size(); j++)
		{	
			if(int(letters[i]) == uniqueLetters.at(j))
				frequencies[j] += 1;
		}
	}
	//now, map the frequencies of each unique letter to the ASCII representation 
	//of the unique letter	
	for (int i = 0; i < uniqueLetters.size(); i++)
	{
		m[uniqueLetters.at(i)] = frequencies[i];
	}
}

//prints out the letters and the frequency of each letter
//precondition: pass in the mapping object (which mapped ASCII values to frequencies)
//postcondition: return nothing, but print out the number of appearances of each character 
//within the file 
void PrintLetters (map<int, int> m)
{
	//mapping ascii character to frequency
	map<int, int>::iterator j;

	//iterate through our map and print out the characters and their respective frequencies	
	for (j = m.begin(); j != m.end(); ++j)
	{
		if((j->first) == 10)
			cout << "Char nl appearances: " << j->second << endl;
		else if((j->first) == 32)
			cout << "Char sp appearances: " << j->second << endl;
		else
			cout << "Char " << char(j->first) << " appearances: " << j->second << endl;
	}
}

//method to insert the letters from the file and their frequency into a Huffman Tree
//precondition: pass in the mapping object, which maps ASCII values to frequencies, and the Huffman tree object
//postcondition: return nothing, but the Huffman tree is filled
void insertLetters(map<int, int> m, HuffmanTree &h)
{
	//mapping ascii character to frequency
	map<int, int>::iterator j;

	//iterate through our map and print out the characters and their respective frequencies	
	for (j = m.begin(); j != m.end(); ++j)
	{
		h.insert(char(j->first), (j->second));
	}
}

//method to return the compression ratio for a zip file
//precondition: pass in the number of bits in the compressed message and 
//the number of characters in the input file
//postcondition: return the compression ratio (double)
double getCompressionRatio(int numBits, int numChars)
{
	int c = numChars*8;
	double d = (double)numBits/(double)c;
	return (1-d);
}

//method to print out how to use the program when a user uses the --help flag
//precondition: pass in nothing
//postcondition: return nothing, however the options will be printed to the screen
void printHelpMenu()
{
	cout << "Usage: ZIP [OPTION]... [FILE]..." << endl;
	cout << "Compress a text file using Huffman encoding.\n" << endl;
  	cout << "--t              Display the Huffman encoding table." << endl;
  	cout << "--help           Provide help on command.\n " << endl;
}

//method to load up the zip file
//precondition: mapping object (ascii value to frequency), fileName for file we're zipping, 
//HuffmanTree object, and long_string(which holds all the values in the input file)
//postcondition: return nothing, but fill the zipped file
void writeZipFile(map<int, int> m, string fileName, HuffmanTree h, string long_string)
{
	string new_name = fileName + ".zip";
        ofstream outfile;
        outfile.open(new_name);

	//integer to hold the total number of bits in the encoded message
	int totalZipBits = 0;
	
	//write out the number of unique letters in input to output file
        outfile << m.size() << "\n";
	
	map<int, int>:: iterator j;
	//iterate through the map of letters from the file and 
	//output their unique binary codes to the zip file
	for (j = m.begin(); j != m.end(); ++j)
	{
		string code = h.GetCode(char(j->first));
		outfile << j->first << " " << code << "\n";
	}
	//now, iterate through long_string (the original input) and
	//print out the encoded character tha corresponds with each
	//character in long_string
	for (int i = 0; i < long_string.size(); i++)
	{
		string code = h.GetCode(long_string[i]);
		outfile << code;
		//since GetCode returns a string, the number of bits in a code 
		//produced from this command is the size of the string
		totalZipBits += code.size();
	}	
        outfile.close();
 
        cout << "File Successfully Compressed To " << totalZipBits << " Bits (";
	//round to two decimal places
	printf("%.2f",  getCompressionRatio(totalZipBits, long_string.size())*100);
	cout << "% Less)." <<endl;	

	int k = remove(fileName.c_str());	
}


