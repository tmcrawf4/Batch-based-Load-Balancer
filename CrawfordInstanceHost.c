/**
* Program simulates a data server and performs batch based load balancing
* utilizing a cloud environment
*
* Completion time: 10 hours
*
* @author Tristan Crawford, Suthar
* @version CLion 2023.2
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "InstanceHost.h"

typedef struct host{
    int instCounter;
    pthread_t *jobThread;
};

void dataProcess(struct job_node* batch);

host* host_create(){
    host *newHost = (struct host *)malloc(sizeof(host));
    newHost->jobThread = (pthread_t *)malloc(sizeof(pthread_t));
    newHost->instCounter = 0;
    return newHost;
}

void host_destroy(host** host){
    struct host* currentHost = *host;
    free(currentHost->jobThread);
    currentHost->jobThread = NULL;
    free(*host);
    *host = NULL;
}

void host_request_instance(host* h, struct job_node* batch){
    printf("LoadBalance: Batch received, starting new instance.\n");
    h->instCounter++;
    pthread_t *tempNode = h->jobThread;
    h->jobThread = (pthread_t *)realloc((tempNode), sizeof(pthread_t) * (int)h->instCounter + 1);
    free(tempNode);
    pthread_create(&h->jobThread[h->instCounter], NULL, dataProcess, (void*)batch);
    pthread_join(h->jobThread[h->instCounter], NULL);
    h->jobThread = NULL;
    h->instCounter--;
}

void dataProcess(struct job_node* dataBatch){

    while(dataBatch != NULL) {
        *dataBatch->resultData = (dataBatch->jobData) * (dataBatch->jobData);
        dataBatch = dataBatch->nextJob;
    }
}
