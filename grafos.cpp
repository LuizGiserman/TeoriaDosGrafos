/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/
#include <algorithm>
#include "grafos.h"
#include <bitset>
#include <iostream>
#include <string>
#include <cerrno>
#include <vector>
#include <queue>
#include <bitset>
#include <list>
#include <fstream>
#include <cstring>

using namespace std;

Verticedata::Verticedata()
{

}

Verticedata::~Verticedata(){
}

Vertice::Vertice(int type, int size)
{

  this->type = type;
  this->size = size;
  // list<int> List;

  if (type == 1)
      this->vertice.adjacenciesRow = new bitset<1> [size];

}

/* Function: void SetVertice()
 * Arguments: None
 * Description: given a row, mark the one related to
    verticeNumber, declaring an adjacency
 * Return value: None */
void Vertice::SetVertice(int verticeNumber)
{
  if ( type == 0)
    vertice.linkedList.push_back(verticeNumber);
  else if (type == 1)
    vertice.adjacenciesRow[verticeNumber - 1 ].set();
}

/* Function: GetVerticeList()
 * Arguments: None
 * Description: no matter what type of structure is being used,
 *  transform it into an adjacencies list and return it
 * Return value: list<int> listVertices */
list<int> Vertice::GetVerticeList()
{
  int i;
  list<int> listVertices;
  if (type == 0)
    listVertices = vertice.linkedList;
  else if (type == 1)
    for (i = 0; i < size; i++) /*ver*/
        if(vertice.adjacenciesRow[i] == 1)
            listVertices.push_back(i + 1);

  return listVertices;
}

/* Function: GetVerticeMatrix()
 * Arguments: None
 * Description: no matter what type of structure is being used,
 *  transform it into an adjacenciesRow and return it
 * Return value: bitset<1>* adjRow
 *               = Array of bitset<1>
 */
bitset<1>* Vertice::getVerticeMatrix()
{
    bitset<1> *adjRow;
    std::list<int>::iterator it;

    if (type == 0)
    {
      adjRow = new bitset<1> [size];
      for (it = vertice.linkedList.begin(); it != vertice.linkedList.end(); ++it)
          adjRow[*it - 1].set();
    }
    else if (type == 1)
        adjRow = vertice.adjacenciesRow;

    return adjRow;

}

/* Function: bool HasEdge()
 * Arguments: int verticeNumber
 * Description:
 * Return value: True if the vertice has an edge between it and
 *  the one labeled verticeNumbe
 *               False otherwhise*/
bool Vertice::HasEdge(int verticeNumber){

    if ( type == 0)
    {
      list<int>::iterator it;

      if (vertice.linkedList.front() == verticeNumber)
        return true;

      it = find(vertice.linkedList.begin(), vertice.linkedList.end(), verticeNumber);
      if (it != vertice.linkedList.end()){
          return true;
      }

      return false;

    }
    else if (type == 1)
      if (vertice.adjacenciesRow[verticeNumber - 1] == 1)
        return true;

    return false;
};

/*****************************************************************************************/

/*Constructor function*/
Grafos::Grafos(std::string fileName, int type)
{

    this->filename = fileName;

    // if 0 -> Adjacency List, if 1 -> an Adjacency Matrix
    this->type = type;

    Grafos::Populate();
    cout<<"Passei do Populate" <<endl;
};

