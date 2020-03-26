//
// Created by Bucky Fellini on 3/20/20.
//

#ifndef DOORCONTROL_MODULEHELPER_H
#define DOORCONTROL_MODULEHELPER_H

enum class MODULE {
	DRIVER,
	PASSENGER
};

enum class MIRROR_ACTION {
	STOP = 0,
	DOWN,
	UP,
	LEFT,
	RIGHT
};

enum class MIRROR_SELECT {
	NONE = 0,
	FOLD,
	DRIVER,
	PASSENGER
};

enum class WINDOW_ACTION {
	AUTO_DOWN = 0,
	DOWN,
	AUTO_UP,
	UP,
	STOP
};

struct MODULE_STATE {
	MIRROR_SELECT mirrorSelect            = MIRROR_SELECT::NONE;
	MIRROR_ACTION mirrorAction            = MIRROR_ACTION::STOP;
	WINDOW_ACTION driverWindowAction      = WINDOW_ACTION::STOP;
	WINDOW_ACTION passengerWindowAction   = WINDOW_ACTION::STOP;
	bool          illuminate              = false;
	bool          reverse                 = false;
	bool          driverMirrorSelected    = false;
	bool          passengerMirrorSelected = false;
	bool          folded                  = false;
};

class ModuleHelper {
public:
	ModuleHelper(MODULE_STATE& state, MODULE& module);

	MODULE_STATE& _state;
	MODULE&       _module;

private:
	static bool readFoldState();
};

#endif //DOORCONTROL_MODULEHELPER_H
