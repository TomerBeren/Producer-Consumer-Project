#include "CoEditor.hpp"

CoEditor::CoEditor(BoundedBuffer* inQueue, BoundedBuffer* outQueue)
    : inQueue(inQueue), outQueue(outQueue) {}

void CoEditor::edit() {
    while (true) {
        std::string message = inQueue->remove();
        if (message == "DONE") {
            outQueue->insert("DONE");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        outQueue->insert(message);
    }
}
