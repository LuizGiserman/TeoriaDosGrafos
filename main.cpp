#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/timeb.h>
#include <chrono>
#include <time.h>
#include "grafos.h"

#define ARGC_TOTAL      2
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

int main(int argc, char *argv[]){

   std::chrono::steady_clock::time_point start;
   std::chrono::steady_clock::time_point end;
   ofstream file;
   double vm, rss;

   file.open("./result.txt");
  /*as_graph*/

    if (argc != ARGC_TOTAL)
        argv[1] = NULL;


     Grafos grafo = Grafos("exemplo.txt", LIST_TYPE, argv[1]);
     cout << "criou grafo" <<endl;
     grafo.Print();
     grafo.BellmanFord();
     // file << "FileName: " << grafo.filename << endl;
     // bool isbipartite = grafo.isBipartite();
     // file << "e bipartido? " << isbipartite << endl;
     // if (isbipartite){
     //   file << "Maximum Matching" << grafo.maximumBipartiteMatching() << endl;
     // }

     file.close();

  return 0;
}
