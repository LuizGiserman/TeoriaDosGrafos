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

int main()
{

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    ofstream file;
    double vm, rss;

    file.open ("./times.txt");
    Grafos grafo1 = Grafos("./grafosTrab2/grafo_3.txt", LIST_TYPE);
    file << "FileName: " << grafo1.filename << endl;
    process_mem_usage(vm, rss);
    file << "Resident set size: " << rss << endl;
    file << "VM: " << vm << endl;
    start = std::chrono::steady_clock::now();
    grafo1.Prim();
    end = std::chrono::steady_clock::now();
    file << "Prim: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() /1000.0 << " ms" << endl;

    file.close();

    return 0;
}
