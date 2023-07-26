#include <iostream>
#include <fstream>
#include <cmath>
#include<armadillo>
#include <utility>
#include <functional>
using namespace arma;
using namespace std;


int main()
{

    float a_left = -8; // левая и правая границы
    float b_right = 8; 
    float X = b_right - a_left; // указываем длина полного отрезка
    float h = 0.1;
    int N = int(X/h)+1;
 
    float speed = -0.5;
    float koeff = speed/pow((1-speed*speed), 0.5);


    vec x(N); 
    vec y(N);
    vec v0(N);

    
    //считывание f(0,x) из файла data.txt

    //change
    double tmp =0;
    double tmp2 =-20;
    

    ifstream in;
    in.open("data_anti.txt");
    for(int j=0; j<N; j++)
    {
        
        if (in.is_open())
    {
        in >> y(j) >> x(j);
        
    }
    }
    in.close();



    //запись в файл
    
    ofstream out;
    out.open("datafixed.txt");
    for(int i=0; i<N; i++)
    {if (out.is_open())
    {
        out<< y(i)<<'\t'<< x(i) <<endl;
    }
    }
    out.close();
    cout<<x;
}