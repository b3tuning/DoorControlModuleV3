//
// Created by b3tuning@gmail.com 3/20/20.
//

#include <ModuleHelper.h>
#include <EasyTransfer.h>

#ifndef DOORCONTROL_COMMSHELPER_H
#define DOORCONTROL_COMMSHELPER_H

class CommsHelper {
public:
	CommsHelper(ModuleHelper& module, MODULE_STATE& rxData, EasyTransfer& ETin,
				EasyTransfer& ETout);

	void update();

private:
	ModuleHelper& _module;
	MODULE_STATE& _rxData;
	EasyTransfer ETin, ETout;

	void receiveData();

	void handleDriverData();

	void handlePassengerData();
};

#endif //DOORCONTROL_COMMSHELPER_H
