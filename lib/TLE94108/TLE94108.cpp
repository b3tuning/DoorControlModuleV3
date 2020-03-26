/*!
 * \file 		TLE94108.cpp
 * \name        TLE94108.cpp - Arduino library to control Infineon's DC Motor Control Shield with TLE94108
 * \author      Infineon Technologies AG
 * \copyright   2019 Infineon Technologies AG
 * \version     1.4.1
 * \brief 		This file has to be included in projects that use Infineon's DC Motor Control Shield with TLE94108
 * \details
 * The Infineon TLE94108EL DC motor controller shield is able to handle 6 motors with a max. current of 0.9 A
 * independently and additional 5 motors cascaded. The eight half-bridges can be arranged
 * also together,
 * so that 3 motors with 1.8 A current or one motor with 3.6 A can be used. Each half bridge can
 * provide a high-Voltage (nominal 5.5-18 V) tristate output and max. input voltage of 40V. It is also
 * capable of PWM with 3 different frequencies for controlling the speed of each motor.
 * Have a look at the datasheet for more information.
 *
 * This library include the basic functions to access the half-bridges.
 *
 *	Have a look at the datasheet for more information.
 */


#include <TLE94108.h>
#include <./util/TLE94108_conf.h>

//SPI address commands
#define TLE94108_CMD_WRITE            0x80;
#define TLE94108_CMD_CLEAR            0x80;

#define TLE94108_STATUS_INV_MASK    (TLE94108::TLE_POWER_ON_RESET)
#define TLE94108_CS_RISETIME        2


TLE94108::TLE94108(void) {
}

void TLE94108::begin(void) {
	begin(SPI, TLE94108_PIN_CS1, TLE94108_PIN_EN);
}

void TLE94108::begin(SPIClass& bus, uint8_t cs, uint8_t en) {
	mEnabled = FALSE;
	mBus     = &bus;
	mCsPin   = cs;
	mEnPin   = en;

	mEnabled = TRUE;
	mBus->begin();
	mBus->setBitOrder(LSBFIRST);
	mBus->setClockDivider(SPI_CLOCK_DIV16);
	mBus->setDataMode(SPI_MODE1);
	pinMode(mEnPin, OUTPUT);
	pinMode(mCsPin, OUTPUT);
	digitalWrite(mCsPin, HIGH);
	digitalWrite(mEnPin, HIGH);
	init();
}

void TLE94108::end(void) {
	mEnabled = FALSE;
	digitalWrite(mCsPin, HIGH);
	digitalWrite(mEnPin, LOW);
}

void TLE94108::configHB(HalfBridge hb, HBState state, PWMChannel pwm) {
	configHB(hb, state, pwm, 0);
}

void TLE94108::configHB(HalfBridge hb, HBState state, PWMChannel pwm, uint8_t activeFW) {
	if(mEnabled == TRUE) {
		configHB(static_cast<uint8_t>(hb),
				 static_cast<uint8_t>(state),
				 static_cast<uint8_t>(pwm),
				 activeFW);
	}
}

void TLE94108::configHB(uint8_t hb, uint8_t state, uint8_t pwm, uint8_t activeFW) {
	uint8_t reg   = mHalfBridges[hb].stateReg;
	uint8_t mask  = mHalfBridges[hb].stateMask;
	uint8_t shift = mHalfBridges[hb].stateShift;
	writeReg(reg, mask, shift, state);

	reg   = mHalfBridges[hb].pwmReg;
	mask  = mHalfBridges[hb].pwmMask;
	shift = mHalfBridges[hb].pwmShift;
	writeReg(reg, mask, shift, pwm);

	reg   = mHalfBridges[hb].fwReg;
	mask  = mHalfBridges[hb].fwMask;
	shift = mHalfBridges[hb].fwShift;
	writeReg(reg, mask, shift, activeFW);
}

void TLE94108::configPWM(PWMChannel pwm, PWMFreq freq, uint8_t dutyCycle) {
	if(mEnabled == TRUE) {
		configPWM(static_cast<uint8_t>(pwm),
				  static_cast<uint8_t>(freq),
				  dutyCycle);
	}
}

