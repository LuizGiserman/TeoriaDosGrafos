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
    
    bitset<1>** matrix = generateSquareMatrix(numVertices);

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