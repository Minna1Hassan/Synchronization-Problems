#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define NUM_SAVAGES 3


void *savage (void*);
void *cook (void*);
static pthread_mutex_t servings_mutex;
static pthread_mutex_t print_mutex;
static int servings = 15;
static int rounds = 2;

void putServingsInPot (int num) {

    	servings += num;
	rounds--;
 	pthread_mutex_lock (&print_mutex);
        printf ("\nCook filled pot\n\n");
        pthread_mutex_unlock (&print_mutex);

}

void getServingsFromPot(void) {
    if (servings <= 0) {
        putServingsInPot(15);
        servings--;
    } else {
        servings--;
    }
}

void *savage (void *id) {
    int savage_id = (int)id;
       do{ 
	pthread_mutex_lock (&servings_mutex);
  	getServingsFromPot();
        pthread_mutex_unlock (&servings_mutex);

        pthread_mutex_lock (&print_mutex);
        printf ("Savage: %d is eating\n", savage_id);
        pthread_mutex_unlock (&print_mutex);
        
        sleep(2);
        
        pthread_mutex_lock (&print_mutex);
        printf ("Savage: %d is DONE eating\n", savage_id);
        pthread_mutex_unlock (&print_mutex);
	
	
	}while(rounds > 0);

    }
  
int main() {

    int i, id[NUM_SAVAGES];
    pthread_t tid[NUM_SAVAGES];

    // Initialize the mutex locks
    pthread_mutex_init(&servings_mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);


    for (i=0; i<NUM_SAVAGES; i++) {
       id[i] = i;
       pthread_create (&tid[i], NULL, savage, (void *)&id[i]);
        }
   
    for (i=0; i<NUM_SAVAGES; i++)
       pthread_join(tid[i], NULL);
	
	pthread_mutex_destroy(&servings_mutex);
    	pthread_mutex_destroy(&print_mutex);
	pthread_exit(NULL);
       
       
}
