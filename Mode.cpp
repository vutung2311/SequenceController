#include "SequenceController.h"

void switchMode()
{
  mode = mode ^ 1;
  if (mode)
  {
    // Tang thoi gian debounce
    // cho cam bien bon lieu va mang lieu
    debouncer[CB_BON_LIEU].interval(3000);
    debouncer[CB_KHUON_LIEU].interval(3000);

    // Tat tat cac cac dau ra ve logic thap
    // Cai dat trang thai cong ra cua cac van va xuat logic thap
    for (int8_t i = 0; i < sizeof(valveOutput) / sizeof(int8_t); i++)
    {
      digitalWriteFast(valveOutput[i], LOW);
    }

    // Cai dat trang thai cong ra cua motor va xuat logic thap
    for (int8_t i = 0; i < sizeof(motorOutput) / sizeof(int8_t); i++)
    {
      digitalWriteFast(motorOutput[i], LOW);
    }

    // Dat cac gia tri input, inputValue va outputLock ve mac dinh
    memset(outputLock, -1, sizeof(outputLock));
    memset(inputValue, 1, sizeof(inputValue));
  }
  if (!mode)
  {
    // Dat interval cua cac cam bien
    // ve 50ms cho phu hop voi nut bam
    debouncer[CB_BON_LIEU].interval(50);
    debouncer[CB_KHUON_LIEU].interval(50);

    // Dat cac gia tri input, inputValue va outputLock ve mac dinh
    memset(outputLock, -1, sizeof(outputLock));
    memset(inputValue, 1, sizeof(inputValue));

    // Tat tat cac cac dau ra ve logic thap
    // Cai dat trang thai cong ra cua cac van va xuat logic thap
    for (int8_t i = 0; i < sizeof(valveOutput) / sizeof(int8_t); i++)
    {
      digitalWriteFast(valveOutput[i], LOW);
    }

    // Cai dat trang thai cong ra cua motor va xuat logic thap
    for (int8_t i = 0; i < sizeof(motorOutput) / sizeof(int8_t); i++)
    {
      digitalWriteFast(motorOutput[i], LOW);
    }
  }
}

