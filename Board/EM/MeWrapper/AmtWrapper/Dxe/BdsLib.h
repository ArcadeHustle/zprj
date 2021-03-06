//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//**********************************************************************
//
// $Header: /Alaska/SOURCE/Modules/SharkBayRefCodes/ME/AmtWrapper/AmtWrapperDxe/BdsLib.h 1     2/08/12 1:08a Klzhan $
//
// $Revision: 1 $
//
// $Date: 2/08/12 1:08a $
//
//**********************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/SharkBayRefCodes/ME/AmtWrapper/AmtWrapperDxe/BdsLib.h $
// 
// 1     2/08/12 1:08a Klzhan
// Initial Check in 
// 
// 3     9/21/11 8:52a Klzhan
// [TAG]  		EIP69500
// [Category]  	Bug Fix
// [Severity]  	Minor
// [Symptom]  	Compiler error when DXE_PERFORMANCE is on
// [RootCause]  	EDK Library doesn't support this function.
// [Files]  		BdsLib.h
// Performance.c
// 
// 1     2/25/11 1:43a Klzhan
// Initial Check-in
// 
// 1     12/03/10 5:10a Klzhan
// Initial Check-in.
// 
//
//**********************************************************************
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:            BdsConsole.h
//
// Description:     BDS library definition, include the file and data structure
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>
/*++

Copyright (c) 2004, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  BdsLib.h

Abstract:

  BDS library definition, include the file and data structure

--*/

#ifndef _BDS_LIB_H_
#define _BDS_LIB_H_

#include "Tiano.h"
#include "EfiDriverLib.h"
#include "EfiPrintLib.h"
#include "BmMachine.h"
#include "EfiHobLib.h"

#include EFI_PROTOCOL_DEFINITION (SerialIo)
#include EFI_PROTOCOL_DEFINITION (BlockIo)
#include EFI_PROTOCOL_DEFINITION (LegacyBios)
#include EFI_PROTOCOL_DEFINITION (AcpiS3Save)
#include EFI_PROTOCOL_DEFINITION (LoadedImage)
#include EFI_PROTOCOL_DEFINITION (SimpleFileSystem)
#include EFI_PROTOCOL_DEFINITION (SimpleNetwork)
#include EFI_PROTOCOL_DEFINITION (LoadFile)
#include EFI_PROTOCOL_DEFINITION (PlatformDriverOverride)
#include EFI_PROTOCOL_DEFINITION (ConsoleControl)
#include EFI_PROTOCOL_DEFINITION (UgaDraw)
#include EFI_PROTOCOL_DEFINITION (Hii)
#include EFI_PROTOCOL_DEFINITION (FirmwareVolume)

#include EFI_PROTOCOL_DEFINITION (TcgService)
#include EFI_GUID_DEFINITION (PcAnsi)
#include EFI_GUID_DEFINITION (Hob)
#include EFI_GUID_DEFINITION (HotPlugDevice)
#include EFI_GUID_DEFINITION (GlobalVariable)
#include EFI_GUID_DEFINITION (EfiShell)
#include EFI_GUID_DEFINITION (ConsoleInDevice)
#include EFI_GUID_DEFINITION (ConsoleOutDevice)
#include EFI_GUID_DEFINITION (StandardErrorDevice)

//
// Include the performance head file and defind macro to add perf data
//
#ifdef EFI_DXE_PERFORMANCE
#include "Performance.h"
#define WRITE_BOOT_TO_OS_PERFORMANCE_DATA WriteBootToOsPerformanceData ()
#else
#define WRITE_BOOT_TO_OS_PERFORMANCE_DATA
#endif

extern EFI_HANDLE mBdsImageHandle;

//
// Constants which are variable names used to access variables
//
#define VarLegacyDevOrder L"LegacyDevOrder"

//
// Data structures and defines
//
#define FRONT_PAGE_QUESTION_ID  0x0000
#define FRONT_PAGE_DATA_WIDTH   0x01

