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
#include <utility>

#define READINGS_SPT          2
#define ERROR_READING_FILE    1

#define INFINITE              std::numeric_limits<int>::max()

typedef unsigned char BYTE;
typedef std::pair< int, int> dist_vertice;
using namespace std;

class Edge
{
    public:
    Edge(int connectedVertice, int weight);
    int connectedVertice;
    float weight = 1;
};

class Vertice
{
public:

    int type;
    int size;
    bool hasWeight;

    vector <int> adjList; /*type = 0*/
    vector <Edge> adjListWeight;

    vector < bitset<1> > adjRow; /*type = 1*/
    vector <int> adjRowWeight;

    Vertice(int type, int size, bool hasWeight);
    void setVertice(int Vertice, int auxWeight = 0);
    bool HasEdge(int numVertice);
};

class Components
{
  public:
    Components();
    void InsertVertice(int numVertice);
    int size;
    list<int> listComp;

    bool compare(const Components &comp1, const Components &comp2);
};

class Grafos
{

public:
  Grafos(string fileName, int type);
  ~Grafos();
  vector<Vertice> grafo;
  bool hasWeight;
  string filename;
  int type;
  int numVertices;
  int numEdges;
  int maxDegree;
  int minDegree;
  int avgDegree;
  int medDegree;

  /*funcoes essenciais*/
  void CreateGrafo(); /*cria os vertices e os coloca em grafo*/
  void Populate(); /*Insere os vértices vizinhos nas listas de adjacências*/
  void BFS(int initialVertice, int search);
  void DFS(int initialVertice, int search);
  void ConnectedComponents(int search);
  void GetDiameter();
  void Distance(int firstVertice, int secondVertice, int search); /*dist entre 2 vertices*/

  void Dijkstra (int initialVertice);

  /*funções auxiliares*/
  void Print(); /*Imprime a estrutura do grafo, independente de qual for*/
  void PrintInformation(); /*Imprime informações sobre o grafo*/


private:

  void BFSGenerica(
    int initialVertice,
    int **BFSinfo,
    Components *auxComponent = NULL,
    int BFStype = 0,
    int StopVertice = 0,
    int *diameter = NULL,
    std::list<int>::iterator *arrayPointer = NULL,
    list<int> *listVerticesforCC = NULL
  );
/* BFSType 0 -> Normal BFS = 1; parada de vértice = 2 ; compenentes conexas = 3 */

  void DFSGenerica(
    int initialVertice,
    int **father_level
  );

  void GetInformation();
  void PrintList();
  void PrintMatrix();

};
#endif
