#include "CyclicBuffer.h"
#include "Arduino.h"

CyclicBuffer::CyclicBuffer(size_t capacity)
{
    _buffer = new int[capacity];
    this->_capacity = capacity;
}

bool CyclicBuffer::enqueue(int element)
{
    if (_count < _capacity)
    {
        _buffer[(_headIndex + _count++) % _capacity] = element;
        return true;
    }
    else
    {
        return false;
    }
}

bool CyclicBuffer::push(int item)
{
    return enqueue(item);
}

bool CyclicBuffer::pop(int &target)
{
    if (isEmpty())
    {
        return false;
    }
    else
    {
        target = _buffer[(--_count) % _capacity];
        return true;
    }
}

bool CyclicBuffer::dequeue(int &target)
{
    if (isEmpty())
    {
        return false;
    }
    else
    {
        _count--;
        target = _buffer[(_headIndex++) % _capacity];
        return true;
    }
}

bool CyclicBuffer::isEmpty()
{
    return _count == 0;
}

size_t CyclicBuffer::count()
{
    return _count;
}

bool CyclicBuffer::get(size_t index, int &target)
{
    if (index < _count)
    {
        target = _buffer[(_headIndex + index) % _capacity];
        return true;
    }
    else
    {
        return false;
    }
}
