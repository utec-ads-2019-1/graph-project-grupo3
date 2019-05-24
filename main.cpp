//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(false);
  string file;
  cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  g.print();
  //Read<int,int> read(file);
  //g=read.getGraph();

  return EXIT_SUCCESS;
}
