#include <iostream>
#include <thread>

#define N 1000000
#define M 4
typedef long mytype;
using namespace std;

mytype sum = 0;
mytype long a[N];

void SummingFunc(mytype number, mytype start, mytype end)
{
    mytype s = 0;
    for (mytype i = start; i <= end; i++)
    {
        s += a[i];
    }
    sum += s;
    cout << "Sum from " << number << "-th thread:" << s << endl;
    return;
}

int main()
{
    for (mytype i = 0; i < M; i++)
    {
        a[i] = i + 1;
    }
    thread threads[M];
    mytype s = N * 0.1;
    for (int i = 0; i < M; i++)
    {
        threads[i] = thread(SummingFunc, s * i, i, s * i - 1);
    }
    for (int i = 0; i < M; i++)
    {
        threads[i].join();
    }
    cout << "Sum total:" << sum << endl;
    return 0;
}