//
// ConnectType
//
#define CONSOLE_OUT 0x00000001
#define STD_ERROR   0x00000002
#define CONSOLE_IN  0x00000004
#define CONSOLE_ALL (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)

//
// Load Option Attributes defined in EFI Specification
//
#define LOAD_OPTION_ACTIVE              0x00000001
#define LOAD_OPTION_FORCE_RECONNECT     0x00000002
#define IS_LOAD_OPTION_TYPE(_c, _Mask)  (BOOLEAN) (((_c) & (_Mask)) != 0)

//
// Define Maxmim characters that will be accepted
//
#define MAX_CHAR            480
#define MAX_CHAR_SIZE       (MAX_CHAR * 2)

#define MIN_ALIGNMENT_SIZE  4
#define ALIGN_SIZE(a)       ((a % MIN_ALIGNMENT_SIZE) ? MIN_ALIGNMENT_SIZE - (a % MIN_ALIGNMENT_SIZE) : 0)

//
// This data structure is the part of BDS_CONNECT_ENTRY that we can hard code.
//
#define BDS_LOAD_OPTION_SIGNATURE EFI_SIGNATURE_32 ('B', 'd', 'C', 'O')

typedef struct {

  UINTN                     Signature;
  EFI_LIST_ENTRY            Link;

  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  CHAR16                    *OptionName;
  UINTN                     OptionNumber;
  UINT16                    BootCurrent;
  UINT32                    Attribute;
  CHAR16                    *Description;
  VOID                      *LoadOptions;
  UINT32                    LoadOptionsSize;

} BDS_COMMON_OPTION;

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     ConnectType;
} BDS_CONSOLE_CONNECT_ENTRY;

//
// Lib Functions
//

//
// Bds boot relate lib functions
//
EFI_STATUS
BdsLibUpdateBootOrderList (
  IN  EFI_LIST_ENTRY                 *BdsOptionList,
  IN  CHAR16                         *VariableName
  );

VOID
BdsLibBootNext (
  VOID
  );

EFI_STATUS
BdsLibBootViaBootOption (
  IN  BDS_COMMON_OPTION             * Option,
  IN  EFI_DEVICE_PATH_PROTOCOL      * DevicePath,
  OUT UINTN                         *ExitDataSize,
  OUT CHAR16                        **ExitData OPTIONAL
  );

EFI_STATUS
BdsLibEnumerateAllBootOption (
  IN OUT EFI_LIST_ENTRY    *BdsBootOptionList
  );

VOID
BdsLibBuildOptionFromHandle (
  IN  EFI_HANDLE          Handle,
  IN  EFI_LIST_ENTRY      *BdsBootOptionList
  );

VOID
BdsLibBuildOptionFromShell (
  IN  EFI_HANDLE                     Handle,
  IN  EFI_LIST_ENTRY                 *BdsBootOptionList
  );

//
// Bds misc lib functions
//
UINT16
BdsLibGetTimeout (
  VOID
  );

EFI_STATUS
BdsLibGetBootMode (
  OUT EFI_BOOT_MODE       *BootMode
  );

VOID
BdsLibLoadDrivers (
  IN  EFI_LIST_ENTRY              *BdsDriverLists
  );

EFI_STATUS
BdsLibBuildOptionFromVar (
  IN  EFI_LIST_ENTRY              *BdsCommonOptionList,
  IN  CHAR16                      *VariableName
  );

VOID                      *
BdsLibGetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize
  );

EFI_STATUS
BdsLibOutputStrings (
  IN EFI_SIMPLE_TEXT_OUT_PROTOCOL   *ConOut,
  ...
  );

BDS_COMMON_OPTION         *
BdsLibVariableToOption (
  IN OUT EFI_LIST_ENTRY               *BdsCommonOptionList,
  IN CHAR16                           *VariableName
  );

EFI_STATUS
BdsLibRegisterNewOption (
  IN  EFI_LIST_ENTRY                 *BdsOptionList,
  IN  EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN  CHAR16                         *String,
  IN  CHAR16                         *VariableName
  );

