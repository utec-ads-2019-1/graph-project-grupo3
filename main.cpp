//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<int,int> g(false);
  string file;
  int n=100;
  for(int i = 0; i < n; i++){
      g.insertNode(rand()%100, rand()%10/100, rand()%10/100);
  }
  for(int i = 0; i < n; i++){
        g.insertEdge(rand()%10+1, rand()%100, rand()%100);
  }

  g.print();
  /*cout<<"Ingresa nombre de file::";
  cin>>file;
  g.readFile(file);
  g.print();
  *///Read<int,int> read(file);
  //g=read.getGraph();

  return EXIT_SUCCESS;
}
