#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/timeb.h>
#include <chrono>
#include <time.h>
#include <stack>
#include <map>
#include "grafos.h"


using namespace std;

void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

void getPath(Grafos grafo, string filename){
    vector <int> father;
    vector <float> distance;
    grafo.Dijkstra(1,father,distance);
    ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::app);
    file << "FileName: " << grafo.filename << endl;
    file << " Indice Pai Distancia" << endl;
    file << "  10   " << "  " << father[9] << "   " << distance[9] <<endl;
    file << "  20   " << "  " << father[19] << "   " << distance[19] <<endl;
    file << "  30   " << "  " << father[29] << "   " << distance[29] <<endl;
    file << "  40   " << "  " << father[39] << "   " << distance[39] <<endl;
    file << "  50   " << "  " << father[49] << "   " << distance[49] <<endl;
    file << endl;
    file << endl;
    file << "Caminho" << endl;
    int iniciais [5] = {10,20,30,40,50};
    for(int i =0; i < 5; i++){
      file << iniciais[i] << endl;
      file << "[ "<< iniciais[i];
      int pai = father[iniciais[i] - 1];
      while(pai != 1){
        file <<" " << pai << ",";
        pai = father[pai - 1];
      }
      file <<"1 ]"<< endl;
      file << endl;
      
    }
    file.close();
}

void eccentricity(Grafos grafo, string filename) {
  int vertices[5] = {10,20,30,40,50};
  vector <int> father;
  vector <float> distance;
  ofstream file;
  file.open(filename, std::ofstream::out | std::ofstream::app);
  file << "FileName: " << grafo.filename << endl;
  file << endl;
  int eccentricity;
  for(int i =0; i < 5; i++){
    file << vertices[i] << endl;
    grafo.Dijkstra(vertices[i],father,distance);
    eccentricity = 0;
    for(int i = 0; i < distance.size(); i++){
      if(distance[i] > eccentricity)
        eccentricity = distance[i];
    }      
    file << "Excentricidade: " << eccentricity << endl << endl;
  }
  file.close();
}

void eccentricityTime(Grafos grafo, string filename){
  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;
  vector <int> father;
  vector <float> distance;
  ofstream file;
  int eccentricity;
  file.open(filename, std::ofstream::out | std::ofstream::app);
  file << "FileName: " << grafo.filename << endl;
  file << endl;
  start = std::chrono::steady_clock::now();
  for(int i =1; i < 101; i++){
    grafo.Dijkstra(i,father,distance);
  }
  eccentricity = 0;
    for(int i = 0; i < distance.size(); i++){
      if(distance[i] > eccentricity)
        eccentricity = distance[i];
    }      
  end = std::chrono::steady_clock::now();
  file << "Tempo para achar 100 Excentricidades: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() /1000.0 << " ms" << endl;
  file.close();
}

void PrimTime(){

  std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    double vm, rss;

    for(int i = 1; i<6; i++){
      string graphName = "./grafo_" + to_string(i) + ".txt";
      cout << graphName << endl;
      Grafos grafo = Grafos(graphName, LIST_TYPE);
      start = std::chrono::steady_clock::now();
      vector<int> distance;
      grafo.Prim(distance);
      end = std::chrono::steady_clock::now();
      cout << "Prim: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() /1000.0 << " ms" << endl;
    }

}

