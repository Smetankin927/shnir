#include<iostream>
#include<armadillo>
#include<fstream>
#include<cmath>
using namespace std;
using namespace arma;

#define PI 3.14159265                                  // число ПИ

    
    
    float h = 0.1;
    float ep = 2.6;
    float bound_left = 0;
    float bound_right = 2*PI;
    float a_left = -10; // левая и правая границы
    float b_right = 10; 
    float X = b_right - a_left; // указываем длина полного отрезка
    int N = int(X/h); 
    float norm_pogr = 0.01;
    float pogr = 0.01;
    //float lambda = (bound_right - bound_left)/(b_right - a_left);
    float lambda = (bound_right - bound_left)/(b_right - a_left)*0.001;

float dg (float y)
{
    return ((1-ep) * cos(y) + ep * (3*y*y -6*PI*y + 2*PI*PI)/(2*PI*PI));  
}

float f (float y)
{
    return ((1-ep) * sin(y) + ep * y * (y - PI) * (y - 2*PI)/(2 * PI * PI)) ;  
}

vec calculateF (vec F, vec V) 
{//вектор F [y][z]
    F(0) = V(0) - bound_left - h*V(N);
    F(N) = V(N) - lambda - h*f(V(0));

    for(int i=1; i<N; i++)
    {
        F(i) = V(i) - V(i-1) -h*V(i+N);

        F(i+N) = V(i+N) - V(i+N-1) - h*f(V(i));
    } 
    return F;
}

mat calculateW (mat W, vec V)
{
    //заполняем матрицу
    W(0,0) = 1;                 W(0,N) = -h;
    W(N,0) = -h*dg(V(0));       W(N,N) = 1;

    for(int i =1; i<N; i++)
    {
        W(i, i-1) = -1;             W(i,i) = 1;             W(i, i+N) = -h;
        W(N+i, i) = -h*dg(V(i));    W(N+i, N+i-1) = -1;     W(N+i, N+i) = 1;
    }
    return W;
}



int main()
{
    
    vec x(N);
    vec V(2*N);
    vec F(2*N);
    mat W(2*N,2*N, fill::zeros);

    // заполняем массив x 
    for(int i=0; i<N; i++)
    {
        x(i) = a_left + h*i + h;
    }
    // начальное приближение вектора V = [y][z]
    for(int i = 0; i< N; i++)
    {
        V(i) = bound_left + (bound_right - bound_left)*(x(i) - a_left)/(b_right - a_left); // y_0
        
        V(i+N) = lambda;        //z_0
    }
    
    //вектор F [y][z]
    F(0) = V(0) - bound_left - h*V(N);
    F(N) = V(N) - lambda - h*f(V(0));

    for(int i=1; i<N; i++)
    {
        F(i) = V(i) - V(i-1) -h*V(i+N);

        F(i+N) = V(i+N) - V(i+N-1) - h*f(V(i));
    } 

    //заполняем матрицу
    W(0,0) = 1;                 W(0,N) = -h;
    W(N,0) = -h*dg(V(0));       W(N,N) = 1;

    for(int i =1; i<N; i++)
    {
        W(i, i-1) = -1;             W(i,i) = 1;             W(i, i+N) = -h;
        W(N+i, i) = -h*dg(V(i));    W(N+i, N+i-1) = -1;     W(N+i, N+i) = 1;
    }
    //первое приближение (ньютон)
    
    while( norm(solve(W, -F), 2) > norm_pogr)
    {
        V = V + solve(W, -F);
        W = calculateW (W, V);
        F = calculateF (F, V);
        
    }
    
   
    


    //заходим в цикл с lambda

    while( abs(V(N-1) - bound_right) > pogr)
    {
        
        if(V(N-1) > bound_right)
        {
            lambda = lambda - lambda * abs(V(N-1) - bound_right)/(100*X);
        }

        if(V(N-1) < bound_right)
        {
            lambda = lambda + lambda * abs(V(N-1) - bound_right)/(100*X);
        }

        // пересчет из-за лямбды
        // начальное приближение вектора V = [y][z]
        for(int i = 0; i< N; i++)
    {
        V(i) = bound_left + (bound_right - bound_left)*(x(i) - a_left)/(b_right - a_left); // here -1 and X --> bound
        
        V(i+N) = lambda;
    }
        W = calculateW (W, V);
        F = calculateF (F, V);


        //ньютон
        
        while( norm(solve(W, -F), 2) > norm_pogr)
        
        {
            V = V + solve(W, -F);
            W = calculateW (W, V);
            F = calculateF (F, V);
            
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
    ansY(i+1) = V(i);
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