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

struct Usel         //Structure for working with stack
{
    float key;
    Usel* next;     
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
        default: cout << "Error! You can't use this operation!"; exit(0);
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
    char s[80], p[100], ch;
    Stack sign, prior;
    int i, j, t;
    cout << "Enter the formula" << endl;
    cin >> s;

    // Example 5*(12+44)-56*2 = 168
    // Example a*(12+44)-b*2 

    // Step 1 - Checking formula for mistakes 
    int bracketCounter = 0;                                             //bracketCounter
    for (int i = 0; i < strlen(s); i++) {
        if (isDigit(s[i]) && (isLetter(s[i + 1]))) { cout << "Error! Two operands in a row!"; exit(0); }
        if (isLetter(s[i]) && (isLetter(s[i + 1]) || isDigit(s[i+1]))) { cout << "Error! Two operands in a row!"; exit(0); }
        if (isSign(s[i]) && (isSign(s[i + 1]))) { cout << "Error! Two signs in a row!"; exit(0); }
        if (s[i] == '(') bracketCounter++;
        if (s[i] == ')') bracketCounter--;
        if (bracketCounter < 0) { 
            cout << "Error! You've lost an open bracket"; exit(0);    //If bracketCounter < 0
        } 
    }
    if (bracketCounter > 0) { 
        cout << "Error! You've lost an closing bracket"; exit(0);        //If bracketCounter > 0 
    } 

    

    
    // Step 2 - Forming RPN

    for (i = j = 0; s[i]; i++)
    {
        if (isDigit(s[i])) {                                            //Checking for digit
            while (isDigit(s[i])) { p[j] = s[i]; j++; i++; }            //While we get number, put them to RPN   
            p[j] = ' '; j++; i--;                                       //Then put whitespace
        }
        else if (isLetter(s[i])) {                                      //Checking for letter
            p[j] = s[i]; j++;                                           //If we get letter, put letter to RPN
            p[j] = ' '; j++;                                            //Putting whitespace
        }
        else if (sign.Empty() || s[i] == '('){                          //If stack is empty or "(" push sign to stack
            sign.Push(s[i]);
            prior.Push(Priority(s[i]));
        }
        else if (s[i]==')' )                                            //If ")" pop all signs before first "("                                                                        
        {                                                               //and put them to RPN
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
    while (!sign.Empty())                                               //If formula has ended push stack  
    {                                                                   //to RPN
        p[j] = sign.Pop();
        p[j + 1] = ' ';
        t = prior.Pop();
        j+=2;
    }
    p[j-1] = '\0';

    cout << "Forming RPN:" << endl << p << endl;
    
    // Step 3 - RPN calculation
    
    float x, y, result;
    Stack Calculation;

    float numberArray[26] = {};                                             //Creating array "0" for letter from a to z
    

    for (i = 0; i<strlen(p); )
    {
        if (isDigit(p[i]))                                                  //If we got digit
        {
            float k = 0;
            for (; p[i] != ' '; i++) k = k * 10 + (p[i] - '0');             //Find number before whitespace        
            Calculation.Push(k);                                            //Push to stack     
        }
        else if(isLetter(p[i]))                                             //If we got letter
        {
            if (numberArray[p[i] - 97] == 0)                                //If variable in array = 0
            {
                cout << "Enter variable " << p[i] << ": ";     //Ask for variable 
                cin >> numberArray[p[i] - 97];
            }                   
            Calculation.Push(numberArray[p[i] - 97]); i++;                  //Push to stack
        }
        else if (p[i])                                                      //If we got operation sign
        {
            y = Calculation.Pop(); x = Calculation.Pop();                   //Pop from stack two top elements
            switch (p[i])                                                   //Get calculations
            {
            case '+': result = x + y; break;
            case '-': result = x - y; break;
            case '*': result = x * y; break;
            case '/': result = x / y; break;
            }
            Calculation.Push(result); i++;                                  //Writing a result back to stack
        }
        i++;
    } 
    result = Calculation.Pop();
 
    cout << "RPN VALUE: " << endl << result << endl; 
}
