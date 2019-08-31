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
#define STOP_BFS              1

#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

using namespace std;

struct Verticedata {
  union {
      bitset<1>* adjacenciesRow;
      list<int> linkedList;
    };

    public:
      Verticedata();
      ~Verticedata();
};

class Vertice
{
    int type;
    int size;
    Verticedata vertice;

  public:
    Vertice(int type, int size);
    void SetVertice(int verticeNumber);
    list<int> GetVerticeList();
    bitset<1>* getVerticeMatrix();
    bool HasEdge(int Vertice);
};

class Grafos
{

public:
  string filename;
  int type;
  int numVertices;
  int numEdges;
  vector<Vertice> grafo;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  Grafos(string, int type);
  void CreateGrafo();
  void Populate();
  void Print();
  int GetNumAdjacenciesVertice(int verticeNumber);

  list<int> Edges(int Vertice);
  void PrintInformation();
  void ConnectedComponents();
  void Distance(int firstVertice, int secondVertice);
  void BFS(int initialVertice);
  void DFS(int initialVertice);


private:
  int** BFSGenerica(int initialVertice, int **bfsInfo,int stop = 0, int stopVertice = 0);
  void GetInformation();
  void PrintMatrix();
  void PrintList();
};

#endif