/* Coloca os dados no grafo */
void Grafos::Populate()
{
    int numVertices;
    int numEdges;
    int auxVertice1;
    int auxVertice2;
    std::ifstream file;
    cout << "entrei no populate" << endl;

    file.open(filename);
    /*Treating the open file function*/
    if (!file && !(file.is_open()))
    {
        std::cout << "Unable to open file" << std::endl;
        std::cout << "Error #" << errno << ": " << strerror (errno) << std::endl;
        exit (ERROR_READING_FILE);
    }

    /*Getting the first line, which contains the info for the number of vertices*/
    file >> numVertices;

    cout << "antes de criar o grafo" << endl;
    CreateGrafo();
    cout << "criei grafo" << endl;

    /*Reading the edges. format: "Vertice1 Vertice2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {
        grafo[auxVertice1-1].SetVertice(auxVertice2);
        grafo[auxVertice2-1].SetVertice(auxVertice1);
        numEdges ++;
    }
    cout << "passei do while" << endl;

    file.close();

    this -> numVertices = numVertices;
    this -> numEdges = numEdges;
}


void Grafos::CreateGrafo()
{
    for (int i = 0; i < numVertices; ++i)
        grafo.push_back(*(new Vertice (type, numVertices)));
}


/* Print function, just to test if the constructor is okay */
void Grafos::Print()
{
    if(type == 0)
      Grafos::PrintList();
    else if( type == 1)
      Grafos::PrintMatrix();
};

/* Print Informations function */
void Grafos::PrintInformation(){

  Grafos::GetInformation();

  ofstream file;

  if(file)
  {
    file.open("./informacoes.txt");
    file << "Numero de Vertices: "<< numVertices << endl;
    file << "Numero de Arestas: " << numEdges << endl;
    file << "Grau Médio: " << avgDegree << endl;
    file << "Grau Maximo: " << maxDegree << endl;
    file << "Grau Minimo: " << minDegree << endl;
    file << "Mediana dos Graus: " << medDegree << endl;
  }

  file.close();

 };

/*2n quando podia ser n pra matriz*/
int Grafos::GetNumAdjacenciesVertice(int verticeNumber)
{
    int numEdges = 0;
    list<int> linkedList = grafo[verticeNumber].GetVerticeList();
    numEdges = linkedList.size();
    return numEdges;
}//;

int** Grafos::BFSGenerica(int initialVertice, int **bfsInfo, int stop /*=0*/, int stopVertice/*=0*/)
{
    unsigned index;
    queue<int> auxQueue; // Queue Created
    int verticeNumber;
    list<int> adjacents;
    std::list<int>::iterator it;
    bitset<1> *adjRow;

    for (index = 0; index < (unsigned) numVertices; ++index)
    {
        bfsInfo[index] = new int[READINGS_SPT + 1];
        //[index]// [0] marcado || nao marcado // [1] pai // [2] nivel na arvore
        bfsInfo[index][0] = 0;               // 0 é nao marcado || 1 é marcado
        bfsInfo[index][1] = numVertices + 1; // numVertices + 1 will be treat like infinity
        bfsInfo[index][2] = numVertices + 1; // numVertices + 1 will be treat like infinity
    }

    bfsInfo[initialVertice - 1][0] = 1; // initialVertice marked
    bfsInfo[initialVertice - 1][1] = 0; // root
    bfsInfo[initialVertice - 1][2] = 0; // no father

    auxQueue.push(initialVertice); // initialVertice first element of list
    while(auxQueue.empty() != true)
    {
        //get first element of Queue
        verticeNumber = auxQueue.front();
        //take off the first element of Queue
        auxQueue.pop();
        if ( type == 0 )
        {
            // List of adjacents of Vertice
            adjacents = grafo[verticeNumber-1].GetVerticeList();
            for (it = adjacents.begin(); it != adjacents.end(); ++it)
            {
                if(bfsInfo[*it - 1][0] == 0)
                {
                    //Mark Vertice
                    bfsInfo[*it - 1][0] = 1;
                    //Define Level of the father as the level of father plus 1
                    bfsInfo[*it - 1][1] = bfsInfo[verticeNumber-1][1] + 1;
                    // Define Vertice as father
                    bfsInfo[*it - 1][2] = verticeNumber;
                    auxQueue.push(*it);
                }
            }
        }
        else
        {
            // List of adjacents of Vertice
            adjRow = grafo[verticeNumber - 1].getVerticeMatrix();
            for (index = 0; index < (unsigned) numVertices; index++)
            {
                if(adjRow[index] == 1)
                {
                    if(bfsInfo[index][0] == 0)
                    {
                        //Mark Vertice
                        bfsInfo[index][0] = 1;
                        //Define Level of the father as the level of father plus 1
                        bfsInfo[index][1] = bfsInfo[verticeNumber-1][1] + 1;
                        // Define Vertice as father
                        bfsInfo[index][2] = verticeNumber;
                        auxQueue.push(index + 1);
                    }
                }
            }
        }
        if(stop == STOP_BFS && verticeNumber == stopVertice)
            if(verticeNumber == stopVertice)
                return bfsInfo;
    }

    return bfsInfo;
}

 void Grafos::BFS(int initialVertice)
 {

    int **bfsInfo;
    int index;

    bfsInfo = new int* [numVertices];
    BFSGenerica(initialVertice, bfsInfo);

    for (index= 0; index < numVertices; index++)
    {
        cout << index + 1 << "  ";
        cout << bfsInfo[index][1] << " " << bfsInfo[index][2];
        cout << endl;
    }
 }

 void Grafos::Distance(int firstVertice, int secondVertice)
 {

     int** bfsInfo = new int* [numVertices];

     bfsInfo = BFSGenerica(firstVertice, bfsInfo, STOP_BFS, secondVertice);

        cout << "BFS from " << firstVertice << " to " << secondVertice << endl;
        cout << endl;
        cout << "Distance:  " << bfsInfo[secondVertice - 1][1] << endl;
        cout << "Father in Generator Tree:  "  << bfsInfo[secondVertice - 1][2] << endl;

 }

void Grafos::PrintMatrix()
{
  unsigned index;
  bitset<1> *adjRow;
  unsigned j;

  for(index = 0; index < (unsigned) numVertices; ++index)
  {
     cout << index + 1 << "  ";
     adjRow = grafo[index].getVerticeMatrix();

     for(j = 0; j < (unsigned) numVertices; ++j)
         cout << adjRow[j] << " ";

     cout << endl;
  }
}

void Grafos::PrintList()
{
  list<int> linkedList;
  unsigned index;
  list<int>::iterator it;
  for (index = 0; index < (unsigned) numVertices; ++index)
  {
    cout << index + 1 << "  ";
    linkedList = grafo[index].GetVerticeList();
    for (it= linkedList.begin(); it != linkedList.end(); ++it)
       cout << *it << " ";
    cout << endl;
  }
}

void Grafos::GetInformation()
{
  vector<int> degrees;
  int degree;
  int totalDegrees = 0;
  int actualVertice = 0;
  unsigned index;
  int degree1, degree2;

  for (index = actualVertice; index < (unsigned) numVertices; ++index)
  {
    degree = GetNumAdjacenciesVertice(index);
    totalDegrees += degree;
    degrees.push_back(degree);
  }

  sort(degrees.begin(),degrees.end());

  if (numVertices%2 == 0)
  {
    degree1 = degrees[numVertices/2];
    degree2 = degrees[numVertices/2+1];
    this->avgDegree = (degree1 + degree2)/2;
  }
  else
    this->avgDegree = degrees[numVertices/2 +1];

     this->minDegree = degrees.front();
     this->maxDegree = degrees.back();
     this->medDegree = totalDegrees/numVertices;

}//;
