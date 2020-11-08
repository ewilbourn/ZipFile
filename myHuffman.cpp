#include "huffman.h"
#include <iostream>
using namespace std;

HuffmanTree:: HuffmanTree()
	: numChars(0), built(false) {}

void HuffmanTree:: insert(char ch, int weight) 
{
    HNode newNode = {ch, weight, -1, -1}; 
    built = false;
    nodes.push_back(newNode);
    numChars++;
}


//returns a boolean value indicating whether a given character is stored in the tree
bool HuffmanTree:: inTree(char ch) 
{
	bool found = false;
	for (int i = 0; i < numNodes; i++)
	{
		if(nodes.at(i).ch == ch)
		{
			found = true;		
			break;
		}
	}	
	return found;
}

int HuffmanTree:: GetFrequency(char ch) 
{
	return GetFrequency(lookUp(ch));
}

//returns the weight at a specific index
int HuffmanTree:: GetFrequency(int i) 
{
	return nodes.at(i).weight;
}

//returns the index of a given character in the tree
//returns -1 if it's not in the tree
int HuffmanTree:: lookUp(char ch) 
{
	int index;
	if(inTree(ch)
	{
		for (int i = 0; i < numNodes; i++)
		{
			if(nodes.at(i).ch == ch)
			{
				index = i;
				break;
			}
		}	
	}
	else	
		index = -1;
	return index;
		

}

//calls a method with the index of wanted character
//returns the binary representation of the given character
string HuffmanTree:: GetCode(char ch) 
{
	return GetCode(lookUp(ch));
}

//returns the binary representation of given character
string HuffmanTree:: GetCode(int i) 
{
	if (nodes[i].parent == 0)
		return "";
	else
		return (GetCode(nodes[i].parent) + (char)(nodes[i].childType+'0'));
}

void HuffmanTree:: PrintTable() 
{
	//t nodes iterator
	int j = 1;
	cout <<"       ++++  ENCODING TABLE  ++++" << endl;
	cout << "Index   Char    Weight  Parent  ChildType" << endl;

	for (int i = 0;  i < numNodes;  i++)
	{	
	  if(int(nodes.at(i).ch) == 10)
		cout << i << "\t" << "nl" << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t" << nodes.at(i).childType << endl;
	  if(int(nodes.at(i).ch) == 32)
		cout << i << "\t" << "sp" << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t" << nodes.at(i).childType << endl;
	  if(int(nodes.at(i).ch == 0)
	  {
		cout << i << "\t" << "T" << j << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t" << nodes.at(i).childType << endl;
		j+=1;
	  }
	  else 
		cout << i << "\t" << nodes.at(i).ch << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t" << nodes.at(i).childType << endl;
		
	}

}

int HuffmanTree:: numNodes() 
{
	return nodes.size();
}

void HuffmanTree:: build() 
{
	vector <HNode> temp;
	vector <int> mergedIndexes;

	int originalNodesSize = numNodes;

	//fill the temp vector up with values in the nodes vector
	//so that we can work on merging nodes without modifying nodes
	for (int i = 0; i < numNodes; i++)
		temp.push_back(nodes.at(i));
	while		
}
