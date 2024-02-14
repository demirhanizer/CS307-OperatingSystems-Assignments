#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t barrier;

pthread_barrier_t barrier1;

sem_t semA;
sem_t semB;
sem_t sem;

int countAF = 0;
int countBF = 0;
int countID = 0;

int preA = 0;
int preB = 0;
int total = 0;
bool check = false;
bool preCheckA = false;
bool preCheckB = false;
bool checkControl = false;
bool woke = false;

void *Support(void *arg) {
    pthread_mutex_lock(&lock);

    char team = *((char *) arg);

    printf("Thread ID: %ld, Team: %c, I am looking for a car\n", pthread_self(), team);
    
    if ((countAF == 1 && countBF >= 2 && team == 'A') || (countAF >= 2 && countBF ==1 && team == 'B')) {
        check = true;
        if (team == 'A' && countBF == 3) {
            countBF = 1;
            countAF = 0;
        }
        if (team == 'A' && countBF == 2) {
            countAF = 0;
            countBF = 0;
        }
        if(team == 'B' && countAF == 3) {
            countAF = 1;
            countBF = 0;
        }
        if (team == 'B' && countAF == 2) {
            countAF = 0;
            countBF = 0;
        }
        sem_post(&semA);
        sem_post(&semB);

        //pthread_mutex_unlock(&lock);
        if (team == 'A') {
            sem_post(&semB);
        }
        if (team == 'B') {
            sem_post(&semA);
        }
    }
    else if (countAF == 3 && team == 'A') {
        check = true;
        countAF = 0;
        //pthread_mutex_unlock(&lock);
        for(int i = 0 ; i < 3 ; i++) {
            sem_post(&semA);
        }
    }
    else if (countBF == 3 && team == 'B') {
        check = true;
        //pthread_mutex_unlock(&lock);
        countBF = 0;
        for(int i = 0 ; i < 3 ; i++) {
            sem_post(&semB);
        }
    }
    else if (team == 'A') {
        countAF++;
        pthread_mutex_unlock(&lock);
        sem_wait(&semA);
    }
     
    else if (team == 'B') {
        countBF++;
        pthread_mutex_unlock(&lock);
        sem_wait(&semB);
    }
    //pthread_barrier_wait(&barrier1);
    printf("Thread ID: %ld, Team: %c, I have found a spot in a car\n", pthread_self(), team);
    
    //pthread_mutex_unlock(&lock);
    
    //bir arabaya oturursa başka arabaya oturamaz
    
    pthread_barrier_wait(&barrier);
    total++;

    if (total != 0 && total % 4 == 0) {
        total = 0;
        check = false;
        printf("Thread ID: %ld, Team: %c, I am the captain and driving the car with ID %d\n", pthread_self(), team, countID);
        countID++;
        pthread_mutex_unlock(&lock);
    }
    //pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
if (argc != 3) {
        printf("The main terminates\n");
        return -1;
    }
    int countA = atoi(argv[1]);
    int countB = atoi(argv[2]);
    
    if (countA % 2 != 0 || countB % 2 !=0 || (countA+countB) % 4 != 0 || countA == 0 || countB == 0) {
    printf("The main terminates\n");
    return -1;
    }

    int countT = 4;
    pthread_barrier_init(&barrier, NULL, 4);
    pthread_barrier_init(&barrier1, NULL, 4);

    int preCountA = countA;
    int preCountB = countB;

    pthread_t *threads = (pthread_t *)malloc((preCountA + preCountB) * sizeof(pthread_t));

    sem_init(&semA, 0, 0);
    sem_init(&semB, 0, 0);
    


while (countA > 0) {

pthread_t thA;
char teamA = 'A';
pthread_create(&thA, NULL, Support, &teamA);

threads[preCountA - countA] = thA;
countA--;
}

while (countB > 0) {
pthread_t thB;
char teamB = 'B';
pthread_create(&thB, NULL, Support, &teamB);

threads[preCountA+preCountB - countB] = thB;
countB--;
}

for (int i = 0 ; i < preCountA+preCountB ; i++) {
//printf("th: %ld\n", threads[i]);
pthread_join(threads[i], NULL);

}
free(threads);

printf("The main terminates\n");
return 0;
}
//asdasd

