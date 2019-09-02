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
    void setVertice(int Vertice);
    bitset<1>* getVerticeMatrix();
    vector<int> getVerticeList();
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
  void ConnectedComponents();
  void Distance(int firstVertice, int secondVertice);
  void BFS(int initialVertice);
  void DFS(int initialVertice);

private:
  void BFSGenerica(
    int initialVertice,
    int** BFSinfo,
    int Stop = 0,
    int StopVertice = 0,
    int *diameter = NULL);

    void DFSGenerica(
        int initialVertice,
        int **father_level);

  void GetInformation();
  void PrintMatrix();
  void PrintList();
};

#endif