//
// Bds connect or disconnect driver lib funcion
//
VOID
BdsLibConnectAllDriversToAllControllers (
  VOID
  );

VOID
BdsLibConnectAll (
  VOID
  );

EFI_STATUS
BdsLibConnectDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePathToConnect
  );

EFI_STATUS
BdsLibConnectAllEfi (
  VOID
  );

EFI_STATUS
BdsLibDisconnectAllEfi (
  VOID
  );

//
// Bds console relate lib functions
//
VOID
BdsLibConnectAllConsoles (
  VOID
  );

EFI_STATUS
BdsLibConnectAllDefaultConsoles (
  VOID
  );

EFI_STATUS
BdsLibUpdateConsoleVariable (
  IN  CHAR16                    *ConVarName,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  );

EFI_STATUS
BdsLibConnectConsoleVariable (
  IN  CHAR16                 *ConVarName
  );

//
// Bds device path relate lib functions
//
EFI_DEVICE_PATH_PROTOCOL  *
BdsLibUnpackDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevPath
  );

BOOLEAN
BdsLibMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  *Single
  );

CHAR16                    *
DevicePathToStr (
  EFI_DEVICE_PATH_PROTOCOL     *DevPath
  );

VOID                      *
EfiLibGetVariable (
  IN CHAR16               *Name,
  IN EFI_GUID             *VendorGuid
  );

//
// Internal definitions
//
typedef struct {
  CHAR16  *str;
  UINTN   len;
  UINTN   maxlen;
} POOL_PRINT;

typedef struct {
  UINT8 Type;
  UINT8 SubType;
  VOID (*Function) (POOL_PRINT *, VOID *);
} DEVICE_PATH_STRING_TABLE;

//
// Internal functions
//
EFI_STATUS
BdsBootByDiskSignatureAndPartition (
  IN  BDS_COMMON_OPTION          * Option,
  IN  HARDDRIVE_DEVICE_PATH      * HardDriveDevicePath,
  IN  UINT32                     LoadOptionsSize,
  IN  VOID                       *LoadOptions,
  OUT UINTN                      *ExitDataSize,
  OUT CHAR16                     **ExitData OPTIONAL
  );

//
// Notes: EFI 64 shadow all option rom
//
#ifdef EFI64
#define EFI64_SHADOW_ALL_LEGACY_ROM() ShadowAllOptionRom ();
VOID
ShadowAllOptionRom();
#else
#define EFI64_SHADOW_ALL_LEGACY_ROM()
#endif

//
// BBS support macros and functions
//

#if defined(EFI32) || defined(EFIX64)
#define REFRESH_LEGACY_BOOT_OPTIONS \
        BdsDeleteAllInvalidLegacyBootOptions ();\
        BdsAddNonExistingLegacyBootOptions (); \
        BdsUpdateLegacyDevOrder ()
#else
#define REFRESH_LEGACY_BOOT_OPTIONS
#endif

EFI_STATUS
BdsDeleteAllInvalidLegacyBootOptions (
  VOID
  );

EFI_STATUS
BdsAddNonExistingLegacyBootOptions (
  VOID
  );

EFI_STATUS
BdsUpdateLegacyDevOrder (
  VOID
  );

EFI_STATUS
BdsRefreshBbsTableForBoot (
  IN BDS_COMMON_OPTION        *Entry
  );

extern VOID
WriteBootToOsPerformanceData (
  VOID
  );

BOOLEAN
BdsLibIsValidEFIBootOptDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath,
  IN BOOLEAN                      CheckMedia
  );

EFI_STATUS
EFIAPI
BdsLibUpdateFvFileDevicePath (
  IN  OUT EFI_DEVICE_PATH_PROTOCOL      ** DevicePath,
  IN  EFI_GUID                          *FileGuid
  );

#endif // _BDS_LIB_H_
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************