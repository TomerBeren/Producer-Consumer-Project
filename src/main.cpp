#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include "BoundedBuffer.hpp"
#include "Producer.hpp"
#include "CoEditor.hpp"
#include "ScreenManager.hpp"
#include "Dispatcher.hpp"

int main(int argc, char* argv[]) {
    // Open the configuration file
    std::ifstream configFile(argv[1]);
    if (!configFile.is_open()) {
        std::cerr << "Unable to open file";
        return 1;
    }

    // Read the configuration file
    std::vector<BoundedBuffer*> producerQueues;
    std::vector<Producer*> producers;
    int numProducers = 0;
    std::string line;

    // Read producer configurations
    while (std::getline(configFile, line)) {
        if (line.find("PRODUCER") != std::string::npos) {
            numProducers++;
            int numProducts, prodQueueSize;

            // Read the number of products
            std::getline(configFile, line);
            numProducts = std::stoi(line);

            // Read the queue size
            std::getline(configFile, line);
            prodQueueSize = std::stoi(line.substr(line.find('=') + 1));

            // Create producer queue and producer objects
            BoundedBuffer* queue = new BoundedBuffer(prodQueueSize);
            producerQueues.push_back(queue);
            producers.push_back(new Producer(numProducers, numProducts, queue));
            
        } else if (line.find("Co-Editor queue size") != std::string::npos) {
            // Read the co-editor queue size
            int coEditorQueueSize = std::stoi(line.substr(line.find('=') + 1));

            // Create dispatcher and co-editor queues
            BoundedBuffer sportsQueue(coEditorQueueSize);
            BoundedBuffer newsQueue(coEditorQueueSize);
            BoundedBuffer weatherQueue(coEditorQueueSize);
            BoundedBuffer coEditorQueue(coEditorQueueSize);

            // Create dispatcher, co-editors, and screen manager
            Dispatcher dispatcher(producerQueues, &sportsQueue, &newsQueue, &weatherQueue);
            CoEditor sportsEditor(&sportsQueue, &coEditorQueue);
            CoEditor newsEditor(&newsQueue, &coEditorQueue);
            CoEditor weatherEditor(&weatherQueue, &coEditorQueue);
            ScreenManager screenManager(&coEditorQueue);

            // Start producer threads
            std::vector<std::thread> producerThreads;
            for (auto producer : producers) {
                producerThreads.emplace_back(&Producer::produce, producer);
            }

            // Start dispatcher thread
            std::thread dispatcherThread(&Dispatcher::dispatch, &dispatcher);

            // Start co-editor threads
            std::thread sportsEditorThread(&CoEditor::edit, &sportsEditor);
            std::thread newsEditorThread(&CoEditor::edit, &newsEditor);
            std::thread weatherEditorThread(&CoEditor::edit, &weatherEditor);

            // Start screen manager thread
            std::thread screenManagerThread(&ScreenManager::display, &screenManager);

            // Join all threads
            for (auto& thread : producerThreads) {
                thread.join();
            }
            dispatcherThread.join();
            sportsEditorThread.join();
            newsEditorThread.join();
            weatherEditorThread.join();
            screenManagerThread.join();
        }
    }

    // Clean up
    for (auto queue : producerQueues) {
        delete queue;
    }
    for (auto producer : producers) {
        delete producer;
    }

    return 0;
}
