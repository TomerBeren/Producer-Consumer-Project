#include "ScreenManager.hpp"

ScreenManager::ScreenManager(BoundedBuffer* queue) : queue(queue) {}

void ScreenManager::display() {
    int doneCount = 0;
    while (doneCount < 3) {
        std::string message = queue->remove();
        if (message == "DONE") {
            doneCount++;
        } else {
            std::cout << message << std::endl;
        }
    }
    std::cout << "DONE" << std::endl;
}
