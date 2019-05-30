//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(true,true);
  string file;
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

  return EXIT_SUCCESS;
}
