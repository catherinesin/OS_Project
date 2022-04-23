#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <errno.h>
//Error handling for pthread_create and pthread_join
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

long totalPts; //total number of points
long ptsInCircle  = 0; //number of points in circle

void *countInCircle(){ 
    // initialize random numbers
    for (long i=0; i < totalPts; i++){	
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        
        if (x * x + y * y <= 1){ // if current point is within the circle
            ptsInCircle++;
        }
    }
    return NULL;
}

int main(){
    printf("\n\nCaculating Pi by Monte Carlo Method(Multithreaded Version)\n\n");
    srand(time(NULL));
    pthread_t thread;

    do{
        printf("Please enter a positive number for the amount of points you would like to generate: ");
        scanf("%ld", &totalPts);  	
	}while (totalPts <= 0);
  	int res1,res2;
    res1 = pthread_create(&thread, NULL, &countInCircle, NULL);
    if (res1 != 0){ // create error occurred
    	handle_error_en(res1, "pthread_create");
	} 
    res2 = pthread_join(thread, NULL);
    if (res2 != 0){ // join error occurred
    	handle_error_en(res2, "pthread_join");
	} 
    //calculate pi using Monte Carlo method and global incircle count
    double pi = (4.0 * ptsInCircle) / totalPts;
    printf("The approximate value of pi for the desired amount of points (%ld) is: %f \n", totalPts, pi);  
    return 0;
}
