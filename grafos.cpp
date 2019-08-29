/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/
#include <algorithm>
#include "grafos.h"
#include <bitset>

using namespace std;


Vertice::Vertice(int type, int size){

  this->type = type;

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

void Vertice::getVertice(){

    if ( type == 0){
      getVerticeList();
  } else if (type == 1){
      getVerticeMatrix();
  }
}

list<int>* Vertice::getVerticeList(){
    return vertice.List;
}

bitset<1>* Vertice::getVerticeMatrix(){
    return vertice.Row;
    
}


bool Vertice::hasEdge(int Vertice){

    if ( type == 0){
        list<int>::iterator it;
        it = find(vertice.List->begin(),vertice.List->end(),Vertice);
        if (it != vertice.List->end()){
            return true;
        } else {
            return false;
        };
    } else if (type == 1){
        if (vertice.Row[Vertice - 1] == 1){
            return true;
        } else {
            return false;
        };
  };
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
/*
 void Grafos::PrintInformation(){

     if(type == 0){
        Grafos::getInformationList();
     } else if(type == 1){
        Grafos::getInformationMatrix();
     }

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

 }
*/

 void Grafos::BFS(int initialVertice)
 {
     if(type == 0){
         Grafos::BFSList(initialVertice);
     } else if(type == 1){
         Grafos::BFSMatrix(initialVertice);
     }
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

    this -> grafo = grafo;
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

 int Grafos::Edges(int Vertice){
    int numEdges = 0;
    if ( type == 0){
         bitset<1>* Row;
         Row = grafo[Vertice]->getVerticeMatrix();
         for(int j = 0; j < numVertices; ++j ){
             numEdges++;
         }
    } else if (type == 1){
        list<int>* List;
         List = grafo[Vertice]->getVerticeList();
         for(list<int>::iterator i = List->begin(); i != List->end(); ++i ){
             numEdges++;
         }
    return numEdges;
}

 void Grafos::getInformation(){
     vector<int> Degrees;
     int totalDegrees = 0;
     int actualVertice = 0;
     for(int i = actualVertice; i < numVertices; ++i ){
        int Degree = grafo.Edges(i);
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

 }


void Grafos::BFSMatrix(int initialVertice)
{ int** vertices = new int* [numVertices];
    for (int i = 0; i < numVertices; ++i){
        vertices[i] = new int[3]; // [0] marcado || nao marcado // [1] pai // [2] nivel na arvore
        vertices[i][0] = 0;               // 0 é nao marcado || 1 é marcado
        vertices[i][1] = numVertices + 1; // numVertices + 1 will be treat like infinity
        vertices[i][3] = numVertices + 1; // numVertices + 1 will be treat like infinity
    }

    vertices[initialVertice][0] = 0; // initialVertice marked
    vertices[initialVertice][0] = 0; // root
    vertices[initialVertice][0] = 0; // no father

    vector<int> Queue; // Queue Created
    Queue.push_back(initialVertice); // initialVertice first element of list


}

void Grafos::BFSList(int initialVertice)
{

 }
