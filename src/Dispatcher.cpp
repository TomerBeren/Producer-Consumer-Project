#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::vector<BoundedBuffer*>& prodQueues, BoundedBuffer* sportsQueue, BoundedBuffer* newsQueue, BoundedBuffer* weatherQueue)
    : prodQueues(prodQueues), sportsQueue(sportsQueue), newsQueue(newsQueue), weatherQueue(weatherQueue) {}

void Dispatcher::dispatch() {
    std::vector<bool> doneFlags(prodQueues.size(), false); // Track DONE status for each producer
    size_t doneCount = 0;

    while (doneCount < prodQueues.size()) {
        for (size_t i = 0; i < prodQueues.size(); ++i) {
            if (!doneFlags[i]) { // Only process if this producer hasn't finished
                std::string message = prodQueues[i]->remove();
                if (message == "DONE") {
                    doneFlags[i] = true;
                    ++doneCount;
                } else {
                    // Sort message into the appropriate dispatcher queue
                    if (message.find("SPORTS") != std::string::npos) {
                        sportsQueue->insert(message);
                    } else if (message.find("NEWS") != std::string::npos) {
                        newsQueue->insert(message);
                    } else if (message.find("WEATHER") != std::string::npos) {
                        weatherQueue->insert(message);
                    }
                }
            }
        }
    }

    // Send "DONE" to all dispatcher queues once all producers are finished
    sportsQueue->insert("DONE");
    newsQueue->insert("DONE");
    weatherQueue->insert("DONE");
}
