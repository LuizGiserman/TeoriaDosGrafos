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
  std::vector<std::list<int>> vectorGraph;
  std::list<int> linkedList1, linkedList2;
  int numberVertices, numberEdges = 0;

  /*Auxiliar variables*/
  std::string auxiliar;
  std::ifstream file;
  std::tuple <std::string, std::string> auxVertices;
  int index;

  file.open(fileName);
  /*Treating the open file function*/
  if (!file && !(file.is_open()))
  {
    std::cout << "Unable to open file" << std::endl;
    std::cout << "Error #" << errno << ": " << strerror (errno) << std::endl;
    exit (ERROR_READING_FILE);
  }

  /*Getting the first line, which contains the info for the number of vertices*/
  std::getline (file, auxiliar);
  numberVertices = std::stoi (auxiliar);

  while (!file.eof())
  {
    /*Reading the edges. format: "Edge1 Edge2"*/
    std::getline (file, auxiliar);
    auxVertices = SplitString (auxiliar, ' ');

    /*if vector of linked lists is empty:
     - Create a linked list for each of the vertices read
     - Add the other vertice to the list.
    */
    if (vectorGraph.empty())
    {

      linkedList1.push_back (std::stoi (std::get<0>(auxVertices)));
      linkedList1.push_back (std::stoi (std::get<1>(auxVertices)));
      vectorGraph.push_back (linkedList1);

      linkedList2.push_back (std::stoi (std::get<1>(auxVertices)));
      linkedList2.push_back (std::stoi (std::get<0>(auxVertices)));
      vectorGraph.push_back (linkedList2);

      linkedList1.clear();
      linkedList2.clear();

    }

    /*check if vertice 1 is in the vector of linked lists.
    * If not, create a Linked list to add it to the vector (and add std::get<1>(auxVertices) to the list)
    *
    * Do the same with vertice 2 but add std::get<0>(auxVertices) instead
    */

    for (auto &list : vectorGraph)
    {

      switch(list.front())
      {
        case std::get<0>(auxVertices) :
          list.push_back (std::get<1>(auxVertices));
          break;
        case std::get<1>(auxVertices) :
          list.push_back (std::get<0>(auxVertices));
          break;
        default:
          // if (list.front() = list.end())

      }

    }


    numberEdges ++;
  }

  file.close();

};
