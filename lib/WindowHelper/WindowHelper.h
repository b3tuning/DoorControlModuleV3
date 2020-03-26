//
// Created by Bucky Fellini on 3/20/20.
//

#ifndef DOORCONTROL_WINDOWHELPER_H
#define DOORCONTROL_WINDOWHELPER_H

#include <Arduino.h>
#include <ModuleHelper.h>
#include <ConfigHelper.h>
#include <MotorHelper.h>


class WindowHelper {
public:
	explicit WindowHelper(ModuleHelper& module);

	void update();

private:
	ModuleHelper&  _module;
	bool          autoUp;
	bool          autoDown;
	WINDOW_ACTION previousAction;

	void adjust(WINDOW_ACTION action);

	void adjustAutoDown();

	void adjustDown();

	void adjustUp();

	void adjustAutoUp();

	static void windowMotor(bool dir);

	void stopWindow();

	static bool currentLimit();
};

#endif //DOORCONTROL_WINDOWHELPER_H
