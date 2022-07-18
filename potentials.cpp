#include<iostream>
#include<armadillo>
#include<fstream>
#include<cmath>
using namespace std;
using namespace arma;

#define PI 3.14159265                                  // число ПИ

    
    
    float h = 0.1;
    float a_left = -10; // левая и правая границы
    float b_right = 10; 
    float X = b_right - a_left; // указываем длина полного отрезка
    int N = int(X/h); 

    float ep = 0;


float V (float y)
{
    
    return ((1-ep) * (1 - cos(y)) + ep * pow(y,2) * pow((y - 2*PI),2)/(8 * PI * PI)) ;  
}



int main()
{
    
    vec x(N);
    vec P(N); 
    // заполняем массив x 
    for(int i=0; i<N; i++)
    {   
        x(i) = a_left + h*i;
    }
    // начальное приближение вектора V = [y][z]
    for(int i = 0; i< N; i++)
    {
        cout<<ep;
        float y = x(i);
        P(i) = (1-ep) * (1 - cos(y*PI/180)) + ep * pow(y,2) * pow((y - 2*PI),2)/(8 * PI * PI);
    }
          /* 
    корректируем ответы 
    */
   
    //запись в файл
    ofstream out;
    out.open("data.txt");
    for(int i=0; i<N; i++)
    {if (out.is_open())
    {
        out<< P(i)<<'\t'<<x(i)<<endl;
    }
    }
    out.close();
}