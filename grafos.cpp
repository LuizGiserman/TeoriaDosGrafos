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

  if ( type == 0){
      this->vertice.List = new list<int>;
  } else if (type == 1){
      this->vertice.Row = new bitset<1> [size];
  }
}

void Vertice::setVertice(int Vertice){

    if ( type == 0){
      vertice.List->push_back(Vertice);
  } else if (type == 1){
      vertice.Row[Vertice - 1 ].set();
  }

}


list<int>* Vertice::getVerticeList(){
    list<int>* listVertices;
    if ( type == 0){
      listVertices = vertice.List;
  } else if (type == 1){
      for (int i = 0; i < vertice.Row->size(); i++){
          if(vertice.Row[i] == 1){
              listVertices->push_back(i + 1);
          }
      }
  }
  return listVertices;
}

/*corrigir. N precisa de ponteiro. como ja sabemos o tamanho
 * da pra botar bitset<1> RowVertices [size] direto.
 */
bitset<1>* Vertice::getVerticeMatrix(){
    bitset<1>* RowVertices;
    if ( type == 0) {
        std::list<int>::iterator it;
        RowVertices = new bitset<1>[size];
        for (it = vertice.List->begin(); it != vertice.List->end(); ++it){
            RowVertices[*it - 1].set();
        }
    } else if (type == 1 ){
        RowVertices = vertice.Row;
    }
    return RowVertices;

}

bool Vertice::hasEdge(int Vertice){

    if ( type == 0){
        list<int>::iterator it;
        it = find(vertice.List->begin(),vertice.List->end(),Vertice);
        if (it != vertice.List->end()){
            return true;
        }

        return false;

    } else if (type == 1){
        if (vertice.Row[Vertice - 1] == 1){
            return true;
        }
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
    if(type == 0){
        Grafos::PrintList();
     } else if(type == 1){
        Grafos::PrintMatrix();
     }
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


int Grafos::numAdjacencyVertices(int Vertice){
    int numEdges = 0;
    list<int>* Vertices = grafo[Vertice]->getVerticeList();
    numEdges = Vertices->size();
    return numEdges;
};

 void Grafos::BFS(int initialVertice)
 {
    int** BFSinfo = new int* [numVertices];
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
        int Vertice = Queue.front();
        //take off the first element of Queue
        Queue.pop();
        if ( type == 0 ){
            // List of adjacents of Vertice
            list<int>* Vertices = grafo[Vertice-1]->getVerticeList();
            std::list<int>::iterator it;
            for (it = Vertices->begin(); it != Vertices->end(); ++it){
                if(BFSinfo[*it - 1][0] == 0){
                    //Mark Vertice
                    BFSinfo[*it - 1][0] = 1;
                    //Define Level of the father as the level of father plus 1
                    BFSinfo[*it - 1][1] = BFSinfo[Vertice-1][1] + 1;
                    // Define Vertice as father
                    BFSinfo[*it - 1][2] = Vertice;
                    Queue.push(*it);
                }
            }
        } else {
            // List of adjacents of Vertice
            bitset<1>* Vertices = grafo[Vertice - 1]->getVerticeMatrix();
            for (int i = 0; i < numVertices; i++){
                if(Vertices[i] == 1) {
                    if(BFSinfo[i][0] == 0){
                        //Mark Vertice
                        BFSinfo[i][0] = 1;
                        //Define Level of the father as the level of father plus 1
                        BFSinfo[i][1] = BFSinfo[Vertice-1][1] + 1;
                        // Define Vertice as father
                        BFSinfo[i][2] = Vertice;
                        Queue.push(i + 1);
                    }
                }
            }
        }

    }/*end While*/

    for (int i = 0; i < numVertices; i++){
        cout << i + 1 << "  ";
        cout << BFSinfo[i][1] << " " << BFSinfo[i][2];
        cout << endl;
    }
 }

 void Grafos::DFS(int initialVertice)
 {
   /*array to mark the vertices*/
   bitset<1> marked [numVertices];
   /*2 arrays to store the father and level of each vertice*/
   int father_level [numVertices][READINGS_SPT]; /*READINGS_SPT = 2*/
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
   bitset<1>  adjRow;

   /*others*/
   unsigned index;


   /*Marking all vertices as not found
    *setting level and father to -1 (nonexistent);
    */
   for (index = 0; index < numVertices; index++)
   {
     marked[index][0] = 0;
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

     switch(type)
     {
       /*if adjacency list*/
       case 0:
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
             auxStack.push(it);
             /*Also, update the father and level of the adjacent vertices*/
             father_level [it-1][0] = auxVertice;
             father_level [it-1][1] = father_level[auxVertice-1] [1] + 1
           }
         }
         break;

       /*if matrix*/
       case 1:

        if (marked[auxVertice-1].test(0) == false)
        {
          marked[auxVertice-1].set();
          adjRow = grafo[auxVertice-1].getVerticeMatrix();

          for (auto const &vertice: adjRow)
          {
            auxStack.push(vertice);
            father_level [vertice-1][0] = auxVertice;
            father_level [vertice-1][1] = father_level[auxVertice-1] [1] + 1
          }

        }
        break;
     }

   }/*end while*/


 }

void Grafos::createGrafo(int rows){
    Vertice** grafo = new Vertice* [rows];

    for (int i = 0; i < rows; ++i){
        grafo[i] = new Vertice(type,rows);
    }
    this->grafo=grafo;
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

    createGrafo(numVertices);


    /*Reading the edges. format: "Vertice1 Vertice2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {
        grafo[auxVertice1-1]->setVertice(auxVertice2);
        grafo[auxVertice2-1]->setVertice(auxVertice1);
        numEdges ++;
    }

    file.close();

    this -> numVertices = numVertices;
    this -> numEdges = numEdges;
}

 void Grafos::PrintMatrix(){
     for(int i = 0; i < numVertices; ++i){
         cout << i + 1 << "  ";
         bitset<1>* Row;
         Row = grafo[i]->getVerticeMatrix();
         for(int j = 0; j < numVertices; ++j ){
             cout << Row[j] << " ";
         }
         cout << endl;
     }
 }

  void Grafos::PrintList(){
     for(int i = 0; i < numVertices; ++i){
         cout << i + 1 << "  ";
         list<int>* List;
         List = grafo[i]->getVerticeList();
         for(list<int>::iterator i = List->begin(); i != List->end(); ++i ){
             cout << *i << " ";
         }
         cout << endl;
     }
 }


void Grafos::GetInformation() {
    vector<int> Degrees;
    int Degree;
    int totalDegrees = 0;
    int actualVertice = 0;
    for(int i = actualVertice; i < numVertices; ++i ){
        Degree = numAdjacencyVertices(i);
        totalDegrees += Degree;
        Degrees.push_back(Degree);
    }

     sort(Degrees.begin(),Degrees.end());

     if( Degrees.size()%2 == 0){
        int degree1 = Degrees[Degrees.size()/2];
        int degree2 = Degrees[Degrees.size()/2+1];

        this->avgDegree = (degree1 + degree2)/2;
    }
    else {
        this->avgDegree = Degrees[(Degrees.size()/2)+1];
    }


     this->minDegree = Degrees.front();
     this->maxDegree = Degrees.back();
     this->medDegree = totalDegrees/numVertices;

};
