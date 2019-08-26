#include "grafos.h"
#include <iostream>

int main(){
  Grafos grafo = Grafos("grafoteste.txt");

  cout << grafo.matrix << " ";
  grafo.Matrix("grafoteste.txt");
  grafo.PrintMatrix();
  grafo.getInformationMatrix();
  grafo.PrintInformation();

  return 0;
}