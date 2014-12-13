#include <Arduino.h>
#include <TimerOne.h>
#include <digitalWriteFast.h>
#include <Bounce.h>

#ifndef SEQUENCECONTROLLER_H_INCLUDED
#define SEQUENCECONTROLLER_H_INCLUDED

#define BOUNCE_LOCK_OUT

// Xac lap chi muc cho tung cam bien
#define CB_BON_LIEU 0
#define CB_KHUON_LIEU 1
#define CB_VT_DAU_BEN_CHINH 2
#define CB_VT_1_BEN_CHINH 3
#define CB_VT_2_BEN_CHINH 4
#define CB_VT_CUOI_BEN_CHINH 6
#define CB_VT_DAU_BEN_CUA 8
#define CB_VT_CUOI_BEN_CUA 7
#define CB_VT_DAU_BEN_SP 9
#define CB_VT_CUOI_BEN_SP 10
#define NUT_CHE_DO 11
#define CB_AP_SUAT 12
#define CB_NHIET_DO_DAU 13
#define NUT_DU_PHONG_1 5
#define NUT_DU_PHONG_2 14
#define NUT_DU_PHONG_3 15

// Xac lap chi  muc cho tung dau ra vao van
#define DR_DU_PHONG_1 0
#define DR_DU_PHONG_2 1
#define DR_DU_PHONG_3 2
#define DR_DU_PHONG_4 3
#define DR_DAY_BEN_CUA 11
#define DR_RUT_BEN_CUA 5
#define DR_DAY_BEN_SP 7
#define DR_RUT_BEN_SP 8
#define DR_RUT_BEN_CHINH_1 9
#define DR_RUT_BEN_CHINH_2 4
#define DR_DAY_BEN_CHINH_1 10
#define DR_DAY_BEN_CHINH_2 6
#define DR_VAN_1_DAU 12

// Xac lap chi muc cho tung dau ra vao dong co
#define DR_DC_BOM_DAU 0
#define DR_DC_CAP_LIEU_BON 1
#define DR_DC_CAP_LIEU_KHUON 2

// Khai bao cac bien
extern uint8_t input[16];
extern uint8_t inputValue[16];
extern uint8_t valveOutput[13];
extern uint8_t motorOutput[3];
extern int8_t outputLock[22];
extern int8_t mode;
extern int8_t step;
extern Bounce debouncer[16];
extern uint32_t oldTime;
extern int8_t lastModeState;

// Khai bao cac ham
void switchMode();
void manualMode(int8_t mode);
void automaticMode(int8_t mode);
void initialize();
bool probe();
void movePiston(int8_t pistonPosition, int8_t direction, int8_t value);
void digitalWriteAware(uint8_t pin, int8_t value);
void lock(uint8_t pin, int8_t key);
void unlock(uint8_t pin, int8_t key);
void sendReport(int8_t report);

#endif // SEQUENCE_CONTROLLER_H_INCLUDED