void TLE94108::configPWM(uint8_t pwm, uint8_t freq, uint8_t dutyCycle) {
	uint8_t reg   = mPwmChannels[pwm].freqReg;
	uint8_t mask  = mPwmChannels[pwm].freqMask;
	uint8_t shift = mPwmChannels[pwm].freqShift;
	writeReg(reg, mask, shift, freq);

	reg   = mPwmChannels[pwm].dcReg;
	mask  = mPwmChannels[pwm].dcMask;
	shift = mPwmChannels[pwm].dcShift;
	writeReg(reg, mask, shift, dutyCycle);
}

uint8_t TLE94108::getSysDiagnosis() {
	uint8_t ret = readStatusReg(SYS_DIAG1);
	return ret ^ TLE94108_STATUS_INV_MASK;
}

uint8_t TLE94108::getSysDiagnosis(DiagFlag mask) {
	return getSysDiagnosis(static_cast<uint8_t>(mask));
}

uint8_t TLE94108::getSysDiagnosis(uint8_t mask) {
	uint8_t ret = readStatusReg(SYS_DIAG1, mask, 0);
	return ret ^ (TLE94108_STATUS_INV_MASK &mask);
}

uint8_t TLE94108::getHBOverCurrent(HalfBridge hb) {
	return getHBOverCurrent(static_cast<uint8_t>(hb));
}

uint8_t TLE94108::getHBOverCurrent(uint8_t hb) {
	uint8_t reg   = mHalfBridges[hb].ocReg;
	uint8_t mask  = mHalfBridges[hb].ocMask;
	uint8_t shift = mHalfBridges[hb].ocShift;
	return readStatusReg(reg, mask, shift);
}

uint8_t TLE94108::getHBOpenLoad(HalfBridge hb) {
	return getHBOpenLoad(static_cast<uint8_t>(hb));
}

uint8_t TLE94108::getHBOpenLoad(uint8_t hb) {
	uint8_t reg   = mHalfBridges[hb].olReg;
	uint8_t mask  = mHalfBridges[hb].olMask;
	uint8_t shift = mHalfBridges[hb].olShift;
	return readStatusReg(reg, mask, shift);
}

void TLE94108::clearErrors() {
	clearStatusReg(SYS_DIAG1);
	clearStatusReg(OP_ERROR_1_STAT);
	clearStatusReg(OP_ERROR_2_STAT);
	clearStatusReg(OP_ERROR_3_STAT);
	clearStatusReg(OP_ERROR_4_STAT);
	clearStatusReg(OP_ERROR_5_STAT);
	clearStatusReg(OP_ERROR_6_STAT);
}

void TLE94108::writeReg(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t data) {
	uint8_t address = mCtrlRegAddresses[reg];
	uint8_t toWrite = mCtrlRegData[reg] & (~mask);
	toWrite |= (data << shift) & mask;
	mCtrlRegData[reg] = toWrite;

	address = address | TLE94108_CMD_WRITE;
	digitalWrite(mCsPin, LOW);
	uint8_t byte0 = mBus->transfer(address);
	uint8_t byte1 = mBus->transfer(toWrite);
	digitalWrite(mCsPin, HIGH);
	delay(TLE94108_CS_RISETIME);
}

uint8_t TLE94108::readStatusReg(uint8_t reg) {
	//read the whole register
	return readStatusReg(reg, 0xFF, 0);
}

uint8_t TLE94108::readStatusReg(uint8_t reg, uint8_t mask, uint8_t shift) {
	uint8_t address = mStatusRegAddresses[reg];

	digitalWrite(mCsPin, LOW);
	uint8_t byte0    = SPI.transfer(address);
	uint8_t received = SPI.transfer(0xFF);    //send dummy byte while receiving
	digitalWrite(mCsPin, HIGH);
	delay(TLE94108_CS_RISETIME);

	received = (received & mask) >> shift;

	return received;
}

void TLE94108::clearStatusReg(uint8_t reg) {
	uint8_t address = mStatusRegAddresses[reg];

	address = address | TLE94108_CMD_CLEAR;
	digitalWrite(mCsPin, LOW);
	uint8_t byte0 = mBus->transfer(address);
	uint8_t byte1 = mBus->transfer(0);    //clear register by writing 0x00
	digitalWrite(mCsPin, HIGH);
	delay(TLE94108_CS_RISETIME);
}

