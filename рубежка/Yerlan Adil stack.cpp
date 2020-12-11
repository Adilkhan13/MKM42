#include <iostream>
#include <stack>
using namespace std;
 
// Добавляем стек 
int main()
{
    stack<string> s;
 
    s.push("A");    // Добавляем "A" в стек
    s.push("B");    // Добавляем "B" в стек
    s.push("C");    // Добавляем "C" в стек
    s.push("D");    // Добавляем "D" в стек
 
    // Возвращает элементы стека
    cout << "Stack size is " << s.size() << endl;
 
    // Выводит начало стека "D"
    cout << "Top element is: " << s.top() << endl;
 
    s.pop();    // Удаляет начало ("D")
    s.pop();    // Удаляет след начало ("C")
 
    cout << "Stack size is " << s.size() << endl;
 
    // Проверка на пустоту 
    if (s.empty())
        cout << "Stack is Empty\n";
    else
        cout << "Stack is not Empty\n";
 
    return 0;
}