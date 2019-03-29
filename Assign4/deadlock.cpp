/*
 * deadlock.cpp
 *
 * Student Name: Juan Luis de Reiset
 * Student Number: 30050167
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *
 * Copyright 2017 University of Calgary. All rights reserved.
 */

/*Program structure
 * 
 * loop:
 * 
 * 		set graph to empty
 * 		loop:
 * 			read line from stdin
 * 			if line starts with '#', or EOF reached
 * 				compute, sort and display deadlocks
 * 				break
 * 			else:
 * 				convert line to edge and add it to edge array
 * 		if EOF was encountered:
 * 				break;
 * 		
 */

#include <iostream>
#include <fstream>
#include <sstream> 
#include <list> 
#include <limits.h> 
#include <algorithm>
  
using namespace std; 

#define MAX_INPUTS 100000
#define MAX_DPROC  100

int dlproc[MAX_DPROC]; //array that contains all deadlocked processes
int countdl=0;		   // index for dlproc

class Graph 
	{ 
		int V;    // No. of vertices 
		list<int> *adj;    // Pointer to an array containing adjacency lists 
		
		
		bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic() 
	public: 
		Graph(int V);   // Constructor 
		void addVertex(int x); //to add vertex to graph
		void addEdge(int v, int w);   // to add an edge to graph
		int getVertex(); 
		bool isCyclic();    // returns true if there is a cycle in this graph 
		
		
	}; 
	
	Graph::Graph(int V) 
	{ 
		this->V = V; 
		adj = new list<int>[MAX_INPUTS];
		 
		 //reset deadlock array
		 for(int i=0;i<MAX_DPROC;i++){
			dlproc[i]=10000;
		}
	} 
  
	void Graph::addEdge(int v, int w) 
	{	 
		adj[v].push_back(w); // Add w to v’s list. 
	} 
	
	void Graph::addVertex(int x) 
	{	 
		this->V=x; //sets a new value of the maximun possible index value
				   //for a vortex found in adyascecy list
	} 
	int Graph::getVertex() 
	{	 
		return this->V;
	} 
	
	// Function that recursively checks for a cycle in adyascency list of the graph
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack) 
{ 
    if(visited[v] == false) 
    { 
        // Mark the current node as visited and part of recursion stack 
        visited[v] = true; 
        recStack[v] = true; 
  
        // Recur for all the vertices adjacent to this vertex 
        list<int>::iterator i; 
        for(i = adj[v].begin(); i != adj[v].end(); ++i) 
        { 
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) ) {
				//cout<<"visited[i]== "<<*i<< "\n";
				dlproc[countdl]=*i;
				countdl++;
                return true; 
			}
            else if (recStack[*i]) {
				//cout<<"recStack[i]== "<<*i<< "\n";
				dlproc[countdl]=*i;
				countdl++;
                return true; 
			}
        } 
  
    } 
    recStack[v] = false;  // remove the vertex from recursion stack 
    return false; 
} 
  
// Returns true if the graph contains a cycle, else false. 
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212 
bool Graph::isCyclic() 
{ 
    // Mark all the vertices as not visited and not part of recursion 
    // stack 
    bool *visited = new bool[V]; 
    bool *recStack = new bool[V]; 
    for(int i = 0; i < V; i++) 
    { 
        visited[i] = false; 
        recStack[i] = false; 
    } 
  
    // Call the recursive helper function to detect cycle in different 
    // DFS trees 
    for(int i = 0; i < V; i++) 
        if (isCyclicUtil(i, visited, recStack)) 
            return true; 
  
    return false; 
} 



int main(int argc, char * const argv[]){
	
	ifstream input_file;  //file handler
	string input_name; //string that contains file name
	 
	  // Read in the config file name from the command-line arguments
    if (argc < 2)
    {
        cout << "Usage: ./deadlock <input file>\n";
        return 0;
    }
    else
    {
        input_name = argv[1];
    }
	
	 // Open the file
    input_file.open(input_name.c_str());
	
	std::string line;
	bool eof_flag=false; //flag that indicates if end of file is reached
	
	//While end of file hasnt been reached...continue
	while(!eof_flag){
		
		bool hashtag_flag=false; //flag that indicates if a hashtag was found
		
		Graph g(0);	//creates empty graph
	
		//while hashtag not found
		while(!hashtag_flag){
			
			std::getline(input_file, line);	//Read one line of the file and store its
											//content on "line"
				//if line starts with "#"
				if(line.find("#")==0){
					//printf("---Found #--- \n");
					//cout<< line << "\n";
					hashtag_flag=true;
					
					//execute find cycle
					if(g.isCyclic()) {
						cout << "Deadlocked processes: "; 
						size_t n= sizeof(dlproc)/sizeof(dlproc[0]);
						for(size_t i = 0; i<n; i++){
							if(dlproc[i]<9999)
								std::cout <<dlproc[i] << " ";
						}
						cout<<"\n";
						 //reset deadock array
						for(int i=0;i<MAX_DPROC;i++){
							dlproc[i]=10000;
						}
					}
					else{
						cout << "Deadlocked processes: none \n";
						
					//printf("---Found EOF--- \n"); 
					}
					// execute find cycle
					 
				}
				else if(input_file.eof()){
					
					eof_flag=true;
					hashtag_flag=true;
					
					//execute find cycle
					if(g.isCyclic()) {
						
						cout << "Deadlocked processes: "; 
						
						int n = sizeof(dlproc)/sizeof(dlproc[0]);  // s isused for traversing
																   //array when sorting and printing
																   
						sort(dlproc, dlproc+n); 					// sorts deadlocked processes array
																    // in ascending order
						//prints array of deadlock processes										    
						for(int i = 0; i<n; i++){
							if(dlproc[i]<9999)
								std::cout <<dlproc[i] << " ";
						}
						cout<<"\n"; 
				
					}
					else{
						cout << "Deadlocked processes: none \n";
						
					//printf("---Found EOF--- \n"); 
					}
					// execute find cycle
					
				}
				else{
					//printf(" Found vortex \n");
					//cout<< line<< "\n";
					
		   ///////add edge to array/////////////////////////
					std::istringstream iss(line);
					int v1, v2;
					string operation;
					
					iss >> v1 >> operation >> v2; //save values found in line separated by " "
					
					v2=v2+(10000); //Label resources so that they can be identified in adyancy array
								 // Used to differentiate resources from processes 
					
					if((v2 > v1) && (v2> g.getVertex())){
						g.addVertex(v2);
					}
					else if((v1 > v2) && (v1> g.getVertex())){
						g.addVertex(v1);
					}
					// Registers N -> M (N makes a request for M)
					if(operation=="->"){
						//printf("operation is -> \n");
						g.addEdge(v1, v2);
						//cout << v1 << " requests resources from " << v2 <<" \n";
					}
					// Registers N <-  (N holds the resource M)
					else if(operation=="<-"){
						//printf("operation is <- \n");
						g.addEdge(v2, v1);
						//cout << v1 << " holds resources from " << v2 <<" \n";
					}
				//Clear iss for allowing saving new values in the next loop iteration
					iss.clear();
					
					 ///////add edge to array//////////////////////////
				}	
		}
	}
	
	
	return 0;
}
