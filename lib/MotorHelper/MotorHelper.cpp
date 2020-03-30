//
// Created by b3tuning@gmail.com 3/20/20.
//

#include <MotorHelper.h>

void controlMotor(int enablePin, int directionPin, bool forward) {
	digitalWrite(enablePin, HIGH);
	digitalWrite(directionPin, forward ? HIGH : LOW);
}

void stopMotor(int enablePin, int directionPin) {
	digitalWrite(enablePin, LOW);
	digitalWrite(directionPin, LOW);
}
