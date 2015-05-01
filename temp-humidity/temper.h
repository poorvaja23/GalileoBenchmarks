#ifndef TEMPER_H
#define TEMPER_H

typedef struct Temper Temper;


Temper *TemperCreateFromDeviceNumber(int deviceNum, int timeout, int debug);
void TemperFree(Temper *t);

int TemperGetTemperatureInC(Temper *t, float *tempC);
int TempterGetOtherStuff(Temper *t, char *buf, int length);

int TemperGetTempAndRelHum(Temper *t, float *tempC, float *relhum);

#endif
