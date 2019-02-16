#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

typedef struct file {
    char fname[MAX_FNAME_SIZE];
    int size;
} file;

int comp (const void *a, const void *b) {
    struct file *l = (struct file*) a;
    struct file *r = (struct file*) b;

    if(l->size < r->size) return 1;

    return -1;
}



int main( int argc, char ** argv) {
    char fsuffix[MAX_FNAME_SIZE];
    int N;

    // handle command line arguments
    if( argc != 3) {
        fprintf(stderr, "\n Need to input two arguments: ./scan <suffix> <N> \n");
        exit(-1);
    }
    else {
        strcpy(fsuffix, argv[1]);
        N = atoi(argv[2]);
    }

    // open 'find...' to recursively get list of files in directory
    FILE * fp = popen("./myFind", "r");
    if( fp == NULL) {
        perror("popen failed:");
        exit(-1);
    }
    char ext[10];

    // read in all filenames ending in suffix into array
    char buff[MAX_FNAME_SIZE];
    int nFiles = 0;
    char * files[MAX_FILES];
    while(fgets(buff,MAX_FNAME_SIZE,fp)) {

      for(int k=0; k<strlen(fsuffix);k++){
        ext[k]=buff[strlen(buff)-1-strlen(fsuffix)+k];
      }
      int elen=strlen(ext)-1;

      if(ext[elen]=='\n')
          ext[elen]=='\0';

      //printf(" %d \n",strcmp(fsuffix, ext)==0);
      if (strcmp(fsuffix, ext)==0) {
        //printf("There is a file type match \n");
          int len = strlen(buff);
          files[nFiles] = strndup(buff,len-1);

          nFiles ++;
      }
    }
    fclose(fp);

    //Get files and sizes, put in array of type file
    //Sum up file sizes
    long long totalSize = 0;
    struct stat st;
    struct file filesArray[MAX_FILES];




    for(int i = 0 ; i < nFiles ; i ++ ) {
        //cout<<i<<endl;
        if( 0 != stat(files[i],&st)) {
            perror("stat failed:");
            exit(-1);
        }

        struct file nf;
        strcpy(nf.fname, files[i]);
        nf.size = st.st_size;
        filesArray[i] = nf;


    }

    //sort array of files by size
    qsort(filesArray, nFiles, sizeof(struct file), comp);


    //print N elements of sorted array
    if(N > nFiles)
        N = nFiles;

    for(int j = 0; j < N; j++) {
        printf("%s : %i\n", filesArray[j].fname, filesArray[j].size);
          totalSize +=filesArray[j].size;
    }


    printf("Total size: %lld bytes.\n", totalSize);
    // clean up
    for(int i = 0; i < nFiles ; i ++ ) {
        free(files[i]);
    }
    // return success
    return 0;
}
