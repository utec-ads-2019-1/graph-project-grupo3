#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

template <typename V>
class DsjSet;
template <typename V>
class Node {
    public:
        //typedef typename G::edge edge;
        typedef vector<Node*> NodeSeq;
        typedef unordered_map<Node*,bool> dictNodeAdj;

        //EdgeSeq edges;
        Node(){}
        Node(V data,double x,double y){
          this->data=data;
          this->x=x;
          this->y=y;
          rank = 0;
          degree = 0;
          parent = this;
        }
        ~Node(){delete this;}

        NodeSeq getNodesAdj(){
          return nodesAdj;
        }
        void insertNodeAdj(Node* nodeAdj){
          //if(!dict.count(nodeAdj)){
           // dict[nodeAdj]=1;
            nodesAdj.push_back(nodeAdj);
            degree++;
         // }
        }
        V getData(){ return data;};
        int getDegree(){
            return degree;
        }

        Node (V value){
            data = value;
            x = y = 0;
            rank = 0;
            parent = this;
            degree = 0;
        }
        template <class>
        friend class DsjSet;



    private:
        V data;
        NodeSeq nodesAdj;
        dictNodeAdj dict;
        double x;
        double y;

protected:
        Node<V>* parent;
        int rank;
        int degree;
};

#endif
