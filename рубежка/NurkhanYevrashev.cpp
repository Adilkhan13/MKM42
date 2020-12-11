#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdexcept>
#include <iostream>
#include <list>
#include <vector>

template <typename T>

class ThreadSafeQueue
{
public:
 
    
    enum QueueResult
    {
        OK,
        CLOSED
    };

    
    explicit ThreadSafeQueue(size_t maxSize = 0) : state(State::OPEN), currentSize(0), maxSize(maxSize)
    {}

    
    void push(T const& v)
    {
        
        decltype(list) tmpList;
        tmpList.push_back(v);

        {
            std::unique_lock<std::mutex> lock(mutex);

            
            while (currentSize == maxSize)
                cvPush.wait(lock);

            
            if (state == State::CLOSED)
                throw std::runtime_error("Trying to push to a closed queue.");

            
            currentSize += 1;
            list.splice(list.end(), tmpList, tmpList.begin());

  
            
            if (currentSize == 1u)
                cvPop.notify_one();
        }
    }

    
    void push(T&& v)
    {
        
        decltype(list) tmpList;
        tmpList.push_back(v);

        {
            std::unique_lock<std::mutex> lock(mutex);

            
            while (currentSize == maxSize)
                cvPush.wait(lock);

            
            if (state == State::CLOSED)
                throw std::runtime_error("Trying to push to a closed queue.");

            
            currentSize += 1;
            list.splice(list.end(), tmpList, tmpList.begin());

            
            cvPop.notify_one();
        }
    }

   
    QueueResult pop(T& v)
    {
        decltype(list) tmpList;

        {
            std::unique_lock<std::mutex> lock(mutex);

            
            while (list.empty() && state != State::CLOSED)
                cvPop.wait(lock);

            
            if (list.empty() && state == State::CLOSED)
                return CLOSED;

           
            currentSize -= 1;
            tmpList.splice(tmpList.begin(), list, list.begin());
           
            cvPush.notify_one();
        }

       
        v = tmpList.front();

        return OK;
    }

    
    void close() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex);
        state = State::CLOSED;

       
        cvPop.notify_all();
    }

private:

    
    enum class State
    {
        OPEN,
        CLOSED
    };

   
    State state;
    
    size_t currentSize;
    
    size_t maxSize;
   
    std::condition_variable cvPush, cvPop;
   
    std::mutex mutex;
    
    std::list<T> list;
};

int main()
{
    unsigned const NUM_THREADS = 20;
    unsigned const COUNT = 10;
    std::mutex cerrMutex;
    std::vector<std::thread> producers, consumers;

    ThreadSafeQueue<int> queue(5);
    std::atomic<int> x(0);

   
    for (unsigned tID = 0; tID < NUM_THREADS; ++tID)
        producers.push_back(std::thread([&, tID]() {
        for (unsigned i = 0; i < COUNT; ++i)
        {
        
            {
                std::lock_guard<std::mutex> lock(cerrMutex);
                std::cerr << "THREAD #" << tID << " pushing " << tID * NUM_THREADS + i << "\n";
            }
            queue.push(tID * NUM_THREADS + i);
        }
            }));

   
    for (unsigned tID = NUM_THREADS; tID < 2 * NUM_THREADS; ++tID)
        consumers.push_back(std::thread([&, tID]() {
        int i = -1;
        ThreadSafeQueue<int>::QueueResult result;
        while ((result = queue.pop(i)) != ThreadSafeQueue<int>::CLOSED)
        {
            std::lock_guard<std::mutex> lock(cerrMutex);
            std::cerr << "THREAD #" << tID << " got: " << i << "\n";
        }

        // Locked I/O.                
        {
            std::lock_guard<std::mutex> lock(cerrMutex);
            std::cerr << "THREAD #" << tID << " is done.\n";
        }
            }));

    
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Closing Queue.\n";
    }
    for (auto& t : producers)
        t.join();

    
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Closing Queue.\n";
    }
    queue.close();

   
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Waiting for consumers...\n";
    }
    for (auto& t : consumers)
        t.join();

    return 0;
}