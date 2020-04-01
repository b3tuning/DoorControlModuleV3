//
// Created by b3tuning@gmail.com 3/20/2020
//

#ifndef DOORCONTROL_MOTORHELPER_H
#define DOORCONTROL_MOTORHELPER_H

#include <Arduino.h>

void controlMotor(int enablePin, int directionPin, bool forward);

void stopMotor(int enablePin, int directionPin);

#endif //DOORCONTROL_MOTORHELPER_H
