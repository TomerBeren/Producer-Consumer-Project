#include "BoundedBuffer.hpp"

BoundedBuffer::BoundedBuffer(int size) : maxSize(size) {}

void BoundedBuffer::insert(const std::string& item) {
    std::unique_lock<std::mutex> lock(mtx);
    cond.wait(lock, [this]() { return buffer.size() < maxSize; });
    buffer.push(item);
    cond.notify_all();
}

std::string BoundedBuffer::remove() {
    std::unique_lock<std::mutex> lock(mtx);
    cond.wait(lock, [this]() { return !buffer.empty(); });
    std::string item = buffer.front();
    buffer.pop();
    cond.notify_all();
    return item;
}
