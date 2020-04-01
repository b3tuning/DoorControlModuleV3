//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <ButtonHelper.h>

ButtonHelper::ButtonHelper(ModuleHelper& module) : _module(module) {
	previousMirrorMillis = millis();
}

void ButtonHelper::update() {
	if(MODULE::DRIVER == _module._module) {
		getDriverWindowAction();
		getMirrorSelect();
		getMirrorAction();
		selectMirror();
	}

	if(MODULE::PASSENGER == _module._module) {
		getIlluminateState();
		getReverseState();
	}

	getPassengerWindowAction();
	illuminate();
}

void ButtonHelper::selectMirror() {
	if(previousTimeElapsed()) {
		clearMirrorSelected();
	}

	if(_module._state.mirrorSelect == MIRROR_SELECT::FOLD) {
		clearMirrorSelected();
	}

	if(_module._state.folded) {
		return;
	}

	if(_module._state.mirrorSelect == MIRROR_SELECT::DRIVER) {
		writeMirrorSelectedPins(true, false);
	}

	if(_module._state.mirrorSelect == MIRROR_SELECT::PASSENGER) {
		writeMirrorSelectedPins(false, true);
	}
}

bool ButtonHelper::previousTimeElapsed() const {
	return ((millis() - previousMirrorMillis) >= MIRROR_SELECT_INTERVAL_MS);
}

void ButtonHelper::clearMirrorSelected() {
	writeMirrorSelectedPins(false, false);
}

void ButtonHelper::writeMirrorSelectedPins(bool driver, bool passenger) {
	_module._state.driverMirrorSelected    = driver;
	_module._state.passengerMirrorSelected = passenger;
	digitalWrite(D_MIR, driver);
	digitalWrite(P_MIR, passenger);
	previousMirrorMillis = millis();
}

void ButtonHelper::getMirrorSelect() {
	_module._state.mirrorSelect = readAsMirrorSelect(MIR_SEL_IN);
}

void ButtonHelper::getMirrorAction() {
	_module._state.mirrorAction = readAsMirrorAction(MIR_ADJ_IN);
}

void ButtonHelper::getDriverWindowAction() {
	_module._state.driverWindowAction = readAsWindowAction(D_WIN_IN);
}

void ButtonHelper::getPassengerWindowAction() {
	_module._state.passengerWindowAction = readAsWindowAction(P_WIN_IN);
}

void ButtonHelper::getIlluminateState() {
	_module._state.illuminate = analogRead(ILLUM_IN) == HIGH;
}

void ButtonHelper::getReverseState() {
	_module._state.reverse = analogRead(REV_IN) == HIGH;
}

void ButtonHelper::illuminate() const {
	digitalWrite(ILLUM_OUT, _module._state.illuminate);
}

MIRROR_ACTION ButtonHelper::readAsMirrorAction(uint8_t pin) {
	switch(analogRead(pin)) {
		case 13:
		case 14:
		case 15:
			return MIRROR_ACTION::DOWN;
		case 16:
		case 17:
		case 18:
			return MIRROR_ACTION::UP;
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			return MIRROR_ACTION::LEFT;
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
			return MIRROR_ACTION::RIGHT;
		default:
			return MIRROR_ACTION::STOP;
	}
}

MIRROR_SELECT ButtonHelper::readAsMirrorSelect(uint8_t pin) {
	switch(analogRead(pin)) {
		case 13:
		case 14:
		case 15:
			return MIRROR_SELECT::FOLD;
		case 16:
		case 17:
		case 18:
		case 19:
			return MIRROR_SELECT::DRIVER;
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			return MIRROR_SELECT::PASSENGER;
		default:
			return MIRROR_SELECT::NONE;
	}
}

WINDOW_ACTION ButtonHelper::readAsWindowAction(uint8_t pin) {
	switch(analogRead(pin)) {
		case 12:
		case 13:
		case 14:
		case 15:
			return WINDOW_ACTION::AUTO_DOWN;
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			return WINDOW_ACTION::DOWN;
		case 21:
		case 22:
		case 23:
		case 24:
			return WINDOW_ACTION::AUTO_UP;
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
			return WINDOW_ACTION::UP;
		default:
			return WINDOW_ACTION::STOP;
	}
}