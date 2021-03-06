/** @file
  Non training specific memory controller configuration definitions.

@copyright
  Copyright (c) 1999 - 2012 Intel Corporation. All rights reserved.
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement.
**/
#ifndef __MrcMcConfiguration_h__
#define __MrcMcConfiguration_h__

#include "MrcTypes.h"
#include "MrcApi.h"
#include "McAddress.h"
#include "MrcAddressDecodeConfiguration.h"
#include "MrcCommandTraining.h"
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcGlobal.h"
#include "MrcOem.h"
#include "MrcOemIo.h"
#include "MrcRefreshConfiguration.h"
#include "MrcSchedulerParameters.h"
#include "MrcTimingConfiguration.h"

#define DISABLE_ODT_STATIC  (0)     ///< May change in the future
#define CDIEVSSHI           (2000)  ///< Constant CdieVssHi = 2000 (value in pF)
#define RCMDREF             (100)   ///< Constant RcmdRef = 100 (value in ohm)

/**
@brief
  This function calculates the two numbers that get you closest to the slope.

  @param[in] Slope - targeted slope (multiplied by 100 for int match)

  @retval Returns the Slope Index to be programmed for VtSlope.
**/
extern
U8
MrcCalcVtSlopeCode (
  const U16 Slope
  );

/**
@brief
  This function performs the memory controller configuration non training sequence.

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status
**/
extern
MrcStatus
MrcMcConfiguration (
  MrcParameters *const MrcData
  );

/**
@brief
  This function init all the necessary registers for the training.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
extern
MrcStatus
MrcPreTraining (
  MrcParameters *const MrcData
  );

/**
@brief
  This function initializes all the necessary registers after main training steps but before LCT.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
extern
MrcStatus
MrcPostTraining (
  MrcParameters *const MrcData
  );

/**
@brief
  Program PCU_CR_DDR_VOLTAGE register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] VddVoltage - Current DDR voltage.

  @retval none
**/
extern
void
MrcSetPcuDdrVoltage (
  IN OUT MrcParameters *MrcData,
  IN     MrcVddSelect  VddVoltage
  );

#endif // __MrcMcConfiguration_h__
