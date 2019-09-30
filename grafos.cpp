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

Edge::Edge(int connectedVertice, int weight)
{
    this->weight = weight;
    this->connectedVertice = connectedVertice;
}

Vertice::Vertice(int type, int size, bool hasWeight)
{

  this->type = type;
  this->size = size;
  this->hasWeight = hasWeight;

  /*allocating memory beforehand*/
  if (type == 1)
    switch(hasWeight)
    {
        case false:
            adjRow.resize(size);
            break;
        case true:
            adjRowWeight.resize(size);
            break;
    }

}

void Vertice::setVertice(int numVertice, float weight)
{

    // if weight = 0
    if (hasWeight)
        switch (type)
        {
            case 0: /*list*/
                adjListWeight.push_back(Edge(numVertice, weight));
                break;
            case 1: /*matrix*/
                adjRowWeight[numVertice-1] = weight;
                break;
        }
    else
        switch (type)
        {
            case 0: /*list*/
                adjList.push_back(numVertice);
                break;

            case 1: /*matrix*/
                adjRow[numVertice-1].set();
                break;
        }


}

bool Vertice::HasEdge(int numberVertice){
  vector<int>::iterator it;

    if ( type == 0)
    {
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

void Components::InsertVertice(int Vertice)
{
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

void Grafos::BFSGenerica(int initialVertice, int** bfsInfo, Components *auxComponent, int BFStype /*=0*/, int stopVertice/*=0*/, int *diameter /*=0*/,std::list<int>::iterator *arrayPointer/* = {}*/, list<int> *listVerticesforCC/*= {0}*/)
 {
     vector < bitset<1> > matrixVertices;
     vector<int> listVertices;
     int auxVertice;
     queue<int> auxQueue; // Queue Created

     if (diameter != NULL)
       *diameter = 0;


    for (int i = 0; i < numVertices; ++i)
    {
        bfsInfo[i] = new int[READINGS_SPT + 1]; // [0] marcado || nao marcado // [1] pai // [2] nivel na arvore
        bfsInfo[i][0] = 0;               // 0 é nao marcado || 1 é marcado
        bfsInfo[i][1] = -1; // numVertices + 1 will be treat like infinity
        bfsInfo[i][2] = -1; // numVertices + 1 will be treat like infinity
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
                            if(*diameter < bfsInfo[i][1])
                                *diameter = bfsInfo[i][1];
                    }
                }
            }
        }

        if(BFStype == 1){
            if(auxVertice == stopVertice){
                return;
            }
        }
    }/*end while*/

 }

 void Grafos::BFS(int initialVertice, int search)
 {

     int** bfsInfo = new int* [numVertices];
     ofstream file;

     BFSGenerica(initialVertice, bfsInfo);

    if (search)
    {
      string fileoutput = "./paisInBFS102030" + to_string(initialVertice) + filename;
      file.open(fileoutput);
      file << "filename: " << filename << endl;
      file << "initialVertice: " << initialVertice << endl;
      file << "vertice\t father\t" << endl;
      file << "10\t" << bfsInfo[10-1][2] << "\t" << endl;
      file << "20\t" << bfsInfo[20-1][2] << "\t" << endl;
      file << "30\t" << bfsInfo[30-1][2] << "\t" << endl;
      file.close();
    }

    for(int i = 0; i < numVertices; ++i)
        delete[] bfsInfo[i];
    delete[] bfsInfo;
 }


 void Grafos::DFSGenerica(int initialVertice, int **father_level)
  {
    /*array to mark the vertices*/
    bitset<1> *marked;
    marked = new bitset<1> [numVertices];    /*stack to implement the dfs*/
    stack<int> auxStack;
    /*int to put on the stack and keep track of the vertices*/
    int auxVertice;

    /*Type = 0 variables*/
    /*list to get the vertice's adjacency list*/
    vector<int> adjacents;

    /*Type = 1 variables*/
    /*adjRow to get the row of adjacencies*/
    vector < bitset<1> > adjRow;

    /*others*/
    int index;

    /*Marking all vertices as not found
     *setting level and father to -1 (nonexistent);
     */
    for (index = 0; index < numVertices; index++)
    {
      // marked [index] = 0;
      father_level[index] = new int [READINGS_SPT];
      father_level [index][0] = -1;
      father_level [index][1] = -1;
    }
    memset(marked, 0, numVertices);
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
            adjacents = grafo[auxVertice - 1].adjList;
            /*for every adjacent vertice, add it to the stack*/
            for (auto const &it: adjacents)
            {
              auxStack.push(it);
              if(father_level[it-1][0] == -1)
              {
                  /*Also, update the father and level of the adjacent vertices*/
                  father_level [it-1][0] = auxVertice;
                  father_level [it-1][1] = father_level[auxVertice-1] [1] + 1;
              }

            }
          }
        }

        /*if matrix*/
        else if(type == 1)
        {
            /*if u is not marked*/
            if (marked[auxVertice-1].test(0) == false)
            {
                /*mark it*/
                marked[auxVertice-1].set();
                adjRow = grafo[auxVertice-1].adjRow;

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

void Grafos::DFS (int initialVertice, int search)
{
    int** father_level = new int* [numVertices];
    ofstream file;
    string fileoutput = "./paisInDFS" + to_string(initialVertice) + filename;

    DFSGenerica(initialVertice, father_level);

    if (search)
    {
      file.open(fileoutput);
      file << "filename: " << filename << endl;
      file << "initialVertice: " << initialVertice << endl;
      file << "vertice\t father\t" << endl;
      file << "10\t" << father_level[10-1][0] << "\t" << endl;
      file << "20\t" << father_level[20-1][0] << "\t" << endl;
      file << "30\t" << father_level[30-1][0] << "\t" << endl;
      file.close();
    }

    for(int i = 0; i < numVertices; ++i)
        delete[] father_level[i];
    delete[] father_level;
}

/*Overload for the Distance between 2 specific vertices function*/
float Grafos::Dijkstra (int initialVertice, int secondVertice, vector <int> &father)
{
    vector <float> distance;
    return Dijkstra(initialVertice, father, distance, secondVertice);
}

/*Overload for the regular Dijkstra (getting distances) */
void Grafos::Dijkstra (int initialVertice, vector <float> &distance)
{
    int secondVertice = 0;
    vector <int> father;
    Dijkstra (initialVertice, father, distance, secondVertice);
}

/*Overload for simple (prints fathers, doesnt returnt it) Dijkstra*/
void Grafos::Dijkstra (int initialVertice)
{
    int secondVertice = 0;
    vector <int> father;
    vector <float> distance;
    Dijkstra (initialVertice, father, distance, secondVertice);
    /*Print fathers*/
}

/*Overload: return fathers*/
void Grafos::Dijkstra (int initialVertice, vector <int> &father)
{
    int secondVertice = 0;
    vector <float> distance;
    Dijkstra (initialVertice, father, distance, secondVertice);
}


/*Actual Dijkstra*/
float Grafos::Dijkstra (int initialVertice, vector <int> &father, vector <float> &distance, int secondVertice)
{
    vector<int> level;

    father.resize(numVertices, -1);
    level.resize(numVertices, -1);
    distance.resize(numVertices, INFINITY);

    /*auxiliar Vertice*/
    int verticeID;

    /*min heap to get the min distance in O(1) + O(logn)*/
    priority_queue < dist_vertice, vector<dist_vertice>, greater<dist_vertice> > distanceHeap;

    int index;

    /*setting the initial values*/
    distance[initialVertice - 1] = 0;
    distanceHeap.push(make_pair(0, initialVertice - 1));

    while(!distanceHeap.empty())
    {
    /*since I am adding the initialVertice - 1 to the heap, I don't need to subtract 1 here*/
        verticeID = distanceHeap.top().second; /*vertice ID = u*/
        distanceHeap.pop();

        /*Searching for distance between 2 specific vertices*/
        if (secondVertice != 0 && verticeID == secondVertice - 1)
            return distance[secondVertice - 1];

        if (type == LIST_TYPE)
        {
            for (auto const &v: grafo[verticeID].adjListWeight)
          /*if dist[v] > dist[u] + w((u,v)) then*/
                if (distance[v.connectedVertice - 1] > distance[verticeID] + v.weight)
                {
                  distance[v.connectedVertice - 1]  = distance[verticeID] + v.weight;
                  distanceHeap.push(make_pair(distance[v.connectedVertice - 1], v.connectedVertice -1));
                  father[v.connectedVertice -1] = verticeID + 1;
                  // cout << "Pai de " << v.connectedVertice << " = " << father[v.connectedVertice - 1] << endl;
                }
        }
        else if(type == MATRIX_TYPE)
        {
            for (index = 0; index < numVertices; index++)
                if (grafo[verticeID].adjRowWeight[index] != 0)
                    if(distance[index] > distance[verticeID] + grafo[verticeID].adjRowWeight[index])
                    {
                      distance[index] = distance[verticeID] + grafo[verticeID].adjRowWeight[index];
                      distanceHeap.push(make_pair(distance[index], index));
                      father[index] = verticeID + 1;
                      // cout << "Pai de " <<  index + 1 << " = " << father[index] << endl;
                    }
        }
    }

    if (secondVertice != 0)
        return -1;
    return 0;

}


void Grafos::GetDiameter(){

    ofstream file;
    string fileoutput = "./DiameterIN" + filename;

    int maxDiameter = 0;
    int diameter;
    int** bfsInfo = new int* [numVertices];

    for(int i = 0; i < numVertices; i++)
    {

        diameter = 0;
        BFSGenerica(i + 1, bfsInfo, NULL, 0, 0, &diameter, NULL, NULL);
        if( diameter > maxDiameter)
            maxDiameter = diameter;

        /*clear bfsInfo from previous iteration*/
        memset(bfsInfo, 0, sizeof(bfsInfo[0][0]) * numVertices * (READINGS_SPT + 1));
    }
        file.open(fileoutput);
        file << "filename: " << filename << endl;
        file << "The diameter of the Graph is " << maxDiameter << endl;
        file.close();

    for(int i = 0; i < numVertices; ++i)
        delete[] bfsInfo[i];
    delete[] bfsInfo;
}



void Grafos::ConnectedComponents(int search){

    ofstream file;
    string fileoutput = "./CCin" + filename;
    int count = 0;
    int maxSize = 0;
    int minSize = numVertices;
    int auxVertice;
    int** bfsInfo = new int* [numVertices];

    std::list<int> listVerticesforCC;
    std::list<int>::iterator arrayPointer[numVertices]; /*holds the pointers to the listVerticesforCC vertices*/
    std::list<int>::iterator itVertices;

    list<Components> listComponents; /*list that holds the vertices of all CCs */
    Components auxComponent; /*Will hold a different CC in each iteration of the loop*/
    list<Components>::iterator itComponents;
    // listVerticesforCC = new std::list<int> [numVertices];
    /*initially, set all vertices to not found*/
    for(int i = 0; i < numVertices; i++)
        listVerticesforCC.push_back(i + 1);

    // arrayPointer = new std::list<int>::iterator [numVertices];
    /*make every iterator point to its respective vertice*/
    for (itVertices = listVerticesforCC.begin(); itVertices != listVerticesforCC.end(); ++itVertices)
        arrayPointer[*itVertices - 1] = itVertices;
    /*while there are vertices that haven't been visited yet*
     * - Pop them so we can visit the first of them next!
     */
    while(!listVerticesforCC.empty())
    {
        auxVertice = listVerticesforCC.front();

        auxComponent.size = 0;
        auxComponent.listComp.clear();
        /*find the CC*/
        BFSGenerica(
            auxVertice,
            bfsInfo,
            &auxComponent,
/*BFSTyoe*/     3,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
/*Stopvertice*/ 0,
/*Diameter */   NULL,
            arrayPointer,
            &listVerticesforCC
        );

        if (auxComponent.size > maxSize)
          maxSize = auxComponent.size;
        if (auxComponent.size < minSize)
          minSize = auxComponent.size;
        listComponents.push_back(auxComponent);
        count++;
    }

    // Sorting List using Lambda Function as comparator
    listComponents.sort(compare);

    if (search == 1)
    {
      file.open(fileoutput);
      file << "filename : " << filename << endl;
      file << "size: " << count << endl;
      file << "maiorTam: "<< maxSize << endl;
      file << "menorTam: "<< minSize << endl;
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
      file.close();
    }

    delete [] bfsInfo;
 }

void Grafos::Distance(int firstVertice, int secondVertice)
{
    float distance;
    vector <int> father;
    int aux;

    if(hasWeight &&allPos)
    {
        distance = Dijkstra (firstVertice, secondVertice, father);
        cout << "Dijkstra from " << firstVertice << " to " << secondVertice << endl;
        cout << "Distance: " << distance << endl;
        aux = father[secondVertice - 1];
        cout << "Path: "<< secondVertice << "<-";
        while (aux != firstVertice)
        {
            cout << aux << "<-";
            aux = father[aux-1];
        }
        cout << aux << endl;
    }
    else if(!hasWeight)
    {
        int** bfsInfo = new int* [numVertices];
        BFSGenerica(firstVertice, bfsInfo, NULL , 1, secondVertice, NULL, NULL, NULL);
        cout << "BFS from " << firstVertice << " to " << secondVertice << endl;
        cout << "Distance:  " << bfsInfo[secondVertice - 1][1] << endl;
        cout << "Father in Generator Tree:  "  << bfsInfo[secondVertice - 1][2] << endl;
    }

   // string fileoutput = "./distancein" + filename + "from" + to_string(firstVertice) + "to" + to_string(secondVertice) + ".txt";
   // int** bfsInfo = new int* [numVertices];
   // ofstream file;
   //
   //
   // if(search)
   // {
   //   file.open(fileoutput);
   //   file << "fileName: " << filename;
   //   file << "BFS from " << firstVertice << " to " << secondVertice << endl;
   //   file << "Distance:  " << bfsInfo[secondVertice - 1][1] << endl;
   //   file << "Father in Generator Tree:  "  << bfsInfo[secondVertice - 1][2] << endl;
   //   file.close();
   // }
   //
   // delete [] bfsInfo;

}


void Grafos::PrintAllPaths (int initialVertice)
{
    vector <int> father;
    Dijkstra (initialVertice, father);
    int index, aux;

    for (index = 0; index < numVertices; index++)
    {
        aux = father[index];
        if (aux != -1)
        {
            cout << "Path " << index + 1 << ":  " << index + 1 << "<-";
            while (aux != initialVertice)
            {
                cout << aux << "<-";
                aux = father[aux-1];
            }
            cout << aux << endl;
        }
    }

}


void Grafos::CreateGrafo()
{
    int index;
    grafo.reserve(numVertices);
    for (index = 0; index < numVertices; index++)
        grafo.push_back (Vertice(type, numVertices, hasWeight));
}

/* Coloca os dados no grafo */
void Grafos::Populate()
{
    int auxVertice1;
    int auxVertice2;
    float auxWeight;
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

    // CreateGrafo();

    /*Read the firs 2 integers. If the next character is '\n', there is no weight. Else, there is.*/
    file >> auxVertice1 >> auxVertice2;
    if (file.peek() == '\n')
        hasWeight = false;
    else
        hasWeight = true;

    /*Creating all of the vertices in the graph*/
    CreateGrafo();

    /*Add the edges and weights according to hasWeight*/
    switch(hasWeight)
    {
        case true:
            file >> auxWeight;
            do
            {
                if (auxWeight < 0)
                    allPos = false;
                grafo[auxVertice1-1].setVertice(auxVertice2, auxWeight);
                grafo[auxVertice2-1].setVertice(auxVertice1, auxWeight);
                numEdges++;
            }
            while (file >> auxVertice1 >> auxVertice2 >> auxWeight);
            break;

        case false:
            do
            {
                grafo[auxVertice1-1].setVertice(auxVertice2);
                grafo[auxVertice2-1].setVertice(auxVertice1);
                numEdges++;
            } while(file >> auxVertice1 >> auxVertice2);
            break;

    }
    file.close();
}

 void Grafos::PrintMatrix()
 {
     int i, j;
     cout << "x   ";
     for (i =0; i < numVertices; i++)
        cout << i + 1 << " ";
     cout << endl << endl;
     for(i = 0; i < numVertices; i++)
     {
         cout << i + 1 << "   ";
         for(j = 0; j < numVertices; j++)
            if (hasWeight)
                cout << grafo[i].adjRowWeight[j] << " ";
            else
                cout << grafo[i].adjRow[j] << " ";

         cout << endl;
     }
 }

  void Grafos::PrintList()
  {
      unsigned index;
      if (hasWeight)
        cout << "Vertice\t(adjacent, weight)" << endl;
      else
        cout << "Vertice\tadjacent" << endl;

      for (index = 0; index < (unsigned) numVertices; index++)
      {
         cout << index + 1 << "\t";
         if (hasWeight)
         {
             for (auto const &aux : grafo[index].adjListWeight)
                cout << "(" << aux.connectedVertice << "," << aux.weight << ") ";
         }
         else
         {
            for (auto const & aux : grafo[index].adjList)
                cout << aux << " ";
         }
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
        degree = grafo[i].size;
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
