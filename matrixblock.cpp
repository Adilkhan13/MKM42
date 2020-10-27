#include <iostream>
#include <thread>
using namespace std;

const int N = 4;

int B[N][N], A[N][N], C[N][N];

void threadFunction(int rows, int col)
{

    for (int i = rows; i < rows + N / 2; ++i)
    {
        for (int j = col; j < col + N / 2; ++j)
        {
            C[i][j] = 0;
            for (int k = 1; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main()
{ //матрица А
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            cout << "A[" << i << "][" << j << "]=";
            cin >> A[i][j];
        }
    }
    //матрица В
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            cout << "B[" << i << "][" << j << "]=";
            cin >> B[i][j];
        }
    }
    //матрица А
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    //матрица B
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    cout << "=" << endl;

    thread thr1;
    thr1 = thread(threadFunction, 0, 0);
    thread thr2;
    thr2 = thread(threadFunction, 0, N / 2);
    thread thr3;
    thr3 = thread(threadFunction, N / 2, 0);
    thread thr4;
    thr4 = thread(threadFunction, N / 2, N / 2);
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();

    //ВЫВОД C
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << C[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}
