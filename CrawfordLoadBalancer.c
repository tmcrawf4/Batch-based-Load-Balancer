/**
*Program simulates a data server and performs batch based load balancing
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
#include "LoadBalancer.h"

struct balancer{
    struct host *instHost;
    struct job_node *headNode;
    int nodeSize;
    int hostRequest;
    pthread_mutex_t *nodeLock;
};

balancer* balancer_create(int batch_size){
    balancer *loadBalance = (balancer*)malloc(sizeof(struct balancer));
    loadBalance->headNode = (struct job_node*)malloc(sizeof(struct job_node) * batch_size);
    loadBalance->headNode = NULL;
    pthread_mutex_init(&loadBalance->nodeLock, NULL);
    loadBalance->instHost = host_create();
    loadBalance->nodeSize = batch_size;
    loadBalance->hostRequest = 0;
    return loadBalance;
}

void balancer_destroy(balancer** loadBalance){
    struct balancer* loadNode = *loadBalance;
    pthread_mutex_lock(&loadNode->nodeLock);
    if(loadNode->headNode != NULL){
        host_request_instance(loadNode->instHost, loadNode->headNode);
        loadNode->hostRequest = 0;
        loadNode->headNode = NULL;
    }
    pthread_mutex_unlock(&loadNode->nodeLock);
    host_destroy(&loadNode->instHost);
    free(loadNode->headNode);
    loadNode->headNode = NULL;
    free(*loadBalance);
    *loadBalance = NULL;
}

void balancer_add_job(balancer* loadBalance, int user_id, int data, int* data_return){
    printf("LoadBalance: New process initialized by user# %d to process data = %d and store it in %p.\n" , user_id, data, data_return);

    pthread_mutex_lock(&loadBalance->nodeLock);

    struct job_node *bNode = (struct job_node*)malloc(sizeof(struct job_node));
    bNode->currentID = user_id;
    bNode->jobData = data;
    bNode->resultData = data_return;
    bNode->nextJob = loadBalance->headNode;
    loadBalance->headNode = bNode;
    loadBalance->hostRequest++;

    if(loadBalance->hostRequest == loadBalance->nodeSize){
        host_request_instance(loadBalance->instHost, loadBalance->headNode);
        loadBalance->hostRequest = 0;
        loadBalance->headNode = NULL;
    }

    pthread_mutex_unlock(&loadBalance->nodeLock);
}
