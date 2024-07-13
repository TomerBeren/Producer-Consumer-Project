#ifndef BOUNDEDBUFFER_HPP
#define BOUNDEDBUFFER_HPP

#include <mutex>
#include <queue>
#include <string>
#include <semaphore.h>

class BoundedBuffer {
public:
    BoundedBuffer(int size);
    ~BoundedBuffer();
    void insert(const std::string& item);
    std::string remove();

private:
    int maxSize;
    std::queue<std::string> buffer;
    std::mutex mtx;
    sem_t emptySlots;
    sem_t fullSlots;
};

#endif // BOUNDEDBUFFER_HPP
