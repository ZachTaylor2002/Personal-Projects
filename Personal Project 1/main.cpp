/*
                Author: Zachary Taylor
                Apr/27th/2023
                Project purpose: Dijkstra's Algorithm: Implementation with C++
   program
                */
#include <climits>
#include <fstream>
#include <iostream>
using namespace std;

const int V = 9;
// Function to print shortest path from source to j
// using parent array
void printPath(int parent[], int j) {
  char str = 65 + j;
  // Base Case : If j is source
  if (parent[j] == -1) {
    cout << str << " ";
    return;
  }

  printPath(parent, parent[j]);

  // printf("%d ", j);
  cout << str << " ";
}

int miniDist(int distance[], bool Tset[]) // finding minimum distance
{
  int minimum = INT_MAX, ind;

  for (int k = 0; k < V; k++) {
    if (Tset[k] == false && distance[k] <= minimum) {
      minimum = distance[k];
      ind = k;
    }
  }
  return ind;
}

void DijkstraAlgo(int graph[V][V], int src, int ending_node) // adjacency matrix
{
  int distance[V]; // array to calculate the minimum distance for each node
  bool Tset[V];    // boolean array to mark visited and unvisited for each node
  int parent[V];   // Parent array to store shortest path tree
                 // Value of parent[v] for a vertex v stores parent vertex of v
                 // in shortest path tree.

  for (int k = 0; k < V; k++) {
    distance[k] = INT_MAX;
    Tset[k] = false;
  }
  parent[src] = -1;  // Parent of root (or source vertex) is -1.
  distance[src] = 0; // Source vertex distance is set 0

  for (int k = 0; k < V; k++) {
    int m = miniDist(distance, Tset);
    Tset[m] = true;
    for (int k = 0; k < V; k++) {
      // updating the distance of neighbouring vertex
      if (!Tset[k] && graph[m][k] && distance[m] != INT_MAX &&
          distance[m] + graph[m][k] < distance[k]) {
        distance[k] = distance[m] + graph[m][k];
        parent[k] = m;
      }
    }
  }
  cout << "Vertex\t\tDistance from source vertex\t\t Path" << endl;

  int k = ending_node;
  char str = 65 + ending_node;
  cout << str << "\t\t\t" << distance[ending_node] << "\t\t\t\t\t\t\t\t";
  printPath(parent,
            ending_node); // Printing out the vertex, the distance and the Path
  cout << endl;
}

int main() {
  ifstream file_object; // Declaring the file object
  int graph[V][V];      // Declaring the 2-D list
  char starting_node;   // Declaring the character for the starting node
  char ending_node;     // Declaring the character for the ending node
  file_object.open("graph.txt"); // Opening the file
  for (int x = 0; x < V;
       x++) { // Nested for loops to store the 2-D list from the Graph.txt file
    for (int z = 0; z < V; z++) {
      file_object >> graph[x][z];
    }
  }
  cout << "Nodes:";
  for (int k = 0; k < V; k++) { // For loop to display the nodes letters
    char str = 65 + k;
    if (k != V - 1)
      cout << str << ", ";
    else
      cout << str << "" << endl;
  }
  cout << "Enter starting node: ";
  cin >> starting_node; // Asking the user for the starting and ending nodes as
                        // characters
  cout << "Enter ending node: ";
  cin >> ending_node;
  cout << endl;

  DijkstraAlgo(graph, starting_node - 65,
               ending_node - 65); // calling the function DijkstraAlgo function
                                  // sending in three parameters.

  return 0;
}
