#include<iostream>
#include<armadillo>
#include<fstream>
#include<cmath>
using namespace std;
using namespace arma;

#define PI 3.14159265                                  // число ПИ
/*
float bound_left = 0;
    float bound_right = 2*PI;
    float a_left = -10; // левая и правая границы
    float b_right = 10; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 0.1;
    int N = int(X/h)-1; 
    int Iteration = 50;
    float ep = 2.5;

    y' = z(i)
    z' = (1-ep)*sin(y(i)*PI/180) - h * h * (ep*y(i)/(2*PI*PI) * (y(i)-2*PI) * (y(i) - PI))

    y(i+1) = h*z(i) +y(i)
    z(i+1) = h*((1-ep)*sin(y(i)*PI/180) - (ep*y(i)/(2*PI*PI) * (y(i)-2*PI) * (y(i) - PI))) + z(i)



/// после первого захода здесь начало цикла while(abs(y(N-1) - bound_right)> pogr)
z(a_left) = z(0) = lambd = (bound_right - bound_left)/(X);
y(a_left) = y(0) =  bound_left;
y(b_left) = bound_right;// для пристрелки свободный правый конец

for(int i=1, i<N; i++) // поставлю i=1 и сдвину индексы
{
    y(i) = h*z(i-1) +y(i-1)
    z(i) = h*((1-ep)*sin(y(i-1)*PI/180) - (ep*y(i-1)/(2*PI*PI) * (y(i-1)-2*PI) * (y(i-1) - PI))) + z(i-1)
}

if(y(N-1) < bound_right)
{
    lambd = lambd + param*lambd; //параметр 0.1
}

else
{
    if(y(N-1) > bound_right)
    {
      lambd = lambd - param*lambd;  
    }
    if(y(N-1) = bound_right)
    {
        lambd = lambd;
    }
}




    */

int main(){

float bound_left = 0;
    float bound_right = 2*PI;
    float a_left = -10; // левая и правая границы
    float b_right = 10; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 0.1;
    int N = int(X/h)-1; 
    float ep = 1;
    float lambd;
    float param = 0.1;
    float pogr = 0.01;
    vec z(N);
    vec y(N);
    vec x(N);
    // заполняем массив x (c учетом того, что мы не находим y(a_left) = -1 и y(b_right) = 1)
    for(int i=0; i<N; i++)
    {
        x(i) = a_left + h*i + h;
    }
    

/// после первого захода здесь начало цикла while(abs(y(N-1) - bound_right)> pogr)

lambd = 0.00001;
cout<<lambd;
z(0) = lambd;
y(0) =  bound_left;


for(int i=1; i<N; i++) // поставлю i=1 и сдвину индексы
{
    // это явный метод. надо неявный
    y(i) = h*z(i-1) +y(i-1);
    z(i) = h*((1-ep)*sin(y(i-1)*PI/180) + (ep*y(i-1)/(2*PI*PI) * (y(i-1)-2*PI) * (y(i-1) - PI))) + z(i-1);
    cout<<y(N-1) <<'\n';
}
cout<< "abs= "<< abs(y(N-1) - bound_right)<<'\n';
if(y(N-1) < bound_right)
    {
        lambd = lambd + param*lambd; //параметр 0.1
    }

if(y(N-1) > bound_right)
    {
        lambd = lambd - param*lambd;  
    }
if(y(N-1) == bound_right)
    {
        lambd = lambd;
    }
//cycle
cout<< "abs= "<< abs(y(N-1) - bound_right) - pogr <<'\n';
while(abs(y(N-1) - bound_right) > pogr)
{
    cout<<abs(y(N-1) - bound_right)<<'\n';
    z(0) = lambd;
    y(0) =  bound_left;

for(int i=1; i<N; i++) // поставлю i=1 и сдвину индексы
{
    y(i) = h*z(i-1) +y(i-1);
    z(i) = h*((1-ep)*sin(y(i-1)*PI/180) + (ep*y(i-1)/(2*PI*PI) * (y(i-1)-2*PI) * (y(i-1) - PI))) + z(i-1);
}

if(y(N-1) < bound_right)
    {
        lambd = lambd + param*lambd; //параметр 0.1
    }

if(y(N-1) > bound_right)
    {
        lambd = lambd - param*lambd;  
    }
if(y(N-1) == bound_right)
    {
        lambd = lambd;
    }
}

    /* 
    корректируем ответы 
    */
   vec ansY(N+2);
   vec ansX(N+2);

    ansX(0) = a_left;
    ansX(N+1) = b_right;
    ansY(0) = bound_left;
    ansY(N+1) = bound_right;
   for(int i =0; i<N; i++)
   {
    ansY(i+1) = y(i);
    ansX(i+1) = x(i);
   }
    //запись в файл
    ofstream out;
    out.open("data.txt");
    for(int i=0; i<N+2; i++)
    {if (out.is_open())
    {
        out<< ansY(i)<<'\t'<<ansX(i)<<endl;
    }
    }
    out.close();

}