#ifndef COEDITOR_H
#define COEDITOR_H

#include <string>
#include <thread>
#include <chrono>
#include "BoundedBuffer.hpp"

class CoEditor {
public:
    CoEditor(BoundedBuffer* inQueue, BoundedBuffer* outQueue);
    void edit();

private:
    BoundedBuffer* inQueue;
    BoundedBuffer* outQueue;
};

#endif // COEDITOR_H
