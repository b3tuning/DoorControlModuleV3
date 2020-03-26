//
// Created by Bucky Fellini on 3/20/20.
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
	ModuleHelper&  _module;
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

	static MIRROR_ACTION convertAnalogToMirrorAction(int value);

	static MIRROR_SELECT convertAnalogToMirrorSelect(int value);

	static WINDOW_ACTION convertAnalogToWindowAction(int value);
};

#endif //DOORCONTROL_BUTTONHELPER_H
