#include <iostream>
#include <stack>  // подключаем библиотеку для 
                  // использования стека
using namespace std;
 
int main() {
  setlocale(LC_ALL,"rus");
  stack <int> steck;  // создаем стек
 
  int i = 0;
 
  cout << "Введите шесть любых целых чисел: " << endl; // предлагаем пользователю 
                                                      // ввести 6 чисел
  while (i != 10) {
    int a;
    cin >> a; 
 
    steck.push(a);  // добавляем введенные числа
    i++;
  }
  
  if (steck.empty()) cout << "Стек не пуст";  // проверяем пуст ли стек (нет)
 
  cout << "Верхний элемент стека: " << steck.top() << endl; // выводим верхний элемент
  cout << "Давайте удалим верхний элемент " << endl;
 
  steck.pop();  // удаляем верхний элемент
 
  cout << "А это новый верхний элемент: " << steck.top(); // выводим уже новый
                                                          // верхний элемент 
  system("pause");
  return 0;
}