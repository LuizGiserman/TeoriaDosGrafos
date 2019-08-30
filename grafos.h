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

#define MAX_NUM_VERTICES      500
#define READINGS_SPT          2
/*The maximum line size is equal to max num vertices * 2 + a space character + \n character. */
#define MAX_LINE_SIZE         (MAX_NUM_VERTICES * 2) + 1 + 1
#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

using namespace std;

struct Verticedata {
  union {
      bitset<1>* Row;
      list<int> List;
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
    void setVertice(int Vertice);
    bitset<1>* getVerticeMatrix();
    list<int> getVerticeList();
    bool hasEdge(int Vertice);
};

class Grafos
{

public:
  Grafos(string, int type);
  string filename;
  int type;
  int numVertices;
  int numEdges;
  Vertice** grafo;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  void createGrafo(int rows);
  void Populate();
  void Print();
  int numAdjacencyVertices(int Vertice);
  list<int> Edges(int Vertice);
  void PrintInformation();
  void ConnectedComponents();
  void Distance(int firstVertice, int secondVertice);
  void BFS(int initialVertice);
  void DFS(int initialVertice);


private:
  int** BFSGenerica(int initialVertice, int** BFSinfo,int Stop = 0, int StopVertice = 0);
  void GetInformation();
  void PrintMatrix();
  void PrintList();
};

#endif
