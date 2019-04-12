/// counts number of primes from standard input
///
/// compile with:
///   $ gcc findPrimes.c -O2 -o count -lm
///
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

#define MAX_INT_INPUTS 10000
#define MAX_threads 256
pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

int64_t all_numbers[MAX_INT_INPUTS];

int total_n=0;
int prime_count=0;
int nThreads;


//partitions defines the struct used for 'work partitions' for each threat
//start: where will the Thread start working in array of nums
//workload: for how many numbers will the Thread continue to work

typedef struct{
	int64_t start, workload;
	
}partitions;

partitions work[MAX_threads];


// Test if a subgroup of all_numbers are prime or not. 
// If prime, prime_count ++
void isPrime_array(int64_t start, int64_t workload)
{
	//pthread_mutex_lock(&count_mutex);
	//printf("Start is %d \n", start);
	//printf("Workload is %d \n", workload);
	int k=start;
	int ending= start+workload;
	//printf("Ending  is %d \n", ending);
	//pthread_mutex_unlock(&count_mutex);
	
	while(k<ending){
		
		//pthread_mutex_lock(&count_mutex);
		//printf("Int being evaluated: %ld <= ? \n",all_numbers[k]);
		if( all_numbers[k] <= 1){
			k++;
			continue; // small numbers are not primes
		}
		if( all_numbers[k] <=3){
			prime_count++;
			//printf("%ld is PRIME because <=3 \n",all_numbers[k]);
			k++;
			continue;; // 2 and 3 are prime
		}
		if( all_numbers[k] % 2 == 0 || all_numbers[k] % 3 == 0){	
			k++;
			//printf("%ld is NOT PRIME because %2 OR %3 \n",all_numbers[k]);
			continue;; // multiples of 2 and 3 are not primes
      
		}
		//pthread_mutex_unlock(&count_mutex);
		//pthread_mutex_lock(&count_mutex);
		int64_t p = 5;
		int64_t max = sqrt(all_numbers[k]);
		while(p<=max){
			if (all_numbers[k] % p == 0 || all_numbers[k] % (p+2) == 0) {
				k++;
				continue;
			}
			
		p += 6;
		}
		//pthread_mutex_unlock(&count_mutex);
		//pthread_mutex_lock(&count_mutex);
		if(all_numbers[k]==1 || all_numbers[k]==4 ){
			continue;
		}	
		prime_count++;
		//printf(" %ld is PRIME \n",all_numbers[k]);
		k++;
		//pthread_mutex_unlock(&count_mutex);
	}
	
	return;
}

//VOid function used for pthcreate.
void *findPrime(void* x){
	
	partitions *crit = (partitions*) x;
	
	isPrime_array(crit->start, crit->workload); //call function that counts primes
	
	pthread_exit(0);
}



int main( int argc, char ** argv)
{
    /// parse command line arguments
    
    if( argc != 1 && argc != 2) {
        printf("Usage: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
        exit(-1);
    }
   

    /// count the primes
    printf("Counting primes using %d thread%s.\n",
           nThreads, nThreads != 1 ? "s" : "");
        
    pthread_t threads[nThreads];
    
    int total_n = 0;
    while(1) {
      if(1 != scanf("%ld", &all_numbers[total_n])) break;
      total_n++;
    }
   // printf("Total integers found: %d \n",total_n);
   if(total_n > nThreads){
	
    int64_t group_partition_size= nearbyint(total_n/nThreads);
    
    int k; 
    int remainder_partition=total_n;
		for(k=0;k<nThreads;k++){
			//Last group will receive different group partition of work
			if(k==nThreads-1){
			work[k].start=k*group_partition_size;
			work[k].workload=group_partition_size+10;
			}
			//The rest of the groups will receie an equal partition of work
			else{
			work[k].start=k*group_partition_size;
			work[k].workload=group_partition_size;
			}
			
		}
		
		
    
	}
	//If the number of threads is bigger thatn the total amount of numbers, each
	// Thread receives one number to work with.
	else{
	int64_t group_partition_size= 1;
		nThreads=total_n;
		for(int k=0;k<nThreads;k++){
			work[k].start=k*group_partition_size;
			work[k].workload=group_partition_size;
		}
		
		
	}
    
    
    //Creates the specified number of Threads
    for(int i=0;i<nThreads;i++){
		long status= pthread_create(&threads[i], NULL, findPrime,(void *) &work[i]);
		if(status != 0) {
	               printf("Error in pthread_create\n");
	               exit(-1);
	             }
	     
		
	}
    
      for(int l=0; l<nThreads; l++) {
	             pthread_join(threads[l], NULL);
	           }
						


    /// report results
    printf("Found %ld primes.\n", prime_count);

    return 0;
}

