#include "huffman.h"
#include <iostream>
#include <algorithm> /*used for sorting the vector of structs in build method*/
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
	for (int i = 0; i < numNodes(); i++)
	{
		if(nodes.at(i).ch == ch)
		{
			found = true;		
			break;
		}
	}	
	return found;
}

//method to return the frequency/weight of a character in a tree
//return -1 if the character is not in the tree
int HuffmanTree:: GetFrequency(char ch) 
{
	return (inTree(ch) ? GetFrequency(lookUp(ch)) : -1);
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
	if(inTree(ch))
	{
		for (int i = 0; i < numNodes(); i++)
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

	for (int i = 0;  i < numNodes();  i++)
	{	
		if(int(nodes.at(i).ch) == 10)
			cout << i << "\t" << "nl" << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t";
		else if(int(nodes.at(i).ch) == 32)
			cout << i << "\t" << "sp" << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t";
		else if(int(nodes.at(i).ch) == 0)
		{
			cout << i << "\t" << "T" << j << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t";
			j+=1;
		}
		else 
			cout << i << "\t" << nodes.at(i).ch << "\t" << nodes.at(i).weight << "\t" << nodes.at(i).parent << "\t";
		
		if (nodes.at(i).childType == -1)
			cout << "N/A" << endl;
		else
			cout << nodes.at(i).childType << endl;
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

	int originalNodesSize = numNodes();

	//fill the temp vector up with values in the nodes vector
	//so that we can work on merging nodes without modifying nodes
	for (int i = 0; i < numNodes(); i++)
		temp.push_back(nodes.at(i));
	while (temp.size() != 1)
	{
		//sort the temp vector by weight
		//found this method from: http://www.cplusplus.com/forum/beginner/62890/
		sort(temp.begin(), temp.end(), [] (const HNode &left, const HNode &right)
		{
			return (left.weight < right.weight);
		});
		
		//grab the first two elements in temp vector, which are the nodes we are merging
		HNode node1 = temp.at(0);
		HNode node2 = temp.at(1);

		//delete node1 and node2 from the temp vector	
		//do this by erasing the first element in vector twice
		temp.erase(temp.begin() + 0);
		temp.erase(temp.begin() + 0);
	
		//instantiate an HNode to represent our merged node
		HNode mergedNode;
		mergedNode.ch = '\0';		
		mergedNode.weight = node1.weight + node2.weight;
		
		//add mergedNode back to temp vector so that we can merge it with other nodes
		//add mergedNode to nodes vector 
		temp.push_back(mergedNode);
		nodes.push_back(mergedNode);
		
		//get the location of the two nodes being merged in the nodes vector
		int pos_node1 = lookUp(node1.ch);
		int pos_node2 = lookUp(node2.ch);

		//instantiate two booleans that will tell us if the position of node1 or
		//the position of node2 is already in the mergedIndexes vector
		//we start by assuming that it's not in the vector
		bool found_pos_node1 = false;
		bool found_pos_node2 = false;

		for (int i = 0; i < mergedIndexes.size(); i++)
		{
			if(mergedIndexes.at(i) == pos_node1)
				found_pos_node1 = true;
			if(mergedIndexes.at(i) == pos_node2)
				found_pos_node2 = true;
		}
		
		//Loop that increments pos_node1 when it is found in mergedIndexes.
		//The only reason an index would already be in the mergedIndexes vector
		//is when we have merged nodes (represented by '\0'). Since all nodes are
		//represented by the same character, and all mergedNodes will be found at
		//the end of the nodes vector, this is okay to do.
		while(found_pos_node1)
		{
			pos_node1 += 1;
			found_pos_node1 = false;
			
			for (int i = 0; i < mergedIndexes.size(); i++)
			{
				if(mergedIndexes.at(i) == pos_node1)
				{
					found_pos_node1 = true;
					break;
				}
			}
		}
		while(found_pos_node2)
		{
			pos_node2 += 1;
			found_pos_node2 = false;
			
			for (int i = 0; i < mergedIndexes.size(); i++)
			{
				if(mergedIndexes.at(i) == pos_node2)
				{
					found_pos_node2 = true;
					break;
				}
			}
		}
		
		//after doing all this, it's still possible for pos_node1 to equal pos_node2,
		//which we don't want. So if we encounter this, increment pos_node2 and then
		//be done.
		if (pos_node1 == pos_node2)
			pos_node2 += 1;
		
		//push back the indexes we merged to mergedIndexes
		mergedIndexes.push_back(pos_node1);
		mergedIndexes.push_back(pos_node2);

		//modify the childType for elements in nodes vector found at pos_node1 and pos_node2
		nodes.at(pos_node1).childType = 0;
		nodes.at(pos_node2).childType = 1;

		//Modify the parent for elements in nodes vector found at pos_node1 and pos_node2.
		//The parent for these nodes is the mergedNode we created.
		//The index of the mergedNode is the size of the nodes vector minus 1, since the 
		//mergedNode is the element most recently added to nodes.
		nodes.at(pos_node1).parent = numNodes()-1;
		nodes.at(pos_node2).parent = numNodes()-1;
	}
	//Make the parent of the last merged node 0 so that we know when to stop recursion
	//when getting the binary code for each letter in the tree.
	//Make the childType -1, since it doesn't have a childType.
	nodes.at(numNodes() -1).parent = 0;
	nodes.at(numNodes() -1).childType = -1;
}
