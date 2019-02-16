/*#######################
# First Name:  Juan Luis
# Last Name:   de Reiset
# Student ID:  30050167
# Course:      CPSC 457
# Tutorial:    T02
# Assignment:  2
# Question:    Q2
# File name:   myFind.cpp
########################*/

#include <dirent.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

void printfiles(const char* path);


int main()
{

    printfiles(".");

    return 0;
}
void printfiles(const char* path){
	
	DIR *dir = opendir(path);
	if(!dir) return;
	struct dirent *entry;
	char fpath[1025];
	
    
	
	
    while (entry = readdir(dir))
    {
		strcpy(fpath, path);
		strcat(fpath, "/");
		strcat(fpath, entry->d_name);
		
		if(strcmp(".", entry->d_name) && strcmp("..",entry->d_name)){
		
		
		 
		 if(entry->d_type!= DT_DIR){
		
		 printf("%s/%s\n", path, entry->d_name);
		}
		 printfiles(fpath);
       
     }
       
    }

    closedir(dir);
}
