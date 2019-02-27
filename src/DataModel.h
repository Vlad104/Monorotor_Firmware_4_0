#ifndef DATA_MODEL_H
#define DATA_MODEL_H

struct DataModel {
    DataModel();
    DataModel(char dozator, float volume, float feedrate, float accel, 
            float gear_A, float gear_B, float ratio_A);

    char    dozator;
    float   volume;     // ul
    float   feedrate;   // ul/min
    float   accel;      // ul/min*sec
    float   gear_A;     // ul to impulses
    float   gear_B;     // ul to impulses
    float   ratio_A;    // ratio A to all
    float   ratio_B;    // ratio A to all
};

#endif // DATA_MODEL_H