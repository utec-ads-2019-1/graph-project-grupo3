//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(false);
  string file;

  int n=8;
  //Tester
  for(int i = 0; i <= n; i++){
      g.insertNode(i, rand()%10/100, rand()%10/100);
  }

  g.insertEdge(4,0,1);
  g.insertEdge(8,1,2);
  g.insertEdge(7,2,3);
  g.insertEdge(9,3,4);
  g.insertEdge(10,4,5);
  g.insertEdge(2,5,6);
  g.insertEdge(1,7,6);
  g.insertEdge(8,7,0);
  g.insertEdge(11,7,1);
  g.insertEdge(7,7,8);
  g.insertEdge(2,2,8);
  g.insertEdge(6,8,6);
  g.insertEdge(4,2,5);
  g.insertEdge(14,3,5);


    g.print();
   // g.density();
    auto nuevo = g.prim(0);
    cout <<"Prim: " << endl;
   // nuevo.print();

    auto nuevo2 = g.kruskal();
    cout <<"Kruskal: " << endl;
   // nuevo2.print();


    /*
  cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  g.print();
  ///Read<int,int> read(file);
  //g=read.getGraph();
     */

  return EXIT_SUCCESS;
}
