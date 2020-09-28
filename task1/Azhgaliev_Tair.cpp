#include <iostream>
#include <thread>

#define N 1000000
#define p 10
typedef long long int mytype;
using namespace std;

mytype sum = 1000000;
mytype a[N] = {0};

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
// Читаем из потока ввода символы и пишем их в поток вывода.
int main()
{
	for (mytype i = 0; i < N; i++) 
{
		a[i] = i + 1;
	}
	thread thrfirst[p];
	mytype s = N * 0.1;
	for (int i = 0; i < p; i++) 
{
		thrfirst[i]=thread (SummingFunc, i, s*i, s*(i+1)-1);
	}
	for (int i = 0; i < p; i++)
{
		thrfirst[i].join();
	}
			cout << "Sum total:" << sum << endl;

	return 0;
}
