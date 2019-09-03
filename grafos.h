/*
* Universidade Federal do Rio de Janeiro
* Autores: Luiz Giserman e Renan Neri
* Disciplina: Teoria dos Grafos
*/

#ifndef GRAFOS_H
#define GRAFOS_H


#include <iostream>
#include <string>
#include <cerrno>
#include <vector>
#include <bitset>
#include <list>
#include <fstream>
#include <cstring>
#include <stack>

#define READINGS_SPT          2
#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

using namespace std;


class Vertice
{
public:

    int type;
    int size;
    vector<int> adjList; /*type = 0*/
    bitset<1>* adjRow = NULL; /*type = 1*/


    Vertice(int type, int size);
    ~Vertice();
    void setVertice(int Vertice);
    bitset<1>* getVerticeMatrix();
    vector<int> getVerticeList();
    bool hasEdge(int Vertice);
};

class Components
{
  public:
    Components();
    void InsertVertice(int Vertice);
    int size;
    list<int> listComp;

    bool compare(const Components &comp1, const Components &comp2);
};

/*listVerticesforCC - int dos index*/
/*arrayPointer [numVertices] - aponta pros index*/
/*listComponents??*/

/*newComponent??*/

class Grafos
{

public:
  Grafos(string fileName, int type);
  ~Grafos();
  string filename;
  int type;
  int numVertices;
  int numEdges;
  vector<Vertice> grafo;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  void createGrafo();
  void Populate();
  void Print();
  int numAdjacencyVertices(int Vertice);
  vector<int> Edges(int Vertice);
  void PrintInformation();
  void GetDiameter();
  void ConnectedComponents(int search);
  void Distance(int firstVertice, int secondVertice, int search);
  void BFS(int initialVertice, int search);
  void DFS(int initialVertice);
  void PrintMatrix();

private:

  void BFSGenerica(
    int initialVertice,
    int **BFSinfo,
    Components *auxComponent = NULL,
    int BFStype = 0,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
    int StopVertice = 0,
    int *diameter = NULL,
    std::list<int>::iterator *arrayPointer = NULL,
    list<int> *listVerticesforCC = NULL
  );



    void DFSGenerica(
        int initialVertice,
        int **father_level);

    void GetInformation();
    void PrintList();
};

#endif
