#include "Producer.hpp"
#include <random>
#include <string>
#include <cstdlib>

Producer::Producer(int id, int numProducts, BoundedBuffer* buf)
    : producerID(id), numProducts(numProducts), buffer(buf) {
    types = { "SPORTS", "NEWS", "WEATHER" };
    for (const auto& type : types) {
        typeCount[type] = 0; // Initialize the count for each type to 0
    }
}

void Producer::produce() {
    for (int i = 0; i < numProducts; ++i) {
        std::string type = types[rand() % 3];
        std::string message = "Producer " + std::to_string(producerID) + " " +
            type + " " + std::to_string(typeCount[type]);
        buffer->insert(message);
        typeCount[type]++; // Increment the count for the chosen type
    }
    buffer->insert("DONE");
}
