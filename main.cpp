//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<char,int> g(false,true);
  string file;
/*
  int n=4;
  //Tester
  for(int i = 0; i <= n; i++)
      g.insertNode(i, rand()%10/100, rand()%10/100);

  g.insertEdge(3,0,1);
  g.insertEdge(9,3,2);
  g.insertEdge(9,2,4);
  g.insertEdge(9,4,0);
  g.insertEdge(9,1,3);

    g.print();
    g.density();
    cout<<"Es conexo:: "<<(g.conexo()?"Si":"No")<<endl;
    cout<<"Es bipartito:: "<<(g.bipartiteGraph()?"Si":"No")<<endl;

    cout <<"\n**************************\n";
    cout <<"  Algoritmo de Prim " << endl;
    cout <<"**************************\n";
    g.prim(0);


    cout <<"\n**************************\n";
    cout <<"   Algoritmo de Kruskal " << endl;
    cout <<"**************************\n";
    g.kruskal();



*/
  cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  //g.print();
  //cout<<g.stronglyConnectedComponent()<<endl;
  //cout<<g.bipartiteGraph()<<endl;
  g.print();
  auto resultado = g.DFS('A');
  resultado->print();

  //g.kruskal();
  //g.prim(2);
  cout<<"es bipartito "<<g.conexo()<<endl;
  cout<<"Es conexo:: "<<(g.conexo()?"Si":"No")<<endl;

  //vector<vector<int>> ma=g.floydwarshall();

  //Read<int,int> read(file);
  //g=read.getGraph();


  return EXIT_SUCCESS;
}
