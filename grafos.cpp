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
  std::string auxVertice1, auxVertice2;
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


  /*input = fopen("./as_graph.txt", "r");
    if(input) {
        fscanf(input, "%d\n", &numVertex);
        while (fscanf(input, "%d %d \n",&verticeOrigem, &verticeDestino) != EOF) {
          int vertice[2] = {verticeOrigem,verticeDestino};
          vertices.push_back(vertice);
          numEdges ++;
        }
    }*/

  while (!file.eof())
  {
    /*Reading the edges. format: "Edge1 Edge2"*/
    fscanf(file, "%d %d\n", &auxVertice1, &auxVertice2);
    auxVertice1 = std::stoi (auxVertice1)
    auxVertice2 = std::stoi (auxVertice2)

    /*if vector of linked lists is empty:
     - Create a linked list for each of the vertices read
     - Add the other vertice to the list.
    */
    if (vectorGraph.empty())
    {

      linkedList1.push_back (auxVertice1);
      linkedList1.push_back (auxVertice2);
      vectorGraph.push_back (linkedList1);

      linkedList2.push_back (auxVertice2);
      linkedList2.push_back (auxVertice1);
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
        case auxVertice1 :
          list.push_back (auxVertice2);
          break;
        case auxVertice2 :
          list.push_back (auxVertice1);
          break;
        default:
        /*Pensei em botar booleans pra verificar se o swith encontrou os dois cases, pq, caso contrário
         *precisamos criar uma lista encadeada nova, já que isso significa que o vértice é novo!
         */
      }

    }
    numberEdges ++;
  }

  file.close();

};
