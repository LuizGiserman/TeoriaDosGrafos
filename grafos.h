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
#include "functions.h"


#define MAX_NUM_VERTICES      500
/*The maximum line size is equal to max num vertices * 2 + a space character + \n character. */
#define MAX_LINE_SIZE         (MAX_NUM_VERTICES * 2) + 1 + 1
#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

using namespace std;


class Grafos
{

public:
  Grafos(string, int type);
  string filename;
  int type;
  int numVertices;
  int numEdges;
  bitset<1>** matrix;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  std::vector<std::list<int>> CriarLista (std::string fileName, int *numberVertices, int *numberEdges);
  bitset<1> **generateSquareMatrix(int rows);
  void Matrix();
  void List();
  void Print();
  void PrintInformation();
  void BFS(int initialVertice);

private:
  void BFSMatrix(int initialVertice);
  void BFSList(int initialVertice);
  void PrintMatrix();
  void PrintList();
  void getInformationMatrix();
  void getInformationList();

};

#endif
