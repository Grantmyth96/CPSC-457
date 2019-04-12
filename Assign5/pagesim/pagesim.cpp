//**********************************
// Author: Juan Luis de Reiset 
// Student ID: 30050167
// Tutorial: T01
//
// Compile with g++ pagesim.cpp -o pagesim
// Run with ./pagesim <number of frames> < filename.txt> 
//*********************************

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
using namespace std;


int main( int argc, char ** argv)
{
	vector<int> v;
	int naframes;
    /// parse command line arguments
    if( argc != 2) {
        printf("Usage: pagesim <number of frames> < filename.txt\n");
        exit(-1);
    }
	else
		naframes = atoi(argv[1]);
	
	/// Get string from input 
    while(1) {
        int64_t num;
        if( 1 != scanf("%ld", & num)) break;
		v.push_back(num);
	}
	
	/// Call algorithms 
	Optimal(v, nframes);
	LRU(v, nframes);
	Clock(v, nframes);
    return 0;
}
