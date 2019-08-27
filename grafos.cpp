/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/
#include <algorithm>
#include "grafos.h"
#include <bitset>

using namespace std;

/*Constructor function*/
Grafos::Grafos(std::string fileName, int type){

    this->filename = fileName;

    // if 0 is an Adjacency List, if 1 is a Adjacency Matrix
    this->type = type;

    if(type == 0){
        Grafos::List();
     } else if(type == 1){
        Grafos::Matrix();
     }
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


 void Grafos::BFS(int initialVertice)
 {
     if(type == 0){
         Grafos::BFSList(initialVertice);
     } else if(type == 1){
         Grafos::BFSMatrix(initialVertice);
     }
 }

std::list<int> *Grafos::AllocateVectorOfLists()
{
  std::list<int> *vectorGraph = new std::list<int> [numEdges];
  unsigned index ;
  std::list<int> list;
  list.push_back(-1);

  for (index = 0; index < numEdges; index++)
    vectorGraph[index] = list;
  // for (auto& list : vectorGraph)
  //   list = NULL;
  return vectorGraph;
}

void Grafos::List ()
{
    std::list<int> *vectorGraph;
    /*Auxiliar variables*/
    std::list<int> linkedList1, linkedList2;
    std::string auxiliar;
    std::ifstream file;
    int auxVertice1, auxVertice2;
    int index;

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

    /*Allocating memory for the array of lists and setting all positions to NULL*/
    vectorGraph = AllocateVectorOfLists();

    /*Reading the edges. format: "Edge1 Edge2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {

        /*Check if the vertice is already in the list (its position would be the same as its "name" (number or index) )*/
        /*If it isn't there, create a linked list adding its corresponding vertice to the front and the other one to the end
         * The list will be stored at the corresponding position (index or number) vector[verticeNumber]
         */
        if (vectorGraph[auxVertice1].front() == -1)
        {
          linkedList1.push_back (auxVertice1);
          linkedList1.push_back (auxVertice2);
          vectorGraph[auxVertice1] = linkedList1;
          linkedList1.clear();
        }
        /*If it is there, then just add the other one to the end of the already existing linked list at vector[verticeNumber]*/
        else
          vectorGraph[auxVertice1].push_back(auxVertice2);

        /*Do the same for the other vertice*/
        if (vectorGraph[auxVertice2].front() == -1)
        {
          linkedList1.push_back (auxVertice2);
          linkedList1.push_back (auxVertice1);
          vectorGraph[auxVertice2] = linkedList1;
          linkedList1.clear();
        }
        else
          vectorGraph[auxVertice2].push_back(auxVertice1);

        /*Every line of the file represents an adge*/
        numEdges ++;

    }
    file.close();
    this -> vectorGraph = vectorGraph;
}

bitset<1> **Grafos::generateSquareMatrix(int rows){
    bitset<1>** matrix = new bitset<1>* [rows];
    for (int i = 0; i < rows; ++i){
        matrix[i] = new bitset<1> [rows];
    }

    return matrix;
}

/* Matrix COnstructor */
void Grafos::Matrix()
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

    matrix = generateSquareMatrix(numVertices);

    /*Reading the edges. format: "Vertice1 Vertice2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {
        matrix[auxVertice1-1][auxVertice2-1].set();
        matrix[auxVertice2-1][auxVertice1-1].set();
        numEdges ++;
    }

    file.close();

    this -> matrix = matrix;
    this -> numVertices = numVertices;
    this -> numEdges = numEdges;
}

 void Grafos::PrintMatrix(){
     for(int i = 0; i < numVertices; ++i){
         for(int j = 0; j < numVertices; ++j ){
             cout << matrix[i][j] << " ";
         }
         cout << endl;
     }
 }

 void Grafos::getInformationMatrix(){
     vector<int> Degrees;
     int totalDegrees = 0;
     int actualVertice = 0;
     for(int i = actualVertice; i < numVertices; ++i ){
         int Degree = 0;
         for(int j = actualVertice; j < numVertices; ++j ){
            if(matrix[i][j] == 1){
                Degree++;
            }
         }
        totalDegrees += Degree;
        Degrees.push_back(Degree);
     }

     sort(Degrees.begin(),Degrees.end());

     if(Degrees.size()%2 == 0){
        int degree1 = Degrees[Degrees.size()/2];
        int degree2 = Degrees[Degrees.size()/2+1];

        this->avgDegree = (degree1 + degree2)/2;
    }
    else{
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
