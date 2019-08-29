#include "grafos.h"
#include <iostream>

int main(){
  Grafos grafo2 = Grafos("grafoteste.txt",1);
  Grafos grafo = Grafos("grafoteste.txt",0);

  grafo2.Print();
  cout << " " << endl;
  cout << " " << endl;
  grafo.Print();

  return 0;
}