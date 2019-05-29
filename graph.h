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

    Graph(bool dirg, bool pondg) : dir(dirg), pond(pondg) {}
    ~Graph()
    {
        while (edges.empty())
        {
            delete edges.back();
            edges.pop_back();
        }
        for (int i = 0; i < size; i++)
            delete nodes[i];
    }

    bool insertNode(GV value, double x, double y)
    {
        if (!dict[value])
        {
            node *n = new node(value, x, y);
            nodes.push_back(n);
            dict[value] = n;
            return true;
        }
        return false;
    }
    bool removeNode(GV value)
    {
        ni = nodes.begin();
        while (ni != nodes.end())
        {
            if (ni->data == value)
            {
                nodes.erase(ni, ni + 1);
                delete dict[value];
                dict[value] = nullptr;
                return true;
            }
            ni++;
        }
        return false;
    }
    bool insertEdge(GV node1, GV node2)
    {
        if (pond)
            throw printf("Debe ingresar un peso para la arista\n");

        if (!dictE[make_pair(node1, node2)])
        {

            edge *e = new edge(dict[node1], dict[node2]);

            edges.push_back(e);
            dictE[make_pair(node1, node2)] = 1;

            node *n1 = dict[node1];
            node *n2 = dict[node2];
            n1->insertNodeAdj(n2);
            if (!dir)
            {
                dictE[make_pair(node2, node1)] = 1;
                n2->insertNodeAdj(n1);
            }
            return true;
        }
        return false;
    }
    bool insertEdge(GE edgeV, GV node1, GV node2)
    {
        if (!pond)
            throw printf("No debe ingresar un peso para la arista\n");
        if (!dictE[make_pair(node1, node2)])
        {

            edge *e = new edge(dict[node1], dict[node2]);
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
            return true;
        }
        return false;
    }

    Graph *BFS(GV begining)
    {
        int count = 0;
        auto bfsGraph = new Graph(dir, pond);
        for (ni = nodes.begin(); ni != nodes.end(); ni++)
        {
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
        }

        bool *frequented = new bool[count];

        for (int i = 0; i < count; i++)
        {
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
            ei = edges.begin();
            for (auto i = listAdjs.begin(); i != listAdjs.end() && ei != edges.end(); ++i, ++ei)
            {

                if (!frequented[(*i)->getData() - 1])
                {
                    bfsGraph->insertEdge((*ei)->getData(), currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData() - 1] = true;
                }
            }
            frequented[currNode->getData() - 1] = true;
        }
        return bfsGraph;
    }
    bool removeEdge(GV node1, GV node2)
    {
        ei = edges.begin();
        if (!dictE[make_pair(node1, node2)] || (!dir && dictE[make_pair(node2, node1)]))
            return false;
        while (ei != edges.end())
        {
            if (ei->nodes[0]->getData() == node1 && ei->nodes[1]->getData() == node2)
            {

                edge *edgetmp = *ei;
                node *n1 = ei->nodes[0];
                node *n2 = ei->nodes[1];
                n1->removeNodeAdj(n2);

                if (!dir)
                {
                    n2->removeNodeAdj(n1);
                    dictE[make_pair(node2, node1)] = false;
                }

                edges.remove(*ei);
                delete edgetmp;
                dictE[make_pair(node1, node2)] = false;
                return true;
            }
            if (!dir && ei->nodes[1]->getData() == node1 && ei->nodes[0]->getData() == node2)
            {
                edge *edgetmp = *ei;
                node *n1 = ei->nodes[1];
                node *n2 = ei->nodes[0];
                n1->removeNodeAdj(n2);
                n2->removeNodeAdj(n1);
                edges.remove(*ei);
                delete edgetmp;
                dictE[make_pair(node1, node2)] = false;
                dictE[make_pair(node2, node1)] = false;
                return true;
            }
            ei++;
        }
        return false;
    }
    bool findNode(GV node1)
    {
        return dict[node1];
    }
    bool findEdge(GV node1, GV node2)
    {
        return dictE[make_pair(node1, node2)];
    }

    Graph *DFS(GV begining)
    {
        int count = 0;
        auto bfsGraph = new Graph(dir, pond);
        for (ni = nodes.begin(); ni != nodes.end(); ni++)
        {
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
        }

        bool *frequented = new bool[count];

        for (int i = 0; i < count; i++)
        {
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
            ei = edges.begin();
            for (auto i = listAdjs.begin(); i != listAdjs.end() && ei != edges.end(); ++i)
            {
                if (!frequented[(*i)->getData() - 1])
                {
                    bfsGraph->insertEdge((*ei)->getData(), currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData() - 1] = true;
                }
            }
            frequented[currNode->getData() - 1] = true;
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
    int size;
    NodeIte ni;
    EdgeIte ei;
    bool dir;
    bool pond;
};

template <typename GV, typename GE>
void Graph<GV, GE>::readFile(string fileName)
{
    fstream f(fileName);
    string line;
    int i;
    GV valueNode1, valueNode2;
    GE valueEdge;
    double pos1, pos2;
    if (f.is_open())
    {

        getline(f, line);
        string::size_type cs;
        size = stoi(line, &cs);

        for (i = 0; i < size; i++)
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
