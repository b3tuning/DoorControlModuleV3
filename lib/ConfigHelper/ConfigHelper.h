//
// Created by b3tuning@gmail.com 3/20/2020
//

#ifndef DOORCONTROL_CONFIGHELPER_H
#define DOORCONTROL_CONFIGHELPER_H

#include <Arduino.h>

#define RX 0
#define TX 1

#define POLOLU_DIR 2
#define POLOLU_PWM 3
#define POLOLU_CUR_SEN_IN A5

#define DRV_IN1 4
#define DRV_IN2 5
#define DRV_FAULT_IN A4

#define TLE_EN    9
#define TLE_SS   10
#define TLE_MOSI 11
#define TLE_MISO 12
#define TLE_SCK  13

#define ILLUM_OUT 6
#define D_MIR     7
#define P_MIR     8

#define D_WIN_IN   A0
#define P_WIN_IN   A1
#define MIR_ADJ_IN A2
#define MIR_SEL_IN A3
#define ILLUM_IN    9
#define REV_IN     A7

const unsigned long FOLD_INTERVAL_MS          = 3000L;
const unsigned long MIRROR_SELECT_INTERVAL_MS = 60000L;

//class ConfigHelper {
//public:
//	ConfigHelper();
//	static void test();
//};

#endif //DOORCONTROL_CONFIGHELPER_H