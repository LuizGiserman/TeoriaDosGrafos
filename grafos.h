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


#define MAX_NUM_VERTICES      500
/*The maximum line size is equal to max num vertices * 2 + a space character + \n character. */
#define MAX_LINE_SIZE         (MAX_NUM_VERTICES * 2) + 1 + 1
#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

using namespace std;

struct Verticedata {
  union {
      bitset<1>* Row;
      list<int>* List;
    };
};

class Vertice
{
    int type;
    Verticedata vertice;

  public:
    Vertice(int type, int size = 0);
    void setVertice(int Vertice);
    void getVertice();
    bool hasEdge(int Vertice);
    bitset<1>* getVerticeMatrix();
    list<int>* getVerticeList();
};

class Grafos
{

public:
  Grafos(string, int type);
  string filename;
  int type;
  int numVertices;
  int numEdges = 0;
  bitset<1>** matrix;
  std::list<int> *vectorGraph = NULL;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  bitset<1> **generateSquareMatrix(int rows);
  void Matrix();
  void List ();
  void Print();
  void PrintInformation();
  void BFS(int initialVertice);

  void AllocateVectorOfLists();


private:
  void setVerticeMatrix(int Vertice1, int Vertice2);
  void getVerticeMatrix(int Vertice);
  void getVerticeList(int Vertice1, int Vertice2);
  void getVerticeList(int Vertice);
  void BFSMatrix(int initialVertice);
  void BFSList(int initialVertice);
  void PrintMatrix();
  void PrintList();
  void getInformationMatrix();
  void getInformationList();

};

#endif
