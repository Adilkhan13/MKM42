#include <iostream>
#include <thread>

#define N 1000000;
typedef int mytype;
using namespace std;

mytype sum = 0;
mytype int a[N];

void SummingFunc(int number, int start, int end)
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
    for (mytype i = 0; i < N; i++)
    {
        a[i] = i + 1;
    }
    thread threads[N];
    mytype s = N * 0.1;
    for (int i = 0; i < N; i++)
    {
        threads[i] = thread(SummingFunc, s * i, i, s * i - 1);
        threads[i].join();
    }
    for (int i = 0; i < N; i++) {
		threads[i].join();
	}
    cout << "Sum total:" << sum << endl;
    return 0;
}
