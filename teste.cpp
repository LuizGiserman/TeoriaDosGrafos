void Grafos::ConnectedComponents(){

    list<int> Vertices;
    const int *arrayPointer [numVertices];

    for(int i = 0; i < numVertices; i++){
        Vertices.push_back(i + 1);
    }

    std::list<int>::iterator it;
    for (it = Vertices.begin(); it != Vertices.end(); ++it){
        arrayPointer[*it - 1] = &*it;
    }

    



     
 }

 void Grafos::DFS(int initialVertice)
 {
   /*array to mark the vertices*/
   bitset<1> marked [numVertices];
   /*2 arrays to store the father and level of each vertice*/
   int father_level [numVertices][READINGS_SPT]; /*READINGS_SPT = 2*/
   /*stack to implement the dfs*/
   stack<int> auxStack;
   /*int to put on the stack and keep track of the vertices*/
   int auxVertice;

   /*Type = 0 variables*/
   /*list to get the vertice's adjacency list*/
   list<int> adjacents;
   list<int>::iterator it;

   /*Type = 1 variables*/
   /*adjRow to get the row of adjacencies*/
   bitset<1>  adjRow;

   /*others*/
   unsigned index;


   /*Marking all vertices as not found
    *setting level and father to -1 (nonexistent);
    */
   for (index = 0; index < numVertices; index++)
   {
     marked[index][0] = 0;
     father_level [index][0] = -1;
     father_level [index][1] = -1;
   }

   /*setting root and level of the starting point*/
   father_level [initialVertice-1][0] = 0;
   father_level [initialVertice-1][1] = 0;

   /*puts the value of the initial vertice in the stack*/
   auxStack.push(initialVertice);

   while (!auxStack.empty())
   {
     /*gets top of the stack*/
     auxVertice = auxStack.top();
     /*remove it from the stack*/
     auxStack.pop();

     switch(type)
     {
       /*if adjacency list*/
       case 0:
         /*if auxVertice is not marked*/
         if (marked[auxVertice-1].test(0) == false)
         {
           /*marking vertice*/
           marked[auxVertice-1].set();
           /*Getting the adjacency list for the Vertice grafos[auxVertice -1]*/
           adjacents = grafo[auxVertice - 1].getVerticeList();
           /*for every adjacent vertice, add it to the stack*/
           for (it = adjacents.begin(); it != adjacents.end(); it++)
           {
             auxStack.push(it);
             /*Also, update the father and level of the adjacent vertices*/
             father_level [it-1][0] = auxVertice;
             father_level [it-1][1] = father_level[auxVertice-1] [1] + 1
           }
         }
         break;

       /*if matrix*/
       case 1:

        if (marked[auxVertice-1].test(0) == false)
        {
          marked[auxVertice-1].set();
          adjRow = grafo[auxVertice-1].getVerticeMatrix();

          for (auto const &vertice: adjRow)
          {
            auxStack.push(vertice);
            father_level [vertice-1][0] = auxVertice;
            father_level [vertice-1][1] = father_level[auxVertice-1] [1] + 1
          }

        }
        break;
     }

   }/*end while*/

 }
