/*
Evan Joseph
3/21/2017, CIS 350, Program 3
DESCRIPTION: This program deals with spanning trees and even flow in pipes. Enough of the pipes will be upgraded to make sure each junction is connected. 
The input consists of multiple data sets, with each input item in a dataset being non-negative. The first line of input has the number of junctions followed 
by a space and then the number of edges. 0 0 can be typed on the first line to signify that you don't want to add any more data sets. Using n for the number 
of junctions and m for the number of edges, n>=2 && 0<= m <= (n(n-1))/2. The following lines of input will be the first junction, a space, the 2nd junction 
that is connected to the 1st (which both cannot be great than n, the number of junctions), a space, and finally the edge weight (pipe capacity, which is a 
positive int less than or equal to 10000). The lines will continue like this until you have input enough edges which was specified in the first line of input. 
If the graph has a complete set of pipes that connect all the junctions, the output will be the set with the smallest range of capacities (max-min) among 
them. If not, -1 will be printed.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Edge
{
	int edgeWeight;
	int vertex1;
	int vertex2;

	/*
	post: sets the edgeWeight and the two vertices that are connected by the edge
	*/
	Edge(int v1, int v2, int weight)
	{
		vertex1 = v1;
		vertex2 = v2;
		edgeWeight = weight;
	}

	/*
	post: the < operator is overloaded. when comparing two edges, it will compare them by their edgeWeight. this is useful for the sort function
	*/
	bool operator < (Edge myEdge)
	{
		return (edgeWeight < myEdge.edgeWeight);
	}
};

class EvenFlow
{
private:
	int numberOfJunctions = 0;
	int numberOfEdges = 0;
	int min = -1;
	int max = -1;
	int range = -1;
	bool isSpanning = false;


public:
	vector<int> pVector;
	vector<Edge> edgeVector;	//a vector of Edge structs which contain the edge weight and junctions connected
	vector<Edge> currentMST;	//the MST that we're currently dealing with
	
	/*
	post: sets the numberOfEdges and numberOfJunctions to the arguments passed in, which is the first line of user input from each data set from the main function
	*/
	EvenFlow(int junctions, int edges)
	{
		numberOfEdges = edges;
		numberOfJunctions = junctions;
	}

	/*
	post: clears the parent vector and pushes back -1 for each junction. this sets up the vector for find/unite calls
	*/
	void resetVector()
	{
		pVector.clear();
		for (int i = 0; i < numberOfJunctions; i++)
		{
			pVector.push_back(-1);
		}
	}

	/*
	post: updates the min and max edge weight in the current MST and the range. min = myMin; max = myMax; range = max - min;
	*/
	void updateMinMax(int myMin, int myMax)
	{
		min = myMin;
		max = myMax;
		range = max - min;
	}

	/*
	post: resets the minimum and maximum edgeWeights, along with the range. the range is set to -1 because that's what should be output if there is no MST found
	*/
	void setMinMaxRange()
	{
		min = edgeVector[0].edgeWeight;	//min is the first edge we use
		max = min;	//max starts out as min and then is updated as we go through all edges
		range = -1;
	}

	/*
	post: recursively calls find until it finds a junction with no parents (negative number in pVector). the set that x is in is that
	junction without parents, and x is returned so we know what set it's in
	*/
	int find(int x)
	{
		if (pVector[x - 1] > 0)	//[x-1] because junction 1 is at index 0, j2 is at index 1, etc.
		{
			return find(pVector[x - 1]);
		}
		return x;
	}

	/*
	post: calls a find on both arguments passed in. using those numbers as the index for the pVector, we can find out how to unite them based on
	whichever one has more children. the junction with more children stays as the parent and the other one points to that junction.
	*/
	void unite(int x, int y)
	{
		int a = find(x);
		int b = find(y);
		if (pVector[a - 1] < pVector[b - 1])		//unite to the one with the most children
		{
			pVector[a - 1] += pVector[b - 1];
			pVector[b - 1] = a;
		}
		else
		{
			pVector[b - 1] += pVector[a - 1];
			pVector[a - 1] = b;
		}
	}

	/*
	pre: edgeVector.empty() != true
	post: returns the smallest range of the many possible MSTs. this algorithm continuously calls finds and nuites until an MST is obtained.
	if (range < 0 || localMax - localMin < range) updateMinMax(localMin, localMax); is how the range is updated.
	*/
	int kruskal()
	{
		while (!edgeVector.empty() && currentMST.size() <= edgeVector.size())
		{
			int localMin = edgeVector[0].edgeWeight;
			int localMax = localMin;
			resetVector();
			currentMST.clear();

			for (int i = 0; i < edgeVector.size(); i++)
			{
				Edge currentEdge = edgeVector[i];
				if (find(currentEdge.vertex1) != find(currentEdge.vertex2))	//if they're not in the same set
				{
					currentMST.push_back(currentEdge);		//add the currentEdge to the currentMST
					unite(currentEdge.vertex1, currentEdge.vertex2);	//unite the two vertices to the same set
					if (currentEdge.edgeWeight > localMax)
					{
						localMax = currentEdge.edgeWeight;	//update the max weight if we add another edge to the MST
					}
				}
			}

			if (range < 0 || localMax - localMin < range)
			{
				updateMinMax(localMin, localMax);
			}

			edgeVector.erase(edgeVector.begin());	//delete the smallest edge and start over again
		}
		return range;
	}

};

int main()
{
	int numberOfJunctions = 0, numberOfEdges = 0;
	cin >> numberOfJunctions >> numberOfEdges;

	vector<int> finalAnswers;

	EvenFlow * myEvenflow = new EvenFlow(numberOfJunctions, numberOfEdges);
	int tempWeight = 0, tempV1 = 0, tempV2 = 0;
	while(!(numberOfEdges == 0 && numberOfJunctions == 0))	//0 0 is for when we don't want to input any more sets of data
	{
		myEvenflow->edgeVector.clear();
		myEvenflow->resetVector();	//resets the vector to all -1
		for (int i = 0; i < numberOfEdges; i++)
		{
			//do		//do-while continues to ask for input until valid input is given
			//{
				cin >> tempV1 >> tempV2 >> tempWeight;
			//} while (tempWeight > 10000 || tempWeight <= 0 || tempV1 > numberOfJunctions || tempV2 > numberOfJunctions);

			myEvenflow->edgeVector.push_back(Edge(tempV1, tempV2, tempWeight));		//add a new edge to the edgeVector
		}
		if (numberOfEdges != 0)
		{
			sort(myEvenflow->edgeVector.begin(), myEvenflow->edgeVector.end());		//sort the edgeVector by their weights
			myEvenflow->setMinMaxRange();
		}
		finalAnswers.push_back(myEvenflow->kruskal());
		
		cin >> numberOfJunctions >> numberOfEdges;		//asks for more input and will continue the while loop if 0 0 is not input
	}

	for each(int answer in finalAnswers)	//prints all the final answers
	{
		cout << answer << endl;
	}
	//system("pause");
	return 0;
}