void RedeColaboradores(Grafos grafo, string filename){
   map<string, int> Vertices;
   map<int,string> VerticesInvertido;
   ifstream infile("rede_colaboracao_vertices.txt");
   int a;
   string b;
   size_t position;
   
   while (getline(infile,b))
{   position = b.find(",");
    a = stoi(b.substr(0,position));
    b = b.substr(position + 1,b.length());
    Vertices[b] = a;
    VerticesInvertido[a] = b; 
}
  string inicial = "Edsger W. Dijkstra";
  string Objetivos [5] ={ "Alan M. Turing", "J. B. Kruskal","Jon M. Kleinberg",
  "Éva Tardos","Daniel R. Figueiredo"};
  int positions [5];

  for(int i = 0; i < 5; i++){
    positions[i] = Vertices[Objetivos[i]];
  }

    vector <int> father;
    vector <float> distance;
    cout << Vertices[inicial] << endl;
    cout <<VerticesInvertido[Vertices[inicial]] << endl;
    /*
    for(int i = 1; i < 5; i++){
      grafo.Distance(Vertices[inicial],Vertices[Objetivos[i]]);
    }
    */

    grafo.Dijkstra(Vertices[inicial],father,distance);
    cout << "Terminou" << endl;
    ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::app);
    file << "FileName: " << grafo.filename << endl;
    file << " Indice Pai Distancia" << endl;
    file << Objetivos[0] << "  " << VerticesInvertido[father[positions[0]-1]] << "   " << distance[Vertices[Objetivos[0]]-1] <<endl;
    file << Objetivos[1] << "  " << VerticesInvertido[father[positions[1]-1]] << "   " << distance[Vertices[Objetivos[1]]-1] <<endl;
    file << Objetivos[2] << "  " << VerticesInvertido[father[positions[2]-1]] << "   " << distance[Vertices[Objetivos[2]]-1] <<endl;
    file << Objetivos[3] << "  " << VerticesInvertido[father[positions[3]-1]] << "   " << distance[Vertices[Objetivos[3]]-1] <<endl;
    file << Objetivos[4] << "  " << VerticesInvertido[father[positions[4]-1]] << "   " << distance[Vertices[Objetivos[4]]-1] <<endl;
    file << endl;
    file << endl;
    cout << father[Vertices[Objetivos[0]]-1];
  
    file << "Caminho" << endl;
    int stop = Vertices[inicial];
    for(int i =0; i < 5; i++){
      file << Objetivos[i] << endl;
      file << "[ "<< Objetivos[i] << ",";
      int pai = father[Vertices[Objetivos[i]]-1];
      while(pai != stop && pai != -1){
        file <<" " << VerticesInvertido[pai] << ",";
        pai = father[pai-1];
      }
      file <<inicial <<" ]"<< endl;
      file << endl;
      
    }
    file.close();  

}

void RedeColaboradoresPrim(Grafos grafo, string filename){
  map<string, int> Vertices;
   map<int,string> VerticesInvertido;
   ifstream infile("rede_colaboracao_vertices.txt");
   int a;
   string b;
   size_t position;
   vector<int> discover;
   vector <float> cost;
   
   while (getline(infile,b))
{   position = b.find(",");
    a = stoi(b.substr(0,position));
    b = b.substr(position + 1,b.length());
    Vertices[b] = a;
    VerticesInvertido[a] = b; 
    cout << a << endl;
} 

  string Objetivos [2] = { "Edsger W. Dijkstra", "Daniel R. Figueiredo"};

  grafo.Prim(discover,cost);

  stack <float> custo;
  stack <float> indice;


  for(int i=0; i< cost.size(); i++){
    if(custo.empty()){
      custo.push(cost[0]);
      indice.push(0);
    }
    if(cost[i] >= custo.top()){
      custo.push(cost[i]);
      indice.push(i);
    }
  }


  cout << "Os tres mais custosos da MST:" << endl;
  cout << custo.top() << "  " << VerticesInvertido[indice.top()+1] << endl;
  indice.pop();
  custo.pop();
  cout << custo.top() << "  " << VerticesInvertido[indice.top()+1] << endl;
  indice.pop();
  custo.pop();
  cout << custo.top() << "  " << VerticesInvertido[indice.top()+1] << endl;
  cout<< endl<< endl;


  for(int i =0; i < 2; i++){
    cout << Objetivos[i] << " na MST" << endl;
    cout << VerticesInvertido[discover[Vertices[Objetivos[1]]-1]+1] << endl;
    for(int j = 0; j < discover.size(); j++ ){
      if(discover[j] == Vertices[Objetivos[i]]){
        cout << VerticesInvertido[j + 1] << endl;
      }
    }
    cout << endl << endl;
  }


}


int main()
{ 
  
  Grafos grafo = Grafos("rede_colaboracao.txt", LIST_TYPE);
  string filename = "resultado_prim_rede_colaboracao.txt";
  RedeColaboradoresPrim(grafo,filename);

    
}