void manualMode(int8_t mode)
{
  if (mode)
  {
    return;
  }

  // Chay dong co bom dau khi nut bam 1 duoc bam
  if (!inputValue[CB_BON_LIEU])
  {
    digitalWriteAware(motorOutput[DR_DC_BOM_DAU], HIGH);
    digitalWriteAware(valveOutput[DR_VAN_1_DAU], HIGH);
    lock(motorOutput[DR_DC_BOM_DAU], input[CB_BON_LIEU]);
    lock(valveOutput[DR_VAN_1_DAU], input[CB_BON_LIEU]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[CB_BON_LIEU]);
    unlock(valveOutput[DR_VAN_1_DAU], input[CB_BON_LIEU]);
    digitalWriteAware(valveOutput[DR_VAN_1_DAU], LOW);
    digitalWriteAware(motorOutput[DR_DC_BOM_DAU], LOW);
  }

  // Chay dong co cap lieu vao bon khi nut bam 2 duoc bam
  digitalWriteAware(motorOutput[DR_DC_CAP_LIEU_BON], !inputValue[CB_KHUON_LIEU]);

  // Chay dong co cap lieu vao khuon khi nut bam 3 duoc bam
  digitalWriteAware(motorOutput[DR_DC_CAP_LIEU_KHUON], !inputValue[CB_VT_1_BEN_CHINH]);

  // Ben chinh tien chi khi cam bien vi tri 4
  // cua ben chinh khong bat va nut bam 4 duoc bam
  if (!inputValue[CB_AP_SUAT])
  {
    movePiston(1, 1, HIGH);
    if (inputValue[CB_VT_CUOI_BEN_CHINH])
    {
      digitalWriteFast(motorOutput[DR_DC_BOM_DAU], LOW);
      digitalWriteFast(valveOutput[DR_VAN_1_DAU], LOW);
      digitalWriteFast(valveOutput[DR_DAY_BEN_CHINH_2], LOW);
    }
    lock(motorOutput[DR_DC_BOM_DAU], input[CB_AP_SUAT]);
    lock(valveOutput[DR_VAN_1_DAU], input[CB_AP_SUAT]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[CB_AP_SUAT]);
    unlock(valveOutput[DR_VAN_1_DAU], input[CB_AP_SUAT]);
    movePiston(1, 1, LOW);
  }

  // Ben chinh lui chi khi cam bien dau ben chinh
  // khong bat va nut bam 5 duoc bam
  if (!inputValue[CB_NHIET_DO_DAU])
  {

    movePiston(1, -1, HIGH);
    if (!inputValue[CB_VT_DAU_BEN_CHINH])
    {
      digitalWriteFast(motorOutput[DR_DC_BOM_DAU], LOW);
      digitalWriteFast(valveOutput[DR_VAN_1_DAU], LOW);
      digitalWriteFast(valveOutput[DR_RUT_BEN_CHINH_2], LOW);
    }
    lock(motorOutput[DR_DC_BOM_DAU], input[CB_NHIET_DO_DAU]);
    lock(valveOutput[DR_VAN_1_DAU], input[CB_NHIET_DO_DAU]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[CB_NHIET_DO_DAU]);
    unlock(valveOutput[DR_VAN_1_DAU], input[CB_NHIET_DO_DAU]);
    movePiston(1, -1, LOW);
  }

  // Ben day san pham tien chi khi cam bien cuoi ben
  // day san pham khong bat va nut bam 6 duoc bam
  if (!inputValue[CB_VT_2_BEN_CHINH])
  {
    movePiston(2, 1, HIGH);
    lock(motorOutput[DR_DC_BOM_DAU], input[CB_VT_2_BEN_CHINH]);
    lock(valveOutput[DR_VAN_1_DAU], input[CB_VT_2_BEN_CHINH]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[CB_VT_2_BEN_CHINH]);
    unlock(valveOutput[DR_VAN_1_DAU], input[CB_VT_2_BEN_CHINH]);
    movePiston(2, 1, LOW);
  }

  // Ben day san pham lui chi khi cam bien dau ben
  // day san pham khong bat va nut bam 7 duoc bam
  if (!inputValue[NUT_DU_PHONG_1])
  {
    movePiston(2, -1, HIGH);
    lock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_1]);
    lock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_1]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_1]);
    unlock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_1]);
    movePiston(2, -1, LOW);
  }

  // Ben cua dong cua chi khi cam bien cuoi
  // ben cua khong bat va nut bam 8 duoc bam
  if (!inputValue[NUT_DU_PHONG_2])
  {
    movePiston(3, 1, HIGH);
    lock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_2]);
    lock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_2]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_2]);
    unlock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_2]);
    movePiston(3, 1, LOW);
  }

  // Ben cua mo cua chi khi cam bien dau
  // ben cua khong bat va nut bam 9 duoc bam
  if (!inputValue[NUT_DU_PHONG_3])
  {
    movePiston(3, -1, HIGH);
    lock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_3]);
    lock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_3]);
  }
  else
  {
    unlock(motorOutput[DR_DC_BOM_DAU], input[NUT_DU_PHONG_3]);
    unlock(valveOutput[DR_VAN_1_DAU], input[NUT_DU_PHONG_3]);
    movePiston(3, -1, LOW);
  }

}

void automaticMode(int8_t mode)
{
  if (!mode)
  {
    return;
  }

  // Hoat dong tu dong mot cach tuan tu
  // Buoc 1
  switch (step)
  {
    case 1:
      digitalWriteAware(motorOutput[DR_DC_CAP_LIEU_KHUON], !inputValue[CB_KHUON_LIEU]);
      if (!inputValue[CB_KHUON_LIEU])
        step = 2;
      break;
    case 2:
      // Di chuyen ben chinh cho den khi cam bien 2 bat
      movePiston(1, 1, inputValue[CB_VT_1_BEN_CHINH]);
      if (!inputValue[CB_VT_1_BEN_CHINH])
        step = 3;
      break;
    case 3:
      movePiston(1, -1, inputValue[CB_VT_DAU_BEN_CHINH]);
      if (!inputValue[CB_VT_DAU_BEN_CHINH])
        step = (step == 2) ? 4 : 5;
      break;
    case 4:
      // Di chuyen ben chinh cho den khi cam bien 3 bat
      movePiston(1, 1, inputValue[CB_VT_2_BEN_CHINH]);
      if (!inputValue[CB_VT_2_BEN_CHINH])
        step = 3;
      break;
    case 5:
      // Di chuyen ben chinh cho den cam bien cuoi cung
      movePiston(1, 1, inputValue[CB_VT_CUOI_BEN_CHINH]);
      if (!inputValue[CB_VT_CUOI_BEN_CHINH])
        step = 5;
      break;
    case 6:
      movePiston(3, -1, !inputValue[CB_VT_DAU_BEN_CUA]);
      if (!inputValue[CB_VT_DAU_BEN_CUA])
      {
        movePiston(2, 1, inputValue[CB_VT_CUOI_BEN_SP]);
      }
      if (!inputValue[CB_VT_CUOI_BEN_SP])
      {
        step++;
      }
      break;
    default:
      if (probe())
        step = 1;
      break;
  }
}
