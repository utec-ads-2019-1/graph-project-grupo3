#include <GLUT/glut.h>
#include <string>
#include <math.h>
#include <iostream>
#include "graph.h"
#include "node.h"
#include "read.h"

using namespace std;

int main(int argc, char **argv)
{
  Graph<int, int> g(false, true);
  string file;
  cout << "Ingresa nombre de file::";
  file = "input.txt";
  g.readFile(file);
  g.print();

  auto bfs1 = g.BFS(2);
  bfs1->print();
  auto dfs1 = g.DFS(2);
  dfs1->print();
  //Read<int,int> read(file);
  //g=read.getGraph();

  return EXIT_SUCCESS;
}
