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


Vertice::Vertice(int type, int size){

  this->type = type;
  this->size = size;

  if (type == 1)
      this->adjRow = new bitset<1> [size];
}

void Vertice::setVertice(int numVertice){

  if ( type == 0)
    adjList.push_back(numVertice);
  else if (type == 1)
    adjRow[numVertice - 1 ].set();

}


list<int> Vertice::getVerticeList(){
    list<int> listVertices;

    if ( type == 0)
      listVertices = adjList;
    else if (type == 1)
      for (int i = 0; i < adjRow->size(); i++)
          if(adjRow[i] == 1)
              listVertices.push_back(i + 1);
    return listVertices;
}

/*corrigir. N precisa de ponteiro. como ja sabemos o tamanho
 * da pra botar bitset<1> RowVertices [size] direto.
 */
bitset<1>* Vertice::getVerticeMatrix()
{
    bitset<1>* adjVertices;
    if (type == 0)
    {
        std::list<int>::iterator it;
        adjVertices = new bitset<1>[size];
        for (it = adjList.begin(); it != adjList.end(); ++it)
            adjVertices[*it - 1].set();
    }
    else if (type == 1 )
        adjVertices = adjRow;
    return adjVertices;
}

bool Vertice::hasEdge(int numberVertice){

    if ( type == 0)
    {
        list<int>::iterator it;
        it = find(adjList.begin(), adjList.end(), numberVertice);
        if (it != adjList.end())
            return true;

        return false;
    }
    else if (type == 1)
    {
        if (adjRow[numberVertice - 1] == 1)
            return true;
    }
        return false;
};


/*Constructor function*/
Grafos::Grafos(std::string fileName, int type){

    this->filename = fileName;

    // if 0 is an Adjacency List, if 1 is a Adjacency Matrix
    this->type = type;

    Grafos::Populate();
};

/* Print function, just for test it the constructor is okay */
void Grafos::Print(){
    if(type == 0)
      Grafos::PrintList();
    else if(type == 1)
      Grafos::PrintMatrix();
};

