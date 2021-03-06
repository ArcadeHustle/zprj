/** @file
  Header file for the PCI Express library.

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _SA_PCI_EXPRESS_LIB_H_
#define _SA_PCI_EXPRESS_LIB_H_

#if defined (EDK_RELEASE_VERSION) && (EDK_RELEASE_VERSION < 0x00020000)
#include "EdkIIGlueBase.h"
#endif

/**
  Gets the base address of PCI Express.

  This internal functions retrieves PCI Express Base Address.

  @return The base address of PCI Express.
**/
VOID*
GetPciExpressBaseAddress (
  VOID
);

#endif
