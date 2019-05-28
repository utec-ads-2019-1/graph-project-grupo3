#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

template <typename V>
class Node {
    public:
        //typedef typename G::edge edge;
        typedef vector<Node*> NodeSeq;
        typedef unordered_map<Node*,bool> dictNodeAdj;
        typedef typename NodeSeq::iterator NodeIte;

        //EdgeSeq edges;
        Node(){}
        Node(V data,double x,double y){
          this->data=data;
          this->x=x;
          this->y=y;
        }
        ~Node(){delete this;}

        NodeSeq getNodesAdj(){
          return nodesAdj;
        }
        void insertNodeAdj(Node* nodeAdj){
            nodesAdj.push_back(nodeAdj);
        }
        void removeNodeAdj(Node *nodeAdj){
          ni=nodesAdj.begin();
          while(ni!=nodesAdj.end()){
            if((*ni)==nodeAdj){
              nodesAdj.erase(ni,ni+1);
            }
            ni++;
          }
        }
        V getData(){ return data;};

    private:
        V data;
        NodeSeq nodesAdj;
        dictNodeAdj dict;
        NodeIte ni;
        double x;
        double y;
};

#endif
