
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdexcept>
#include <iostream>
#include <list>
#include <vector>

//созадем шаблон  класса
template <typename T>
//потокобезопасный класс
class ThreadSafeQueue
{
public:
 
    //Обьявляю перечисление чтобы проверить наличие значений queue (и закрыт ли он)
    enum QueueResult
    {
        OK,
        CLOSED
    };

    // инициализирую очередь максимального размера
    explicit ThreadSafeQueue(size_t maxSize = 0) : state(State::OPEN), currentSize(0), maxSize(maxSize)
    {}

    // отправляю v в очередь/ блокирую если очередь заполнена
    void push(T const& v)
    {
        // создаю временную очередь
        decltype(list) tmpList;
        tmpList.push_back(v);

        {
            std::unique_lock<std::mutex> lock(mutex);

            // ожидаю заполнения очереди
            while (currentSize == maxSize)
                cvPush.wait(lock);

            // проверяю на заполение .
            if (state == State::CLOSED)
                throw std::runtime_error("Trying to push to a closed queue.");

            // закидываю в очередь
            currentSize += 1;
            list.splice(list.end(), tmpList, tmpList.begin());

  
            // использую 1u чтобы избежать нулевой размерности 
            if (currentSize == 1u)
                cvPop.notify_one();
        }
    }

    // отправляю в очередь ссылку
    void push(T&& v)
    {
        // создаю временную очередь
        decltype(list) tmpList;
        tmpList.push_back(v);

        {
            std::unique_lock<std::mutex> lock(mutex);

            // Ожидаю заполнения очереди
            while (currentSize == maxSize)
                cvPush.wait(lock);

            // проверяю на заполение .
            if (state == State::CLOSED)
                throw std::runtime_error("Trying to push to a closed queue.");

            // закидываю в очередь
            currentSize += 1;
            list.splice(list.end(), tmpList, tmpList.begin());

            // использую ожидающий поток (можно не использовать блокировку)
            cvPop.notify_one();
        }
    }

    // удаляю элемент из очереди и записываю в v.
    //
    //  OK при успехе
    // CLOSED есди очередь пустая 
    QueueResult pop(T& v)
    {
        decltype(list) tmpList;

        {
            std::unique_lock<std::mutex> lock(mutex);

            // ожидаем элемент .
            while (list.empty() && state != State::CLOSED)
                cvPop.wait(lock);

            // если очередь пуста то ничего не возвращаем 
            if (list.empty() && state == State::CLOSED)
                return CLOSED;

            //присутвие элемента
            currentSize -= 1;
            tmpList.splice(tmpList.begin(), list, list.begin());
            // будим поток ожидания
            cvPush.notify_one();
        }

        // записываем значение
        v = tmpList.front();

        return OK;
    }

    // если поток не доступен
    void close() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex);
        state = State::CLOSED;

        // уведомляем все процессы
        cvPop.notify_all();
    }

private:

    // на случай если очередь закрыта 
    enum class State
    {
        OPEN,
        CLOSED
    };

    // текущее состояние
    State state;
    // текущая размерность
    size_t currentSize;
    // максимальная размерность
    size_t maxSize;
    // переменные состояния
    std::condition_variable cvPush, cvPop;
   
    std::mutex mutex;
    // реализация очереди
    std::list<T> list;
};

int main()
{

    //пример серверной нагрузки
    //одновременная обработка запросов вида продовец/покупаетель
    unsigned const NUM_THREADS = 20;
    unsigned const COUNT = 10;
    std::mutex cerrMutex;
    std::vector<std::thread> producers, consumers;

    ThreadSafeQueue<int> queue(5);
    std::atomic<int> x(0);

    // создаем продавцов.
    for (unsigned tID = 0; tID < NUM_THREADS; ++tID)
        producers.push_back(std::thread([&, tID]() {
        for (unsigned i = 0; i < COUNT; ++i)
        {
         ///////////////////////
            {
                std::lock_guard<std::mutex> lock(cerrMutex);
                std::cerr << "THREAD #" << tID << " pushing " << tID * NUM_THREADS + i << "\n";
            }
            queue.push(tID * NUM_THREADS + i);
        }
            }));

    // создаем покупателей.
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

    // ожидаем продавцов.
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Closing Queue.\n";
    }
    for (auto& t : producers)
        t.join();

    // закрываем поток.
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Closing Queue.\n";
    }
    queue.close();

    //ожидаем покупателей.
    {
        std::lock_guard<std::mutex> lock(cerrMutex);
        std::cerr << "Waiting for consumers...\n";
    }
    for (auto& t : consumers)
        t.join();

    return 0;
}