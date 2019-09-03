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

Vertice::~Vertice()
{
  delete [] adjRow;
}

void Vertice::setVertice(int numVertice){

  if ( type == 0)
    adjList.push_back(numVertice);
  else if (type == 1)
    adjRow[numVertice - 1 ].set();

}


vector<int> Vertice::getVerticeList(){
    vector<int> listVertices;
    unsigned i;

    if ( type == 0)
      listVertices = adjList;
    else if (type == 1)
      for (i = 0; i < adjRow->size(); i++)
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
        std::vector<int>::iterator it;
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
        vector<int>::iterator it;
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

// Components Struct Implemantation

Components::Components(){
    size = 0;
};

void Components::InsertVertice(int Vertice){
    listComp.push_back(Vertice);
    size++;
}

bool compare(const Components &comp1, const Components &comp2){
    return comp1.size > comp2.size;
}

/************************************************************/

/*Constructor function*/
Grafos::Grafos(std::string fileName, int type){

    this->filename = fileName;

    // if 0 is an Adjacency List, if 1 is a Adjacency Matrix
    this->type = type;

    Grafos::Populate();
};

Grafos::~Grafos()
{
}

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
/***/
 };


int Grafos::numAdjacencyVertices(int numberVertice){
    int numAdj = 0;
    vector<int> verticeList = grafo[numberVertice].getVerticeList();
    numAdj = verticeList.size();
    return numAdj;
};

/*overloading*/
// void Grafos::BFSGenerica (int initialVertice, int** BFSinfo,
//                          Components *auxComponent,
//                          int BFStype,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
//                          int StopVertice, int *diameter,
//                          std::list<int>::iterator *arrayPointer)
// {
//  list<int> listVerticesforCC;
//  BFSGenerica (initialVertice, BFSinfo,
//                            auxComponent,
//                            BFStype,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
//                            StopVertice, int diameter,
//                            arrayPointer,
//                            listVerticesforCC)
//
// }

