//
// Created by b3tuning@gmail.com 3/20/20.
//

#include <ModuleHelper.h>
#include <EEPROM.h>

ModuleHelper::ModuleHelper(MODULE_STATE& state, MODULE& module) : _state(state), _module
		(module) {
	_state.folded = readFoldState();
}

bool ModuleHelper::readFoldState() {
	return EEPROM.read(0);
}