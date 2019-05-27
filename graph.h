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
#include <queue>

#include "node.h"
#include "edge.h"
#include "dsjset.h"

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
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

        Graph(){}
        Graph(bool dirg):dir(dirg){}

        ~Graph(){ //Revisar destructor y read file
          /*while(edges.empty()){
            //delete edges.back();
            edges.pop_back();
          }*/
          for(int i=0;i<size;i++)
            delete nodes[i];
        }

        void insertNode(GV value,double x,double y){
          node *n=new node(value,x,y);
          nodes.push_back(n);
          dict[value]=n;
        }
        void insertEdge(GE edgeV,GV node1,GV node2){
          if(!dictE[make_pair(node1,node2)]){

            edge *e=new edge(edgeV,dict[node1],dict[node2]);
            ei=edges.begin();

            while(ei!=edges.end() && (*ei)->getData()>edgeV)
             ei++;

            edges.insert(ei,e);
            dictE[make_pair(node1,node2)]=1;

            node* n1=dict[node1];
            node* n2=dict[node2];
            n1->insertNodeAdj(n2);
            if(!dir){
              dictE[make_pair(node2,node1)]=1;
              n2->insertNodeAdj(n1);
            }

          }
        }
        void print(){
          ni=nodes.begin();
          cout<<"Imprimiendo nodes:\n"<<endl;
          while(ni!=nodes.end()){
              cout<<(*ni)->getData()<<"\t";
            ni++;

          }
          cout << endl;
          cout<<"\nImprimiendo edges:\n"<<endl;
          ei=edges.begin();
          while(ei!=edges.end()){
            node** arr=(*ei)->getNodes();
            cout << "Vertices( " << arr[0]->getData()<<", "<<arr[1]->getData() << ")";
            cout << "\t|\tPeso: "<< (*ei)->getData() << endl;
            ei++;
          }
        }

        float calcDensity(bool dir){
            if (dir)
                return (edges.size()*2)/(nodes.size()*(nodes.size()-1));
            else
                return (edges.size())/(nodes.size()*(nodes.size()-1));
        }

        void density(float cuota){
            if (calcDensity(dir)<cuota) cout << "Grafo Disperso"<<endl;
            else cout <<"Grafo Denso" << endl;
        }

        void type(){
            if (dir) cout << "Grafo dirigido" << endl;
            else cout << "Grafo no dirigido" << endl;

        }


        int degre(){}
            //todo
        bool isConexo(){
            //todo
        }
        bool isBipartite(){
            //todo
        }

        EdgeSeq kruskal() {
            EdgeSeq krusk;
            auto mySet = new DsjSet<GE>;
            for (ni = nodes.begin(); ni!=nodes.end(); ni++){
                auto name = (*ni)->getData();
                mySet->makeSet(name, *ni);
            }

                edges.sort([](edge* first, edge* second){
                    int wgtFirst = first->getData();
                    int wgtSecond = second->getData();
                    return wgtFirst < wgtSecond;
                });


                int tWeight = 0, i=0;
                ei = edges.begin();

                for (int i = 0; i < edges.size(); i++){
                    auto first = (*ei)->nodes[0];
                    auto second = (*ei)->nodes[1];
                    auto weight = (*ei)->getData();

                    if( mySet->findSet(first->getData()) != mySet->findSet(second->getData()) ) {
                        mySet->unionS(first->getData(), second->getData());
                        tWeight += weight;
                        krusk.push_back(*ei);
                    }
                    ei++;
                }
                return krusk;

        }
        void printKruskal(){
            auto result = kruskal();
            cout<<"\nImprimiendo Kruskal:\n"<<endl;
            ei=result.begin();
            while(ei!=result.end()){
                node** arr=(*ei)->getNodes();
                cout<<"Vértices( "<<arr[0]->getData()<<", "<<arr[1]->getData()<<")"<<endl;
                ei++;
            }

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
};

template <typename GV,typename GE>
void Graph<GV,GE>::readFile(string fileName){
  fstream f(fileName);
  string line;
  int i;
  GV valueNode1,valueNode2;
  GE valueEdge;
  double pos1,pos2;
  if(f.is_open()){

    getline(f,line);
    string::size_type cs;
    size=stoi(line,&cs);

    for(i=0;i<size;i++){
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
  }
}

#endif