void Grafos::BFSGenerica(int initialVertice, int** bfsInfo, Components *auxComponent, int BFStype /*=0*/, int StopVertice/*=0*/, int *diameter /*=0*/,std::list<int>::iterator *arrayPointer/* = {}*/, list<int> *listVerticesforCC/*= {0}*/)
 {
     bitset<1> *matrixVertices;
     vector<int> listVertices;
     int auxVertice;
     queue<int> auxQueue; // Queue Created

     if (diameter != NULL)
        *diameter = 0;

    for (int i = 0; i < numVertices; ++i)
    {
        bfsInfo[i] = new int[READINGS_SPT + 1]; // [0] marcado || nao marcado // [1] pai // [2] nivel na arvore
        bfsInfo[i][0] = 0;               // 0 é nao marcado || 1 é marcado
        bfsInfo[i][1] = numVertices + 1; // numVertices + 1 will be treat like infinity
        bfsInfo[i][2] = numVertices + 1; // numVertices + 1 will be treat like infinity
    }

    bfsInfo[initialVertice - 1][0] = 1; // initialVertice marked
    bfsInfo[initialVertice - 1][1] = 0; // root
    bfsInfo[initialVertice - 1][2] = 0; // no father

    auxQueue.push(initialVertice); // initialVertice first element of list

    while(auxQueue.empty() != true)
    {
        //get first element of Queue
        auxVertice = auxQueue.front();
        //take off the first element of Queue
        auxQueue.pop();

        if (BFStype == 3){
            listVerticesforCC->erase(arrayPointer[auxVertice - 1]);
            auxComponent->InsertVertice(auxVertice); /*also doing size++*/
        }

        if ( type == 0 )
        {
            // List of adjacents of Vertice
            listVertices = grafo[auxVertice-1].adjList;
            for(auto const &it : listVertices)
            {
                if(bfsInfo[it - 1][0] == 0)
                {
                    //Mark Vertice
                    bfsInfo[it - 1][0] = 1;
                    //Define Level of the father as the level of father plus 1
                    bfsInfo[it - 1][1] = bfsInfo[auxVertice-1][1] + 1;
                    // Define Vertice as father
                    bfsInfo[it - 1][2] = auxVertice;
                    auxQueue.push(it);

                    if(diameter != NULL)
                    {
                        if(*diameter < bfsInfo[it -1][1])
                            *diameter = bfsInfo[it -1][1];
                    }

                }
            }
        }
        else
        {
            // List of adjacents of Vertice
            matrixVertices = grafo[auxVertice - 1].adjRow;
            for (int i = 0; i < numVertices; i++)
            {
                if(matrixVertices[i] == 1)
                {
                    if(bfsInfo[i][0] == 0)
                    {
                        //Mark Vertice
                        bfsInfo[i][0] = 1;
                        //Define Level of the father as the level of father plus 1
                        bfsInfo[i][1] = bfsInfo[auxVertice-1][1] + 1;
                        // Define Vertice as father
                        bfsInfo[i][2] = auxVertice;
                        auxQueue.push(i + 1);

                        if(diameter != NULL)
                        {
                            cout << *diameter << " " << endl;
                            if(*diameter < bfsInfo[i][1])
                                *diameter = bfsInfo[i][1];
                            cout << *diameter << " " << endl;
                        }
                    }
                }
            }
        }
        // if(CountDiameter == 1){
        //     Diameter = bfsInfo[auxVertice-1][1] + 1;
        // }
        if(BFStype == 1){
            if(auxVertice == StopVertice){
                return;
            }
        }
    }/*end while*/
 }

 void Grafos::BFS(int initialVertice, int search){

     int** bfsInfo = new int* [numVertices];
     ofstream file;

     BFSGenerica(initialVertice, bfsInfo);

    if (search)
    { 
      string fileoutput = "./pais102030" + filename;
      file.open(fileoutput);
      file << "filename: " << filename << endl;
      file << "initialVertice: " << initialVertice << endl;
      file << "vertice\t father\t" << endl;
      file << "10\t" << bfsInfo[10][2] << "\t" << endl;
      file << "20\t" << bfsInfo[20][2] << "\t" << endl;
      file << "30\t" << bfsInfo[30][2] << "\t" << endl;
    }
    // cout << "vertice\tfather\tlevel" << endl;
    // for (int i = 0; i < numVertices; i++){
    //     cout << i + 1 << "\t";
    //     cout << bfsInfo[i][2] << "\t" << bfsInfo[i][1];
    //     cout << endl;
    //}
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
    vector<int> adjacents;

    /*Type = 1 variables*/
    /*adjRow to get the row of adjacencies*/
    bitset<1> *adjRow;

    /*others*/
    int index;


    /*Marking all vertices as not found
     *setting level and father to -1 (nonexistent);
     */
    for (index = 0; index < numVertices; index++)
    {
      father_level[index] = new int [READINGS_SPT];
      father_level [0][index] = -1;
      father_level [1][index] = -1;
    }
    memset(marked, 0, numVertices);

    /*setting root and level of the starting point*/
    father_level [0][initialVertice-1] = 0;
    father_level [1][initialVertice-1] = 0;
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
            for (auto const &it : adjacents)
            {
              auxStack.push(it);
              if(father_level[0][it-1] == -1)
              {
                  /*Also, update the father and level of the adjacent vertices*/
                  father_level [0][it-1] = auxVertice;
                  father_level [1][it-1] = father_level[auxVertice-1] [1] + 1;
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
                       if (father_level[0][index] == -1)
                       {
                           father_level[0][index] = auxVertice;
                           father_level[1][index] = father_level[auxVertice-1][1] + 1;
                       }

                   }
            }
    }

    }/*end while*/

  }

void Grafos::DFS (int initialVertice)
{

    int** father_level = new int* [READINGS_SPT + 1];
    // int father_level [numVertices][READINGS_SPT + 1];

    DFSGenerica(initialVertice, father_level);

    cout << "vertice\tfather\tlevel\t" <<endl;
    for (int i = 0; i < numVertices; i++)
    {
        cout << i + 1 << "\t";
        cout << father_level[0][i] << "\t" << father_level[1][i];
        cout << endl;
    }
}

void Grafos::GetDiameter(){

    ofstream file;

    int maxDiameter = 0;
    int diameter;
    int** bfsInfo = new int* [numVertices];
    for(int i = 0; i < numVertices; i++)
    {
        diameter = 0;
        BFSGenerica(i + 1, bfsInfo, NULL, 0, 0, &diameter, NULL, NULL);
        if( diameter > maxDiameter)
            maxDiameter = diameter;

    }
        string fileoutput = "./DiameterIN" + filename;
        file.open(fileoutput);
        file << "filename: " << filename << endl;
        file << "The diameter of the Graph is " << maxDiameter << endl;
}



