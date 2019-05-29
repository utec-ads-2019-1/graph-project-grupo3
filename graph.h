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
#include <set>

#include "node.h"
#include "edge.h"
#include "dsjset.h"

using namespace std;
template <typename GV>
struct orderbyrank {
    bool operator() (Node<GV>* const &a, Node<GV>* const &b) {
        return a->rank > b->rank;
    }
};

template <typename GV,typename GE>
class Graph {
    public:
    //Falta actualizar el size del grafo
        void readFile(string);
        typedef Node<GV> node;
        typedef Edge<GE,GV> edge;
        typedef vector<node*> NodeSeq;
        typedef list<edge*> EdgeSeq;
        typedef set<node*> NodeSet;
        typedef unordered_map<GV,node*> dictNode;
        typedef map<pair<GV,GV>,bool> dictEdges;
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

        Graph(){}
        Graph(bool dirg):dir(dirg){}

        ~Graph(){
          while(edges.empty()){
            delete edges.back();
            edges.pop_back();
          }
          for(int i=0;i<size;i++)
            delete nodes[i];
        }

        void insertNode(GV value,double x,double y){ //agregar un valor de x e y aleatorio
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
            if (dir) return (edges.size()*2)/(nodes.size()*(nodes.size()-1));
            else return (edges.size())/(nodes.size()*(nodes.size()-1));
        }

        void density(){ //Controlar que esté en el rango de 0<a<1
            int cuota; cout << "Ingrese una cuota para evaluar la densidad: "; cin >> cuota;
            if (calcDensity(dir)<cuota) cout << "El grafo es Disperso"<<endl;
            else cout <<"El grafo es Denso" << endl;
        }

        void type(){ //? Hundido o Fuente
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

    Graph<GV,GV> kruskal() { //Pasar como grafo
            Graph krusky (false);
            EdgeSeq krusk;
            auto mySet = new DsjSet<GE>;
            int totalWeight = 0;

            for (ni = nodes.begin(); ni!=nodes.end(); ni++)
                mySet->makeSet((*ni)->getData(), *ni);

            edges.sort([](edge* first, edge* second){ //Ordenar de acuerdo a los pesos
                int wgtFirst = first->getData(); //Peso del primero
                int wgtSecond = second->getData();
                return wgtFirst < wgtSecond;
            });

            ei = edges.begin();
            while(ei!=edges.end()){
                auto first = (*ei)->nodes[0];
                auto second = (*ei)->nodes[1];
                if( mySet->findSet(first->getData()) != mySet->findSet(second->getData()) ) { //si no tienen los mismos padres
                    mySet->unionS(first->getData(), second->getData());
                    totalWeight += (*ei)->getData();
                    krusky.insertNode((*ei)->nodes[0]->getData(),0,0);
                    krusky.insertNode((*ei)->nodes[1]->getData(),0,0);
                    krusky.insertEdge((*ei)->getData(),(*ei)->nodes[0]->getData() ,(*ei)->nodes[1]->getData());
                    krusk.push_back(*ei);
                }
                ei++;

            }
                //cout << "Peso: "<< totalWeight<<endl;
                return krusky;
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
        NodeSet difference (NodeSet first, NodeSet second){
            NodeSet result;
            auto it = first.begin();
            while(it!=first.end()){
                if (second.find(*it) == second.end())
                    result.insert(*it);
                it++;
            }
            return result;
        }
/*

*/
        Graph<GV,GV> prim(GV etiqueta){

            auto start = dict[etiqueta];
            Graph result (false);
            NodeSeq resultado;
            NodeSet visited, visitedResult, totalNodes, remainded;
            visited.insert(start);
            //Insertar inicial en el grafo
            resultado.push_back(start);
            //Llenar el set de nodos
            for (ni = nodes.begin(); ni!=nodes.end(); ni++) //llenar los nodes
                totalNodes.insert(*ni);

            edges.sort([](edge* first, edge* second){ //Ordenar de acuerdo a los pesos
                int wgtFirst = first->getData();
                int wgtSecond = second->getData();
                return wgtFirst < wgtSecond;
            });

            while(visited != totalNodes) {
                remainded = difference(totalNodes, visited);
                int currentMin = 99999;
                int currentMin2 = 99999;
                auto node1 = new node;
                auto node2 = new node;
                auto node11 = new node;
                auto node22 = new node;

                ei = edges.begin();
                while (ei != edges.end()) {
                    auto v = visited.find((*ei)->nodes[0]);
                    auto r = remainded.find((*ei)->nodes[1]);
                    if ((v != visited.end()) && (r != remainded.end())) {
                        int min = (*ei)->getData();
                        if(min < currentMin) {
                            currentMin = min;
                            node1 = (*ei)->nodes[0];
                            node2 = (*ei)->nodes[1];
                        }
                    }
                    ei++;
                }

                ei = edges.begin();
                while (ei != edges.end()) {
                    auto v = visited.find((*ei)->nodes[1]);
                    auto r = remainded.find((*ei)->nodes[0]);
                    if ((v != visited.end()) && (r != remainded.end())) {
                       int min = (*ei)->getData();
                        if(min < currentMin2){
                            currentMin2 = min;
                            node11 = (*ei)->nodes[1];
                            node22 = (*ei)->nodes[0];
                        }
                    }ei++;
                }
                //Rellenar el grafo y controlar cuando hay varias opciones
                if (currentMin > currentMin2) {
                    auto v = visitedResult.find(node11); //Conntrolar no volver a insertar el mismo nodo
                    auto v2 = visitedResult.find(node22);
                    if(v2 ==visitedResult.end()) {
                        result.insertNode(node22->getData(), 0, 0);
                        visitedResult.insert(node22);
                    }if(v ==visitedResult.end()) {
                        result.insertNode(node11->getData(), 0, 0);
                        visitedResult.insert(node11);
                    }
                    result.insertEdge(currentMin2,node11->getData(), node22->getData());

                    resultado.push_back(node22);
                    visited.insert(node22);
                } else if (currentMin < currentMin2) {
                    auto v2 = visitedResult.find(node1); //Conntrolar no volver a insertar el mismo nodo
                    auto v = visitedResult.find(node2);
                    if(v ==visitedResult.end()) {
                        result.insertNode(node2->getData(), 0, 0);
                        visitedResult.insert(node2);
                    }
                    if(v2 ==visitedResult.end()) {
                        result.insertNode(node1->getData(), 0, 0);
                        visitedResult.insert(node1);
                    }



                    result.insertEdge(currentMin,node1->getData(), node2->getData());
                    resultado.push_back(node2);
                    visited.insert(node2);
                } else{
                    if(currentMin!=99999 && currentMin2!=99999){
                        auto v = visitedResult.find(node1); //Conntrolar no volver a insertar el mismo nodo
                        auto v2 = visitedResult.find(node2);
                        if(v2 ==visitedResult.end()) {
                            result.insertNode(node2->getData(), 0, 0);
                            visitedResult.insert(node2);
                        }if(v ==visitedResult.end()) {
                            result.insertNode(node1->getData(), 0, 0);
                            visitedResult.insert(node1);
                        }
                        result.insertEdge(currentMin,node1->getData(), node2->getData());

                        resultado.push_back(node2);
                        visited.insert(node2);
                    }

                }

            }

            return result;
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
