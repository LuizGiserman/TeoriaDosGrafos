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
#include <list>
#include <fstream>
#include <cstring>
#include "functions.h"


#define MAX_NUM_VERTICES      500
/*The maximum line size is equal to max num vertices * 2 + a space character + \n character. */
#define MAX_LINE_SIZE         (MAX_NUM_VERTICES * 2) + 1 + 1
#define ERROR_READING_FILE    1

typedef unsigned char BYTE;

class Grafos
{

public:
  Grafos(std::string);
  std::vector<std::list<int>> CriarLista (std::string fileName, int *numberVertices, int *numberEdges);
};

#endif
