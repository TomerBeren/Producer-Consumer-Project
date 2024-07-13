#include "BoundedBuffer.hpp"

BoundedBuffer::BoundedBuffer(int size) : maxSize(size) {
    sem_init(&emptySlots, 0, size); // Initialize emptySlots with maxSize
    sem_init(&fullSlots, 0, 0);     // Initialize fullSlots with 0
}

BoundedBuffer::~BoundedBuffer() {
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
}

void BoundedBuffer::insert(const std::string& item) {
    sem_wait(&emptySlots);  // Wait for an empty slot
    std::unique_lock<std::mutex> lock(mtx);
    buffer.push(item);
    lock.unlock();
    sem_post(&fullSlots);   // Signal that there is a new full slot
}

std::string BoundedBuffer::remove() {
    sem_wait(&fullSlots);   // Wait for a full slot
    std::unique_lock<std::mutex> lock(mtx);
    std::string item = buffer.front();
    buffer.pop();
    lock.unlock();
    sem_post(&emptySlots);  // Signal that there is a new empty slot
    return item;
}
