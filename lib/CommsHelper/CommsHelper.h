//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <ModuleHelper.h>
#include <EasyTransfer.h>

#ifndef DOORCONTROL_COMMSHELPER_H
#define DOORCONTROL_COMMSHELPER_H

class CommsHelper {
public:
	explicit CommsHelper(ModuleHelper& module);

	void update();

private:
	ModuleHelper& _module;

	void receiveData();

	void handleDriverData();

	void handlePassengerData();
};

#endif //DOORCONTROL_COMMSHELPER_H
