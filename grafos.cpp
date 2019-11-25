/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/
#include <algorithm>
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
#include <unistd.h>
#include <sys/timeb.h>
#include <chrono>
#include "grafos.h"


using namespace std;

Edge::Edge(int connectedVertice, float weight)
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
Grafos::Grafos(std::string fileName, int type, char *dir){
    if(dir != NULL)
    {
        string auxString(dir);
        if (auxString == DAG)
            this->isDAG = true;
    }
    this->filename = fileName;
    this->type = type;     // if 0 -> Adjacency List, if 1 -> Adjacency Matrix

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

void Grafos::BFSGenerica(int initialVertice, vector<vector<int>> &bfsInfo, Components *auxComponent, int BFStype /*=0*/, int stopVertice/*=0*/, int *diameter /*=0*/,std::list<int>::iterator *arrayPointer/* = {}*/, list<int> *listVerticesforCC/*= {0}*/)
 {
     vector < bitset<1> > matrixVertices;
     vector<int> listVertices;
     int auxVertice;
     queue<int> auxQueue; // Queue Created

     if (diameter != NULL)
       *diameter = 0;

    bfsInfo.resize(3);
    bfsInfo[0].resize(numVertices, 0);
    bfsInfo[1].resize(numVertices, -1);
    bfsInfo[2].resize(numVertices, -1);

    bfsInfo[0][initialVertice - 1] = 1; // initialVertice marked
    bfsInfo[1][initialVertice - 1] = 0; // root
    bfsInfo[2][initialVertice - 1] = 0; // no father

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
                if(bfsInfo[0][it - 1] == 0)
                {
                    //Mark Vertice
                    bfsInfo[0][it - 1] = 1;
                    //Define Level of the father as the level of father plus 1
                    bfsInfo[1][it - 1] = bfsInfo[1][auxVertice-1] + 1;
                    // Define Vertice as father
                    bfsInfo[2][it - 1] = auxVertice;
                    auxQueue.push(it);
                    if(diameter != NULL)
                    {
                        if(*diameter < bfsInfo[1][it -1])
                            *diameter = bfsInfo[1][it -1];
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
                    if(bfsInfo[0][i] == 0)
                    {
                        //Mark Vertice
                        bfsInfo[0][i] = 1;
                        //Define Level of the father as the level of father plus 1
                        bfsInfo[1][i] = bfsInfo[1][auxVertice-1] + 1;
                        // Define Vertice as father
                        bfsInfo[2][i] = auxVertice;
                        auxQueue.push(i + 1);

                        if(diameter != NULL)
                            if(*diameter < bfsInfo[1][i])
                                *diameter = bfsInfo[1][i];
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

     vector<vector<int>> bfsInfo;
     ofstream file;

     BFSGenerica(initialVertice, bfsInfo);

    if (search)
    {
      string fileoutput = "./paisInBFS102030" + to_string(initialVertice) + filename;
      file.open(fileoutput);
      file << "filename: " << filename << endl;
      file << "initialVertice: " << initialVertice << endl;
      file << "vertice\t father\t" << endl;
      file << "10\t" << bfsInfo[2][10-1] << "\t" << endl;
      file << "20\t" << bfsInfo[2][20-1] << "\t" << endl;
      file << "30\t" << bfsInfo[2][30-1] << "\t" << endl;
      file.close();
    }

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

void Grafos::Dijkstra (int initialVertice, vector <int> &father, vector <float> &distance){

    int secondVertice = 0;
    Dijkstra (initialVertice, father, distance, secondVertice );
};


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

void Grafos::Prim(){
    vector <float> cost;
    vector<int> level;
    vector<int> discover;
    Prim(discover,cost,level);
}

void Grafos::Prim(vector<int> &discover){
    vector<int> level;
    vector <float> cost;
    Prim(discover,cost,level);

}

void Grafos::Prim(vector<int> &discover, vector<float> &cost,vector<int> &level)
{
    level.resize(numVertices,-1);
    cost.resize(numVertices,INFINITY);
    discover.resize(numVertices, -1);
    priority_queue < dist_vertice, vector<dist_vertice>, greater<dist_vertice> > distanceHeap;
    vector<bool> marked (numVertices, false);
    int verticeID;
    distanceHeap.push(make_pair(0, 0));
    discover[0] = 0;
    cost [0] = 0;
    level[0] = 0;
    unsigned totalCost = 0;

    while (!distanceHeap.empty())
    {
        verticeID = distanceHeap.top().second;
        distanceHeap.pop();
        marked[verticeID] = true;
        /*para cada vizinho v de u faça*/
        for (auto const &v: grafo[verticeID].adjListWeight)
        {
            /*Se custo[v] > w((u,v)) então*/
            if ( (cost[v.connectedVertice - 1] > v.weight) && (marked[v.connectedVertice -1] == false) )
            {
                cost[v.connectedVertice - 1] = v.weight;
                discover[v.connectedVertice - 1] = verticeID+1;
                level[v.connectedVertice-1] = level[verticeID] + 1;
                // cout << "Cost " << v.connectedVertice << " = " << cost[v.connectedVertice -1] << endl;
                distanceHeap.push(make_pair(cost[v.connectedVertice - 1], v.connectedVertice - 1));
            }
        }

    }



    for (auto const &v: cost)
    {
        // cout << "v: " << v << endl;
        if (v != INFINITY)
            totalCost += v;
    }

    cout << "Total Cost MST: " << totalCost << endl;



}

void Grafos::GetDiameter(){

    ofstream file;
    string fileoutput = "./DiameterIN" + filename;

    int maxDiameter = 0;
    int diameter;
    vector<vector<int>> bfsInfo;

    for(int i = 0; i < numVertices; i++)
    {

        diameter = 0;
        BFSGenerica(i + 1, bfsInfo, NULL, 0, 0, &diameter, NULL, NULL);
        if( diameter > maxDiameter)
            maxDiameter = diameter;

        /*clear bfsInfo from previous iteration*/
        bfsInfo.clear();
    }
        file.open(fileoutput);
        file << "filename: " << filename << endl;
        file << "The diameter of the Graph is " << maxDiameter << endl;
        file.close();

}



void Grafos::ConnectedComponents(int search){

    ofstream file;
    string fileoutput = "./CCin" + filename;
    int count = 0;
    int maxSize = 0;
    int minSize = numVertices;
    int auxVertice;
    vector<vector<int>> bfsInfo;

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

 }

void Grafos::Distance(int firstVertice, int secondVertice)
{
    float distance;
    vector <int> father;
    int aux;

    if(hasWeight && allPos)
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
        vector<vector<int>> bfsInfo;
        BFSGenerica(firstVertice, bfsInfo, NULL , 1, secondVertice, NULL, NULL, NULL);
        cout << "BFS from " << firstVertice << " to " << secondVertice << endl;
        cout << "Distance:  " << bfsInfo[1][secondVertice - 1] << endl;
        cout << "Father in Generator Tree:  "  << bfsInfo[2][secondVertice - 1] << endl;

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
    int index, aux;
    vector<vector<int>> bfsInfo;

    if (hasWeight)
    {
        Dijkstra (initialVertice, father);
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
    else /*not working*/
    {
        BFSGenerica (initialVertice, bfsInfo);
        for (index = 0; index < numVertices; index++)
        {
            aux = bfsInfo[2][index];
            if (aux != -1)
            {
                cout << "Path " << index + 1 << ":  " << index + 1 << "<-";
                while (aux != initialVertice)
                {
                    cout << aux << "<-";
                    aux = bfsInfo[2][aux-1];
                }
                cout << aux << endl;
            }
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
                if (!isDAG)
                    grafo[auxVertice2-1].setVertice(auxVertice1, auxWeight);
                numEdges++;
            }
            while (file >> auxVertice1 >> auxVertice2 >> auxWeight);
            break;

        case false:
            do
            {
                grafo[auxVertice1-1].setVertice(auxVertice2);
                if(!isDAG)
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

bool Grafos::isBipartite(int initialVertice, vector<int> &color){

    color[initialVertice] = 1;

    queue <int> q;
    q.push(initialVertice);

    while (!q.empty()){

        int u = q.front();
        q.pop();

        for(auto const &it : grafo[u].adjList) {
            if (color[it - 1] == -1) {
                color[it - 1] = 1 - color[u];
                q.push(it - 1);
            }

            else if (color[it - 1] == color[u]){
                return false;
            }
        }
    }

    return true;
}

/*Function checks if the graph is bipartite and, in  case it is,
 *separate both groups in different vectors*/
bool Grafos::isBipartite(){

    vector<int> color;
    color.resize(numVertices, -1);

    int i;
    bool result;
    i = indexNotVisited(color);
    while(i != numVertices){
        result = isBipartite(i,color);
        if(!result)
            return false;
       i = indexNotVisited(color);
    };

    int numGroup1 = 0;
    int numGroup2 = 0;
    vector<int> group1;
    vector<int> group2;
    group1.reserve(numVertices/2);
    group2.reserve(numVertices/2);

    for(i = 0; i < numVertices; i++){
        if(color[i] == 0){
            numGroup1++;
            group1.push_back(i);
        }
          else
        {
            numGroup2++;
            group2.push_back(i);
        }

    }

    this->numGroup1 = numGroup1;
    this->numGroup2 = numGroup2;
    this->group1 = group1;
    this->group2 = group2;

    return true;
}


int Grafos::indexNotVisited(vector<int> color){
    int i;
    for(i = 0; i < numVertices; i++){
        if (color[i] == -1){
            return i;
        };
    }

    return numVertices;
}


bool Grafos::HasAugmentingPath (vector<int> &pairG1, vector<int> &pairG2, vector<int> &dist)
{
        queue<int> auxQ;
        int auxVertice;
        int g1;
        /*setup*/
        for (auto const &g1: group1)
        {
            if (pairG1[g1+1] == NIL)
            {
                dist[g1+1] = 0;
                auxQ.push(g1+1);
            }
            else
                dist[g1+1] = INFINITY;
        }

        dist[NIL] = INFINITY;

        while(!auxQ.empty())
        {
            auxVertice = auxQ.front();
            auxQ.pop();

            if(dist[auxVertice] < dist[NIL])
                for(auto const &viz: grafo[auxVertice-1].adjList)
                    if(dist[pairG2[viz]] == INFINITY)
                    {
                        dist[pairG2[viz]] = dist[auxVertice] + 1;
                        auxQ.push(pairG2[viz]);
                    }
        }

        return (dist[NIL] != INFINITY);
}

void Grafos::HopcroftKarp()
{
    int index;
    int maxMatching = 0;
    vector<int> pairG1;
    vector<int> pairG2;
    vector<int> dist;
    int g1;

    pairG1.resize(numVertices + 1, NIL);
    pairG2.resize(numVertices + 1, NIL);
    dist.resize(numVertices + 1);

    while(HasAugmentingPath(pairG1, pairG2, dist))
    {
        for (auto const &index: group1)
            if ((pairG1[index+1]==NIL) && AugmentStartingAt(index+1, pairG1, pairG2, dist))
                maxMatching++;
    }

    // for (auto const &g1: group1)
    // {
    //     if(pairG1[g1+1] != NIL)
    //         cout << g1+1 << "--" << pairG1[g1+1] << endl;
    // }
    cout << "MaxMatching: " << maxMatching << endl;

}

bool Grafos::AugmentStartingAt (int initialVertice, vector<int> &pairG1, vector<int> &pairG2, vector<int> &dist)
{
    if (initialVertice != NIL)
    {
        for (auto const &viz: grafo[initialVertice-1].adjList)
        {
            if(dist[pairG2[viz]] == dist[initialVertice] + 1)
                if(AugmentStartingAt(pairG2[viz], pairG1, pairG2, dist) == true)
                {
                    pairG2[viz] = initialVertice;
                    pairG1[initialVertice] = viz;
                    return true;
                }
        }
        dist[initialVertice] = INFINITY;
        return false;
    }
    return true;
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

/*Finds shortest path for every pair of vertices*/
bool Grafos::BellmanFord(int initialVertice, vector<int> &distance)
{
    distance[initialVertice] = 0;
    int i, j;
    int destination;
    int weight;
    bool tableUpdated;

    for (i = 0; i < numVertices - 1 + 1; i++)
    {
      tableUpdated = false;
      for (j = 0; j < numVertices; j++)
        for (auto const &v: grafo[j].adjListWeight)
        {
            destination = v.connectedVertice - 1;
            weight = v.weight;
            if (distance[j] != INFINITY && distance[j] + weight < distance[destination])
            {
                distance[destination] = distance[j] + weight;
                tableUpdated = true;
            }
        }
      if(!tableUpdated)
          return OK;

      if(i == numVertices - 1 && tableUpdated)
        return CICLO_NEGATIVO;
    }
    return OK;
}

void Grafos::BellmanFord(){

    vector<int> auxDistance;
    vector<vector<int>> distance;
    int index;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    int k = 0;
    bool auxBool1 = !CICLO_NEGATIVO;
    bool auxBool2 = !CICLO_NEGATIVO;

    start = std::chrono::steady_clock::now();
    for (index = 0; index < numVertices; index++)
    {
        auxDistance.resize(numVertices, INFINITY);
        auxBool1 = BellmanFord(index,auxDistance);
        if (auxBool1 == CICLO_NEGATIVO)
            auxBool2 = auxBool1;

        distance.push_back(auxDistance);
        k++;
        auxDistance.clear();
    }
    end = std::chrono::steady_clock::now();

    cout << "Ciclo negativo: " << auxBool2 <<endl;
    cout << "Tempo de execucao: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "µs | " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000.0 << "ms" << endl;
    cout << "(1, 10): " << distance[0][9] << "\t(2, 20): " << distance[1][19] << "\t(3, 30): " << distance[2][29] << endl;

}
