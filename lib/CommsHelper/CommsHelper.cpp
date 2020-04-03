//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <CommsHelper.h>

MODULE_STATE rxData;
EasyTransfer ETin, ETout;


CommsHelper::CommsHelper(ModuleHelper& module) : _module(module) {
	ETin.begin(details(rxData), &Serial);
	ETout.begin(details(_module._state), &Serial);

}

void CommsHelper::update() {
	ETout.sendData();
	receiveData();
}

void CommsHelper::receiveData() {
	for(int i = 0; i < 5; i++) {
		if(ETin.receiveData()) {
			if(MODULE::DRIVER == _module._module) {
				handleDriverData();
			} else {
				handlePassengerData();
			}
		}
		delay(10);
	}
}

void CommsHelper::handleDriverData() {
	_module._state.illuminate = rxData.illuminate;
	_module._state.reverse    = rxData.reverse;
}

void CommsHelper::handlePassengerData() {
	_module._state.passengerMirrorSelected = rxData.passengerMirrorSelected;
	_module._state.mirrorSelect            = rxData.mirrorSelect;
	_module._state.mirrorAction            = rxData.mirrorAction;
	if(_module._state.passengerWindowAction == rxData.passengerWindowAction) {
		return;
	}
	if(WINDOW_ACTION::STOP == _module._state.passengerWindowAction) {
		_module._state.passengerWindowAction = rxData.passengerWindowAction;
		return;
	} else if(WINDOW_ACTION::STOP == rxData.passengerWindowAction) {
		return;
	} else {
		_module._state.passengerWindowAction = WINDOW_ACTION::STOP;
	}
}