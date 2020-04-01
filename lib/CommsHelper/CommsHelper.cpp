//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <CommsHelper.h>

CommsHelper::CommsHelper(ModuleHelper& module, MODULE_STATE& rxData, EasyTransfer&
ETin, EasyTransfer& ETout) : _module(module), _rxData(rxData), ETin(ETin), ETout(ETout) {
	ETin.begin(details(_rxData), &Serial);
	ETout.begin(details(_module._state), &Serial);
}

void CommsHelper::update() {
	ETout.sendData();
	receiveData();
}

void CommsHelper::receiveData() {
	if(ETin.receiveData()) {
		MODULE::DRIVER == _module._module ? handleDriverData() : handlePassengerData();
	}
}

void CommsHelper::handleDriverData() {
	_module._state.illuminate = _rxData.illuminate;
	_module._state.reverse    = _rxData.reverse;
}

void CommsHelper::handlePassengerData() {
	_module._state.passengerMirrorSelected = _rxData.passengerMirrorSelected;
	_module._state.mirrorSelect            = _rxData.mirrorSelect;
	if(_module._state.passengerWindowAction == _rxData.passengerWindowAction) {
		return;
	}
	if(WINDOW_ACTION::STOP == _module._state.passengerWindowAction) {
		_module._state.passengerWindowAction = _rxData.passengerWindowAction;
		return;
	} else if(WINDOW_ACTION::STOP == _rxData.passengerWindowAction) {
		return;
	} else {
		_module._state.passengerWindowAction = WINDOW_ACTION::STOP;
	}
}