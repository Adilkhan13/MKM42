#include <iostream>
#include <thread>

#define N 1000000
using namespace std;

long long int sum = 1000000;
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
	for (long long int i = 0; i < 999999; i++) {
		a[i] = i + 1;
	}
	long long  int s = N / 10;
	for (int i = 0; i < 10; i++) {
		thread thrfirst(SummingFunc, i, s*i, s*(i+1));
		thrfirst.join();
		cout << "Sum total:" << sum << endl;
	}
	return 0;
}