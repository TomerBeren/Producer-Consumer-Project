#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class BoundedBuffer {
public:
    BoundedBuffer(int size);
    void insert(const std::string& item);
    std::string remove();

private:
    std::queue<std::string> buffer;
    int maxSize;
    std::mutex mtx;
    std::condition_variable cond;
};

#endif // BOUNDEDBUFFER_H
