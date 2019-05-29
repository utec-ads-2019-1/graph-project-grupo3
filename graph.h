#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

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
        node *n = new node(value, x, y);
        nodes.push_back(n);
        dict[value] = n;
        count++;
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

    Graph *BFS(GV begining)
    {
        int count = 0;
        auto bfsGraph = new Graph(dir);
        for (ni = nodes.begin(); ni != nodes.end(); ni++)
        {
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
        }
     
         bool *frequented = new bool[count];

        for (int i = 0; i < count ; i++){
            frequented[i] = false;
        }
        queue<node *> container;
        auto currNode = getNode(begining);
        container.push(currNode);
        while (!container.empty())
        {
            currNode = container.front();
            container.pop();
            vector<node *> listAdjs = currNode->getNodesAdj();
            for (auto i =listAdjs.begin(); i != listAdjs.end(); ++i)
            {
                if (!frequented[(*i)->getData()-1])
                {
                    bfsGraph->insertEdge(NULL, currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData()-1]=true;
                }   
            }
            frequented[currNode->getData()-1] = true;
        }
        return bfsGraph;
    }
    
    Graph *DFS(GV begining)
    {
        int count = 0;
        auto bfsGraph = new Graph(dir);
        for (ni = nodes.begin(); ni != nodes.end(); ni++)
        {
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
        }
     
         bool *frequented = new bool[count];

        for (int i = 0; i < count ; i++){
            frequented[i] = false;
        }
        stack<node *> container;
        auto currNode = getNode(begining);
        container.push(currNode);
        while (!container.empty())
        {
            currNode = container.top();
            container.pop();
            vector<node *> listAdjs = currNode->getNodesAdj();
            for (auto i =listAdjs.begin(); i != listAdjs.end(); ++i)
            {
                if (!frequented[(*i)->getData()-1])
                {
                    bfsGraph->insertEdge(NULL, currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData()-1]=true;
                }   
            }
            frequented[currNode->getData()-1] = true;
        }
        return bfsGraph;
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

    node *getNode(GV value)
    {
        if (dict[value])
            return dict[value];
        return nullptr;
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
