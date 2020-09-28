#include <iostream>
#include <thread>

#define N 1000000
#define n 10
using namespace std;

long long int sum = N;
long long int a[N] = { 0 };

void SummingFunc(int number, int start, int end) {
	long long int s = 0;
	for (long long int i = start; i <= end; i++) {
		s += a[i];
	}
	sum += s;
	cout << "Sum from " << number << "-th thread:" << s << endl;
	return;
}
// Читаем из потока ввода символы и пишем их в поток вывода.
int main()
{
	for (long long int i = 0; i < N-1; i++) {
		a[i] = i + 1;
	}
	thread thrfirst[n];
	long long  int s = N * 0.1;


	for (int i = 0; i < n; i++) {
		thrfirst[i]=thread (SummingFunc, i, s*i, s*(i+1)-1);
	}
	for (int i = 0; i < n; i++) {
		thrfirst[i].join();
	}
	cout<<"Sum total:"<<sum<<endl;
	return 0;
}