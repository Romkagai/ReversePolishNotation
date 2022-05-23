#pragma once
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <ctime>
#include <locale>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <string.h>

#define UI unsigned int

using namespace std;

struct Usel         //Структура для работы со стеком
{
    float key;      //Информационное поле типа !float!
    Usel* next;     //Указатель на следующий элемент
};
struct Stack
{
    Usel* head;
    Stack() { head = 0; }
    ~Stack(){ Clear(); } 
    void Push(float k)
    {
        Usel* p = new Usel;
        p->key = k;
        p->next = head;
        head = p;
    }
    float Pop()
    {
        float k = head->key;
        Usel* p = head;
        head = head->next;
        delete p;
        return k;
    }
    float Top()
    {
        return head->key;
    }
    bool Empty()
    {
        if (head == 0) return true;
        else return false;
    } 
    void Clear();
};
void Stack::Clear()
{
    int k;
    while (head) k = Pop();
}

int Priority(char s)
{
    switch (s) 
    {
        case '(': return 0;
        case ')': return 1;
        case '+': return 2;
        case '-': return 2;
        case '*': return 3;
        case '/': return 3;
        default: cout << "Ошибка! Данная операция не предусмотрена программой!"; exit(0);
    }
}
bool isDigit(char ch)
{
    if ('0' <= ch && ch <= '9') return true;
    return false;
}
bool isLetter(char ch)
{
    if ('a' <= ch && ch <= 'z') return true;
    return false;
}
bool isSign(char ch) 
{
    switch (ch)
    {
    case '+': return true;
    case '-': return true;
    case '*': return true;
    case '/': return true;
    default: return false;
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    char s[80], p[100], ch;
    Stack sign, prior;
    int i, j, t;
    cout << "Введите формулу" << endl;
    cin >> s;

    // Пример 5*(12+44)-56*2 = 168
    // Пример a*(12+44)-b*2 

    // Шаг 1 - Проверка введенной формулы на ошибки 
    int bracketCounter = 0;                                             //Счетчик скобок
    for (int i = 0; i < strlen(s); i++) {
        if (isDigit(s[i]) && (isLetter(s[i + 1]))) { cout << "Ошибка! Два операнда подряд. Проверьте правильность ввода формулы."; exit(0); }
        if (isLetter(s[i]) && (isLetter(s[i + 1]) || isDigit(s[i+1]))) { cout << "Ошибка! Два операнда подряд. Проверьте правильность ввода формулы."; exit(0); }
        if (isSign(s[i]) && (isSign(s[i + 1]))) { cout << "Ошибка! Два знака подряд. Проверьте правильность ввода формулы."; exit(0); }
        if (s[i] == '(') bracketCounter++;
        if (s[i] == ')') bracketCounter--;
        if (bracketCounter < 0) { 
            cout << "Ошибка! Потеряна открывающая скобка!"; exit(0);    //Если в какой-то момент счетчик скобок <0 - Ошибка
        } 
    }
    if (bracketCounter > 0) { 
        cout << "Ошибка! Потеряна закрывающая скобка!"; exit(0);        //Если после всего цикла счетчик скобок >0 - Ошибка
    } 

    

    
    // Шаг 2 - Формирование ПОЛИЗ'а

    for (i = j = 0; s[i]; i++)
    {
        if (isDigit(s[i])) {                                            //Проверка на число
            while (isDigit(s[i])) { p[j] = s[i]; j++; i++; }            //Пока идут цифры пишем в ПОЛИЗ, как только закончились    
            p[j] = ' '; j++; i--;                                       //Ставим пробел
        }
        else if (isLetter(s[i])) {                                      //Проверка на букву
            p[j] = s[i]; j++;                                           //Если встречаем букву, пишем в ПОЛИЗ
            p[j] = ' '; j++;                                            //Ставим пробел
        }
        else if (sign.Empty() || s[i] == '('){                          //Если в стеке пусто или '(' заносим знак в стек
            sign.Push(s[i]);
            prior.Push(Priority(s[i]));
        }
        else if (s[i]==')' )                                            //Если знак ')' извлекаем все знаки операций до первой                                                                        
        {                                                               //встречающейся '(' и заносим в ПОЛИЗ
            while (sign.Top() != '(')
            {
                p[j] = sign.Pop();
                p[j + 1] = ' ';
                t = prior.Pop();
                j+=2;
            }
            ch = sign.Pop();
            t = prior.Pop();
        }
        else if (prior.Top() < Priority(s[i]))
        {
            sign.Push(s[i]);
            prior.Push(Priority(s[i]));
        }
        else
        {
            while (!sign.Empty() && prior.Top() >= Priority(s[i]))
            {
                p[j] = sign.Pop();
                p[j + 1] = ' ';
                t = prior.Pop();
                j+=2;
            }
            sign.Push(s[i]);
            prior.Push(Priority(s[i]));
        }      
    }
    while (!sign.Empty())                                               //Если "формула" закончилась, переносим остаток стека  
    {                                                                   //В ПОЛИЗ
        p[j] = sign.Pop();
        p[j + 1] = ' ';
        t = prior.Pop();
        j+=2;
    }
    p[j-1] = '\0';

    cout << "Формируем ПОЛИЗ:" << endl << p << endl;
    
    // Шаг 3 - Вычисление ПОЛИЗ'а
    
    float x, y, result;
    Stack Calculation;

    float numberArray[26] = {};                                             //Создаём массив 0 для переменных от a до z
    

    for (i = 0; i<strlen(p); )
    {
        if (isDigit(p[i]))                                                  //Если встречаем цифру
        {
            float k = 0;
            for (; p[i] != ' '; i++) k = k * 10 + (p[i] - '0');             //Ищем число до знака пробела           
            Calculation.Push(k);                                            //Записываем в стек         
        }
        else if(isLetter(p[i]))                                             //Если встречаем букву
        {
            if (numberArray[p[i] - 97] == 0)                                //Если переменная в массиве = 0
            {
                cout << "Введите значение переменной " << p[i] << ": ";     //Просим значение
                cin >> numberArray[p[i] - 97];
            }                   
            Calculation.Push(numberArray[p[i] - 97]); i++;                  //Записываем в стек 
        }
        else if (p[i])                                                      //Если встречаем операцию
        {
            y = Calculation.Pop(); x = Calculation.Pop();                   //Достаем из стека два верхних элемента
            switch (p[i])                                                   //Проводим операцию
            {
            case '+': result = x + y; break;
            case '-': result = x - y; break;
            case '*': result = x * y; break;
            case '/': result = x / y; break;
            }
            Calculation.Push(result); i++;                                  //Пишем результат обратно в стек
        }
        i++;
    } 
    result = Calculation.Pop();
 
    cout << "RPN VALUE: " << endl << result << endl; 
}
