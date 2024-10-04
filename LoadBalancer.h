#ifndef LOADBALANCER_H
#define LOADBALANCER_H
#include "InstanceHost.h"

typedef struct balancer balancer;

struct job_node {
    int currentID;
    int jobData;
    int* resultData;
    struct job_node* nextJob;
};


balancer* balancer_create(int batch_size);

void balancer_destroy(balancer** loadBalance);

void balancer_add_job(balancer* loadBalance, int user_id, int data, int* data_return);
#endif //LOADBALANCER_H
