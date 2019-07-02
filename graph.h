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
#include<ctime>
#include<cmath>
#include <limits>
#include "node.h"
#include "edge.h"
#include "dsjset.h"
#define max_int 214748364


struct pairAStar{
    int indice;
    int value;

    bool operator<(const pairAStar &other){
       return value<other.value;
    }
};

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
        typedef map<pair<GV,GV>,bool> dictEdges;
        typedef map<pair<GV,GV>,GE> matrixAdj;
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;
        typedef void * (*THREADFUNCPTR)(void *);

        Graph(bool dirg,bool pondg):dir(dirg),pond(pondg){size=0;stateFloyd=false;}

        ~Graph(){
            /*
          while(!edges.empty()){
            delete edges.back();
            edges.pop_back();
          }
          for(int i=0;i<size;i++)
            delete nodes[i];
             */
        }

        bool insertNode(GV value,double x,double y){
          if(!dict[value]){
            node *n=new node(value,x,y);
            nodes.push_back(n);
            dict[value]=n;
            size++;
            stateFloyd=false;
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

              for(auto x:nodes){
                if(x->getData()==value)
                  continue;
                removeEdge(value,x->getData());
                if(dir)
                  removeEdge(x->getData(),value);
              }

              delete dict[value];
              dict[value]=nullptr;
              size--;
              stateFloyd=false;
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
            dictE[make_pair(node1,node2)]=true;

            node* n1=dict[node1];
            node* n2=dict[node2];
            mAdj[make_pair(node1,node2)]=1;
            n1->insertNodeAdj(n2);
            if(!dir){
              dictE[make_pair(node2,node1)]=true;
              mAdj[make_pair(node2,node1)]=1;
              n2->insertNodeAdj(n1);
            }
            stateFloyd=false;
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
            dictE[make_pair(node1,node2)]=true;

            mAdj[make_pair(node1,node2)]=edgeV;
            node* n1=dict[node1];
            node* n2=dict[node2];
            n1->insertNodeAdj(n2);
            if(!dir){
              dictE[make_pair(node2,node1)]=true;
              n2->insertNodeAdj(n1);
              mAdj[make_pair(node2,node1)]=edgeV;
            }
            stateFloyd=false;
            return true;
          }
          return false;
        }
        bool removeEdge(GV node1,GV node2){
          ei=edges.begin();
          if(!dictE[make_pair(node1,node2)])
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
              stateFloyd=false;
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
              stateFloyd=false;
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

        bool conexo(){

          if(dir)
            throw ("Esta funcion no sirve para grafos dirigidos\n");

          node* tmp;
          queue<node*> q;
          q.push(nodes[0]);
          int cont=1;
          NodeSeq nodesCurr;
          unordered_map<node*,bool> states;
          states[nodes[0]]=true;
          while(!q.empty()){
            tmp=q.front();
            q.pop();
            nodesCurr=tmp->getNodesAdj();
            for(auto x:nodesCurr){
              if(!states[x]){
                states[x]=true;
                q.push(x);
                cont++;
              }
            }
          }
          if(size==cont)
            return true;

          return false;
        }

        //Floyd Warshall
        vector<vector<GE>> floydwarshall(){

          if(stateFloyd)
            return matrixFloydW;

          int i,j;

          matrixFloydW.clear();
          matrixFloydPath.clear();

          for(i=0;i<size;i++){
            vector<GE> aux(size,1<<30);
            vector<GV> aux2(size,nodes[i]->getData());
            node* tmp=nodes[i];
            for(j=0;j<size;j++){
              if(mAdj[{tmp->getData(),nodes[j]->getData()}]){
                aux[j]=mAdj[{tmp->getData(),nodes[j]->getData()}];
              }
            }
            matrixFloydW.push_back(aux);
            matrixFloydPath.push_back(aux2);
          }


          for(int k=0;k<size;k++){
            for(i=0;i<size;i++){
              for(j=0;j<size;j++){
                if(matrixFloydW[i][k]==(1<<30) || matrixFloydW[k][j]==(1<<30) || i==j) continue;
                if(matrixFloydW[i][k]+matrixFloydW[k][j] < matrixFloydW[i][j]){
                  matrixFloydW[i][j]=matrixFloydW[i][k]+matrixFloydW[k][j];
                  matrixFloydPath[i][j]=nodes[k]->getData();
                }
              }
            }
          }

          printMatrixFloydWarshall();
          printMatrixFloydWarshallPath();

          stateFloyd=true;
          return matrixFloydW;
        }
        void printMatrixFloydWarshallPath(){
          int i,j;
          cout<<"\t";
            for(i=0;i<size;i++)
              cout<<nodes[i]->getData()<<"\t";
            cout<<endl;
            for(i=0;i<size;i++){
              cout<<nodes[i]->getData()<<"\t";
              for(j=0;j<size;j++){
                  cout<<matrixFloydPath[i][j]<<"\t";
              }
              cout<<endl;
            }
        }
        void printMatrixFloydWarshall(){
          int i,j;
          cout<<"\t";
            for(i=0;i<size;i++)
              cout<<nodes[i]->getData()<<"\t";
            cout<<endl;
            for(i=0;i<size;i++){
              cout<<nodes[i]->getData()<<"\t";
              for(j=0;j<size;j++){
                if(matrixFloydW[i][j]==(1<<30))
                  cout<<"INF\t";
                else
                  cout<<matrixFloydW[i][j]<<"\t";
              }
              cout<<endl;
            }
        }
/*
        void aStar(){
          if(stateFloyd)
            return;

          pthread_t workers[size];
          indiceAStar=0;
          int i;
          vector<pair<int,int>*> pairN;
          srand(time(NULL));
          int x=-1,y=-1;
          for(i=0;i<5;i++){
            while(x==y){
              x=rand()%size;
              y=rand()%size;
            }
            pair<int,int>*paux;
            paux->first=x;
            paux->second=y;
            pairN.push_back(paux);
          }
          stateFloyd=true;

          for(i=0;i<size;i++)
            dictAStar[nodes[i]->getData()]=i;

          for(i=0;i<size;i++)
            pthread_create(&workers[i],NULL,jobAStar,pairN[i]);
          for(i=0;i<size;i++)
            pthread_join(workers[i],NULL);

        }*/
        GE pitagoras(node* n1,node* n2){
          return sqrt(pow(n1->getX()-n2->getX(),2)+pow(n1->getY()-n2->getY(),2));
        }
        void *jobAStar(void* p){

          pair<int,int>*varp=(pair<int,int>*)p;
          node* f=nodes[varp->first];
          node* d=nodes[varp->second];

          vector<GE> distances(size,1<<30);
          vector<GV> path(size,f->getData());
          bool states[size]={false};

          distances[dictAStar[f->getData()]]=0;
          priority_queue<pairAStar> pq;
          pq.push(pairAStar(dictAStar[f->getData()],0));

          while(!pq.empty()){
            pairAStar tmpPair=pq.top();
            pq.pop();
            states[tmpPair.indice]=true;
            node* tmpnode=nodes[tmpPair.indice];
            vector<node*> tmpnodesAdj=tmpnode->getNodesAdj();
            for(node* x:tmpnodesAdj){
              if(!states[dictAStar[x->getData()]]){
                if(distances[dictAStar[x->getData()]]>mAdj[{tmpnode->getData(),x->getData()}]+pitagoras(tmpnode,x)){
                  distances[dictAStar[x->getData()]]=mAdj[{tmpnode->getData(),x->getData()}]+pitagoras(tmpnode,x);
                  path[dictAStar[x->getData()]]=tmpnode->getData();
                }
                pq.push(pairAStar(dictAStar[x->getData()],0));
              }
              if(x->getData()==d->getData()){
                GV tmpPath=x->getData();
                while(tmpPath!=f->getData()){
                  cout<<tmpPath<<" ";
                }
                cout<<tmpPath<<endl;
              }
            }
          }
        }

        /*   void aStar(){
          pthread_t workers[nodes.size()];
          indiceAStar=0;
          int i;

          for(i=0;i<nodes.size();i++)
            pthread_create(&workers[i],NULL,jobAStar,NULL);
          for(i=0;i<nodes.size();i++)
            pthread_join(workers[i],NULL);
        }
        void jobAStar(){
          int indcurr=indiceAStar++;
        }
*/
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

    Graph kruskal() { //Pasar como grafo
      cout<<"Encontro a kruskal\n";
            if(!dir && conexo()) {
                Graph krusky(false, true);
                EdgeSeq krusk;
                NodeSet visitedNode;
                auto mySet = new DsjSet<GV>;
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
                            krusky.insertNode((*ei)->nodes[0]->getData(),(*ei)->nodes[0]->getX() , (*ei)->nodes[0]->getY());
                            visitedNode.insert((*ei)->nodes[0]);
                        }
                        if (v2 == visitedNode.end()) {
                            krusky.insertNode((*ei)->nodes[1]->getData(), (*ei)->nodes[1]->getX(), (*ei)->nodes[1]->getY());
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

        Graph prim(GV etiqueta){

            if(!dir && conexo()) { //Si no es dirigido y es conexo

                node* start = dict[etiqueta];
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
                        int currentMin = max_int;
                        int currentMin2 = max_int;
                        auto node1 = new  node;
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
                                result.insertNode(node22->getData(), node22->getX(), node22->getY());
                                visitedResult.insert(node22);
                            }
                            if (v == visitedResult.end()) {
                                result.insertNode(node11->getData(), node11->getX(), node11->getY());
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
                                result.insertNode(node2->getData(), node2->getX(), node2->getY());
                                visitedResult.insert(node2);
                            }
                            if (v2 == visitedResult.end()) {
                                result.insertNode(node1->getData(), node1->getX(), node1->getY());
                                visitedResult.insert(node1);
                            }
                            weight+=currentMin;
                            result.insertEdge(currentMin, node1->getData(), node2->getData());
                            cout << "Vertices( " << node1->getData()<<", "<<node2->getData() << ")";
                            cout << "\t|\tPeso: "<<currentMin << endl;

                            visited.insert(node2);
                        } else {
                            if (currentMin != max_int && currentMin2 != max_int) {
                                auto v = visitedResult.find(node1); //Cnntrolar no volver a insertar el mismo nodo
                                auto v2 = visitedResult.find(node2);
                                if (v2 == visitedResult.end()) {
                                    result.insertNode(node2->getData(), node2->getX(), node2->getY());
                                    visitedResult.insert(node2);
                                }
                                if (v == visitedResult.end()) {
                                    result.insertNode(node1->getData(), node1->getX(), node1->getY());
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

         Graph dijkstra(GV etiqueta){
            if (dict[etiqueta]) {
                Graph resultGraph(false, true);
                set <pair<int, GV>> setDst;
                vector<int> dist(size, max_int);
                setDst.insert(make_pair(0, etiqueta));

                for (ni = nodes.begin(); ni != nodes.end(); ni++)
                    resultGraph.insertNode((*ni)->getData(), (*ni)->getX(), (*ni)->getY());


                if (isalpha(etiqueta))
                    dist [int(etiqueta)-96-1] = 0;
                else
                    dist [etiqueta-1] = 0;

                while(!setDst.empty()){
                    pair <int, GV> temp = *(setDst.begin());
                    setDst.erase(setDst.begin());
                    GV u = temp.second;
                    NodeSeq nodestmp = dict[u]->getNodesAdj();
                    NodeIte ni2=nodestmp.begin();
                    for(;ni2!=nodestmp.end();ni2++){
                        GV v = (*ni2)->getData();
                        int weight = 0;

                        ei = edges.begin();
                        while (ei != edges.end()) {
                            if(((*ei)->nodes[0]->getData() == u && (*ei)->nodes[1]->getData()==v) || ((*ei)->nodes[1]->getData() == u && (*ei)->nodes[0]->getData()==v)) {
                                weight = (*ei)->getData();
                                break;
                            }
                            ei++;
                        }

                        if(isalpha(etiqueta))
                        {
                            if (dist[int(v) - 96-1] > dist[int(u) - 96- 1] + weight) {
                                if (dist[int(v)- 96 - 1] != max_int)
                                    setDst.erase(setDst.find(make_pair(dist[int(v) -96-1], v)));
                                dist[int(v) -96- 1] = dist[int(u) - 96-1] + weight;
                                setDst.insert(make_pair(dist[int(v) -96- 1], v));
                                resultGraph.insertEdge(weight, char(u + 97), char(v + 97));
                            }
                        }
                        else
                        {
                            if (dist[v - 1] > dist[u - 1] + weight) {
                                if (dist[v - 1] != max_int)
                                    setDst.erase(setDst.find(make_pair(dist[v - 1], v)));
                                dist[v - 1] = dist[u - 1] + weight;
                                setDst.insert(make_pair(dist[v - 1], v));
                                resultGraph.insertEdge(weight, u, v);
                            }
                        }
                    }
                }
                cout << "¡Dikjstra!" <<endl;
                for(int i = 0; i < size; i++){
                    if(isalpha(etiqueta))
                        cout <<"From: "<< etiqueta << " to: " << char(i+1+96) << " Peso: " << dist[i] << endl;
                    else
                        cout <<"From: "<< etiqueta << " to: " << i+1 << " Peso: " << dist[i] << endl;
                }

                return resultGraph;
            }else
                throw out_of_range("El nodo no está en el grafo");

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
          for(int i = 0; i < V;i++){
            dist[i] = max_int;
          }
          if (isalpha(begining))
              dist[begining-96] = 0;
          else
              dist[begining] = 0;

          vector<node *> hasEdge;
          for (int i = 0; i <= V - 1; i++)
          {
            ei = edges.begin();
            while (ei != edges.end())
            {
              node **arr = (*ei)->getNodes();
              int u = arr[0]->getData();
              int v = arr[1]->getData();
              int weight = (*ei)->getData();
              if(isalpha(begining)) {
                  if (dist[u-96] != max_int) {
                      if ((dist[u-96] + weight) < dist[v-96]) {
                          dist[v-96] = dist[u-96] + weight;
                          // if(find(hasEdge.begin(),hasEdge.end(),arr[1])==hasEdge.end()){
                          // hasEdge.push_back(arr[1]);
                          BellmanFordGraph->insertEdge((*ei)->getData(), arr[0]->getData(), arr[1]->getData());
                          //}
                      }
                  }
              }
              else{
                  if (dist[u] != max_int) {
                      if ((dist[u] + weight) < dist[v]) {
                          dist[v] = dist[u] + weight;
                          // if(find(hasEdge.begin(),hasEdge.end(),arr[1])==hasEdge.end()){
                          // hasEdge.push_back(arr[1]);
                          BellmanFordGraph->insertEdge((*ei)->getData(), arr[0]->getData(), arr[1]->getData());
                          //}
                      }
                  }
              }
              ei++;
            }

          }
          cout << "Bellman Ford\n";

          for(int i = 1; i <= V; i++){
              if(isalpha(begining))
                  cout << "From: " << char(begining) << " go to: " << char(i + 96) << " with: " << dist[i] << endl;
              else
                  cout << "From: " << begining << " go to: " << i << " with: " << dist[i] << endl;
          }

          return BellmanFordGraph;
        }

      

private : NodeSeq nodes;
        EdgeSeq edges;
        dictNode dict;
        dictEdges dictE;
        matrixAdj mAdj;
        int size;
        NodeIte ni;
        EdgeIte ei;
        bool dir;
        bool pond;
        vector<vector<GE>> matrixFloydW;
        vector<vector<GV>> matrixFloydPath;
        bool stateFloyd;
        int indiceAStar;
        unordered_map<GV,int> dictAStar;
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
