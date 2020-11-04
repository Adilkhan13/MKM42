#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
using namespace std;

double total_thread = 0;
double f(double x) //f(x) = x;
{
    return x;
}

void MonteCarlo (double a, double b)
{
    auto start = std::chrono::system_clock::now();
    double n = 10;
    double eps=0.01;// Погрешность (eps).
    if(a > b)
    {
        return MonteCarlo(b, a);
    }    
    double sum = 0.0;  

    for (int i = 1; i <= n; i++) 
    {
        sum = sum + f(a+((b-a)*eps));
    }
    sum = ((b-a)/n)*sum; 
    cout << sum << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time_th: " << elapsed.count() << "s\n";
    total_thread += elapsed.count();
}
   

int main() {
    int a =0, b = 1000;
    const int n = 18;
    thread thr[n];
    // Циклы для запуска всех потоков для решения
    for (int i = 0; i < n; i++)
        thr[i] = thread(MonteCarlo, a + b/n*i, a + b/n * (i+1));
    for (int i = 0; i < n; i++) 
        thr[i].join();
    // Выводим результат
    double th= total_thread;
    cout << "Total time of threads: " << th << endl;
    total_thread = 0;
    MonteCarlo(0, 1000);
    std::cout << "\nFor one thread:" << total_thread<<endl;
    std::cout <<"\nTime won: " << th/total_thread+ th * 100<<"%";
    
    return 0;
}