#include "grafos.h"
#include <iostream>

int main(){
  Grafos grafo = Grafos("grafoteste.txt", 0);
  grafo.Print();


  // cout << grafo.matrix << " ";
  // grafo.Matrix("grafoteste.txt");
  // grafo.PrintMatrix();
  // grafo.getInformationMatrix();
  // grafo.PrintInformation();

  return 0;
}
