#include <iostream>
#include <thread>
using namespace std;

///////////////
///Передаем массивы методом слайсов:
const int n = 16;
//int c[n][n];
int B[n][n], A[n][n], c[n][n];
/*int B[n][n] = { {1,2,3,4},
                    {5,6,7,8},
                    {9,10,11,12},
                    {9,10,11,12} };
int A[n][n] = { {11,22,33,44},
                    {51,61,71,81},
                    {91,101,111,121},
                    {91,101,111,121} };
                    */
void threadFunction(int p1, int p2) {
    
    for (int i=p1; i < p1 + n / 2; ++i) {
        for (int j=p2; j < p2 + n / 2; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; ++k)
            {
                c[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void checkMatrix() {
    int H[n][n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j++) {
            H[i][j] = 0;
            for (int k = 0; k < n; ++k)
            {
                H[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    cout << "Checking:" << endl;
    for (int i = 0; i < n; i++) {                            //ВЫВОД c
        for (int j = 0; j < n; j++) {
            cout << H[i][j] << ' ';
        }
        cout << endl;
    }
}




int main() {
    //int B[n][n], A[n][n];
  
    for (int i = 0; i < n; i++) {             //матрица А
        for (int j = 0; j < n; j++) {
           // cout << "A["<<i<<"]["<<j<<"]=";
            A[i][j] = rand() % 300;
            //cin >> A[i][j];
        }
    }
   

   for (int i = 0; i < n; i++) {                            //матрица В
        for (int j = 0; j < n; j++) {
            //cout << "B[" << i << "][" << j << "]=";
            B[i][j] = rand() % 300;
            //cin >> B[i][j];
        }
    } 
    for (int i = 0; i < n; i++) {             //матрица А
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << " ";
        }cout << endl;
    }cout << endl;
    for (int i = 0; i < n; i++) {             //матрица А
        for (int j = 0; j < n; j++) {
            cout << B[i][j] << " ";
        }cout << endl;
    }
    
    cout << "=" << endl;

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
    
    
    for (int i = 0; i < n; i++) {                            //ВЫВОД c
        for (int j = 0; j < n; j++) {
            cout << c[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    checkMatrix();
    return 0;
}