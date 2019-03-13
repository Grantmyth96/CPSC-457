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

#define MAX_SIZE 10000

int count = 0;
int nThreads;
int volatile prime_flag = 1;
pthread_mutex_t mutex;

typedef struct {
	int64_t n, i, max;
}data;
data *crit;

/// primality test, if n is prime, return 1, else return 0
int isPrime_initial(int64_t n) {
     if( n <= 1) return 0; // small numbers are not primes
     if( n <= 3) return 1; // 2 and 3 are prime
     if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
		 int64_t i = 5;
	   int64_t max = sqrt(n);
	   while( i <= max) {
	       if (n % i == 0 || n % (i+2) == 0) return 0;
	       i += 6;
	   }
	   return 1;
}

void isPrime_post(int64_t n, int64_t i, int64_t max) {

  while( i <= max) {
      if (n % i == 0 || n % (i+2) == 0) {
				prime_flag = 0;
				return;
			}
      i += 6;
  }
  return;
}

void *findPrime(void * d) {
	data *crit = (data*) d;
	isPrime_post(crit->n, crit->i, crit->max);
	pthread_exit(0);
}


int main( int argc, char ** argv) {
    /// parse command line arguments
    nThreads = 1;
    if( argc != 1 && argc != 2) {
        printf("Uasge: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }
    if( nThreads != 1) {
        //printf("I am not multithreaded yet :-(\n");
        //exit(-1);
    }

    /// count the primes
    printf("Counting primes using %d thread%s.\n",
           nThreads, nThreads == 1 ? "s" : "");
/////////////////adding numbers to list//////////////////////////////////////////////////////////////
    pthread_t threads[nThreads];
    int64_t num_list[MAX_SIZE];
    int nTotal = 0;
    while(1) {
      if(1 != scanf("%ld", & num_list[nTotal])) break;
      nTotal++;
    }

/////////////calculating prime///////////////////////////////////////////////////////////////////////
    for(int n=0; n<nTotal; n++) {

			int64_t num = num_list[n];

      if(sqrt(num)<=5 || nThreads>(sqrt(num))-5) {
					 if(isPrime_initial(num)) count++;

				}else {
				int64_t first, second;

				crit =new data[nThreads];

				float temp = sqrt(num) - float(5);
				int64_t temp_2 = nearbyint(temp);
				double temp_3 = (double)temp_2 / (double)6;
				int64_t T = nearbyint(temp_3);

				int64_t f_Partition = T%nThreads;
				first = (T/nThreads) + 1;
				second = T/nThreads;
				int64_t i = 5;
				int64_t max;

				if(f_Partition != 0) {
					max = (first*6)-1;
				}else {
					max = second*6;
				}

				for(int64_t l=0; l<f_Partition; l++) {
					crit[l].n = num;
					crit[l].i = i;
					crit[l].max = max;
					if ((max-5)%6!=0){
		        i = max+5;
		      }
		      else{
		        i = max+6;
		      }
		      max =first*6 + i;
				}

				if (max!= (second*6)-1) {
						max = i + (second*6) +12;
				}

				for(int64_t l=f_Partition; l<nThreads; l++) {
					crit[l].n = num;
					crit[l].i = i;
					crit[l].max = max;
					if((max-5)%6!=0) {
						i = max + 5;
					}else {
						i = max;
					}
					max = second*6 + i;
				}

				for(int l=0; l<nThreads; l++) {
	           long status = pthread_create(&threads[l], NULL, findPrime, (void *) &crit[l]);
	           if(status != 0) {
	               printf("Error in pthread_create\n");
	               exit(-1);
	             }
	           }
	           for(int l=0; l<nThreads; l++) {
	             pthread_join(threads[l], NULL);
	           }
						 if(prime_flag==1) count++;
						 prime_flag = 1;
          }
      }

    /// report results
    printf("Found %ld primes.\n", count);

    return 0;
}
