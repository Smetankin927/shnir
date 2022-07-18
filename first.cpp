#include<iostream>
#include<armadillo>
#include<fstream>
using namespace std;
using namespace arma;
/*

    equation 
                f'' - 2f * (f^2 -1) = 0 
    boud conditions 
                f(-X) = -1. f(1) = 1
    
 */
int main()
{
    float bound_left = -1;
    float bound_right = 1;
    float a_left = -3; // левая и правая границы
    float b_right = 3; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 1;
    int N = int(X/h); 
    int Iteration = 3;
    
    vec x (N+1);
    vec y(N+1);
    vec pre(N+1);
    vec next(N+1);
    // заполняем массив x
    for(int i=0; i<N+1; i++)
    {
        x(i) = a_left + h*i;
    }
    
    /*заполняем первое приближение*/

    for(int i = 0; i< N+1; i++)
    {
        y(i) = bound_left + (bound_right - bound_left)*(x(i) - a_left)/(b_right - a_left); // here -1 and X --> bound
    }
    
    
    for(int i = 0; i< N+1; i++)
    {
        pre(i) = h * h * 2 * y(i) * (y(i) * y(i) -1);
    }
   
// запоняем матрицу
    mat A(N+1,N+1, fill::zeros);
    //первая и последняя строки
    A(0,0) = 1;
    A(N,N) = 1;
    for(int i=1; i<N; i++)
    {
        A(i,i) = -2;
        A(i, i-1) = 1;
        A(i, i+1) = 1;
    }
cout<<A.i();
//основной цикл
    /*
    учитываем краевые  условия в векторе pre
    для нулевого элемента
     - 2f[n][i] + f[n][i+1]  = h^2 * 2f[n-1][i] * (f[n-1][i]^2 -1) - f[n][i-1] = pre(0)

    для последнего
    f[n][i-1] - 2f[n][i]  = h^2 * 2f[n-1][i] * (f[n-1][i]^2 -1) - f[n][i+1] = pre(N-1)
    */
    for(int i=0; i< Iteration ; i++)
    {
    pre(0) = bound_left;
    pre(N) = bound_right;
    next = solve(A,pre);
   
        for(int j = 0; j< N+1; j++) // пересчет правой части
        {   
        pre(j) = h * h * 2 * next(j) * (next(j) * next(j) -1);
        }
    }
    
    
    //запись в файл
    ofstream out;
    out.open("data.txt");
    for(int i=0; i<N+1; i++)
    {if (out.is_open())
    {
        out<< next(i)<<'\t'<<x(i)<<endl;
    }
    }
    out.close();
}