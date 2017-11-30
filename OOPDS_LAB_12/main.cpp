/** main.cpp */
#include <iostream>
#include <string>
#include "Graph.hpp"
#include "BreadthFirstSearch.hpp"

#define NUM_NODES 25
#define NUM_EDGES 50

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VtxList;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Vertex>::iterator VtxItor;
typedef std::list<Graph::Edge>::iterator EdgeItor;

void main()
{
	VtxList path;

	Vertex v[NUM_NODES] = {
		Vertex("V00",  0, UN_VISITED),
		Vertex("V01",  1, UN_VISITED),
		Vertex("V02",  2, UN_VISITED),
		Vertex("V03",  3, UN_VISITED),
		Vertex("V04",  4, UN_VISITED),
		Vertex("V05",  5, UN_VISITED),
		Vertex("V06",  6, UN_VISITED),
		Vertex("V07",  7, UN_VISITED),
		Vertex("V08",  8, UN_VISITED),
		Vertex("V09",  9, UN_VISITED),
		Vertex("V10", 10, UN_VISITED),
		Vertex("V11", 11, UN_VISITED),
		Vertex("V12", 12, UN_VISITED),
		Vertex("V13", 13, UN_VISITED),
		Vertex("V14", 14, UN_VISITED),
		Vertex("V15", 15, UN_VISITED),
		Vertex("V16", 16, UN_VISITED),
		Vertex("V17", 17, UN_VISITED),
		Vertex("V18", 18, UN_VISITED),
		Vertex("V19", 19, UN_VISITED),
		Vertex("V20", 20, UN_VISITED),
		Vertex("V21", 21, UN_VISITED),
		Vertex("V22", 22, UN_VISITED),
		Vertex("V23", 23, UN_VISITED),
		Vertex("V24", 24, UN_VISITED),
	};
	Graph::Edge edges[NUM_EDGES] =
	{
		Edge(v[ 0], v[ 1], 1), Edge(v[ 1], v[ 0], 1),
		Edge(v[ 0], v[ 5], 1), Edge(v[ 5], v[ 0], 1),
		Edge(v[ 1], v[ 2], 1), Edge(v[ 2], v[ 1], 1),
		Edge(v[ 1], v[ 6], 1), Edge(v[ 6], v[ 1], 1),
		Edge(v[ 3], v[ 8], 1), Edge(v[ 8], v[ 3], 1),
		Edge(v[ 4], v[ 9], 1), Edge(v[ 9], v[ 4], 1),
		Edge(v[ 6], v[ 7], 1), Edge(v[ 7], v[ 6], 1),
		Edge(v[ 6], v[11], 1), Edge(v[11], v[ 6], 1),
		Edge(v[ 7], v[ 8], 1), Edge(v[ 8], v[ 7], 1),
		Edge(v[ 8], v[13], 1), Edge(v[13], v[ 8], 1),
		Edge(v[ 9], v[14], 1), Edge(v[14], v[ 9], 1),
		Edge(v[10], v[11], 1), Edge(v[11], v[10], 1),
		Edge(v[10], v[15], 1), Edge(v[15], v[10], 1),
		Edge(v[12], v[17], 1), Edge(v[17], v[12], 1),
		Edge(v[13], v[14], 1), Edge(v[14], v[13], 1),
		Edge(v[14], v[19], 1), Edge(v[19], v[14], 1),
		Edge(v[15], v[16], 1), Edge(v[16], v[15], 1),
		Edge(v[15], v[20], 1), Edge(v[20], v[15], 1),
		Edge(v[16], v[21], 1), Edge(v[21], v[16], 1),
		Edge(v[17], v[18], 1), Edge(v[18], v[17], 1),
		Edge(v[17], v[22], 1), Edge(v[22], v[17], 1),
		Edge(v[18], v[19], 1), Edge(v[19], v[18], 1),
		Edge(v[18], v[23], 1), Edge(v[23], v[18], 1),
		Edge(v[21], v[22], 1), Edge(v[22], v[21], 1),
		Edge(v[23], v[24], 1), Edge(v[24], v[23], 1),
	};
	int test_start = 0;
	int test_end = 24;
	Graph simpleGraph(NUM_NODES);

	ofstream fout;	//define ofstream fout
	fout.open("output.txt");

	if (fout.fail())
	{
		cout << "Fail to open output.txt file !!" << endl;
		exit(1);
	}
	fout << "Inserting vertices .." << endl;
	for (int i = 0; i<NUM_NODES; i++) {
		simpleGraph.insertVertex(v[i]);
	}
	VtxList vtxLst;
	simpleGraph.vertices(vtxLst);
	int count = 0;
	fout << "Inserted vertices: ";
	for (VtxItor vItor = vtxLst.begin(); vItor != vtxLst.end(); ++vItor) {
		fout << *vItor << ", ";
	}
	fout << endl;
	fout << "Inserting edges .." << endl;
	for (int i = 0; i<NUM_EDGES; i++)
	{
		simpleGraph.insertEdge(edges[i]);
	}
	fout << "Inserted edges: " << endl;
	count = 0;
	EdgeList egLst;
	simpleGraph.edges(egLst);
	for (EdgeItor p = egLst.begin(); p != egLst.end(); ++p)
	{
		count++;
		fout << *p << ", ";
		if (count % 5 == 0)
			fout << endl;
	}
	fout << endl;
	fout << "Print out Graph based on Adjacency List .." << endl;
	simpleGraph.printGraph(fout);
	BreadthFirstSearch bfsGraph(simpleGraph);
	cout << "\nTesting Breadth First Search" << endl;
	bfsGraph.initDistMtrx();
	bfsGraph.printDistMtrx(fout);
	bfsGraph.findShortestPath(v[test_start], v[test_end], path, fout);
	fout << "Path found by BFS (shortest) from " << v[test_start] << " to " << v[test_end] << " : ";
	for (VtxItor vItor = path.begin(); vItor != path.end(); ++vItor)
		fout << *vItor << " ";

	fout << endl;
	cout << "\nFinding paths with smallest number of edges from node " << test_start << " " << endl;
	fout << "\nFinding paths with smallest number of edges from node " << test_start << " " << endl;
	bfsGraph.findShortestPaths(v[test_start], fout);

	cout << "\nFinding paths with smallest number of edges from node " << test_end << " " << endl;
	fout << "\nFinding paths with smallest number of edges from node " << test_end << " " << endl;
	bfsGraph.findShortestPaths(v[test_end], fout);
	fout.close();
}