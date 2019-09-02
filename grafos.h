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

class Components 
{
  public:
    Components();
    void InsertVertice(int Vertice);
    int size;
    list<int> listComp;
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
    list<Components> listComponents, // Não consegui botar como default parameter, vou botar para todas as unfçoes enviarem esse parametro, mas ele nao vai fazer nada sem ser no connected components
    int BFStype = 0,  // 0 -> Normal BFS, 1 -> Stop when discover vertice, 2 -> Diameter, 3 -> Connected Components
    int StopVertice = 0,
    int *diameter = NULL,
    std::list<int>::iterator *arrayPointer = NULL,
    list<int> listVerticesforCC = list<int>(0)
    );

    void DFSGenerica(
        int initialVertice,
        int **father_level);

    void GetInformation();
    void PrintMatrix();
    void PrintList();
};

#endif