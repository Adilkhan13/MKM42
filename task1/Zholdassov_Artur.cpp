#include <iostream>
#include <thread>

#define N 1000000
using namespace std;

long long int sum = 0;
long long int a[N];

void SummingFunc(int number, int start, int end)
{
    long long int s = 0;
    for (long long int i = start; i <= end; i++)
    {
        s += a[i];
    }
    sum += s;
    cout << "Sum from " << number << "-th thread:" << s << endl;
    return;
}

int main()
{
    for (long long int i = 0; i < N; i++)
    {
        a[i] = i + 1;
    }
    thread threads[5];
    long long int s = N * 0.1;
    for (int i = 0; i < 5; i++)
    {
        threads[i] = thread(SummingFunc, s * i, i, s * i - 1);
        threads[i].join();
        cout << "Sum total:" << sum << endl;
    }

    return 0;
}