#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

#include "node.h"
#include "edge.h"

using namespace std;

template <typename GV, typename GE>
class Graph
{
public:
  void readFile(string);
  typedef Node<GV> node;
  typedef Edge<GE, GV> edge;
  typedef vector<node *> NodeSeq;
  typedef list<edge *> EdgeSeq;
  typedef unordered_map<GV, node *> dictNode;
  typedef map<pair<GV, GV>, bool> dictEdges;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;
  int count = 0;
  Graph() {}
  Graph(bool dirg) : dir(dirg) {}

  void insertNode(GV value, double x, double y)
  {
    count++;
    node *n = new node(value, x, y);
    nodes.push_back(n);
    dict[value] = n;
  }
  void insertEdge(GE edgeV, GV node1, GV node2)
  {
    if (!dictE[make_pair(node1, node2)])
    {

      edge *e = new edge(edgeV, dict[node1], dict[node2]);
      ei = edges.begin();

      while (ei != edges.end() && (*ei)->getData() > edgeV)
        ei++;

      edges.insert(ei, e);
      dictE[make_pair(node1, node2)] = 1;

      node *n1 = dict[node1];
      node *n2 = dict[node2];
      n1->insertNodeAdj(n2);
      if (!dir)
      {
        dictE[make_pair(node2, node1)] = 1;
        n2->insertNodeAdj(n1);
      }
    }
  }

  Graph BFS(int begining)
  {

    bool *isVisited = new bool[count];
    for (int i = 0; i < count; i++)
    {
      isVisited[i] = false;
    }
    list<int> isVisitedQueue;
    isVisited[begining] = true;
    isVisitedQueue.push_back(begining);

    list<int>::iterator iter;
    while (!isVisitedQueue.empty())
    {
      begining = isVisitedQueue.front();
      return begining;
      isVisitedQueue.pop_front();

      for (iter = NodeSeq[begining].begin(); iter != NodeSeq[begining].end(); ++iter)
      {
        if (!isVisited[*iter])
        {
          isVisited[*iter] = true;
          isVisitedQueue.push_back(*iter);
        }
      }
    }
  }
  void print()
  {
    ni = nodes.begin();
    cout << "Imprimiendo nodes" << endl;
    while (ni != nodes.end())
    {
      cout << (*ni)->getData() << endl;
      ni++;
    }
    cout << "Imprimiendo edges" << endl;
    ei = edges.begin();
    while (ei != edges.end())
    {
      cout << (*ei)->getData() << " - Vertices :: ";
      node **arr = (*ei)->getNodes();
      cout << " v1::" << arr[0]->getData() << " / v2::" << arr[1]->getData() << endl;
      ei++;
    }
  }

private:
  NodeSeq nodes;
  EdgeSeq edges;
  dictNode dict;
  dictEdges dictE;
  NodeIte ni;
  EdgeIte ei;
  bool dir;
};

template <typename GV, typename GE>
void Graph<GV, GE>::readFile(string fileName)
{
  fstream f(fileName);
  string line;
  int rows, i, weight;
  GV valueNode1, valueNode2;
  GE valueEdge;
  double pos1, pos2;
  if (f.is_open())
  {

    getline(f, line);
    string::size_type cs;
    rows = stoi(line, &cs);

    for (i = 0; i < rows; i++)
    {
      getline(f, line);
      stringstream s(line);
      s >> valueNode1 >> pos1 >> pos2;
      insertNode(valueNode1, pos1, pos2);
    }
    getline(f, line);
    while (getline(f, line))
    {
      stringstream s(line);
      s >> valueNode1 >> valueNode2 >> valueEdge;
      insertEdge(valueEdge, valueNode1, valueNode2);
    }
  }
};

#endif
