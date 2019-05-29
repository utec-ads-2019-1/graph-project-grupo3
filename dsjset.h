//
// Created by roosevelt on 27/05/19.
//
#include "node.h"
#include <map>
#ifndef GRAPH_PROJECT_GRUPO3_DSJSET_H
#define GRAPH_PROJECT_GRUPO3_DSJSET_H

template <typename G>
class DsjSet{
private:
    map<G, Node<G>*> nodes;
public:
    DsjSet(){};
    void makeSet(G value, Node<G>* node){
        this->nodes[value] = node;
    }


    Node <G>* findSet(G value){
        return findSet(this->nodes[value]);
    }
    Node<G> * findSet (Node<G>* node){
        auto temp = node;
        while(temp != temp->parent) {
            temp = temp->parent;
        }
        node->parent = temp;
        return temp;
    }

    bool unionS(G value1, G value2){
        auto parent1 = findSet(value1);
        auto parent2 = findSet(value2);
        if (parent1 != parent2){
            if(parent1->rank >= parent2->rank){
                parent1->rank = (parent1->rank == parent2->rank)? parent1->rank + 1 : parent1-> rank;
                parent2->parent = parent1;
            }else
                parent1->parent = parent2;
            return true;
        }
        return false;
    }

};

#endif //GRAPH_PROJECT_GRUPO3_DSJSET_H