//
// Created by b3tuning@gmail.com 3/20/2020
//

#ifndef DOORCONTROL_MIRRORHELPER_H
#define DOORCONTROL_MIRRORHELPER_H

#include <Arduino.h>
#include <ModuleHelper.h>

class MirrorHelper {
public:
	explicit MirrorHelper(ModuleHelper& module);

	void update();

private:
	ModuleHelper& _module;
	unsigned long previous_fold_ms;
	bool          folding;
	bool          adjusting;
	MIRROR_ACTION previousAction;

	void adjust();

	bool previousFoldTimeElapsed() const;

	static void adjustDown();

	static void adjustUp();

	static void adjustLeft();

	static void adjustRight();

	static void adjustVertical(bool dir);

	static void adjustHorizontal(bool dir);

	void stopFold();

	void foldMirror();

	void stopAdjust();

	static void coast();

	static void coastFold();

	void writeFoldState() const;

	static void getDiag();
};

#endif //DOORCONTROL_MIRRORHELPER_H
