#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

struct Node {
    int ID;
    int SIZE;
    int INDEX;
    int totalSize;
    Node* next;
    Node *prev;
    
    Node(int id, int size, int index) : ID(id), SIZE(size), INDEX(index), next(NULL) {}
};

class HeapManager {
private:
    Node* heap;
    mutex mtx;
    sem_t sem;
    pthread_mutex_t lock;
public:
    HeapManager() : heap(NULL) {}

    int initHeap(int size) {
        sem_init(&sem, 0, 1);
        lock = PTHREAD_MUTEX_INITIALIZER;
        if (size <= 0) {
            return -1;
        }

        heap = new Node(-1, size, 0);
        heap->totalSize = size;
        cout << "Memory initialized" << endl;
        print();

        return 1;
    }

int myMalloc(int ID, int size) {
    sem_wait(&sem);

    Node* curr = heap;
    Node* prev = NULL;

    while (curr != NULL) {
        bool checkEq = false;
        if (curr->ID == -1 && curr->SIZE >= size) {
            Node* newNode;
            if (curr->SIZE > size) {
                newNode = new Node(ID, size, curr->INDEX);
                newNode->next = curr;
                newNode->prev = curr->prev;
                if (curr->prev != nullptr) {
                    curr->prev->next = newNode;
                    
                }
                else {
                    heap = newNode;
                }
                curr->prev = newNode;
                curr->INDEX += size;
                curr->SIZE -= size;
                
            } else {
                curr->ID = ID;
                checkEq = true;
            }
            cout << "Allocated for thread " << ID << endl;
            pthread_mutex_lock(&lock);
            print();
            pthread_mutex_unlock(&lock);
            sem_post(&sem);
            if (!checkEq) {
                return newNode->INDEX;
            }
            return curr->INDEX;
            
        }
        
        prev = curr;
        curr = curr->next;
    }
    cout << "Can not allocate, requested size " << size << " for thread " << ID << " is bigger than remaining size" << endl;
    pthread_mutex_lock(&lock);
    print();
    pthread_mutex_unlock(&lock);
    sem_post(&sem);
    return -1;
}



int myFree(int ID, int index) {
    sem_wait(&sem);

    Node* curr = heap;
    Node* prev = NULL;
    while (curr) {
        if (curr->ID == ID && curr->INDEX == index) {
            curr->ID = -1;
            if (curr && curr->next && curr->next->ID == -1) {
                curr->SIZE += curr->next->SIZE;
                curr->next = curr->next->next;
                
            }
            
            
            
            if (prev && prev->ID == -1) {
                prev->SIZE += curr->SIZE;
                prev->next = curr->next;
                if (curr->next) {
                    curr->next->prev = prev;
                }
                
                delete curr;
                curr = prev;
            }
            
            
            cout << "Freed for thread " << ID << endl;
            pthread_mutex_lock(&lock);
            print();
            pthread_mutex_unlock(&lock);
            sem_post(&sem);
            return 1;
        }
        if (curr) {
            prev = curr;
            curr = curr->next;
        }
        
    }
    cout << "Failure in free" << endl;
    sem_post(&sem);
    return -1;
}


    
    void print() {
        
        Node* curr = heap;
        while (curr) {
            cout << "[" << curr->ID << "][" << curr->SIZE << "][" << curr->INDEX << "]" ;
            if (curr->next != NULL) {
                cout << "---";
            }
            curr = curr->next;
        }
        cout << endl;
        
    }
    



};

