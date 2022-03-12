#ifndef STORE_H
#define STORE_H

#include "mbed.h"
#include "params.h"

const float GEAR = 4414.1;

struct DataModel {
    long volume;
    long reverse;
    float feedrate;
    float accel;

    DataModel() :
        volume(0),
        reverse(0),
        feedrate(0.0),
        accel(500.0)
    {}
};

struct Buffer {
    char D;
    float V;
    float F;
    float A;
    float R;
    float x;
    float a;
    float b;
    float r;

    Buffer() :
        D(2), V(0.0), F(0.0), A(500.0), R(0.0),
        x(0.0), a(GEAR), b(GEAR), r(0.5)
    {}
};

struct Store {
    Store();

    Buffer buffer;
    DataModel data[2];

    void calculate();

    void calculateA();
    void calculateB();
    void calculateAB();
    void calculateAx();
    void calculateBx();

    void setValues(int index, float V, float R, float F, float A);
    void calculate_times(int& timesA, int& timesB);
};

#endif // STORE_H