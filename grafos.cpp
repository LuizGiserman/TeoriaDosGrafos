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
  int numberVertices, numberEdges = 0;


};

std::vector<std::list<int>> Grafos::CriarLista (std::string fileName, int *numberVertices, int *numberEdges)
{
    std::vector<std::list<int>> vectorGraph;

    /*Auxiliar variables*/
    std::list<int> linkedList1, linkedList2;
    std::string auxiliar;
    std::ifstream file;
    int auxVertice1, auxVertice2;
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
    file >> *numberVertices;

    /*Reading the edges. format: "Edge1 Edge2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {

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

            if (list.front() == auxVertice1)
                list.push_back (auxVertice2);
            else if (list.front() == auxVertice2)
                list.push_back (auxVertice1);
            /*Pensei em botar booleans pra verificar se o swith encontrou os dois cases, pq, caso contrário
            *precisamos criar uma lista encadeada nova, já que isso significa que o vértice é novo!
            */
        }

    }
        numberEdges ++;

    file.close();
    return vectorGraph;
}
