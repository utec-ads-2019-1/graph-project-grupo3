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
#include <set>
#include<queue>
#include <stdexcept>
#include<stack>
#include <limits>
#include "node.h"
#include "edge.h"
#include "dsjset.h"
#define max_int 214748364
using namespace std;
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
        typedef map<pair<GV,GV>,edge*> dictGetEdge;
        typedef map<pair<GV,GV>,bool> dictEdges;
        typedef map<pair<GV,GV>,GE> matrixAdj;
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

        Graph(bool dirg,bool pondg):dir(dirg),pond(pondg){size=0;}

        ~Graph(){
          while(edges.empty()){
            //delete edges.back();
            edges.pop_back();
          }
          /*
          for(int i=0;i<size;i++){
            cout<<nodes[i]<<endl;
            delete nodes[i];
          }*/

        }

        bool insertNode(GV value,double x,double y){
          if(!dict[value]){
            node *n=new node(value,x,y);
            nodes.push_back(n);
            dict[value]=n;
            size++;
            return true;
          }
          return false;
        }
        bool removeNode(GV value){

          if(!dict[value])
            throw printf("Nodo no existe en el grafo\n");

          ni=nodes.begin();
          while(ni!=nodes.end()){
            if((*ni)->getData()==value){
              nodes.erase(ni,ni+1);

              for(auto x:nodes)
                removeEdge(value,x->getData());

              delete dict[value];
              dict[value]=nullptr;
              size--;
              return true;
            }
            ni++;
          }
          return false;
        }

        bool insertEdge(GV node1,GV node2){
          if(pond)
            throw printf("Debe ingresar un peso para la arista\n");

          if(!dictE[make_pair(node1,node2)]){

            edge *e=new edge(dict[node1],dict[node2]);

            edges.push_back(e);
            dictE[make_pair(node1,node2)]=1;

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
        bool insertEdge(GE edgeV,GV node1,GV node2){
          if(!pond)
            throw printf("No debe ingresar un peso para la arista\n");

          if(!dictE[make_pair(node1,node2)]){

            edge *e=new edge(edgeV,dict[node1],dict[node2]);
            ei=edges.begin();

            while(ei!=edges.end() && (*ei)->getData()>edgeV)
             ei++;

            edges.insert(ei,e);
            dictE[make_pair(node1,node2)]=1;

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
          if(!dictE[make_pair(node1,node2)] || (!dir && !dictE[make_pair(node2,node1)]))
            throw printf("Arista no existe en el grafo\n");

          while(ei!=edges.end()){
            if((*ei)->nodes[0]->getData()==node1 && (*ei)->nodes[1]->getData()==node2){

              edge* edgetmp=(*ei);
              node* n1=(*ei)->nodes[0];
              node* n2=(*ei)->nodes[1];
              n1->removeNodeAdj(n2);

              if(!dir){
                n2->removeNodeAdj(n1);
                dictE[make_pair(node2,node1)]=false;
              }
              edges.erase(ei);
              delete edgetmp;
              dictE[make_pair(node1,node2)]=false;
              return true;
            }
            if(!dir && (*ei)->nodes[1]->getData()==node1 && (*ei)->nodes[0]->getData()==node2){
              edge* edgetmp=(*ei);
              node* n1=(*ei)->nodes[1];
              node* n2=(*ei)->nodes[0];
              n1->removeNodeAdj(n2);
              n2->removeNodeAdj(n1);
              edges.erase(ei);
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
        auto hasNode = dict[begining];
        if (hasNode)
        {
            auto bfsGraph = new Graph(dir, pond);
            for (ni = nodes.begin(); ni != nodes.end(); ni++)
            {
                bfsGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
                count++;
            }

            bool *frequented = new bool[count];

            for (int i = 0; i < count; i++)
                frequented[i] = false;

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
                  node **arr = (*ei)->getNodes();
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
        else
            throw out_of_range("No se enceuntra el nodo");
    }


    Graph *DFS(GV begining){
        int count = 0;
        auto hasNode = dict[begining];
        if(hasNode){
            auto bfsGraph = new Graph(dir,pond);
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
        throw out_of_range("No se enceuntra el nodo");
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

          NodeSeq nodestmp;
          if(dir){
            for(node* x:nodes){
              if(degreInNode(x)==0)
                nodestmp.push_back(x);
            }
          }
          else
            nodestmp=nodes;

          int sizetmp=nodestmp.size();

          for(int i=0;i<sizetmp;i++){
            if(dictColour[nodestmp[i]]=='b'){
              queue<node*> s;
              s.push(nodestmp[i]);
              dictColour[nodestmp[i]]='r';
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
             }
          }
          for(int i=0;i<size;i++){
            if(dictColour[nodes[i]]=='b'){
              queue<node*> s;
              s.push(nodes[i]);
              dictColour[nodes[i]]='r';
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
            }
          }
          return true;
        }

        bool dfsUseFull2(node* v,node* dest,unordered_map<GV,bool>& visited){
          visited[v->getData()] = true;
          NodeSeq nodesadj=v->getNodesAdj();
          NodeIte ni2;
          for (ni2 = nodesadj.begin(); ni2 != nodesadj.end(); ++ni2){
            if((*ni2)==dest)
              return true;
            if (!visited[(*ni2)->getData()])
               if(dfsUseFull2(*ni2,dest,visited))
                 return true;
          }
          return false;
        }
        bool conexo(){
          for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
              if(i!=j){
                unordered_map<GV,bool> visited;
                for(int k=0;k<size;k++)
                  visited[nodes[k]->getData()]=0;
                if(!dfsUseFull2(nodes[i],nodes[j],visited))
                    return false;
              }
            }
          }
          return true;
        }

        void print(){
          ni=nodes.begin();
          cout <<"==========================";
          cout<<"\n   Imprimiendo Nodes:"<<endl;
          cout <<"==========================\n";

          while(ni!=nodes.end()){
            cout<<"Vértice: "<<(*ni)->getData();
            cout<<" | DegIn: "<<degreInNode(*ni)<<" DegOut: "<<degreOutNode(*ni);
            NodeSeq nodestmp=(*ni)->getNodesAdj();
            NodeIte ni2=nodestmp.begin();
            cout<<" | Tipo: ";type(*ni);
            cout <<" | Vertices Adj: ";
            for(;ni2!=nodestmp.end();ni2++){
              cout<<(*ni2)->getData()<<" ";
            }
            cout<<endl;
            cout<<"-----------------------------"<<endl;
            ni++;

          }
          cout << endl;
            cout <<"==========================";
            cout<<"\n   Imprimiendo Edges:"<<endl;
            cout <<"==========================\n";
          ei=edges.begin();
          while(ei!=edges.end()){
            node** arr=(*ei)->getNodes();
            cout << "Vertices( " << arr[0]->getData()<<", "<<arr[1]->getData() << ")";
            cout << "\t|\tPeso: "<<(*ei)->getData() << endl;
            ei++;
          }
        }

        double calcDensity(bool dir){
            double resultado=0;
            if (!dir) resultado = double((edges.size()*2))/double(nodes.size()*(nodes.size()-1));
            else resultado = double(edges.size())/double(nodes.size()*(nodes.size()-1));
            return resultado;
        }

        void density(){ //Controlar que esté en el rango de 0<a<1
            double cuota;
            do{
            cout << "\nIngrese una cuota para evaluar la densidad: "; cin >> cuota;
            }while(cuota <=0 || cuota >=1);
            cout <<"\n=========================================================";
            if (calcDensity(dir)<cuota) cout << "\nDensidad: "<<calcDensity(dir)<< " | Resultado: el grafo es DISPERSO"<<endl;
            else cout << "\nDensidad: "<<calcDensity(dir)<<" | Resultado: el grafo es DENSO" << endl;
            cout <<"===========================================================\n";
        }

        void type(node *nodo){ // Hundido o Fuente
            if (degreInNode(nodo)==0)
                cout << "Fuente ";
            else if(degreOutNode(nodo)==0)
                cout <<"Hundido ";
            else
                cout << "Normal ";
        }


        int degreOutNode(node *nodo){
            if(dir){
                int degree = 0;
                 typename dictEdges::iterator it;
                 it = dictE.begin();
                 while(it != dictE.end()){ //typedef map<pair<GV,GV>,bool> dictEdges;
                     if((*it).second && (((*it).first).first == nodo->getData()) ){
                         degree++;
                     }
                     it++;
                 }
                return degree;
            }else
                return nodo->getCountNodesAdj();
        }
        int degreInNode(node* nodo){
            if(dir){
                int degree = 0;
                typename dictEdges::iterator it;
                it = dictE.begin();
                while(it != dictE.end()){ //typedef map<pair<GV,GV>,bool> dictEdges;
                    if((*it).second && (((*it).first).second == nodo->getData()) ){
                        degree++;
                    }
                    it++;
                }
                return degree;
            }else
            return nodo->getCountNodesAdj();
        }

    Graph<GV,GV> kruskal() { //Pasar como grafo
      cout<<"Encontro a kruskal\n";
            if(!dir && conexo()) {
                Graph krusky(false, true);
                EdgeSeq krusk;
                NodeSet visitedNode;
                auto mySet = new DsjSet<GE>;
                int totalWeight = 0;

                for (ni = nodes.begin(); ni != nodes.end(); ni++)
                    mySet->makeSet((*ni)->getData(), *ni);

                edges.sort([](edge *first, edge *second) { //Ordenar de acuerdo a los pesos
                    int wgtFirst = first->getData(); //Peso del primero
                    int wgtSecond = second->getData();
                    return wgtFirst < wgtSecond;
                });

                ei = edges.begin();
                while (ei != edges.end()) {
                    auto first = (*ei)->nodes[0];
                    auto second = (*ei)->nodes[1];
                    if (mySet->findSet(first->getData()) !=
                        mySet->findSet(second->getData())) { //si no tienen los mismos padres
                        mySet->unionS(first->getData(), second->getData());
                        totalWeight += (*ei)->getData();


                        auto v = visitedNode.find((*ei)->nodes[0]); //Conntrolar no volver a insertar el mismo nodo
                        auto v2 = visitedNode.find((*ei)->nodes[1]);
                        if (v == visitedNode.end()) {
                            krusky.insertNode((*ei)->nodes[0]->getData(), 0, 0);
                            visitedNode.insert((*ei)->nodes[0]);
                        }
                        if (v2 == visitedNode.end()) {
                            krusky.insertNode((*ei)->nodes[1]->getData(), 0, 0);
                            visitedNode.insert((*ei)->nodes[1]);
                        }

                        cout << "Vertices( " << (*ei)->nodes[0]->getData()<<", "<< (*ei)->nodes[1]->getData() << ")";
                        cout << "\t|\tPeso: "<<(*ei)->getData() << endl;
                        krusky.insertEdge((*ei)->getData(), (*ei)->nodes[0]->getData(), (*ei)->nodes[1]->getData());
                        krusk.push_back(*ei);
                    }
                    ei++;

                }
                cout << "Peso total: " << totalWeight << endl;
                return krusky;
            } else
                throw out_of_range("El algoritmo de Kruskal no funciona para grafos dirigidos");
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

        Graph<GV,GV> prim(GV etiqueta){

            if(!dir && conexo()) { //Si no es dirigido y es conexo
                auto start = dict[etiqueta];
                int weight=0;
                if (start) {
                    Graph result(false, true);
                    NodeSet visited, visitedResult, totalNodes, remainded;
                    visited.insert(start);
                    //Llenar el set de nodos
                    for (ni = nodes.begin(); ni != nodes.end(); ni++) //llenar los nodes
                        totalNodes.insert(*ni);

                    edges.sort([](edge *first, edge *second) { //Ordenar de acuerdo a los pesos
                        int wgtFirst = first->getData();
                        int wgtSecond = second->getData();
                        return wgtFirst < wgtSecond;
                    });

                    while (visited != totalNodes) {
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
                                if (min < currentMin) {
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
                                if (min < currentMin2) {
                                    currentMin2 = min;
                                    node11 = (*ei)->nodes[1];
                                    node22 = (*ei)->nodes[0];
                                }
                            }
                            ei++;
                        }
                        //Rellenar el grafo y controlar cuando hay varias opciones
                        if (currentMin > currentMin2){
                            auto v = visitedResult.find(node11); //Conntrolar no volver a insertar el mismo nodo
                            auto v2 = visitedResult.find(node22);
                            if (v2 == visitedResult.end()) {
                                result.insertNode(node22->getData(), 0, 0);
                                visitedResult.insert(node22);
                            }
                            if (v == visitedResult.end()) {
                                result.insertNode(node11->getData(), 0, 0);
                                visitedResult.insert(node11);
                            }
                            weight+=currentMin2;
                            result.insertEdge(currentMin2, node11->getData(), node22->getData());
                            cout << "Vertices( " << node11->getData()<<", "<<node22->getData() << ")";
                            cout << "\t|\tPeso: "<<currentMin2 << endl;

                            visited.insert(node22);
                        } else if (currentMin < currentMin2) {
                            auto v2 = visitedResult.find(node1); //Conntrolar no volver a insertar el mismo nodo
                            auto v = visitedResult.find(node2);
                            if (v == visitedResult.end()) {
                                result.insertNode(node2->getData(), 0, 0);
                                visitedResult.insert(node2);
                            }
                            if (v2 == visitedResult.end()) {
                                result.insertNode(node1->getData(), 0, 0);
                                visitedResult.insert(node1);
                            }
                            weight+=currentMin;
                            result.insertEdge(currentMin, node1->getData(), node2->getData());
                            cout << "Vertices( " << node1->getData()<<", "<<node2->getData() << ")";
                            cout << "\t|\tPeso: "<<currentMin << endl;

                            visited.insert(node2);
                        } else {
                            if (currentMin != 99999 && currentMin2 != 99999) {
                                auto v = visitedResult.find(node1); //Cnntrolar no volver a insertar el mismo nodo
                                auto v2 = visitedResult.find(node2);
                                if (v2 == visitedResult.end()) {
                                    result.insertNode(node2->getData(), 0, 0);
                                    visitedResult.insert(node2);
                                }
                                if (v == visitedResult.end()) {
                                    result.insertNode(node1->getData(), 0, 0);
                                    visitedResult.insert(node1);
                                }

                                weight+=currentMin;
                                result.insertEdge(currentMin, node1->getData(), node2->getData());
                                cout << "Vertices( " << node1->getData()<<", "<<node2->getData() << ")";
                                cout << "\t|\tPeso: "<<currentMin << endl;

                                visited.insert(node2);
                            }

                        }

                    }
                    cout << "Peso Total: "<<weight<<endl;

                    return result;
                } else {
                    throw out_of_range("El nodo no está en el grafo");
                }
            }else
                throw out_of_range("El algoritmo de Prim no funciona para grafos dirigidos");
        }


        node* getNode(GV value){
          if(dict[value])
            return dict[value];
          return nullptr;
        }

        void writeOn(string nameOf)
        {
          ofstream File;
          File.open(nameOf);
          ei = edges.begin();
          while (ei != edges.end())
          {
            node **arr = (*ei)->getNodes();
            File << arr[0]->getData() << " " << arr[1]->getData() << " " << (*ei)->getData() << "\n";
            ei++;
          }
        }

        Graph *BellmanFord(int begining)
        {
          int countV = 0;
          auto BellmanFordGraph = new Graph(dir, pond);
          for (ni = nodes.begin(); ni != nodes.end(); ni++)
          {
            BellmanFordGraph->insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());
            countV++;
          }

          int V = countV;
          int dist[V];
          for(int i = 1; i <= V;i++){
            dist[i] = max_int;
          }

          dist[begining] = 0;
          int count = 0;
          for (int i = 0; i <= V - 1; i++)
          {
            ei = edges.begin();
            while (ei != edges.end())
            {
              node **arr = (*ei)->getNodes();
              int u = arr[0]->getData();
              int v = arr[1]->getData();
              int weight = (*ei)->getData();
              if (dist[u] != max_int)
              {
                if ((dist[u] + weight) < dist[v]){
                  dist[v] = dist[u] + weight;
                    BellmanFordGraph->insertEdge((*ei)->getData(), arr[0]->getData(), arr[1]->getData());
                  }
                }
              }
              ei++;
            }
      

          for (int i = 0; i <= V - 1; i++)
          {
            ei = edges.begin();
            while (ei != edges.end())
            {
              node **arr = (*ei)->getNodes();
              int u = arr[0]->getData();
              int v = arr[1]->getData();
              int weight = (*ei)->getData();
              if (dist[u] != max_int)
              {
                if ((dist[u] + weight) < dist[v])
                {
                  throw out_of_range("Contiene ciclos negativos");
                }
              }
              ei++;
            }
          }

          for(int i = 1; i <= V; i++)
          {
            cout << "From: " << begining << " go to: " << i << " with: " << dist[i] << endl;
          }

          return BellmanFordGraph;
        }

      

private : NodeSeq nodes;
        EdgeSeq edges;
        dictNode dict;
        dictEdges dictE;
        dictGetEdge dictGetE;
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
  }
}

#endif
