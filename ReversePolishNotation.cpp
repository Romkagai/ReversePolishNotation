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

struct Usel         //��������� ��� ������ �� ������
{
    float key;      //�������������� ���� ���� !float!
    Usel* next;     //��������� �� ��������� �������
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
        default: cout << "������! ������ �������� �� ������������� ����������!"; exit(0);
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
    cout << "������� �������" << endl;
    cin >> s;

    // ������ 5*(12+44)-56*2 = 168
    // ������ a*(12+44)-b*2 

    // ��� 1 - �������� ��������� ������� �� ������ 
    int bracketCounter = 0;                                             //������� ������
    for (int i = 0; i < strlen(s); i++) {
        if (isDigit(s[i]) && (isLetter(s[i + 1]))) { cout << "������! ��� �������� ������. ��������� ������������ ����� �������."; exit(0); }
        if (isLetter(s[i]) && (isLetter(s[i + 1]) || isDigit(s[i+1]))) { cout << "������! ��� �������� ������. ��������� ������������ ����� �������."; exit(0); }
        if (isSign(s[i]) && (isSign(s[i + 1]))) { cout << "������! ��� ����� ������. ��������� ������������ ����� �������."; exit(0); }
        if (s[i] == '(') bracketCounter++;
        if (s[i] == ')') bracketCounter--;
        if (bracketCounter < 0) { 
            cout << "������! �������� ����������� ������!"; exit(0);    //���� � �����-�� ������ ������� ������ <0 - ������
        } 
    }
    if (bracketCounter > 0) { 
        cout << "������! �������� ����������� ������!"; exit(0);        //���� ����� ����� ����� ������� ������ >0 - ������
    } 

    

    
    // ��� 2 - ������������ �����'�

    for (i = j = 0; s[i]; i++)
    {
        if (isDigit(s[i])) {                                            //�������� �� �����
            while (isDigit(s[i])) { p[j] = s[i]; j++; i++; }            //���� ���� ����� ����� � �����, ��� ������ �����������    
            p[j] = ' '; j++; i--;                                       //������ ������
        }
        else if (isLetter(s[i])) {                                      //�������� �� �����
            p[j] = s[i]; j++;                                           //���� ��������� �����, ����� � �����
            p[j] = ' '; j++;                                            //������ ������
        }
        else if (sign.Empty() || s[i] == '('){                          //���� � ����� ����� ��� '(' ������� ���� � ����
            sign.Push(s[i]);
            prior.Push(Priority(s[i]));
        }
        else if (s[i]==')' )                                            //���� ���� ')' ��������� ��� ����� �������� �� ������                                                                        
        {                                                               //������������� '(' � ������� � �����
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
    while (!sign.Empty())                                               //���� "�������" �����������, ��������� ������� �����  
    {                                                                   //� �����
        p[j] = sign.Pop();
        p[j + 1] = ' ';
        t = prior.Pop();
        j+=2;
    }
    p[j-1] = '\0';

    cout << "��������� �����:" << endl << p << endl;
    
    // ��� 3 - ���������� �����'�
    
    float x, y, result;
    Stack Calculation;

    float numberArray[26] = {};                                             //������ ������ 0 ��� ���������� �� a �� z
    

    for (i = 0; i<strlen(p); )
    {
        if (isDigit(p[i]))                                                  //���� ��������� �����
        {
            float k = 0;
            for (; p[i] != ' '; i++) k = k * 10 + (p[i] - '0');             //���� ����� �� ����� �������           
            Calculation.Push(k);                                            //���������� � ����         
        }
        else if(isLetter(p[i]))                                             //���� ��������� �����
        {
            if (numberArray[p[i] - 97] == 0)                                //���� ���������� � ������� = 0
            {
                cout << "������� �������� ���������� " << p[i] << ": ";     //������ ��������
                cin >> numberArray[p[i] - 97];
            }                   
            Calculation.Push(numberArray[p[i] - 97]); i++;                  //���������� � ���� 
        }
        else if (p[i])                                                      //���� ��������� ��������
        {
            y = Calculation.Pop(); x = Calculation.Pop();                   //������� �� ����� ��� ������� ��������
            switch (p[i])                                                   //�������� ��������
            {
            case '+': result = x + y; break;
            case '-': result = x - y; break;
            case '*': result = x * y; break;
            case '/': result = x / y; break;
            }
            Calculation.Push(result); i++;                                  //����� ��������� ������� � ����
        }
        i++;
    } 
    result = Calculation.Pop();
 
    cout << "RPN VALUE: " << endl << result << endl; 
}
