#ifndef INSTANCEHOST_H
#define INSTANCEHOST_H
#include "LoadBalancer.h"

typedef struct host host;

struct job_node;

host* host_create();

void host_destroy(host**);

void host_request_instance(host* h, struct job_node* batch);
#endif //INSTANCEHOST_H
