#include <iostream>
#include <thread>
using namespace std;

const int n = 4;
int A[n][n], B[n][n], c[n][n];

void threadFunction(int p1, int p2) {
    
    for (int i=p1; i < p1 + n / 2; ++i) {
        for (int j=p2; j < p2 + n / 2; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; ++k)
                c[i][j] += A[i][k] * B[k][j];
        }
    }
}

//Функция проверки правильности вычислений
void check() {
    int H[n][n];
    for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < n; j++) {
        	H[i][j] = 0;
            	for (int k = 0; k < n; ++k)
                	H[i][j] += A[i][k] * B[k][j];
        }
    }
    cout << "Right answer:" << endl;
    for (int i = 0; i < n; i++) {                         
        for (int j = 0; j < n; j++) {
            cout << H[i][j] << ' ';
        }
        cout << endl;
    }
}




int main() {

  
// Задаем первую матрицу (А) случайными числами
    for (int i = 0; i < n; i++) {            
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 300;
	}

// Вывод на экран матрицы А
    cout << "Matrix_A" << endl;
    for (int i = 0; i < n; i++) {             
        for (int j = 0; j < n; j++)
            cout << A[i][j] << " ";
	cout << endl;
    }
    cout << endl;

    cout << "	*" << endl << endl;
   
// Задаем вторую матрицу (В) случайными числами
   for (int i = 0; i < n; i++) {                            
        for (int j = 0; j < n; j++) 
            B[i][j] = rand() % 300;
	} 

// Выводим на экран матрицу В
    cout << "Matrix_B" << endl;
    for (int i = 0; i < n; i++) {            
        for (int j = 0; j < n; j++)
            cout << B[i][j] << " ";
	cout << endl;
    }
    cout << endl;
    
    cout << "=" << endl << endl;

    thread  thr1;
    thr1 = thread(threadFunction, 0, 0);
    thread  thr2;
    thr2 = thread(threadFunction, 0, n/2);
    thread  thr3;
    thr3 = thread(threadFunction, n / 2, 0);
    thread  thr4;
    thr4 = thread(threadFunction, n / 2, n / 2);
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    
// Выводим результат умножения
    for (int i = 0; i < n; i++) {                            
        for (int j = 0; j < n; j++)
            cout << c[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
    
// Проверяем ответ
    check();
    return 0;
}