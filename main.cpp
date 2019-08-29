#include "grafos.h"
#include <iostream>

int main(){
  Grafos grafo = Grafos("grafoteste.txt",0);
  
  grafo.PrintInformation();
  grafo.Print();

  return 0;
}