//
// Created by b3tuning@gmail.com 3/20/20.
//

#ifndef DOORCONTROL_BUTTONHELPER_H
#define DOORCONTROL_BUTTONHELPER_H

#include <Arduino.h>
#include <ConfigHelper.h>
#include <ModuleHelper.h>

class ButtonHelper {
public:
	explicit ButtonHelper(ModuleHelper& module);

	void update();

private:
	ModuleHelper& _module;
	unsigned long previousMirrorMillis;

	void selectMirror();

	bool previousTimeElapsed() const;

	void clearMirrorSelected();

	void writeMirrorSelectedPins(bool driver, bool passenger);

	void getMirrorSelect();

	void getMirrorAction();

	void getDriverWindowAction();

	void getPassengerWindowAction();

	void getIlluminateState();

	void getReverseState();

	void illuminate() const;

	static MIRROR_ACTION readAsMirrorAction(uint8_t pin);

	static MIRROR_SELECT readAsMirrorSelect(uint8_t pin);

	static WINDOW_ACTION readAsWindowAction(uint8_t pin);
};

#endif //DOORCONTROL_BUTTONHELPER_H
