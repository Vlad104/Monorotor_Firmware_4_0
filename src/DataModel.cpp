#include "DataModel.h"

DataModel::DataModel() :
    dozator('2'),
    volume(0.0), feedrate(0.0), accel(0.0),
    gear_A(1.0), gear_B(1.0), ratio_A(0.5), ratio_B(0.5)
{}

DataModel::DataModel(char new_dozator, float new_volume, float new_feedrate, float new_accel, 
                float new_gear_A, float new_gear_B, float new_ratio_A) :

    dozator(new_dozator),
    volume(new_volume), feedrate(new_feedrate), accel(new_accel), 
    gear_A(new_gear_A), gear_B(new_gear_B), ratio_A(new_ratio_A), ratio_B(1 - new_ratio_A)
{}