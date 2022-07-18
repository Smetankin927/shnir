#include<iostream>
#include<armadillo>
#include<fstream>
#include<cmath>
using namespace std;
using namespace arma;

#define PI 3.14159265                                  // число ПИ

/*

    equation 
                f'' - 2f * (f^2 -1) = 0 
    boud conditions 
                f(-X) = -1. f(1) = 1
    метод ньютона
 */
int main()
{
    float bound_left = 0;
    float bound_right = 1;
    float a_left = -4; // левая и правая границы
    float b_right = 4; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 0.1;
    int N = int(X/h)-1; 
    int Iteration = 50;
    
    vec x(N);
    vec y(N);
    vec F(N);
    vec delta_y(N);
    vec dg(N);
    // заполняем массив x (c учетом того, что мы не находим y(a_left) = -1 и y(b_right) = 1)
    for(int i=0; i<N; i++)
    {
        x(i) = a_left + h*i + h;
    }
    
    
    /*заполняем первое приближение теперь у у нас будет начинааться как бы с у_1*/

    for(int i = 0; i< N; i++)
    {
        y(i) = bound_left + (bound_right - bound_left)*(x(i) - a_left)/(b_right - a_left); // here -1 and X --> bound
    }
  
    /* заполняем первый вектор ответов и производную для dF (меняя здесь -- меняй в цикле) FIX POTENTIAL*/
    F(0) = bound_left - 2*y(0) + y(1) - h * h * (y(0) * pow(1-y(0)*y(0), 2) - 2 * pow(y(0), 3) * (1-y(0)*y(0))) ; // первая минус 1 соответствует у(a_left)
    F(N-1) =  y(N-2) - 2*y(N-1) + bound_right - h * h * (y(N-1) * pow(1-y(N-1)*y(N-1), 2) - 2 * pow(y(N-1), 3) * (1-y(N-1)*y(N-1))) ; // первая + 1 соответствует у(b_right)
    
    for(int i = 1; i< N-1; i++)
    {
        F(i) = y(i-1) - 2*y(i) + y(i+1) - h * h * (y(i) * pow(1-y(i)*y(i), 2) - 2 * pow(y(i), 3) * (1-y(i)*y(i))); //FIX POTENTIAL
    }
        
    for(int i = 0; i< N; i++) //индекс 0 соответствует у(a_left + h)
    {
        dg(i) = h*h * (pow(1-y(i)*y(i), 2) - 2 * pow(y(i), 2) * (1-y(i)*y(i)) + 4*pow(y(i),4)); // FIX POTENTIAL
    }

// запоняем матрицу (меняя здесь -- меняй в цикле)
    mat dF(N,N, fill::zeros);
    //первая и последняя строки
    dF(0,0) = - 2 - dg(0);
    dF(0,1) = 1;
    dF(N-1,N-1) = -2 - dg(N-1);
    dF(N-1, N-2) = 1;
    //остальная часть
    for(int i=1; i<N-1; i++)
    {
        dF(i,i) = -2 - dg(i);
        dF(i, i-1) = 1;
        dF(i, i+1) = 1;
    }
    
//основной цикл
    /*
    delta_y = y_n - y_(n-1)
    y = y + delta_y; // первое приближение линейно

    dF * delta_y = - F

    пересчитываем F, dF (dg).
    // code#

for(int i =0; i < Iteration; i++)
{
    y = y + dF.i() * (-F);

    //пересчет вектора ответов

        for(int j = 0; j< N+1; j++)
    {
        F(j) = h * h * 2 * y(j) * (y(j) * y(j) -1);
        dg(j) = h*h * (6* y(j) * y(j) - 2);
    }

    //пересчет матрицы
    //первая и последняя строки
    dF(0,0) = 1 - dg(0);
    dF(N,N) = 1 - dg(N);
    for(int j=1; j<N; j++)
    {
        dF(j,j) = -2 - dg(j);
        //остальные не надо
    }

}

    */
for(int i =0; i < Iteration; i++)
{
    y = y - solve(dF, F);

    //пересчет вектора ответов
    F(0) = bound_left - 2*y(0) + y(1) - h * h * (y(0) * pow(1-y(0)*y(0), 2) - 2 * pow(y(0), 3) * (1-y(0)*y(0))) ; // первая минус 1 соответствует у(a_left)
    F(N-1) =  y(N-2) - 2*y(N-1) + bound_right - h * h * (y(N-1) * pow(1-y(N-1)*y(N-1), 2) - 2 * pow(y(N-1), 3) * (1-y(N-1)*y(N-1))) ; // первая + 1 соответствует у(b_right)
    
    for(int i = 1; i< N-1; i++)
    {
        F(i) = y(i-1) - 2*y(i) + y(i+1) - h * h * (y(i) * pow(1-y(i)*y(i), 2) - 2 * pow(y(i), 3) * (1-y(i)*y(i))); //FIX POTENTIAL
    }
        
    for(int i = 0; i< N; i++)
    {
        dg(i) = h*h * (pow(1-y(i)*y(i), 2) - 2 * pow(y(i), 2) * (1-y(i)*y(i)) + 4*pow(y(i),4)); //FIX POTENTIAL
    }
    // пересчет матрицы якоби
    //первая и последняя строки
    dF(0,0) = - 2 - dg(0);
    dF(0,1) = 1;
    dF(N-1,N-1) = -2 - dg(N-1);
    dF(N-1, N-2) = 1;
    //остальная часть
    for(int i=1; i<N-1; i++)
    {
        dF(i,i) = -2 - dg(i);
        dF(i, i-1) = 1;
        dF(i, i+1) = 1;
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