void Grafos::ConnectedComponents(int search){

    ofstream file;

    int auxVertice;
    int** bfsInfo = new int* [numVertices];

    std::list<int> listVerticesforCC;
    std::list<int>::iterator *arrayPointer; /*holds the pointers to the listVerticesforCC vertices*/
    std::list<int>::iterator itVertices;

    list<Components> listComponents; /*list that holds the vertices of all CCs */
    std::list<Components>::iterator itComponents;
    Components *auxComponent; /*Will hold a different CC in each iteration of the loop*/

    // listVerticesforCC = new std::list<int> [numVertices];
    /*initially, set all vertices to not found*/
    for(int i = 0; i < numVertices; i++)
        listVerticesforCC.push_back(i + 1);

    arrayPointer = new std::list<int>::iterator [numVertices];
    /*make every iterator point to its respective vertice*/
    for (itVertices = listVerticesforCC.begin(); itVertices != listVerticesforCC.end(); ++itVertices)
        arrayPointer[*itVertices - 1] = itVertices;
    /*while there are vertices that haven't been visited yet*
     * - Pop them so we can visit the first of them next!
     */
    while(!listVerticesforCC.empty())
    {
        auxVertice = listVerticesforCC.front();

        auxComponent = new Components();
        auxComponent->size = 0;

        /*find the CC*/
        BFSGenerica(
            auxVertice,
            bfsInfo,
            auxComponent,
/*BFSTyoe*/     3,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
/*Stopvertice*/ 0,
/*Diameter */   NULL,
            arrayPointer,
            &listVerticesforCC
        );
        listComponents.push_back(*auxComponent);
        delete auxComponent;
    }

    // Sorting List using Lambda Function as comparator
    listComponents.sort(compare);

    if (search == 1)
    {
    string fileoutput = "./CCin" + filename;
      file.open(fileoutput);
      file << "filename : " << filename << endl;
      for (itComponents = listComponents.begin(); itComponents != listComponents.end(); ++itComponents)
      {
        file << "Size: "<< itComponents->size << endl;
        for(itVertices = itComponents->listComp.begin(); itVertices != itComponents->listComp.end(); ++itVertices)
        {
          file << *itVertices << "->";
        }
        file << endl;
        file << endl;
      }
    }


 }

void Grafos::Distance(int firstVertice, int secondVertice, int search)
{

   int** bfsInfo = new int* [numVertices];
   ofstream file;
   BFSGenerica(firstVertice, bfsInfo, NULL , 1, secondVertice, NULL, NULL, NULL);

   if(search)
   {
    string fileoutput = "./distancein" + filename + "from" + to_string(firstVertice) + "to" + to_string(secondVertice) + ".txt";
     file.open(fileoutput);
     file << "fileName: " << filename;
     file << "BFS from " << firstVertice << " to " << secondVertice << endl;
     file << endl;
     file << "Distance:  " << bfsInfo[secondVertice - 1][1] << endl;
     file << "Father in Generator Tree:  "  << bfsInfo[secondVertice - 1][2] << endl;

   }

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

    createGrafo();


    /*Reading the edges. format: "Vertice1 Vertice2"*/
    while (file >> auxVertice1 >> auxVertice2)
    {
        grafo[auxVertice1-1].setVertice(auxVertice2);
        grafo[auxVertice2-1].setVertice(auxVertice1);
        numEdges ++;
    }

    file.close();
}

 void Grafos::PrintMatrix()
 {
     for(int i = 0; i < numVertices; ++i)
     {
         cout << i + 1 << "  ";
         for(int j = 0; j < numVertices; ++j)
           cout << grafo[i].getVerticeMatrix()[j] << " ";

         cout << endl;
     }
 }

  void Grafos::PrintList()
  {
    vector<int> auxList;
     for(int i = 0; i < numVertices; ++i)
       {
         cout << i + 1 << "  ";
         auxList = grafo[i].getVerticeList();
         for(vector<int>::iterator i = auxList.begin(); i != auxList.end(); ++i )
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
