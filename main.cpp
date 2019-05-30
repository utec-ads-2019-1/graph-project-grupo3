//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(true,true);
  string file;

  int n=3;
  //Tester
  for(int i = 0; i <= n; i++){
      g.insertNode(i, rand()%10/100, rand()%10/100);
  }

  g.insertEdge(3,0,1);
  g.insertEdge(4,1,3);
  g.insertEdge(7,2,3);
  g.insertEdge(9,0,2);

    g.print();
    g.print();
    g.density();
    g.conexo();






    cout <<"\n**************************\n";
    cout <<"  Algoritmo de Prim " << endl;
    cout <<"**************************\n";
    g.prim(0);


    cout <<"\n**************************\n";
    cout <<"   Algoritmo de Kruskal " << endl;
    cout <<"**************************\n";
    g.kruskal();
    cout<<"Es bipartito"<<endl;
    cout<<g.bipartiteGraph()<<endl;


/*
  cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  //g.print();
  //cout<<g.stronglyConnectedComponent()<<endl;
  //cout<<g.bipartiteGraph()<<endl;
  g.print();
  cout<<"es bipartito "<<g.conexo()<<endl;

  //Read<int,int> read(file);
  //g=read.getGraph();

*/
  return EXIT_SUCCESS;
}
