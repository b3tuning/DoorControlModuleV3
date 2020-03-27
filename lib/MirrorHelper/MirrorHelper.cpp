//
// Created by Bucky Fellini on 3/20/20.
//

#include <MirrorHelper.h>
#include <TLE94108.h>
#include <EEPROM.h>
#include <ConfigHelper.h>

TLE94108 controller = TLE94108();

MirrorHelper::MirrorHelper(ModuleHelper& module) : _module(module) {
	previous_fold_ms = millis();
	controller.begin();
	stopFold();
	stopAdjust();
	folding = false;
	adjusting = false;
	previousAction = MIRROR_ACTION::STOP;
}

void MirrorHelper::update() {
	getDiag();
	if(folding && previousFoldTimeElapsed()) {
		stopFold();
	}

	if(_module._state.folded &&
	   adjusting &&
	   (MIRROR_SELECT::FOLD != _module._state.mirrorSelect)) {
		adjusting = false;
		return;
	}

	if (MIRROR_SELECT::FOLD == _module._state.mirrorSelect) {
		foldMirror();
		return;
	}

	if(_module._state.mirrorAction == previousAction) {
		return;
	}

	adjust();
}

void MirrorHelper::adjust() {
	if(_module._state.folded && adjusting) {
		adjusting = false;
		return;
	}

	if((MODULE::DRIVER == _module._module && _module._state.driverMirrorSelected) ||
	   (MODULE::PASSENGER == _module._module && _module._state.passengerMirrorSelected)) {
		switch(_module._state.mirrorAction) {
			case MIRROR_ACTION::DOWN:
				adjustDown();
				break;
			case MIRROR_ACTION::UP:
				adjustUp();
				break;
			case MIRROR_ACTION::LEFT:
				if(MODULE::PASSENGER == _module._module) {
					adjustRight();
					break;
				} else {
					adjustLeft();
					break;
				}
			case MIRROR_ACTION::RIGHT:
				if(MODULE::PASSENGER == _module._module) {
					adjustLeft();
					break;
				} else {
					adjustRight();
					break;
				}
			case MIRROR_ACTION::STOP:
				stopAdjust();
				break;
		}
		adjusting      = _module._state.mirrorAction != MIRROR_ACTION::STOP;
		previousAction = _module._state.mirrorAction;
	}
}

bool MirrorHelper::previousFoldTimeElapsed() const {
	return ((millis() - previous_fold_ms) >= FOLD_INTERVAL_MS);
}

void MirrorHelper::adjustDown() {
	adjustVertical(false);
	getDiag();
}

void MirrorHelper::adjustUp() {
	adjustVertical(true);
	getDiag();
}

void MirrorHelper::adjustLeft() {
	adjustHorizontal(false);
	getDiag();
}

void MirrorHelper::adjustRight() {
	adjustHorizontal(true);
	getDiag();
}

void MirrorHelper::adjustVertical(bool dir) {
	coast();
	if(dir) {
		controller.configHB(controller.TLE_HB1, controller.TLE_FLOATING,controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB5, controller.TLE_FLOATING,controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB2, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB8, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB6, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB4, controller.TLE_LOW,controller.TLE_NOPWM);
	} else {
		controller.configHB(controller.TLE_HB1, controller.TLE_FLOATING, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB5, controller.TLE_FLOATING, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB2, controller.TLE_HIGH, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB8, controller.TLE_HIGH,controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB6, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB4, controller.TLE_LOW,controller.TLE_NOPWM);
	}
}

void MirrorHelper::adjustHorizontal(bool dir) {
	coast();
	if(dir) {
		controller.configHB(controller.TLE_HB1, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB5, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB2, controller.TLE_HIGH, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB8, controller.TLE_HIGH,controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB6, controller.TLE_FLOATING, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB4, controller.TLE_FLOATING,controller.TLE_NOPWM);
	} else {
		controller.configHB(controller.TLE_HB1, controller.TLE_HIGH, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB5, controller.TLE_HIGH, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB2, controller.TLE_LOW, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB8, controller.TLE_LOW,controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB6, controller.TLE_FLOATING, controller.TLE_NOPWM);
		controller.configHB(controller.TLE_HB4, controller.TLE_FLOATING,controller.TLE_NOPWM);
	}
}

void MirrorHelper::stopFold() {
	coastFold();
	folding = false;
}

void MirrorHelper::foldMirror() {
	stopFold();
	digitalWrite(DRV_IN1, !_module._state.folded);
	digitalWrite(DRV_IN2, _module._state.folded);
	_module._state.folded = !_module._state.folded;
	writeFoldState();
	folding = true;
	previous_fold_ms = millis();
}

void MirrorHelper::stopAdjust() {
	coast();
	previousAction = MIRROR_ACTION::STOP;
	adjusting = false;
}

void MirrorHelper::coast() {
	controller.configHB(controller.TLE_HB1, controller.TLE_FLOATING, controller.TLE_NOPWM);
	controller.configHB(controller.TLE_HB5, controller.TLE_FLOATING, controller.TLE_NOPWM);
	controller.configHB(controller.TLE_HB2, controller.TLE_FLOATING, controller.TLE_NOPWM);
	controller.configHB(controller.TLE_HB8, controller.TLE_FLOATING,controller.TLE_NOPWM);
	controller.configHB(controller.TLE_HB6, controller.TLE_FLOATING, controller.TLE_NOPWM);
	controller.configHB(controller.TLE_HB4, controller.TLE_FLOATING,controller.TLE_NOPWM);
}

void MirrorHelper::coastFold() {
	digitalWrite(DRV_IN1, LOW);
	digitalWrite(DRV_IN2, LOW);
}

void MirrorHelper::writeFoldState() const {
	EEPROM.update(0, _module._state.folded);
}

void MirrorHelper::getDiag() {
	int diag = controller.getSysDiagnosis();

	if(diag == TLE94108::TLE_STATUS_OK) {
		Serial.print("TLE NO ERRORS");
	} else {
		if(controller.getSysDiagnosis(controller.TLE_SPI_ERROR) != 0) {
			Serial.print("TLE_SPI_ERROR!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_UNDER_VOLTAGE) != 0) {
			Serial.print("TLE_UNDER_VOLTAGE!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_OVER_VOLTAGE) != 0) {
			Serial.print("TLE_OVER_VOLTAGE!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_POWER_ON_RESET) != 0) {
			Serial.print("TLE_POWER_ON_RESET!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_TEMP_SHUTDOWN) != 0) {
			Serial.print("TLE_TEMP_SHUTDOWN!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_TEMP_WARNING) != 0) {
			Serial.print("TLE_TEMP_WARNING!!!!!!");
		}
		if(controller.getSysDiagnosis(controller.TLE_LOAD_ERROR) != 0) {
			Serial.print("TLE_LOAD_ERROR!!!!!!");
		}
	}
	Serial.println();
	controller.clearErrors();
}