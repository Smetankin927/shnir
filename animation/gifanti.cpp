#include <iostream>
#include <fstream>
#include <cmath>
#include<armadillo>
#include <utility>
#include <functional>
using namespace arma;
using namespace std;
/*
    f_tt  - f_xx +2 * f * (f^2 - 1) = 0 
*/

vec diff(vec v, int N, float h)
{
    vec ddv(N);
    ddv(0) = 0; // так как лучшего предложить не можем
    ddv(N-1) = 0;
    for(int i =1; i<N-1; i++)
    {
        ddv(i) = (v(i-1) - 2 * v(i) + v(i+1))/(h*h);
    }
    return ddv;
}

vec kv (vec y, vec ddy, int N)
{
    vec kv(N);
    kv = ddy - 2 * y % (y%y -1);
    return kv;
}

vec ky (vec v)
{
    return v;
}

pair<vec,vec> Runge_Kutta(vec y0, vec v0, int N, float h, float ts)
{
        
    //step 1
    vec kv1(N);
    vec ky1(N);
    vec y1(N);
    vec v1(N);

    vec ddy(N);
    ddy = diff(y0, N, h);

    kv1 = kv(y0, ddy, N);
    ky1 = ky(v0);

    

    y1 = y0 + 0.5 * ts * ky1;
    v1 = v0 + 0.5 * ts * kv1;

    // step 2
    vec kv2(N);
    vec ky2(N);
    vec y2(N);
    vec v2(N);

    //vec ddy(N);
    ddy = diff(y1, N, h);

    kv2 = kv(y1, ddy, N);
    ky2 = ky(v1);

    y2 = y1 + 0.5 * ts * ky2;
    v2 = v1 + 0.5 * ts * kv2;

    //step 3
    vec kv3(N);
    vec ky3(N);
    vec y3(N);
    vec v3(N);

    //vec ddy(N);
    ddy = diff(y2, N, h);

    kv3 = kv(y2, ddy, N);
    ky3 = ky(v2);

    y3 = y2 + 0.5 * ts * ky3;
    v3 = v2 + 0.5 * ts * kv3;    
    //step 4
    vec kv4(N);
    vec ky4(N);
    vec y4(N);// вроде нам не надо вычислять это
    vec v4(N);// вроде нам не надо вычислять это

    //vec ddy(N);
    ddy = diff(y3, N, h);

    kv4 = kv(y3, ddy, N);
    ky4 = ky(v3);

    y4 = y3 + 0.5 * ts * ky4; // вроде нам не надо вычислять это
    v4 = v3 + 0.5 * ts * kv4; // 
    return make_pair((ts/6)*(kv1 + 2*kv2 + 2*kv3 + kv4), (ts/6)*(ky1 + 2*ky2 + 2*ky3 + ky4));// <V,Y>
}

double ts = 0.01; // time step

int calculate_and_write(int Iter)
{

    float a_left = -10; // левая и правая границы
    float b_right = 10; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 0.1;
    int N = int(X/h)+1;
    int Iteration = Iter;
    float speed = -0.5;
    float koeff = speed/pow((1-speed*speed), 0.5);
    
    vec x(N);
    vec v0(N); 
    vec y0(N);
    vec tsY(N); // time step y
    vec tsV(N);

    double tmp;
    //считывание f(0,x) из файла data.txt

    ifstream in;
    in.open("data_anti.txt");
    for(int i=0; i<N; i++)
    {if (in.is_open())
    {
        in >> y0(i) >> x(i);
        //v0
        v0(i) = koeff * (1/pow(cosh(x(i)-2),2)); // проблемы с корнем v/pow((1-v*v), 0.5)
        
    }
    }
    in.close();
    //done!


    
    //try 
    tsY = y0;
    tsV = v0;
    //Рунге-Кутта


    /*sum up    будем выводить профиль череза какое-то 
      кол-во шагов по времени
    */ 
    pair <vec,vec> VY;

    //основной цикл
    for(int i=0; i<Iteration; i++)
    {
    VY = Runge_Kutta(tsY, tsV, N, h, ts);
    tsV = tsV + VY.first;
    tsY = tsY + VY.second;
    }

    //запись в файл
    ofstream out;
    out.open("data2.txt", ios::app);
    for(int i=0; i<N; i++)
    {if (out.is_open())
    {
        out<<x(i)<<'\t'<< tsY(i)<<endl;
    }
    }
    out<< endl <<endl;
    out.close();
    return 0;
}

int main()
{
    int Iter = 0;
    while(Iter<1600)
    {
        int a=0;
        calculate_and_write(Iter);
        cout<<a<<endl;
        Iter = Iter + 10;
    }
}