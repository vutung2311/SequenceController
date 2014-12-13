#include "SequenceController.h"

uint8_t input[16] =
{
  A15, // 1
  A14, // 2
  A13, // 3
  A12, // 4
  A11, // 5
  A10, // 6
  A9,  // 7
  A8,  // 8
  A7,  // 9
  A6,  // 10
  A5,  // 11
  A4,  // 12
  A3,  // 13
  A2,  // 14
  A1,  // 15
  A0,  // 16
};
uint8_t inputValue[16] = {};
uint8_t valveOutput[13] =
{
  4,  // 1
  21, // 2
  20, // 3
  19, // 4
  18, // 5
  17, // 6
  16, // 7
  15, // 8
  14, // 9
  0,  // 10
  1,  // 11
  2,  // 12
  3,  // 13
};
uint8_t motorOutput[3] =
{
  7, // 14
  6, // 15
  5, // 16
};
int8_t outputLock[22] = {};

int8_t mode = 0;
int8_t step = 0;
uint32_t oldTime = 0;
int8_t lastModeState = 1;

Bounce debouncer[16];

void initialize()
{
  // Vo hieu tat ca ngat
  noInterrupts();

  // Cai dat trang thai cong ra cua cac van va xuat logic thap
  for (int8_t i = 0; i < sizeof(valveOutput) / sizeof(int8_t); i++)
  {
    pinMode(valveOutput[i], OUTPUT);
    digitalWriteFast(valveOutput[i], LOW);
  }

  // Cai dat trang thai cong ra cua motor va xuat logic thap
  for (int8_t i = 0; i < sizeof(motorOutput) / sizeof(int8_t); i++)
  {
    pinMode(motorOutput[i], OUTPUT);
    digitalWriteFast(motorOutput[i], LOW);
  }

  // Cai dat cam bien cong vao
  for (int8_t i = 0; i < sizeof(input) / sizeof(int8_t); i++)
  {
    pinMode(input[i], INPUT);
    debouncer[i] = Bounce();
    debouncer[i].attach(input[i]);
    if (i == NUT_CHE_DO)
    {
      debouncer[i].interval(1000);
    }
    else
    {
      debouncer[i].interval(50);
    }
  }

  // Dat cac gia tri input, inputValue va outputLock ve mac dinh
  memset(outputLock, -1, sizeof(outputLock));
  memset(inputValue, 1, sizeof(inputValue));

  // Bat cac ngat tro lai
  interrupts();

  // Khoi tao giao thuc RS-232
  //  Serial.begin(112500);

}

bool probe()
{
  // Rut ben ep chinh
  if (inputValue[CB_VT_DAU_BEN_CHINH])
  {
    movePiston(1, -1, HIGH);
  }

  // Day ben dong cua
  if (inputValue[CB_VT_CUOI_BEN_CUA])
  {
    movePiston(3, 1, HIGH);
  }

  // Rut ben day san pham
  if (inputValue[CB_VT_DAU_BEN_SP])
  {
    movePiston(2, -1, HIGH);
  }

  // Thoat het sau khi cac dieu kien da thoa
  if (!inputValue[CB_VT_DAU_BEN_CHINH] && !inputValue[CB_VT_CUOI_BEN_CUA] && !inputValue[CB_VT_DAU_BEN_SP])
  {
    return true;
  }
  return false;
}

void movePiston(int8_t pistonPosition, int8_t direction, int8_t value)
{
  switch (pistonPosition)
  {
    case 1: // Dieu khien ben chinh
      if (direction == 1)
      {
        // Tien ben chinh
        if (!inputValue[CB_VT_CUOI_BEN_CHINH])
        {
          digitalWriteAware(valveOutput[DR_DAY_BEN_CHINH_2], value);
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
        }
      }
      else if (direction == -1)
      {
        // Lui  ben chinh
        if (inputValue[CB_VT_DAU_BEN_CHINH])
        {
          digitalWriteAware(valveOutput[DR_RUT_BEN_CHINH_2], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
        }
      }
      break;
    case 2: // Dieu khien ben san pham
      if (direction == 1)
      {
        // Tien ben san pham
        if (inputValue[CB_VT_CUOI_BEN_SP])
        {
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
          digitalWriteAware(valveOutput[DR_DAY_BEN_SP], value);
        }
      }
      else if (direction == -1)
      {
        // Lui ben san pham
        if (inputValue[CB_VT_DAU_BEN_SP])
        {
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
          digitalWriteAware(valveOutput[DR_RUT_BEN_SP], value);
        }
      }
      break;
    case 3: // Dieu khien ben cua
      if (direction == 1)
      {
        // Tien ben cua
        if (inputValue[CB_VT_CUOI_BEN_CUA])
        {
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
          digitalWriteAware(valveOutput[DR_DAY_BEN_CUA], value);
        }
      }
      else if (direction == -1)
      {
        // Lui ben cua
        if (inputValue[CB_VT_DAU_BEN_CUA])
        {
          digitalWriteAware(valveOutput[DR_VAN_1_DAU], value);
          digitalWriteAware(motorOutput[DR_DC_BOM_DAU], value);
          digitalWriteAware(valveOutput[DR_RUT_BEN_CUA], value);
        }
      }
      break;
    default:
      break;
  }
}

void digitalWriteAware(uint8_t pin, int8_t value)
{
  if (outputLock[pin] == -1)
  {
    digitalWriteFast(pin, value);
  }
}

void lock(uint8_t pin, int8_t key)
{
  if (outputLock[pin] == -1)
    outputLock[pin] = key;
  return;
}

void unlock(uint8_t pin, int8_t key)
{
  if (outputLock[pin] == key || key == -1)
    outputLock[pin] = -1;
  return;
}

void sendReport(int8_t report)
{
  switch (report)
  {
    case 1:
      for (int8_t i = 0; i < sizeof(input) / sizeof(int8_t); i++)
      {
        Serial.print("input[");
        Serial.print(i);
        Serial.print("] is: ");
        Serial.println(inputValue[i]);
      }
      break;
    case 2:
      for (int8_t i = 0; i < sizeof(outputLock) / sizeof(int8_t); i++)
      {
        Serial.print("outputLock[");
        Serial.print(i);
        Serial.print("] is: ");
        Serial.println(outputLock[i]);
      }
      break;
    case 3:
      for (int8_t i = 0; i < sizeof(valveOutput) / sizeof(int8_t); i++)
      {
        Serial.print("valveOutput[");
        Serial.print(i);
        Serial.print("] is: ");
        Serial.println(digitalRead(valveOutput[i]));
      }
      break;
    case 4:
      Serial.print("The current mode is: ");
      if (mode)
        Serial.println("automatic");
      if (!mode)
        Serial.println("manual");
      break;
    case 5:
      for (int8_t i = 0; i < sizeof(input) / sizeof(int8_t); i++)
      {
        Serial.print("interval[");
        Serial.print(i);
        Serial.print("] is: ");
        Serial.println(debouncer[i].getInterval());
      }
      break;
    default:
      break;
  }
}
