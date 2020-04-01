//
// Created by b3tuning@gmail.com 3/20/2020
//

#include <Arduino.h>

#include <MirrorHelper.h>
#include <WindowHelper.h>
#include <ConfigHelper.h>
#include <ButtonHelper.h>
#include <ModuleHelper.h>
#include <CommsHelper.h>

/**
 * ***************************************************************************************
 * AMP SuperSeal 26 Pinout
 *
 *  1 --> FOLD_A
 *  2 --> FOLD_B
 *  3 <-- P_WIN
 *  4 <-- MIR_ADJ
 *  5 <-- VBAT
 *  6 <-- VBAT
 *  7 --> WIN_A
 *  8 --> MIR_COM
 *  9 <-- REV_IN
 * 10 --> P_MIR
 * 11 --> D_MIR
 * 12 <-- MIR_SEL
 * 13 --> WIN_A
 * 14 --> MIR_V
 * 15 <-- ILLUM_IN
 * 16 <-- RX
 * 17 <-- GND
 * 18 <-- GND
 * 19 --> WIN_B
 * 20 --> MIR_H
 * 21 <-- D_WIN
 * 22 --> ILLUM_OUT
 * 23 --> TX
 * 24 <-- GND
 * 25 <-- GND
 * 26 --> WIN_B
 *
 * ***************************************************************************************
 * Arduino Pinout
 *
 *  1 -->  D1 --> TX            --> 26 Pin --> 23
 *  2 <--  D0 <-- RX            <-- 26 Pin <-- 16
 *  5 -->  D2 --> DIR           --> Pololu
 *  6 -->  D3 --> PWM           --> Pololu
 *  7 -->  D4 --> DRV_IN1       --> DRV8872
 *  8 -->  D5 --> DRV_IN2       --> DRV8872
 *  9 -->  D6 --> ILLUM_OUT     --> 26 Pin --> 22
 * 10 -->  D7 --> D_MIR         --> 26 Pin --> 11
 * 11 -->  D8 --> P_MIR         --> 26 Pin --> 10
 * 12 -->  D9 --> TLE_EN        --> TLE94108
 * 13 --> D10 --> TLE_SS        --> TLE94108
 * 14 --> D11 --> TLE_MOSI      --> TLE94108
 * 15 <-- D12 <-- TLE_MISO      <-- TLE94108
 * 16 --> D13 --> TLE_SCK       --> TLE94108
 * 19 <--  A0 <-- D_WIN_IN      <-- 26 Pin <-- 21
 * 20 <--  A1 <-- P_WIN_IN      <-- 26 Pin <--  3
 * 21 <--  A2 <-- MIR_ADJ_IN    <-- 26 Pin <--  4
 * 22 <--  A3 <-- MIR_SEL_IN    <-- 26 Pin <-- 12
 * 23 <--  A4 <-- DRV_FAULT_IN  <-- DRV8872
 * 24 <--  A5 <-- CUR_SEN_IN    <-- Pololu
 * 25 <--  D9 <-- ILLIUM_IN     <-- 26 Pin <-- 15
 * 26 <--  A7 <-- REV_IN        <-- 26 Pin <--  9
 *
 *
 * ***************************************************************************************
 * TLE94108 Pinout
 *
 * 1  --> Ground
 * 2  --> HB1  --> Mirror Common            <-- DT-15 --> Pin 10
 * 3  --> HB5  --> Mirror Fold              <-- DT-15 --> Pin 3
 * 4  --> HB7  --> Mirror Fold              <-- DT-15 --> Pin 3
 * 5  --> SDI  --> MOSI                     <-- Arduino --> D11
 * 6  --> VDD  --> 5V
 * 7  --> SDO  --> MISO                     <-- Arduino --> D12
 * 8  --> EN   -->                          <-- Arduino --> D9
 * 9  --> N/C
 * 10 --> HB6  --> Mirror Fold              <-- DT-15 --> Pin 3
 * 11 --> HB4  --> Mirror Horiz             <-- DT-15 --> Pin 2
 * 12 --> Ground
 * 13 --> Ground
 * 14 --> HB3  --> Mirror Common            <-- DT-15 --> Pin 10
 * 15 --> N/C
 * 16 --> VS1
 * 17 --> N/C
 * 18 --> N/C
 * 19 --> CSN  --> SS                       <-- Arduino --> D10
 * 20 --> SCLK --> SCK                      <-- Arduino --> D13
 * 21 --> VS2
 * 22 --> HB8  --> Mirror Vert              <-- DT-15 --> Pin 11
 * 23 --> HB2  --> Mirror Common            <-- DT-15 --> Pin 10
 * 24 --> Ground
 * 25 --> Ground
 *
 * ***************************************************************************************
 * DT-15 Pinout
 *
 * 1  --> Window B --> Pololu
 * 2  --> Mirror Horiz                      <-- TLE94108 --> Pin 11
 * 3  --> Mirror Fold                       <-- TLE94108 --> Pin 3, 4, 10
 * 4  --> Ground
 * 5  --> Ground
 * 6  --> N/C
 * 7  --> N/C
 * 8  --> VBAT
 * 9  --> VBAT
 * 10 --> Mirror Common                     <-- TLE94108 --> Pin 2, 14, 23
 * 11 --> Mirror Vert                       <-- TLE94108 --> Pin 22
 * 12 --> Window A --> Pololu
 *
 * ***************************************************************************************
 * DTM-15 Pinout
 *
 * 1  --> TX                                <-- Arduino --> D1
 * 2  --> Illum_IN                          <-- Arduino --> A2
 * 3  --> Driver Window                     <-- Arduino --> A4 <-- Red/Yellow
 * 4  --> Mirror Select                     <-- Arduino --> A6 <-- Purple/White
 * 5  --> Driver Mirror Selected            <-- Arduino --> D5 <-- Green/Black
 * 6  --> Ground                                               <-- Red/Green
 * 7  --> Ground
 * 8  --> Passenger Mirror Selected         <-- Arduino --> D6 <-- Blue/Black
 * 9  --> Illum_OUT                         <-- Arduino --> D4 <-- Red/Blue
 * 10 --> Passenger Window                  <-- Arduino --> A3 <-- Red/White
 * 11 --> Mirror Adjust                     <-- Arduino --> A5 <-- Purple/Red
 * 12 --> RX                                <-- Arduino --> D0
 *
 * ***************************************************************************************
 * Porsche Window Switch (Driver)
 *
 *  1 <-- Red/Green    -- Ground
 *  2 <-- Red/Blue     -- Illumination
 *  3 --> Purple/Red   -- Mirror Adjust
 *  4 <-- Green/Black  -- Driver Mirror Selected
 *  5 <-- Blue/Black   -- Passenger Mirror Selected
 *  6 --> Purple/White -- Mirror Select
 *  7 --> Red/Yellow   -- Driver Window
 *  8 --> Red/White    -- Passenger Window
 *
 */

