#include "Store.h"

Store::Store() {}

void Store::calculate()
{
    switch (buffer.D)
    {
    case '0':
        calculateA();
        break;
    case '1':
        calculateB();
        break;
    case '3':
        calculateAx();
        break;
    case '4':
        calculateBx();
        break;
    default:
        calculateAB();
        break;
    }
}

void Store::setValues(int index, float V = 0.0, float R = 0.0, float F = 0.0, float A = 0.0) {
    data[index].volume = (long) V;
    data[index].reverse = (long) R;
    data[index].feedrate = F;
    data[index].accel = A;
}

void Store::calculateA()
{
    float corr = buffer.a;
    float corr_to_min = corr / 60;

    setValues(0,
              buffer.V * corr,
              buffer.R * corr * (-1),
              buffer.F * corr_to_min,
              buffer.A * corr_to_min);

    setValues(1);
}

void Store::calculateB()
{
    float corr = buffer.b;
    float corr_to_min = corr / 60;

    setValues(0);

    setValues(1,
              buffer.V * corr,
              buffer.R * corr * (-1),
              buffer.F * corr_to_min,
              buffer.A * corr_to_min);
}

void Store::calculateAB()
{
    float corrA = buffer.a * buffer.r;
    float corrB = buffer.b * (1.0 - buffer.r);
    float corrA_to_min = corrA / 60;
    float corrB_to_min = corrB / 60;

    setValues(0,
              buffer.V * corrA,
              buffer.R * corrA * (-1),
              buffer.F * corrA_to_min,
              buffer.A * corrA_to_min);

    setValues(1,
              buffer.V * corrB,
              buffer.R * corrB * (-1),
              buffer.F * corrB_to_min,
              buffer.A * corrB_to_min);
}

void Store::calculateAx()
{
    float corrA = buffer.a * buffer.r;
    float corrB = buffer.b * (1.0 - buffer.r);
    float corrA_to_min = corrA / 60;
    float corrB_to_min = corrB / 60;

    setValues(0,
              buffer.V * corrA + buffer.x * buffer.a,
              buffer.R * buffer.a * (-1),
              buffer.F * corrA_to_min,
              buffer.A * corrA_to_min);

    setValues(1,
              buffer.V * corrB,
              0,
              buffer.F * corrB_to_min,
              buffer.A * corrB_to_min);
}

void Store::calculateBx()
{
    float corrA = buffer.a * buffer.r;
    float corrB = buffer.b * (1.0 - buffer.r);
    float corrA_to_min = corrA / 60;
    float corrB_to_min = corrB / 60;

    setValues(0,
              buffer.V * corrA,
              0,
              buffer.F * corrA_to_min,
              buffer.A * corrA_to_min);

    setValues(1,
              buffer.V * corrB + buffer.x * buffer.b,
              buffer.R * buffer.b * (-1),
              buffer.F * corrB_to_min,
              buffer.A * corrB_to_min);
}

void Store::calculate_times(int& timesA, int& timesB) {
    if (buffer.D == '0' || buffer.D == '1' || buffer.r - 0.5 < 0.01) {
        timesA = 1;
        timesB = 1;

        return;
    }

    if (buffer.a > buffer.b) {
        timesA = 1;
        timesB = (int) ( (1.0 - buffer.r) / buffer.r + 0.5);
    } else {
        timesA = 1;
        timesB = (int) ( buffer.r / (1.0 - buffer.r) + 0.5);
    }
}
