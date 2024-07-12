#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <vector>
#include <string>
#include "BoundedBuffer.hpp"

class Dispatcher {
public:
    Dispatcher(std::vector<BoundedBuffer*>& prodQueues, BoundedBuffer* sportsQueue, BoundedBuffer* newsQueue, BoundedBuffer* weatherQueue);
    void dispatch();

private:
    std::vector<BoundedBuffer*> prodQueues;
    BoundedBuffer* sportsQueue;
    BoundedBuffer* newsQueue;
    BoundedBuffer* weatherQueue;
};

#endif // DISPATCHER_H