/**
 * MIRROR WIRING
 *
 * Driver Mirror
 * Connector - Mirror
 * R -> W
 * W -> G
 * G -> R
 *
 * Pass Mirror
 * R = Vertical
 * G = Horizontal
 * B/W = Common
 *
 *
 * R -> G
 * W -> R
 * G -> W
 *
 */

/**
 * configure MODULE as either 'DRIVER' or 'PASSENGER' for specific code handling
 */
MODULE module = MODULE::DRIVER;

//ConfigHelper config;
MODULE_STATE state;
MODULE_STATE rxData;
EasyTransfer in, out;
ModuleHelper moduleHelper(state, module);
CommsHelper  commsHelper(moduleHelper, rxData, in, out);
ButtonHelper buttonHelper(moduleHelper);
MirrorHelper mirrorHelper(moduleHelper);
WindowHelper windowHelper(moduleHelper);

void setup() {
	Serial.begin(9600);
//	while(!Serial) {
//		delay(100);
//	}
//	Serial.println("STARTING....");

	pinMode(RX, INPUT);
	pinMode(TX, OUTPUT);

	pinMode(POLOLU_DIR, OUTPUT);
	pinMode(POLOLU_PWM, OUTPUT);
	pinMode(POLOLU_CUR_SEN_IN, INPUT);

	pinMode(DRV_IN1, OUTPUT);
	pinMode(DRV_IN2, OUTPUT);
	pinMode(DRV_FAULT_IN, INPUT_PULLUP);

//	pinMode(TLE_EN, OUTPUT);
//	pinMode(TLE_SS, OUTPUT);
	pinMode(TLE_MOSI, OUTPUT);
	pinMode(TLE_MISO, INPUT_PULLUP);
	pinMode(TLE_SCK, OUTPUT);

	pinMode(ILLUM_OUT, OUTPUT);
	pinMode(D_MIR, OUTPUT);
	pinMode(P_MIR, OUTPUT);

	pinMode(D_WIN_IN, INPUT_PULLUP);
	pinMode(P_WIN_IN, INPUT_PULLUP);
	pinMode(MIR_ADJ_IN, INPUT_PULLUP);
	pinMode(MIR_SEL_IN, INPUT_PULLUP);
	pinMode(ILLUM_IN, INPUT_PULLUP);
	pinMode(REV_IN, INPUT_PULLUP);

	digitalWrite(POLOLU_DIR, LOW);
	digitalWrite(POLOLU_PWM, LOW);
	digitalWrite(DRV_IN1, LOW);
	digitalWrite(DRV_IN2, LOW);
//	digitalWrite(TLE_EN, LOW);
//	digitalWrite(TLE_SS, LOW);
	digitalWrite(TLE_MOSI, LOW);
	digitalWrite(TLE_SCK, LOW);
	digitalWrite(D_MIR, LOW);
	digitalWrite(P_MIR, LOW);
//	ConfigHelper::test();
}

void loop() {
	buttonHelper.update();
	commsHelper.update();
	windowHelper.update();
	mirrorHelper.update();
}