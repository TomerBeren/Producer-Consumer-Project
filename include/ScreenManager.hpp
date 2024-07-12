#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>
#include <string>
#include "BoundedBuffer.hpp"

class ScreenManager {
public:
    ScreenManager(BoundedBuffer* queue);
    void display();

private:
    BoundedBuffer* queue;
};

#endif // SCREENMANAGER_H
