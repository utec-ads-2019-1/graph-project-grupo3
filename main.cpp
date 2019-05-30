//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(false,true);
  string file;
  cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  //g.print();
  //cout<<g.stronglyConnectedComponent()<<endl;
  //cout<<g.bipartiteGraph()<<endl;
  g.print();
  cout<<g.removeEdge(1,2)<<endl;
  g.print();
  cout<<g.removeNode(1)<<endl;
  g.print();
  //Read<int,int> read(file);
  //g=read.getGraph();

  return EXIT_SUCCESS;
}
