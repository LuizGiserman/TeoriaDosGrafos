#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/timeb.h>
#include <chrono>
#include <time.h>
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

void ThousandsBDFS(Grafos grafo_atual, int type)
{
  int index;
  for (index = 1; index < 10001; index++)
  {
    if (type == 0)
      grafo_atual.BFS(index, 0);
    else
      grafo_atual.DFS(index, 0);
  }
}

void GetFathers(Grafos grafo_atual, int searchType)
{
  //teste
    int index;
    if(searchType == 0)
    {
    for (index = 1; index < 4; index++)
      grafo_atual.BFS(index, 1);
    }
     else
    {
      cout << "print" << endl;
      for (index = 1; index < 4; index++)
      grafo_atual.DFS(index, 1);
    }

}

int main(){

   std::chrono::steady_clock::time_point start;
   std::chrono::steady_clock::time_point end;
   ofstream file;
   double vm, rss;

   file.open("./times.txt");
  /*as_graph*/


     // Grafos grafo_as_graph = Grafos("as_graph.txt",0);
     // file << "FileName: " << grafo_as_graph.filename << endl;
     // process_mem_usage(vm, rss);
     // file << "Resident set size: " << rss << endl;
     // file << "VM: " << vm << endl;
     // start = std::chrono::steady_clock::now();
     // ThousandsBDFS(grafo_as_graph, 0);
     // end = std::chrono::steady_clock::now();
     // file << "1000BFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << endl;
     // start = std::chrono::steady_clock::now();
     // ThousandsBDFS(grafo_as_graph, 1);
     // end = std::chrono::steady_clock::now();
     // file << "1000DFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << endl;
    // GetFathers(grafo_as_graph, 0);
     // GetFathers(grafo_as_graph, 1);
     // grafo_as_graph.Distance(10, 20, 1);
     // grafo_as_graph.Distance(10, 30, 1);
     // grafo_as_graph.Distance(20, 30, 1);
    // grafo_as_graph.ConnectedComponents(1);
    // grafo_as_graph.GetDiameter();


     Grafos grafo_dbpl = Grafos("grafoteste2.txt", 0);
     file << "FileName: " << grafo_dbpl.filename << endl;
     // process_mem_usage(vm, rss);
     // file << "Resident set size: " << rss << endl;
     // start = std::chrono::steady_clock::now();
     // ThousandsBDFS(grafo_dbpl, 0);
     // end = std::chrono::steady_clock::now();
     // file << "1000BFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()  << " µs" << endl;
     // start = std::chrono::steady_clock::now();
     // ThousandsBDFS(grafo_dbpl, 1);
     // end = std::chrono::steady_clock::now();
     // file << "1000DFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << endl;
     // GetFathers(grafo_dbpl,1);
     // GetFathers(grafo_dbpl,0);
     //
     // grafo_dbpl.Distance(10, 20, 1);
     // grafo_dbpl.Distance(10, 30, 1);
     // grafo_dbpl.Distance(20, 30, 1);
     // grafo_dbpl.ConnectedComponents(1);
     // grafo_dbpl.GetDiameter();
     // grafo_dbpl.Print();
     grafo_dbpl.Dijkstra(1);
     file.close();

/*
    Grafos grafo_live_journal = Grafos("live_journal.txt",0);
    file << "FileName: " << grafo_live_journal.filename << endl;
    process_mem_usage(vm, rss);
    file << "Resident set size: " << rss << endl;
    start = std::chrono::steady_clock::now();
    ThousandsBDFS(grafo_live_journal, 0);
    end = std::chrono::steady_clock::now();
    file << "1000BFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << endl;
    start = std::chrono::steady_clock::now();
    ThousandsBDFS(grafo_live_journal, 1);
    end = std::chrono::steady_clock::now();
    file << "1000DFS: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << endl;
    GetFathers(grafo_live_journal,0);
    GetFathers(grafo_live_journal,1);
    grafo_live_journal.Distance(10, 20, 1);
    grafo_live_journal.Distance(10, 30, 1);
    grafo_live_journal.Distance(20, 30, 1);
    grafo_live_journal.ConnectedComponents(1);
    grafo_live_journal.GetDiameter();

  */

  return 0;
}
