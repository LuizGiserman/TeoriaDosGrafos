#include "grafos.h"
#include <iostream>

int main(){
  Grafos grafo = Grafos("grafoteste.txt",1);
  
  grafo.BFS(1);
  cout << endl;
  cout << endl;
  grafo.BFS(2);
  cout << endl;
  cout << endl;
  grafo.BFS(3);
  cout << endl;
  cout << endl;
  grafo.BFS(4);
  cout << endl;
  cout << endl;
  grafo.BFS(5);

  return 0;
}