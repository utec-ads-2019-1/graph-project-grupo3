//#include <GL/glut.h>
#include <iostream>
#include "graph.h"
#include "read.h"

using namespace std;

int main() {


  Graph<char,int> g(false,true);
  string file;

  cout<<"Ingresa nombre de file::\n";
  cin >> file;
  g.readFile(file);
  g.print();
  cout << "Prim" << endl;
  g.prim('a');

  cout << "Kruskal" << endl;
  g.kruskal();
  //cout<<g.stronglyConnectedComponent()<<endl;
  //cout<<g.bipartiteGraph()<<endl;
  //g.print();

  cout << "Dikjstra" << endl;

  g.dijkstra('a');

  auto bellman =  g.BellmanFord('a');
  bellman->print();
  bellman->writeOn("bellmanford.txt");
   
  //auto resultado = g.DFS(2);
  //resultado->print();
  //resultado->writeOn("Bfs.txt");
  //g.kruskal();
  //g.prim(2);


  cout<<"es bipartito "<<g.conexo()<<endl;
  cout<<"Es conexo:: "<<(g.conexo()?"Si":"No")<<endl;

  //Read<int,int> read(file);
  //g=read.getGraph();


  return EXIT_SUCCESS;
}
