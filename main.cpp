#include "grafos.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ctime>

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

int main(){
  Grafos grafo = Grafos("as_graph.txt",0);
  clock_t start = clock();
  grafo.BFS(1);
  clock_t end = clock();
  cout<< (double)(end-start)/CLOCKS_PER_SEC<<endl;
  // grafo.ConnectedComponents();



  return 0;
}
