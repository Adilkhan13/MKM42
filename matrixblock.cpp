//Жолдасов Артур
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

void multiply(const int *m_1, const int *m_2,
              std::size_t size, std::size_t row, std::size_t col, int *m_res)
{
    for (std::size_t i = 0; i < size; ++i)
        m_res[row * size + col] += m_1[row * size + i] * m_2[i * size + col];
}

int main()
{
    constexpr int N = 4, M = 1, THREAD_NUM = 2;

    std::unique_ptr<int[]> A(new int[N * N]{
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4});
    std::unique_ptr<int[]> B(new int[M * N]{
        5, 6, 7, 8});
    std::unique_ptr<int[]> C(new int[N * N]{});

    // create vector for running threads then assign threads to its elements
    std::vector<std::thread> thread_group(THREAD_NUM);

    for (int thread_i = 0; thread_i < THREAD_NUM; ++thread_i)
        thread_group[thread_i] = std::thread([&, thread_i]() {
            for (int row = thread_i; row < N; row += THREAD_NUM)
            {
                for (int col = 0; col < N; ++col)
                    multiply(A.get(), B.get(), N, row, col, C.get());
            }
        });

    for (auto &t : thread_group)
        t.join();

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
            std::cout << (j ? "\t" : "") << C[i * N + j];
        std::cout << std::endl;
    }
}
