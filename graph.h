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

  Graph() {}
  Graph(bool dirg) : dir(dirg) {}

  void insertNode(GV value, double x, double y)
  {
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
  void drawNode(GV node1, GV node2, float x1, float y1, int level)
  {
    if (node1 == NULL || nod2 == NULL)
    {
      return;
    }
    float segments = 25;
    float radius = 1.0;
    float left_angle = 245;
    float right_angle = 115;
    float branch_length = 12 - level * 2.5;
    float angle_change = 20;

    drawCircle(segments, radius, x1, y1);

    float angle = left_angle - level * angle_change;
    double radian = angle * 3.14 / 180;
    float m = (double)tan((double)radian);
    float x2 = x1 + branch_length * sin((double)radian);
    float y2 = y1 + branch_length * cos((double)radian);
    drawNode(node1, node2, x2, y2, level + 1);
    draw_line(x1, y1, x2, y2);

    float x3 = (x1 + x2) / 2;
    float y3 = (y1 + y2) / 2;
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
}

#endif
