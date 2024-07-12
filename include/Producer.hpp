#ifndef PRODUCER_H
#define PRODUCER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "BoundedBuffer.hpp"

class Producer {
public:
    Producer(int id, int numProducts, BoundedBuffer* buf);
    void produce();

private:
    int producerID;
    int numProducts;
    BoundedBuffer* buffer;
    std::vector<std::string> types;
    std::unordered_map<std::string, int> typeCount;
};

#endif // PRODUCER_H