/* Print Informations function */
 void Grafos::PrintInformation(){

    Grafos::GetInformation();

     ofstream file;

     if(file){
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


int Grafos::numAdjacencyVertices(int numberVertice){
    int numAdj = 0;
    list<int> verticeList = grafo[numberVertice].getVerticeList();
    numAdj = verticeList.size();
    return numAdj;
};

void Grafos::BFSGenerica(int initialVertice, int** BFSinfo,int Stop /*=0*/, int StopVertice/*=0*/, int *diameter /*=0*/)
 {
     bitset<1> *matrixVertices;
     list<int> listVertices;
     int auxVertice;

     if (diameter != NULL)
        *diameter = 0;

    for (int i = 0; i < numVertices; ++i){
        BFSinfo[i] = new int[3]; // [0] marcado || nao marcado // [1] pai // [2] nivel na arvore
        BFSinfo[i][0] = 0;               // 0 é nao marcado || 1 é marcado
        BFSinfo[i][1] = numVertices + 1; // numVertices + 1 will be treat like infinity
        BFSinfo[i][2] = numVertices + 1; // numVertices + 1 will be treat like infinity
    }

    BFSinfo[initialVertice - 1][0] = 1; // initialVertice marked
    BFSinfo[initialVertice - 1][1] = 0; // root
    BFSinfo[initialVertice - 1][2] = 0; // no father

    queue<int> Queue; // Queue Created
    Queue.push(initialVertice); // initialVertice first element of list
    while(Queue.empty() != true){
        //get first element of Queue
        auxVertice = Queue.front();
        //take off the first element of Queue
        Queue.pop();
        if ( type == 0 ){
            // List of adjacents of Vertice
            listVertices = grafo[auxVertice-1].getVerticeList();
            std::list<int>::iterator it;
            for (it = listVertices.begin(); it != listVertices.end(); ++it)
            {
                if(BFSinfo[*it - 1][0] == 0)
                {
                    //Mark Vertice
                    BFSinfo[*it - 1][0] = 1;
                    //Define Level of the father as the level of father plus 1
                    BFSinfo[*it - 1][1] = BFSinfo[auxVertice-1][1] + 1;
                    // Define Vertice as father
                    BFSinfo[*it - 1][2] = auxVertice;
                    Queue.push(*it);

                    if(diameter != NULL)
                    {
                        cout << "before Updating: " << *diameter <<" BFSinfo[*it -1][1] : " <<  BFSinfo[*it -1][1] << endl;
                        if(*diameter < BFSinfo[*it -1][1])
                            *diameter = BFSinfo[*it -1][1];
                        cout << "new Diameter : " << *diameter << " " << endl;
                    }

                }
            }
        } else {
            // List of adjacents of Vertice
            matrixVertices = grafo[auxVertice - 1].getVerticeMatrix();
            for (int i = 0; i < numVertices; i++){
                if(matrixVertices[i] == 1) {
                    if(BFSinfo[i][0] == 0){
                        //Mark Vertice
                        BFSinfo[i][0] = 1;
                        //Define Level of the father as the level of father plus 1
                        BFSinfo[i][1] = BFSinfo[auxVertice-1][1] + 1;
                        // Define Vertice as father
                        BFSinfo[i][2] = auxVertice;
                        Queue.push(i + 1);

                        if(diameter != NULL)
                        {
                            cout << *diameter << " " << endl;
                            if(*diameter < BFSinfo[i][1])
                                *diameter = BFSinfo[i][1];
                            cout << *diameter << " " << endl;
                        }
                    }
                }
            }
        }
        // if(CountDiameter == 1){
        //     Diameter = BFSinfo[auxVertice-1][1] + 1;
        // }
        if(Stop == 1){
            if(auxVertice == StopVertice){
                return;
            }
        }
    }/*end while*/
 }

 void Grafos::BFS(int initialVertice){

     int** BFSinfo = new int* [numVertices];

     BFSGenerica(initialVertice,BFSinfo);

    cout << "vertice\tfather\tlevel" << endl;
    for (int i = 0; i < numVertices; i++){
        cout << i + 1 << "\t";
        cout << BFSinfo[i][2] << "\t" << BFSinfo[i][1];
        cout << endl;
    }
 }

 void Grafos::DFSGenerica(int initialVertice, int **father_level)
  {
    /*array to mark the vertices*/
    bitset<1> marked [numVertices];
    /*stack to implement the dfs*/
    stack<int> auxStack;
    /*int to put on the stack and keep track of the vertices*/
    int auxVertice;

    /*Type = 0 variables*/
    /*list to get the vertice's adjacency list*/
    list<int> adjacents;
    list<int>::iterator it;

    /*Type = 1 variables*/
    /*adjRow to get the row of adjacencies*/
    bitset<1>  *adjRow;

    /*others*/
    int index;


    /*Marking all vertices as not found
     *setting level and father to -1 (nonexistent);
     */
    for (index = 0; index < numVertices; index++)
    {
      marked [index] = 0;
      father_level[index] = new int [READINGS_SPT];
      father_level [index][0] = -1;
      father_level [index][1] = -1;
    }

    /*setting root and level of the starting point*/
    father_level [initialVertice-1][0] = 0;
    father_level [initialVertice-1][1] = 0;
    /*puts the value of the initial vertice in the stack*/
    auxStack.push(initialVertice);

    while (!auxStack.empty())
    {
        /*gets top of the stack*/
        auxVertice = auxStack.top();
        /*remove it from the stack*/
        auxStack.pop();
        /*if adjacency list*/
        if(type ==  0)
        {
          /*if auxVertice is not marked*/
          if (marked[auxVertice-1].test(0) == false)
          {
            /*marking vertice*/
            marked[auxVertice-1].set();
            /*Getting the adjacency list for the Vertice grafos[auxVertice -1]*/
            adjacents = grafo[auxVertice - 1].getVerticeList();
            /*for every adjacent vertice, add it to the stack*/
            for (it = adjacents.begin(); it != adjacents.end(); it++)
            {
              auxStack.push(*it);
              if(father_level[*it-1][0] == -1)
              {
                  /*Also, update the father and level of the adjacent vertices*/
                  father_level [*it-1][0] = auxVertice;
                  father_level [*it-1][1] = father_level[auxVertice-1] [1] + 1;
              }

            }
          }
        }

        /*if matrix*/
        else if(type == 1)
        {
            cout << auxVertice << endl;
            /*if u is not marked*/
            if (marked[auxVertice-1].test(0) == false)
            {
                /*mark it*/
                marked[auxVertice-1].set();
                adjRow = grafo[auxVertice-1].getVerticeMatrix();

                /*for every adjacency*/
                for (index = numVertices - 1; index >= 0; --index)\
                    // cout << "in for" << index << endl;
                   if (adjRow[index] == 1)
                   {
                       /*put every adjacency in the stack*/
                       /*if it doesn't have a father, add it*/
                       auxStack.push(index + 1);
                       if (father_level[index][0] == -1)
                       {
                           father_level[index][0] = auxVertice;
                           father_level[index][1] = father_level[auxVertice-1][1] + 1;
                       }

                   }
            }
    }

    }/*end while*/

  }

void Grafos::DFS (int initialVertice)
{

    int** father_level = new int* [numVertices];

    DFSGenerica(initialVertice, father_level);

    cout << "vertice\tfather\tlevel\t" <<endl;
    for (int i = 0; i < numVertices; i++)
    {
        cout << i + 1 << "\t";
        cout << father_level[i][0] << "\t" << father_level[i][1];
        cout << endl;
    }
}

void Grafos::GetDiameter(){
    int maxDiameter = 0;
    int diameter;
    int** BFSinfo = new int* [numVertices];
    for(int i = 0; i < numVertices; i++)
    {
        diameter = 0;
        BFSGenerica(i + 1, BFSinfo, 0, 0, &diameter);
        if( diameter > maxDiameter)
            maxDiameter = diameter;

    }
        cout << "The diameter of the Graph is " << maxDiameter << endl;
}



void Grafos::ConnectedComponents(){

    list<int> listVertices;
    std::list<int>::iterator arrayPointer [numVertices];
    std::list<int>::iterator it;

    for(int i = 0; i < numVertices; i++)
        listVertices.push_back(i + 1);



    for (it = listVertices.begin(); it != listVertices.end(); ++it)
        arrayPointer[*it - 1] = it;

    listVertices.erase(arrayPointer[0]);
    listVertices.erase(arrayPointer[2]);
    listVertices.erase(arrayPointer[4]);

    for (it = listVertices.begin(); it != listVertices.end(); ++it)
        cout << *it << endl;

 }

  void Grafos::Distance(int firstVertice, int secondVertice){

     int** BFSinfo = new int* [numVertices];

     BFSGenerica(firstVertice,BFSinfo,1,secondVertice);

        cout << "BFS from " << firstVertice << " to " << secondVertice << endl;
        cout << endl;
        cout << "Distance:  " << BFSinfo[secondVertice - 1][1] << endl;
        cout << "Father in Generator Tree:  "  << BFSinfo[secondVertice - 1][2] << endl;
 }



void Grafos::createGrafo()
{
   Vertice *auxVertice = new Vertice(type, numVertices);
   for (int i = 0; i < numVertices; ++i)
       grafo.push_back(*auxVertice);
}

/* Coloca os dados no grafo */
void Grafos::Populate()
{
    int numVertices;
    int numEdges;
    int auxVertice1;
    int auxVertice2;
    std::ifstream file;
    bitset<1>** matrix;

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

    createGrafo();


    /*Reading the edges. format: "Vertice1 Vertice2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {
        grafo[auxVertice1-1].setVertice(auxVertice2);
        grafo[auxVertice2-1].setVertice(auxVertice1);
        numEdges ++;
    }

    file.close();

    this -> numVertices = numVertices;
    this -> numEdges = numEdges;
}

 void Grafos::PrintMatrix()
 {
     bitset<1>* auxRow;
     for(int i = 0; i < numVertices; ++i)
     {
         cout << i + 1 << "  ";
         auxRow = grafo[i].getVerticeMatrix();
         for(int j = 0; j < numVertices; ++j)
             cout << auxRow[j] << " ";
         cout << endl;
     }
 }

  void Grafos::PrintList()
  {
    list<int> auxList;
     for(int i = 0; i < numVertices; ++i)
       {
         cout << i + 1 << "  ";
         auxList = grafo[i].getVerticeList();
         for(list<int>::iterator i = auxList.begin(); i != auxList.end(); ++i )
             cout << *i << " ";
         cout << endl;
     }
 }


void Grafos::GetInformation() {
    vector<int> degrees;
    int degree;
    int totalDegrees = 0;
    int actualVertice = 0;
    for(int i = actualVertice; i < numVertices; ++i )
    {
        degree = numAdjacencyVertices(i);
        totalDegrees += degree;
        degrees.push_back(degree);
    }

     sort(degrees.begin(), degrees.end());

     if(degrees.size()%2 == 0)
     {
        int degree1 = degrees[degrees.size()/2];
        int degree2 = degrees[degrees.size()/2+1];

        this->avgDegree = (degree1 + degree2)/2;
    }
    else
        this->avgDegree = degrees[(degrees.size()/2)+1];


     this->minDegree = degrees.front();
     this->maxDegree = degrees.back();
     this->medDegree = totalDegrees/numVertices;

};
