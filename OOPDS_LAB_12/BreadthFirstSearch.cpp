#include <iostream>
#include <iomanip>
#include <list>
#include <algorithm>

#include "Graph.hpp"
#include "BreadthFirstSearch.hpp"

using namespace std;

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VertexList;
typedef std::list<Graph::Vertex>::iterator VertexItor;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Edge>::iterator EdgeItor;

void BreadthFirstSearch::initialize()
{
	VertexList verts;
	graph.vertices(verts);
	for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv)
	{
		unvisit(*pv);
	}
	EdgeList edges;
	graph.edges(edges);
	for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe)
		unvisit(*pe);

	done = false;
}

enum BFS_PROCESS_STATUS { NOT_SELECTED, SELECTED };
void BreadthFirstSearch::bfsTraversal(Vertex & v, Vertex & target, VertexList & path, ofstream & fout)
{
	int** ppDistMtrx;
	int* pLeastCost;
	int num_nodes, num_selected;
	int* pPrev;
	int minID, minCost;
	BFS_PROCESS_STATUS* pBFS_Process_Stat;

	Vertex* pVrtxArray;
	Vertex vrtx, *pPrevVrtx;
	Edge e;
	int start_vrtxid, target_vrtxid, curVrtx_ID, vrtxID;
	EdgeList* pAdjLstArray;

	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	start_vrtxid = start.getID();
	target_vrtxid = target.getID();

	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	pLeastCost = new int[num_nodes];
	pPrev = new int[num_nodes];
	pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes];

	// initialize L(n) = w(start, n);
	for (int i = 0; i < num_nodes; i++)
	{
		pLeastCost[i] = ppDistMtrx[start_vrtxid][i];
		pPrev[i] = start_vrtxid;
		pBFS_Process_Stat[i] = NOT_SELECTED;
	}

	pBFS_Process_Stat[start_vrtxid] = SELECTED;
	num_selected = 1;

	int round = 0;
	fout << "\nLeast Cost from start at each round : " << endl;
	fout << "   |";
	for (int i = 0; i < num_nodes; i++)
	{
		fout << setw(5) << i;
	} fout << endl;
	fout << "-----------+";
	for (int i = 0; i < num_nodes; i++)
	{
		fout << setw(5) << "-----";
	} fout << endl;

	while (num_selected < num_nodes)
	{
		round++;
		fout << setw(2) << round << " |";	//print for debugging
		cout << "=== round " << round << " ==== " << endl;

		// find a node with LeastCost from the NOT_SELECTED vertex pool
		minID = -1;
		minCost = PLUS_INF;
		for (int i = 0; i < num_nodes; i++) {
			if ((pLeastCost[i] < minCost) && (pBFS_Process_Stat[i] != SELECTED)) {
				minID = i;
				minCost = pLeastCost[i];
			}
		}
		if (minID == -1) {
			cout << "Error in FindShortestPath() -";
			cout << "target is not connected to the start !!" << endl;
			break;
		}
		else {
			pBFS_Process_Stat[minID] = SELECTED;
			num_selected++;
			if (minID == target_vrtxid) // target reached
			{
				cout << "reached to the target node !!" << endl;
				fout << " reached to the target node !!" << endl;	//print for debugging
				cout << "Least Cost = " << minCost << endl;
				vrtxID = minID;
				do {
					vrtx = pVrtxArray[vrtxID];
					path.push_front(vrtx);
					vrtxID = pPrev[vrtxID];
				} while (vrtxID != start_vrtxid);
				vrtx = pVrtxArray[vrtxID];
				path.push_front(vrtx); // start node
				break;
			}
		}

		int pLS, ppDistMtrx_i;
		for (int i = 0; i < num_nodes; i++)
		{
			pLS = pLeastCost[i];
			ppDistMtrx_i = ppDistMtrx[minID][i];
			if ((pBFS_Process_Stat[i] != SELECTED) && (pLeastCost[i] > (pLeastCost[minID] + ppDistMtrx[minID][i])))
			{ // update distances with relaxation
				pPrev[i] = minID;
				pLeastCost[i] = pLeastCost[minID] + ppDistMtrx[minID][i];
			}
		}

		// print out the pLeastCost[] for debugging
		for (int i = 0; i < num_nodes; i++)
		{
			int vrtx_id = pVrtxArray[i].getID();
			int cost = pLeastCost[i];
			if (cost == PLUS_INF)
				fout << "  +oo";
			else
				fout << setw(5) << pLeastCost[i];
		}
		fout << endl;
	} // end while()
}

