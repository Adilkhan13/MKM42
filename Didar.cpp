#include <exception>
#include <stack>
#include <mutex>
#include <memory>
#include <iostream>
#include <stack>  
using namespace std;
 
int main() {
  setlocale(LC_ALL,"rus");
  stack <int> steck; 
 
  int i = 0;
 
  cout << "Введите шесть любых целых чисел: " << endl; 
  while (i != 6) {
    int a;
    cin >> a; 
 
    steck.push(a);  
    i++;
  }
  
  if (steck.empty()) cout << "Стек не пуст";  
 
  cout << "Верхний элемент стека: " << steck.top() << endl; 
  cout << "Давайте удалим верхний элемент " << endl;
 
  steck.pop();  
 
  cout << "А это новый верхний элемент: " << steck.top(); 
  system("pause");
  return 0;
}
struct empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack";
    }
};
template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data=other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value=std::move(data.top());
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
}
;

