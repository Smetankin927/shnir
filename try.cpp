#include<iostream>
#include<armadillo>
#include<fstream>
using namespace std;
using namespace arma;

 
int main()
{
    vec y(3);
    y = {2, 3, 4};
    cout<<y;
    vec b(3);
    b = y%y - 1;
    cout<<b;


}