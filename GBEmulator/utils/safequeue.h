#pragma once

#include <mutex>
#include <deque>
#include <exception>


template <typename Data>
class SafeQueue
{
public:
    SafeQueue();

    bool push(const Data& d);

    Data& front() const;
    size_t size() const;


private:
    std::deque<Data> d_queue;
    std::mutex d_mutex;
};

template <typename Data>
SafeQueue<Data>::SafeQueue()
    : d_queue(), d_mutex()
{
}

template <typename Data>
bool SafeQueue<Data>::push(const Data& d)
{
    throw std::runtime_error("SafeQueue unimplemented");
}

template <typename Data>
size_t SafeQueue<Data>::size() const
{
    throw std::runtime_error("SafeQueue unimplemented");
}

template <typename Data>
Data& SafeQueue<Data>::front() const
{
    throw std::runtime_error("SafeQueue unimplemented");
}