//marking utility *****************************************************************************************************

void BreadthFirstSearch::visit(Vertex & v)
{
	Graph::Vertex* pVtx;
	int numNodes = getGraph().getNumVertices();
	int vtx_ID = v.getID();
	if (vtx_ID >= 0 && vtx_ID < numNodes) {
		//pVrtxStatus[vtx_ID] = VISITED;
		v.setVtxStatus(VISITED);
	}
	else {
		cout << "Vertex (" << v << ") ID is out-of-range (";
		cout << numNodes << ")" << endl;
	}
}

void BreadthFirstSearch::visit(Edge & e)
{
	Vertex vtx_1, vtx_2;
	int vtx_1_ID, vtx_2_ID;
	int numNodes = getGraph().getNumVertices();
	vtx_1 = *e.getpVrtx_1();
	vtx_1_ID = vtx_1.getID();
	vtx_2 = *e.getpVrtx_2();
	vtx_2_ID = vtx_2.getID();
	if ((vtx_1_ID >= 0 && vtx_1_ID < numNodes) && (vtx_2_ID >= 0 && vtx_2_ID < numNodes)) {
		//ppEdgeStatus[vtx_1_ID][vtx_2_ID] = EDGE_VISITED;
		e.setEdgeStatus(EDGE_VISITED);
	}
	else {
		cout << "Vertex IDs (" << vtx_1_ID << ", " << vtx_2_ID;
		cout << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

void BreadthFirstSearch::unvisit(Vertex & v)
{
	Graph::Vertex* pVtx;
	int numNodes = getGraph().getNumVertices();
	int vtx_ID = v.getID();
	if (vtx_ID >= 0 && vtx_ID < numNodes) {
		//pVrtxStatus[vtx_ID] = UNEXPLORED;
		v.setVtxStatus(UN_VISITED);
	}
	else {
		cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")";
		cout << endl;
	}
}

void BreadthFirstSearch::unvisit(Edge & e)
{
	Vertex vtx_1, vtx_2;
	int vtx_1_ID, vtx_2_ID;
	int numNodes = getGraph().getNumVertices();

	vtx_1 = *e.getpVrtx_1();
	vtx_1_ID = vtx_1.getID();
	vtx_2 = *e.getpVrtx_2();
	vtx_2_ID = vtx_2.getID();

	if ((vtx_1_ID >= 0 && vtx_1_ID < numNodes) && (vtx_2_ID >= 0 && vtx_2_ID < numNodes)) {
		//ppEdgeStatus[vtx_1_ID][vtx_2_ID] = EDGE_UNEXPLORED;
		e.setEdgeStatus(EDGE_UN_VISITED);
	}
	else {
		cout << "Vertex IDs (" << vtx_1_ID << ", " << vtx_2_ID << ") of Edge (";
		cout << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

bool BreadthFirstSearch::isVisited(Vertex & v)
{
	Graph::Vertex* pVtx;
	int numNodes = getGraph().getNumVertices();
	int vtx_ID = v.getID();
	if (vtx_ID >= 0 && vtx_ID < numNodes)
	{
		//return (pVrtxStatus[vtx_ID] == VISITED);
		return (v.getVtxStatus() == VISITED);
	}
	else {
		cout << "Vertex (" << v << ") ID is out-of-range (";
		cout << numNodes << ")" << endl;
		return false;
	}
}

bool BreadthFirstSearch::isVisited(Edge & e)
{
	Vertex vtx_1, vtx_2;
	int vtx_1_ID, vtx_2_ID;
	EdgeStatus eStat;
	int numNodes = getGraph().getNumVertices();

	vtx_1 = *e.getpVrtx_1();
	vtx_1_ID = vtx_1.getID();
	vtx_2 = *e.getpVrtx_2();
	vtx_2_ID = vtx_2.getID();

	if ((vtx_1_ID >= 0 && vtx_1_ID < numNodes) && (vtx_2_ID >= 0 && vtx_2_ID < numNodes)) {
		//eStat = ppEdgeStatus[vtx_1_ID][vtx_2_ID];
		eStat = e.getEdgeStatus();
		if ((eStat == EDGE_VISITED) || (eStat == DISCOVERY) || (eStat == BACK))
			return true;
		else
			return false;
	}
	else {
		cout << "Vertex IDs (" << vtx_1_ID << ", " << vtx_2_ID << ") of Edge (";
		cout << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
	return false;
}

void BreadthFirstSearch::setEdgeStatus(Edge & e, EdgeStatus es)
{
	Vertex vtx_1, vtx_2;
	int vtx_1_ID, vtx_2_ID;
	int numNodes = getGraph().getNumVertices();

	vtx_1 = *e.getpVrtx_1();
	vtx_1_ID = vtx_1.getID();
	vtx_2 = *e.getpVrtx_2();
	vtx_2_ID = vtx_2.getID();

	if ((vtx_1_ID >= 0 && vtx_1_ID < numNodes) && (vtx_2_ID >= 0 && vtx_2_ID < numNodes)) {
		//ppEdgeStatus[vtx_1_ID][vtx_2_ID] = es;
		e.setEdgeStatus(es);
	}
	else {
		cout << "Vertex IDs (" << vtx_1_ID << ", " << vtx_2_ID << ") of Edge (";
		cout << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

EdgeStatus BreadthFirstSearch::getEdgeStatus(Edge & e)
{
	Vertex vtx_1, vtx_2;
	int vtx_1_ID, vtx_2_ID;
	int numNodes = getGraph().getNumVertices();
	EdgeStatus eStat;

	vtx_1 = *e.getpVrtx_1();
	vtx_1_ID = vtx_1.getID();
	vtx_2 = *e.getpVrtx_2();
	vtx_2_ID = vtx_2.getID();

	if ((vtx_1_ID >= 0 && vtx_1_ID < numNodes) && (vtx_2_ID >= 0 && vtx_2_ID < numNodes)) {
		//eStat = ppEdgeStatus[vtx_1_ID][vtx_2_ID];
		eStat = e.getEdgeStatus();
		return eStat;
	}
	else {
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
		return EDGE_NOT_FOUND;
	}
}

void BreadthFirstSearch::initDistMtrx()
{
	int** ppDistMtrx;
	int* pLeaseCostMtrx;
	int num_nodes;
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int curVrtx_ID, vrtxID;

	num_nodes = graph.getNumVertices();
	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();

	ppDistMtrx = getppDistMtrx();
	for (int i = 0; i < num_nodes; i++) {
		curVrtx_ID = pVrtxArray[i].getID();
		EdgeItor pe = pAdjLstArray[curVrtx_ID].begin();
		while (pe != pAdjLstArray[curVrtx_ID].end()) {
			vrtxID = (*(*pe).getpVrtx_2()).getID();
			ppDistMtrx[curVrtx_ID][vrtxID] = (*pe).getDistance();
			pe++;
		}
		ppDistMtrx[curVrtx_ID][curVrtx_ID] = 0;
	}
}

void BreadthFirstSearch::printDistMtrx(ofstream & fout)
{
	int** ppDistMtrx;
	int num_nodes;
	int dist;

	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	cout << "   |";
	for (int i = 0; i < num_nodes; i++) {
		cout << setw(5) << (char)(i + 'A');
	}
	cout << endl;

	cout << "----+";
	for (int i = 0; i < num_nodes; i++) {
		cout << "-----";
	}
	cout << endl;

	for (int i = 0; i < num_nodes; i++) {
		cout << setw(2) << (char)(i + 'A') << " |";
		for (int j = 0; j < num_nodes; j++) {
			dist = ppDistMtrx[i][j];
			if (dist == PLUS_INF)
				cout << "  +oo";
			else
				cout << setw(5) << dist;
		}
		cout << endl;
	}
	cout << endl;
}

void BreadthFirstSearch::findShortestPath(Vertex & s, Vertex & t, VertexList & path, ofstream & fout)
{
	initialize();
	path.clear();
	start = s;
	initDistMtrx();
	//printDistMtrx();
	bfsTraversal(start, t, path, fout);
}

void BreadthFirstSearch::findShortestPaths(Vertex & s, ofstream & fout)
{
	int** ppDistMtrx;
	int* pLeastCost;
	int num_nodes, num_selected;
	int* pPrev;
	int minID, minCost;
	BFS_PROCESS_STATUS* pBFS_Process_Stat;

	Vertex* pVrtxArray;
	Vertex vrtx, *pPrevVrtx;
	Edge e;
	int start_vrtxid, target_vrtxid, curVrtx_ID, vrtxID;
	EdgeList* pAdjLstArray;

	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	start_vrtxid = s.getID();

	num_nodes = graph.getNumVertices();
	ppDistMtrx = getppDistMtrx();

	pLeastCost = new int[num_nodes];
	pPrev = new int[num_nodes];
	pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes];

	VertexList *path = new VertexList[num_nodes];

	for (int i = 0; i < num_nodes; i++)
	{
		// initialize L(n) = w(start, n);
		for (int j = 0; j < num_nodes; j++)
		{
			pLeastCost[j] = ppDistMtrx[start_vrtxid][j];
			pPrev[j] = start_vrtxid;
			pBFS_Process_Stat[j] = NOT_SELECTED;
		}
		num_selected = 0;

		int round = 0;
		target_vrtxid = graph.getpVrtxArray()[i].getID();

		while (num_selected < num_nodes)
		{
			round++;
			// find a node with LeastCost from the NOT_SELECTED vertex pool
			minID = -1;
			minCost = PLUS_INF;
			for (int j = 0; j < num_nodes; j++) {
				if ((pLeastCost[j] < minCost) && (pBFS_Process_Stat[j] != SELECTED)) {
					minID = j;
					minCost = pLeastCost[j];
				}
			}
			if (minID == -1) {
				cout << "Error in FindShortestPath() -";
				cout << "target is not connected to the start !!" << endl;
				break;
			}
			else {
				pBFS_Process_Stat[minID] = SELECTED;
				num_selected++;
				if (minID == target_vrtxid) // target reached
				{
					cout << "path from " << setw(2) <<start_vrtxid << " to " << setw(2) << target_vrtxid << ", Least Cost = " << minCost << endl;
					vrtxID = minID;
					while (vrtxID != start_vrtxid)
					{
						vrtx = pVrtxArray[vrtxID];
						path[i].push_front(vrtx);
						vrtxID = pPrev[vrtxID];
					} 
					vrtx = pVrtxArray[vrtxID];
					path[i].push_front(vrtx); // start node
					break;
				}
			}

			int pLS, ppDistMtrx_i;
			for (int j = 0; j < num_nodes; j++)
			{
				pLS = pLeastCost[j];
				ppDistMtrx_i = ppDistMtrx[minID][j];
				if ((pBFS_Process_Stat[j] != SELECTED) && (pLeastCost[j] >(pLeastCost[minID] + ppDistMtrx[minID][j])))
				{ // update distances with relaxation
					pPrev[j] = minID;
					pLeastCost[j] = pLeastCost[minID] + ppDistMtrx[minID][j];
				}
			}
		} // end while()
	}

	for (int i = 0; i < num_nodes; i++)
	{
		VertexItor vItor = path[i].begin();
		fout << "shortest path from " << setw(2) << start_vrtxid << " to " << setw(2) << i << " : ";
		//if (vItor == path[i].end())
		//	fout << setw(2) << i << endl;
		while (vItor != path[i].end())
		{
			//fout << *vItor << " ";
			fout << setw(2) << vItor._Ptr->_Myval.getID();
			if (++vItor != path[i].end())
				fout << " -> ";
			else
				fout << endl;
		}
	}
	delete[] path;
	path = NULL;
}