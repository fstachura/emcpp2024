#include <iostream>
using namespace std;

/**
    Computes velocity in meters per seconds.
    @param distance  distance in meters
    @param time      time in seconds
    @return velocity in meters per seconds.
*/
double computeVelocity(double distance, double time){
    return distance/time;
}

long double operator ""_s(unsigned long long v) {
    return v;
}

long double operator ""_h(long double v) {
    return v*(60*60);
}

long double operator ""_ms(long double v) {
    return v/1000;
}

long double operator ""_m(unsigned long long v) {
    return v;
}

long double operator ""_km(unsigned long long v) {
    return v*1000;
}

long double operator ""_km(long double v) {
    return v*1000;
}

long double operator ""_cm(unsigned long long v) {
    return ((long double)v)/100;
}

int main(){
    cout << computeVelocity(100_m, 5_s) << endl;     //20
    cout << computeVelocity(360_km, 2.0_h) << endl;  //50
    cout << computeVelocity(3.6_km, 0.02_h) << endl; //50
    cout << computeVelocity(250_cm, 2.5_ms) << endl; //1000
    return 0;
}
