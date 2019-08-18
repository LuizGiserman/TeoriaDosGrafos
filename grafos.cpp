/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/

#include "grafos.h"

/*Constructor function*/
Grafos::Grafos(std::string fileName)
{
  std::vector<BYTE> adjacencyMatrix;
  std::list<int> linkedList;
  int numberEdges, numberVertices;

  std::ifstream file;

  file.open(fileName);
  if (!file)
  {
    std::cout << "Unable to open file" << std::endl;
    std::cout << "Error #" << errno << ":" << strerror(errno) << std::endl;
    exit(ERROR_READING_FILE);
  }



  file.close();

  /*Read from the file and update the class's constructor variables*/
  // getVariables (fileName);


};

/*Read from the file and update the class's constructor variables*/
// void Grafos::getVariables (std::string fileName, int *numberEdges, int *numbe)
// {
//
//
//
// }
