#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//Error handling for pthread_create and pthread_join
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
        
#define NUN_PTS 10000000
#define NUM_THREADS 5
#define PTS_PER_THREAD NUN_PTS/NUM_THREADS

long ptsInCircle = 0;	// total points in circle
pthread_t *threads;		// Thread Identifier Decleration
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex init

void *calPi(void *thread_ID) {
	int tid = (int) thread_ID;		// the number of current thread
	long incircle_thread = 0; 		// # of points in this threads circle
	
	// create PTS_PER_THREAD # of points 
	for (long i = 0; i < PTS_PER_THREAD; i++) {		
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		
		if (x * x + y * y <= 1){ // if current point is within the circle		
			incircle_thread++;			
			}
		}
	// calculation of pi in this thread
	double pi = (4. * (double)incircle_thread) / ((double)PTS_PER_THREAD * 1);
	printf("Thread [%d] estimates pi to be [%f]\n", tid, pi);
	
	// preventing race conditions on Global ptsInCircle
	pthread_mutex_lock(&mutex); //apply the mutual exclusion lock
	ptsInCircle += incircle_thread; // add # of points inside the circle to the Global total ptsInCircle
	pthread_mutex_unlock(&mutex); // unlock the mutual exclusion lock
	return NULL;
}
// createThreads() creates NUM_THREADS # of threads,allocates space for each one
void createThreads(){
	threads = malloc(NUM_THREADS * sizeof(pthread_t)); // allocate space for next thread
	pthread_attr_t attr; // pthread attribute init		
	pthread_attr_init(&attr); // pthread attribute init
	
	printf("\n----------------------------------------\n*Creating [%d] Threads\n\n", NUM_THREADS);
	// Create 1 thread for each NUM_THREADS 
	for (int i = 0; i < NUM_THREADS; i++){
		int res = pthread_create(&threads[i], &attr, calPi,  (void *) i);
	 	 //if we recieve anything other than 0 we have a create error
		if (res != 0){ // create error occurred
			handle_error_en(res, "pthread_create");		 
		}
	}
}
// joinThreads() joins NUM_THREADS # of threads, frees space for each one
void joinThreads(){	
	// join 1 thread for each NUM_THREADS
	for (int i = 0; i < NUM_THREADS; i++) {		
		int res = pthread_join(threads[i], NULL);
		if (res != 0){ // join error occurred
			handle_error_en(res, "pthread_join");		 
		}
	}
	pthread_mutex_destroy(&mutex); // Destroy mutex 
	printf("\n*[%d] Threads Rejoined\n\n", NUM_THREADS);		
	free(threads); // Free allocated space used by threads
}
/* main() acts as the controller, first calling createThreads
   to create our 5 threads, then calling joinThreads to join each thread
   and finally return calculated pi using the 5 threads results
*/
int main() {
	createThreads(); 
	joinThreads(); 	
	// calculate pi using Monte Carlo method and global total points in circle
	double pi = (4. * (double)ptsInCircle) / ((double)PTS_PER_THREAD * NUM_THREADS);	
	printf("The final estimation of pi for the total amount of points (%ld) is: %f\n", NUN_PTS, pi);
	return 0;
}
