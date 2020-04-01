//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <WindowHelper.h>

WindowHelper::WindowHelper(ModuleHelper& module) : _module(module) {
	autoDown = false;
	autoUp = false;
	previousAction = WINDOW_ACTION::STOP;
	stopWindow();
}

void WindowHelper::update() {
	// if (action != WINDOW_ACTION::STOP) {
	//     Serial.print("Current action = ");
	//     printWindowAction(action);
	//     Serial.print("Previous action = ");
	//     printWindowAction(previousAction);
	// }


	// // If auto down and hit the current limit, stop window
	if(autoDown && currentLimit()) {
		stopWindow();
		return;
	}

	// // If auto up and hit the current limit, stop window
	if(autoUp && currentLimit()) {
		stopWindow();
		return;
	}

	// If any window action except STOP, adjust window
	// if (action != WINDOW_ACTION::STOP) {
	WINDOW_ACTION action = (MODULE::DRIVER == _module._module ? _module._state.driverWindowAction
															  : _module._state.passengerWindowAction);
	adjust(action);
	// return;
	// }

	// // If already adjusting window in auto up/down, do nothing
	// if (autoDown || autoUp) {
	//     // Serial.println("Already AUTO UP or AUTO DOWN");
	//     previousAction == WINDOW_ACTION::STOP;
	//     return;
	// }

	// If we reach here, stop window
	// stopWindow();
}

/**
 * Using WINDOW_ACTION, decide what to do with the windowMotor
 */
void WindowHelper::adjust(WINDOW_ACTION action) {

	// nothing to see here
	if(previousAction == action) {
		return;
	}

	switch(action) {
		case WINDOW_ACTION::AUTO_DOWN:
			adjustAutoDown();
			break;
		case WINDOW_ACTION::DOWN:
			if(previousAction == WINDOW_ACTION::AUTO_DOWN) {
				break;
			}
			adjustDown();
			break;
		case WINDOW_ACTION::AUTO_UP:
			if(previousAction <= WINDOW_ACTION::DOWN) {
				break;
			}
			adjustAutoUp();
			break;
		case WINDOW_ACTION::UP:
			if(previousAction <= WINDOW_ACTION::AUTO_UP) {
				break;
			}
			adjustUp();
			break;
		case WINDOW_ACTION::STOP:
			if(autoDown || autoUp) {
				break;
			}
			stopWindow();
			break;
//		default:
//			break;
	}

	previousAction = action;
}

void WindowHelper::adjustAutoDown() {
	autoUp = false;

	// // Hit the current limit so stop window
	if(currentLimit()) {
		stopWindow();
		return;
	}

	// Already auto down
	if(autoDown) {
		return;
	}

	autoDown = true;
	windowMotor(false);
}

void WindowHelper::adjustDown() {
	autoUp = false;

	// // Hit the current limit so stop window
	if(currentLimit()) {
		stopWindow();
		return;
	}

	// If not already auto down go ahead and adjust window
	if(!autoDown) {
		windowMotor(false);

	}
}

void WindowHelper::adjustUp() {
	autoDown = false;

	// // Hit the current limit so stop window
	if(currentLimit()) {
		stopWindow();
		return;
	}

	// If not already auto up go ahead and adjust window
	if(!autoUp) {
		windowMotor(true);
	}
}

void WindowHelper::adjustAutoUp() {
	autoDown = false;

	// // Hit the current limit so stop window
	if(currentLimit()) {
		stopWindow();
		return;
	}

	// Already auto up
	if(autoUp) {
		return;
	}

	autoUp = true;
	windowMotor(true);
}

void WindowHelper::windowMotor(bool dir) {
	controlMotor(POLOLU_PWM, POLOLU_DIR, dir);
}

void WindowHelper::stopWindow() {
	if(previousAction == WINDOW_ACTION::STOP) {
		return;
	}
	stopMotor(POLOLU_PWM, POLOLU_DIR);
	autoDown       = false;
	autoUp         = false;
	previousAction = WINDOW_ACTION::STOP;
}

bool WindowHelper::currentLimit() {
//	int i = analogRead(POLOLU_CUR_SEN_IN);
//	return i > 100;
	return analogRead(POLOLU_CUR_SEN_IN) > 100;
}
