#ifndef GRAPH_H
#define GRAPH_H

#include<vector>
#include<list>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>
#include<map>
#include<queue>
#include<stack>

#include "node.h"
#include "edge.h"

using namespace std;

template <typename GV,typename GE>
class Graph {
    public:
        void readFile(string);
        typedef Node<GV> node;
        typedef Edge<GE,GV> edge;
        typedef vector<node*> NodeSeq;
        typedef list<edge*> EdgeSeq;
        typedef unordered_map<GV,node*> dictNode;
        typedef map<pair<GV,GV>,bool> dictEdges;
        typedef map<pair<GV,GV>,GE> matrixAdj;
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

        Graph(bool dirg,bool pondg):dir(dirg),pond(pondg){size=0;}
        ~Graph(){
          while(edges.empty()){
            delete edges.back();
            edges.pop_back();
          }
          /*
          for(int i=0;i<size;i++){
            cout<<nodes[i]<<endl;
            delete nodes[i];
          }*/

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
            dict[value]=n;
            size++;
            return true;
          }
          return false;
        }
        bool removeNode(GV value){
          ni=nodes.begin();
          while(ni!=nodes.end()){
            if(ni->data==value){
              nodes.erase(ni,ni+1);
              delete dict[value];
              dict[value]=nullptr;
              size--;
              return true;
            }
            ni++;
          }
          return false;
        }
        bool removeNode(GV value){
          ni=nodes.begin();
          while(ni!=nodes.end()){
            if(ni->data==value){
              nodes.erase(ni,ni+1);
              delete dict[value];
              dict[value]=nullptr;
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


            edges.push_back(e);
            dictE[make_pair(node1, node2)] = 1;

            node* n1=dict[node1];
            node* n2=dict[node2];
            mAdj[make_pair(node1,node2)]=1;
            n1->insertNodeAdj(n2);
            if(!dir){
              dictE[make_pair(node2,node1)]=1;
              mAdj[make_pair(node2,node1)]=1;
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

            edge *e=new edge(dict[node1],dict[node2]);
            ei=edges.begin();

            while (ei != edges.end() && (*ei)->getData() > edgeV)
                ei++;

            edges.insert(ei, e);
            dictE[make_pair(node1, node2)] = 1;

            mAdj[make_pair(node1,node2)]=edgeV;
            node* n1=dict[node1];
            node* n2=dict[node2];
            n1->insertNodeAdj(n2);
            if(!dir){
              dictE[make_pair(node2,node1)]=1;
              n2->insertNodeAdj(n1);
              mAdj[make_pair(node2,node1)]=edgeV;
            }
            return true;
          }
          return false;
        }
        bool removeEdge(GV node1,GV node2){
          ei=edges.begin();
          if(!dictE[make_pair(node1,node2)]|| (!dir && dictE[make_pair(node2,node1)]))
              return false;
          while(ei!=edges.end()){
            if(ei->nodes[0]->getData()==node1 && ei->nodes[1]->getData()==node2){

              edge* edgetmp=*ei;
              node* n1=ei->nodes[0];
              node* n2=ei->nodes[1];
              n1->removeNodeAdj(n2);

              if(!dir){
                n2->removeNodeAdj(n1);
                dictE[make_pair(node2,node1)]=false;
              }

              edges.remove(*ei);
              delete edgetmp;
              dictE[make_pair(node1,node2)]=false;
              return true;
            }
            if(!dir && ei->nodes[1]->getData()==node1 && ei->nodes[0]->getData()==node2){
              edge* edgetmp=*ei;
              node* n1=ei->nodes[1];
              node* n2=ei->nodes[0];
              n1->removeNodeAdj(n2);
              n2->removeNodeAdj(n1);
              edges.remove(*ei);
              delete edgetmp;
              dictE[make_pair(node1,node2)]=false;
              dictE[make_pair(node2,node1)]=false;
              return true;
            }
            ei++;
          }
          return false;
        }
        node* findNode(GV node1){
          return dict[node1];
        }
        edge* findEdge(GV node1,GV node2){
            return dictE[make_pair(node1,node2)];
        }

        Graph *BFS(GV begining){
          int count = 0;
          auto bfsGraph = new Graph(dir);
          for (ni = nodes.begin(); ni != nodes.end(); ni++)
          {
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
          }

          bool *frequented = new bool[count];

          for (int i = 0; i < count; i++)
            frequented[i] = false;

          queue<node*> container;
          auto currNode = getNode(begining);
          container.push(currNode);
          while (!container.empty()){
            currNode = container.front();
            container.pop();
            vector<node*> listAdjs = currNode->getNodesAdj();
            ei = edges.begin();
            for (auto i = listAdjs.begin(); i != listAdjs.end() && ei != edges.end(); ++i, ++ei){

                if (!frequented[(*i)->getData() - 1]){
                    bfsGraph->insertEdge((*ei)->getData(), currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData() - 1] = true;
                }
            }
            frequented[currNode->getData() - 1] = true;
          }
          return bfsGraph;
        }


        Graph *DFS(GV begining){
          int count = 0;
          auto bfsGraph = new Graph(dir);
          for (ni = nodes.begin(); ni != nodes.end(); ni++){
            bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            count++;
          }

          bool *frequented = new bool[count];

          for (int i = 0; i < count; i++)
            frequented[i] = false;

          stack<node*> container;
          auto currNode = getNode(begining);
          container.push(currNode);
          while (!container.empty()){
            currNode = container.top();
            container.pop();
            vector<node *> listAdjs = currNode->getNodesAdj();
            ei = edges.begin();
            for (auto i = listAdjs.begin(); i != listAdjs.end() && ei != edges.end(); ++i){
                if (!frequented[(*i)->getData() - 1]){
                    bfsGraph->insertEdge((*ei)->getData(), currNode->getData(), (*i)->getData());
                    container.push(*i);
                    frequented[(*i)->getData() - 1] = true;
                }
            }
            frequented[currNode->getData() - 1] = true;
          }
        return bfsGraph;
      }

        void fillOrder(GV n, unordered_map<GV,bool> &visited, stack<GV> &Stack){
          visited[n] = true;
          NodeSeq n1=dict[n]->getNodesAdj();

          NodeIte ni2=n1.begin();
          for(int i=0; i<n1.size(); ++ni,i++){
            if(!visited[(*ni2)->getData()])
              fillOrder((*ni2)->getData(), visited, Stack);
          }
          Stack.push(n);
        }
        Graph getTranspose(){
          Graph graphTrans(dir,pond);
          int sizeAdj=nodes.size();
          for(int i=0;i<sizeAdj;i++){
            GV v1=nodes[i]->getData();
            graphTrans.insertNode(nodes[i]->getData(),nodes[i]->getX(),nodes[i]->getY());
            NodeSeq nodeadj=nodes[i]->getNodesAdj();
            for(ni=nodeadj.begin();ni!=nodeadj.end();ni++){
              graphTrans.insertNode((*ni)->getData(),(*ni)->getX(),(*ni)->getY());
              if(pond)
                graphTrans.insertEdge(mAdj[make_pair(v1,(*ni)->getData())],(*ni)->getData(),v1);
              else
                graphTrans.insertEdge((*ni)->getData(),v1);
            }
          }
          return graphTrans;
        }

        void dfsUseFull(node* v,unordered_map<GV,bool>& visited){
          visited[v->getData()] = true;
          NodeSeq nodesadj=v->getNodesAdj();
          NodeIte ni2;
          for (ni2 = nodesadj.begin(); ni2 != nodesadj.end(); ++ni2)
            if (!visited[(*ni2)->getData()])
              dfsUseFull(*ni2, visited);
        }
        bool stronglyConnectedComponent(){
          if(!dir)
            throw printf("Esta propiedad solo funciona con grafos dirigidos");
          int sizeVNode=nodes.size();
          unordered_map<GV,bool> visited;
          stack<GV> s;

          Graph gr=getTranspose();
          for(int i=0;i<sizeVNode;i++){
            if(!visited[nodes[i]->getData()])
              fillOrder(nodes[i]->getData(),visited,s);
          }

          for(int i=0;i<sizeVNode;i++)
            visited[nodes[i]->getData()]=0;

          bool faro=false;
          while(!s.empty()){
            GV ntmp=s.top();
            s.pop();
            if(!visited[ntmp])
              if(faro)
               return false;
              gr.dfsUseFull(dict[ntmp],visited);
              faro=true;
            }
            return true;
          }

        //Coloreamos cada nodo con rojo(r) o azul(a) para saber si es bipartito, por defecto todos son blancos(b)
        bool bipartiteGraph(){

          unordered_map<node*,char> dictColour;

          for(auto x:nodes)
            dictColour[x]='b';

          queue<node*> s;
          s.push(nodes[0]);
          dictColour[nodes[0]]='r';
          while(!s.empty()){
            node* tmp=s.front();
            s.pop();
            NodeSeq tmpSeq=tmp->getNodesAdj();
            NodeIte ni2=tmpSeq.begin();
            for(;ni2!=tmpSeq.end();ni2++){
              if(dictColour[*ni2]=='b'){
                if(dictColour[tmp]=='r')
                  dictColour[*ni2]='a';
                else
                  dictColour[*ni2]='r';
                s.push(*ni2);
              }
              else if(dictColour[tmp]==dictColour[*ni2])
                return false;
            }
          }

          return true;
        }

        void print(){
          ni=nodes.begin();
          cout<<"Imprimiendo nodes"<<endl;
          while(ni!=nodes.end()){
            cout<<(*ni)->getData()<<" count:"<<(*ni)->getCountNodesAdj()<<endl;
            NodeSeq nodestmp=(*ni)->getNodesAdj();
            NodeIte ni2=nodestmp.begin();
            for(;ni2!=nodestmp.end();ni2++){
              cout<<(*ni2)->getData()<<" ";
            }
            cout<<endl;
            cout<<"-----------------------------"<<endl;
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

        node* getNode(GV value){
          if(dict[value])
            return dict[value];
          return nullptr;
        }

    private:
        NodeSeq nodes;
        EdgeSeq edges;
        dictNode dict;
        dictEdges dictE;
        matrixAdj mAdj;
        int size;
        NodeIte ni;
        EdgeIte ei;
        bool dir;
        bool pond;
};

template <typename GV,typename GE>
void Graph<GV,GE>::readFile(string fileName){
  fstream f(fileName);
  string line;
  int i,sizetmp;
  GV valueNode1,valueNode2;
  GE valueEdge;
  double pos1,pos2;
  if(f.is_open()){

    getline(f,line);
    string::size_type cs;
    sizetmp=stoi(line,&cs);

    for(i=0;i<sizetmp;i++){
      getline(f,line);
      stringstream s(line);
      s>>valueNode1>>pos1>>pos2;
      insertNode(valueNode1,pos1,pos2);
    }
    getline(f,line);
    while(getline(f,line)){
      stringstream s(line);
      s>>valueNode1>>valueNode2>>valueEdge;
      insertEdge(valueEdge,valueNode1,valueNode2);
    }
};

#endif
