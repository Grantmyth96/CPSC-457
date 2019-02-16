#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_SIZE 512
#define MAX_NUM  1000000


char filename[MAX_SIZE];
int T;
int count_ints;
int int_Array[MAX_NUM];
int SUM;
pthread_mutex_t mutex_sum;
int tsum_counter=0;
int ind_pos=0;


void* add_threads(void *tid){

  //int start,ending,lastPart;
  int div =count_ints/T;

  //float div =count_ints/T;
  //int part_at= ceil(div);
  //int remaining =part_at *T;
  int thrSum=0;
  long int itid= (long int) tid;

  int firstgroup_partition=(count_ints%T);
  int secondgroup_partition=(T-count_ints%T);

/*
  if((remaining-count_ints)==0)
    lastPart= part_at;
  else
    lastPart= remaining -count_ints;


  if(itid == (T-1)){
    start= itid *part_at;
    ending = start + part_at -lastPart;
  }
  else{
    start= (itid * part_at);
    ending = start +part_at;
  }
*/
  pthread_mutex_lock(&mutex_sum);

if(tsum_counter<firstgroup_partition){

  
  for(int i=0;i<div+1;i++){
    thrSum += int_Array[ind_pos];
    ind_pos++;
    }
    tsum_counter++;
  }

else{

  for(int i=0;i<div;i++){
    thrSum += int_Array[ind_pos];
    ind_pos++;
  }
  tsum_counter++;
}
  //pthread_mutex_unlock(&mutex_sum);
//printf("The program has counter: %d   SUMS in total");
  //pthread_mutex_lock(&mutex_sum);
  printf("\t Thread %ld: %d \n", (itid+1),thrSum);
//  pthread_mutex_unlock(&mutex_sum);

//  pthread_mutex_lock(&mutex_sum);
  SUM += thrSum;
  pthread_mutex_unlock(&mutex_sum);

  pthread_exit(0);


}



int main( int argc, char ** argv) {

  if(argc !=3){
    fprintf(stderr,"\n You need to provide two arguments: <filename> <T> \n\n");
    exit(-1);
  }
  else{
    strcpy(filename,argv[1]);
    T=atoi(argv[2]);
  }

  //open file
  FILE *fp = fopen(filename, "r");
  if(fp == NULL){
    perror("popen failed: ");
    exit(-1);
  }

  //read in all integers found in the file and store them in an array
  char buff[MAX_SIZE];
  count_ints=0;

  while(fgets(buff,MAX_SIZE,fp)){
    int len= strlen(buff);
    int k= atoi(strndup(buff,len));
    int_Array[count_ints]=k;
    count_ints++;
  }
  fclose(fp);

  pthread_t threads [T];
  long i, status;
  SUM=0;

  for(i=0;i< T; i++){
    status =pthread_create(&threads[i], NULL, add_threads,(void*)i);
    if(status!=0){
      printf("Error in pthread_create \n");
      exit(-1);
    }

  }

for(i=0;i< T;i++){
  pthread_join(threads[i],NULL);
}

pthread_mutex_destroy(&mutex_sum);
printf("\t SUM = %d \n",SUM);
return 0;
}
