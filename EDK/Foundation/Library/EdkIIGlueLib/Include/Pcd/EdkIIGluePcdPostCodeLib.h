/*++

Copyright (c) 2004 - 2006, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  EdkIIGluePcdPostCodeLib.h
  
Abstract: 

  PCD values for library customization

--*/

#ifndef __EDKII_GLUE_PCD_POST_CODE_LIB_H__
#define __EDKII_GLUE_PCD_POST_CODE_LIB_H__

//
// Following Pcd values are hard coded at compile time.
// Override these through compiler option "/D" in PlatformTools.env if needed
//

//
// PostCode Pcds
//
#ifndef __EDKII_GLUE_PCD_PcdPostCodePropertyMask__
#define __EDKII_GLUE_PCD_PcdPostCodePropertyMask__     EDKII_GLUE_PostCodePropertyMask
#endif

#include "Pcd/EdkIIGluePcd.h"

#endif
