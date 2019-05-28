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
  g.print();
  g.insertEdge(2,4);
  //Read<int,int> read(file);
  //g=read.getGraph();

  return EXIT_SUCCESS;
}
