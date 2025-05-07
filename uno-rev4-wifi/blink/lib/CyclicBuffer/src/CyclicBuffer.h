#include "Arduino.h"

class CyclicBuffer {
    private:
    int *_buffer;
    size_t _headIndex;
    size_t _capacity;
    size_t _count;

    public:
    CyclicBuffer(size_t capacity);

    bool enqueue(int item);
    bool push(int item);
    bool pop(int &target);
    bool dequeue(int &target);

    bool isEmpty();
    size_t count();

    bool get(size_t index, int &target);
};
