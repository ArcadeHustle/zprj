/*++

Copyright (c) 2004 - 2009, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  PeiCore.h

Abstract:

  Definition of Pei Core Structures and Services

Revision History

--*/

#ifndef _PEICORE_H_
#define _PEICORE_H_

#include "EfiImage.h"
#include "Pei.h"
#include "PeiLib.h"
#include "PeiApi.h"
#include "PeiHob.h"
#include "PeiDebug.h"
#include "EfiFirmwareFileSystem.h"
#include "EfiFirmwareVolumeHeader.h"
#ifdef EFI64
#include "SalApi.h"
#endif

#include "EfiCommonLib.h"
#include EFI_GUID_DEFINITION (FirmwareFileSystem2)
#include EFI_GUID_DEFINITION (PeiApriori)
#include EFI_GUID_DEFINITION (PeiPeCoffLoader)
#include EFI_PPI_DEFINITION (Reset)
#include EFI_PPI_DEFINITION (DxeIpl)
#include EFI_PPI_DEFINITION (StatusCode)
#include EFI_PPI_DEFINITION (Security2)
#include EFI_PPI_DEFINITION (Decompress)
#include EFI_PPI_DEFINITION (FirmwareVolumeInfo)
#include EFI_PPI_DEFINITION (LoadFile2)
#include EFI_PPI_DEFINITION (FirmwareVolume)
#include EFI_PPI_DEFINITION (GuidedSectionExtraction)


#ifdef  EFI_NT_EMULATOR
#include EFI_PPI_DEFINITION (NtPeiLoadFile)
#endif

//*** AMI PORTING BEGIN ***//
#include <PPI/LoadedImagePpi.h>
//*** AMI PORTING END *****//  

//
//Build private HOB to PEI core to transfer old NEM-range data to new NEM-range
//
#define EFI_PEI_CORE_PRIVATE_GUID  \
  {0xd641a0f5, 0xcb7c, 0x4846, 0xa3, 0x80, 0x1d, 0x01, 0xb4, 0xd9, 0xe3, 0xb9}

#define PEI_CORE_INTERNAL_FFS_FILE_DISPATCH_TYPE   0xff

//
// Pei Core private data structures
//
typedef union {
  EFI_PEI_PPI_DESCRIPTOR      *Ppi;
  EFI_PEI_NOTIFY_DESCRIPTOR   *Notify;
  VOID                        *Raw;
} PEI_PPI_LIST_POINTERS;


#define PEI_STACK_SIZE 0x20000
#define MAX_PPI_DESCRIPTORS 128

typedef struct {
  INTN                    PpiListEnd;
  INTN                    NotifyListEnd;
  INTN                    DispatchListEnd;
  INTN                    LastDispatchedInstall;
  INTN                    LastDispatchedNotify;
  PEI_PPI_LIST_POINTERS   PpiListPtrs[MAX_PPI_DESCRIPTORS];
} PEI_PPI_DATABASE;


#define PEI_CORE_MAX_FV_SUPPORTED   8
#define PEI_CORE_MAX_PEIM_PER_FV    64

//
// PEI_CORE_FV_HANDE.PeimState
// Do not change these values as there is code doing math to change states.
// Look for Private->Fv[FvCount].PeimState[PeimCount]++;
//
#define PEIM_STATE_NOT_DISPATCHED         0x00
#define PEIM_STATE_DISPATCHED             0x01
#define PEIM_STATE_REGISITER_FOR_SHADOW   0x02
#define PEIM_STATE_DONE                   0x03

typedef struct {
  EFI_FIRMWARE_VOLUME_HEADER          *FvHeader;
  UINT8                               PeimState[PEI_CORE_MAX_PEIM_PER_FV];   
  EFI_PEI_FILE_HANDLE                 FvFileHandles[PEI_CORE_MAX_PEIM_PER_FV];
  BOOLEAN                             ScanFv;
} PEI_CORE_FV_HANDLE;

//
// Pei Core private data structure instance
//
#define PEI_CORE_HANDLE_SIGNATURE  EFI_SIGNATURE_32('P','e','i','C')

typedef struct{
  UINTN                              Signature;
  EFI_PEI_SERVICES                   *PS;    
  PEI_PPI_DATABASE                   PpiData;
  UINTN                              FvCount;
  PEI_CORE_FV_HANDLE                 Fv[PEI_CORE_MAX_FV_SUPPORTED];
  EFI_PEI_FILE_HANDLE                CurrentFvFileHandles[PEI_CORE_MAX_PEIM_PER_FV];
  UINTN                              AprioriCount;
  UINTN                              CurrentPeimFvCount; 
  UINTN                              CurrentPeimCount;
  EFI_PEI_FILE_HANDLE                CurrentFileHandle;
  BOOLEAN                            PeimNeedingDispatch;
  BOOLEAN                            PeimDispatchOnThisPass;
  BOOLEAN                            PeimDispatcherReenter;  
  UINTN                              AllFvCount;
  EFI_PEI_FV_HANDLE                  AllFv[PEI_CORE_MAX_FV_SUPPORTED];
  EFI_PEI_HOB_POINTERS               HobList;
  BOOLEAN                            SwitchStackSignal;
  BOOLEAN                            PeiMemoryInstalled;
  EFI_PHYSICAL_ADDRESS               StackBase;
  UINT64                             StackSize;
  VOID                               *BottomOfCarHeap;
  VOID                               *TopOfCarHeap;
  VOID                               *CpuIo;
  EFI_PEI_SECURITY2_PPI              *PrivateSecurityPpi;
  EFI_PEI_SERVICES                   ServiceTableShadow;
  UINTN                              SizeOfCacheAsRam;
  VOID                               *MaxTopOfCarHeap;
  EFI_PEI_PPI_DESCRIPTOR             *XipLoadFile;
  EFI_PEI_PE_COFF_LOADER_PROTOCOL    *PeCoffLoader; 
  EFI_PEI_PE_COFF_LOADER_READ_FILE   ImageReadFile;
//*** AMI PORTING BEGIN ***//
  EFI_PEI_PPI_DESCRIPTOR             *LoadedImage;
  EFI_PEI_LOADED_IMAGE_PPI           LoadedImagePpi;
//*** AMI PORTING END *****// 
} PEI_CORE_INSTANCE;


//
// Pei Core Instance Data Macros
//

#define PEI_CORE_INSTANCE_FROM_PS_THIS(a) \
  (PEI_CR(a, PEI_CORE_INSTANCE, PS, PEI_CORE_HANDLE_SIGNATURE))


typedef 
EFI_STATUS
(EFIAPI *PEI_CORE_ENTRY_POINT) (
  IN CONST EFI_SEC_PEI_HAND_OFF        *SecCoreData,
  IN CONST EFI_PEI_PPI_DESCRIPTOR      *PpList,
  IN       PEI_CORE_INSTANCE           *OldCoreData
  );


VOID
SwitchCoreStacks (
  IN VOID  *EntryPoint,
  IN UINTN Parameter1,
  IN UINTN Parameter2,
  IN UINTN Parameter3,
  IN VOID  *NewStack
  )
/*++

Routine Description:

  Routine for PEI switching stacks.

Arguments:

  EntryPoint - Entry point with new stack.
  Parameter1 - First parameter for entry point.
  Parameter2 - Second parameter for entry point.
  NewStack   - Pointer to new stack.
  
Returns:

  None
  
--*/
;
extern  EFI_PEI_PPI_DESCRIPTOR     gPpiLoadFilePpiList;


typedef
VOID
(EFIAPI *SWITCH_STACK_ENTRY_POINT) (
  IN      VOID                      *Context1,  OPTIONAL
  IN      VOID                      *Context2   OPTIONAL
  );



//
// Main PEI entry
//
EFI_STATUS
EFIAPI
PeiMain (
  IN CONST EFI_SEC_PEI_HAND_OFF   *SecCoreData,
  IN CONST EFI_PEI_PPI_DESCRIPTOR *PpList
  )
/*++

Routine Description:

  Main entry point to Pei Core.

Arguments:

  SecCoreData   - Information and services provided by SEC phase.
  PpList        - Pointer to Ppi list provided by SEC phase.

Returns:

  This function never returns

--*/
;


EFI_STATUS
EFIAPI
PeiCore (
  IN CONST EFI_SEC_PEI_HAND_OFF        *SecCoreData,
  IN CONST EFI_PEI_PPI_DESCRIPTOR      *PpList,
  IN       PEI_CORE_INSTANCE           *OldCoreData
  )
/*++

Routine Description:

  The entry routine to Pei Core, invoked by PeiMain during transition
  from SEC to PEI. After switching stack in the PEI core, it will restart
  with the old core data.

Arguments:

  SecCoreData        - Information and services provided by SEC phase.
  PpList             - Pointer to Ppi list provided by SEC phase.
  OldCoreData        - Pointer to old core data that is used to initialize the
                         core's data areas.

Returns:

  This function never returns

--*/
;


//
// Dispatcher support functions
//

BOOLEAN
PeimDispatchReadiness (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN VOID               *DependencyExpression
  )
/*++

Routine Description:

  This is the POSTFIX version of the dependency evaluator.  When a
  PUSH [PPI GUID] is encountered, a pointer to the GUID is stored on
  the evaluation stack.  When that entry is poped from the evaluation
  stack, the PPI is checked if it is installed.  This method allows
  some time savings as not all PPIs must be checked for certain
  operation types (AND, OR).

Arguments:

  PeiServices               - Calling context.

  DependencyExpression      - Pointer to a dependency expression.  The Grammar adheres to 
                              the BNF described above and is stored in postfix notation.
  Runnable                  - is True if the driver can be scheduled and False if the driver 
                              cannot be scheduled.  This is the value that the schedulers 
                              should use for deciding the state of the driver.

Returns:

  Status = EFI_SUCCESS            if it is a well-formed Grammar
           EFI_INVALID_PARAMETER  if the dependency expression overflows
                                  the evaluation stack
           EFI_INVALID_PARAMETER  if the dependency expression underflows
                                  the evaluation stack
           EFI_INVALID_PARAMETER  if the dependency expression is not a
                                  well-formed Grammar.
--*/
;


VOID
PeiDispatcher (
  IN CONST EFI_SEC_PEI_HAND_OFF        *SecCoreData,
  IN PEI_CORE_INSTANCE                 *Private
  )

/*++

Routine Description:

  Conduct PEIM dispatch.

Arguments:

  SecCoreData          - Information and services provided by SEC phase.
  Private              - Pointer to the private data passed in from caller

Returns:

  EFI_SUCCESS   - Successfully dispatched PEIM.
  EFI_NOT_FOUND - The dispatch failed.
            
--*/
;


VOID
InitializeDispatcherData (
  IN PEI_CORE_INSTANCE                *PrivateData,
  IN PEI_CORE_INSTANCE                *OldCoreData,
  IN CONST EFI_SEC_PEI_HAND_OFF       *SecCoreData
  )
/*++

Routine Description:

  Initialize the Dispatcher's data members

Arguments:

  PeiServices          - The PEI core services table.
  OldCoreData          - Pointer to old core data (before switching stack).
                         NULL if being run in non-permament memory mode.
  SecCoreData          - Information and services provided by SEC phase.


Returns:

  None

--*/
;


BOOLEAN
DepexSatisfied (
  IN PEI_CORE_INSTANCE          *Private,
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN UINTN                      PeimCount
  )
/*++

Routine Description:

  This routine parses the Dependency Expression, if available, and
  decides if the module can be executed.

Arguments:

  Private        - Pointer to the private data passed in from caller.
  FileHandle     - File handle of PEIM.   
  PeimCount      - The PEIM sequence in one FV.

Returns:
  TRUE  - Can be dispatched
  FALSE - Cannot be dispatched

--*/
;

#ifdef EFI64
  //
  // In Ipf we should make special changes for the PHIT pointers to support
  // recovery boot in cache mode.
  //
#define  SWITCH_TO_CACHE_MODE(CoreData)  SwitchToCacheMode(CoreData)
#define  CACHE_MODE_ADDRESS_MASK         0x7FFFFFFFFFFFFFFFULL
VOID
SwitchToCacheMode (
  IN PEI_CORE_INSTANCE           *CoreData
)
/*++

Routine Description:

 Switch the PHIT pointers to cache mode after InstallPeiMemory in CAR.

Arguments:

  CoreData   - The PEI core Private Data
  
Returns:
        
--*/
;

#else

#define  SWITCH_TO_CACHE_MODE(CoreData)

#endif

//
// PPI support functions
//
VOID
InitializePpiServices (
  IN PEI_CORE_INSTANCE   *PrivateData,
  IN PEI_CORE_INSTANCE   *OldCoreData
  )
/*++

Routine Description:

  Initialize PPI services.

Arguments:

  PeiServices - The PEI core services table.
  OldCoreData - Pointer to the PEI Core data.
                NULL if being run in non-permament memory mode.

Returns:
  Nothing

--*/
;

VOID
ConvertPpiPointers (
  IN CONST EFI_PEI_SERVICES        **PeiServices,
  IN EFI_HOB_HANDOFF_INFO_TABLE    *OldHandOffHob,
  IN EFI_HOB_HANDOFF_INFO_TABLE    *NewHandOffHob
  )
/*++

Routine Description:

  Migrate the Hob list from the CAR stack to PEI installed memory.

Arguments:

  PeiServices   - The PEI core services table.
  OldHandOffHob - The old handoff HOB list.
  NewHandOffHob - The new handoff HOB list.

Returns:
            
--*/
;

EFI_STATUS
EFIAPI
PeiInstallPpi (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN CONST EFI_PEI_PPI_DESCRIPTOR      * PpiList
  )
/*++

Routine Description:

  Install PPI services.

Arguments:

  PeiServices - Pointer to the PEI Service Table
  PpiList     - Pointer to a list of PEI PPI Descriptors.

Returns:

    EFI_SUCCESS             - if all PPIs in PpiList are successfully installed.
    EFI_INVALID_PARAMETER   - if PpiList is NULL pointer
    EFI_INVALID_PARAMETER   - if any PPI in PpiList is not valid
    EFI_OUT_OF_RESOURCES    - if there is no more memory resource to install PPI

--*/
;

EFI_STATUS
EFIAPI
PeiReInstallPpi (
  IN CONST EFI_PEI_SERVICES                **PeiServices,
  IN CONST EFI_PEI_PPI_DESCRIPTOR          * OldPpi,
  IN CONST EFI_PEI_PPI_DESCRIPTOR          * NewPpi
  )
/*++

Routine Description:

  Re-Install PPI services.

Arguments:

  PeiServices - Pointer to the PEI Service Table
  OldPpi      - Pointer to the old PEI PPI Descriptors.
  NewPpi      - Pointer to the new PEI PPI Descriptors.

Returns:

  EFI_SUCCESS           - if the operation was successful
  EFI_INVALID_PARAMETER - if OldPpi or NewPpi is NULL
  EFI_INVALID_PARAMETER - if NewPpi is not valid
  EFI_NOT_FOUND         - if the PPI was not in the database

--*/
;

EFI_STATUS
EFIAPI
PeiLocatePpi (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN CONST EFI_GUID                    * Guid,
  IN UINTN                             Instance,
  IN OUT EFI_PEI_PPI_DESCRIPTOR        **PpiDescriptor OPTIONAL,
  IN OUT VOID                          **Ppi
  )
/*++

Routine Description:

  Locate a given named PPI.

Arguments:

  PeiServices   - Pointer to the PEI Service Table
  Guid          - Pointer to GUID of the PPI.
  Instance      - Instance Number to discover.
  PpiDescriptor - Pointer to reference the found descriptor. If not NULL,
                returns a pointer to the descriptor (includes flags, etc)
  Ppi           - Pointer to reference the found PPI

Returns:

  Status -  EFI_SUCCESS   if the PPI is in the database           
            EFI_NOT_FOUND if the PPI is not in the database
--*/
;


VOID *
PeiReturnPpi (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN CONST EFI_GUID               *Guid
  );

EFI_STATUS
EFIAPI
PeiNotifyPpi (
  IN CONST EFI_PEI_SERVICES                **PeiServices,
  IN CONST EFI_PEI_NOTIFY_DESCRIPTOR       * NotifyList
  )
/*++

Routine Description:

  Install a notification for a given PPI.

Arguments:

  PeiServices - Pointer to the PEI Service Table
  NotifyList  - Pointer to list of Descriptors to notify upon.

Returns:

  Status - EFI_SUCCESS          if successful
           EFI_OUT_OF_RESOURCES if no space in the database
           EFI_INVALID_PARAMETER if not a good decriptor

--*/
;

VOID
ProcessNotifyList (
  IN EFI_PEI_SERVICES    **PeiServices
  )
/*++

Routine Description:

  Process the Notify List at dispatch level.

Arguments:

  PeiServices - Pointer to the PEI Service Table

Returns:

--*/
;

VOID
DispatchNotify (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN UINTN                     NotifyType,
  IN INTN                      InstallStartIndex,
  IN INTN                      InstallStopIndex,
  IN INTN                      NotifyStartIndex,
  IN INTN                      NotifyStopIndex
  )
/*++

Routine Description:

  Dispatch notifications.

Arguments:

  PeiServices         - Pointer to the PEI Service Table
  NotifyType          - Type of notify to fire.
  InstallStartIndex   - Install Beginning index.
  InstallStopIndex    - Install Ending index.
  NotifyStartIndex    - Notify Beginning index.
  NotifyStopIndex    - Notify Ending index.

Returns:  None

--*/
;

//
// Boot mode support functions
//
EFI_STATUS
EFIAPI
PeiGetBootMode (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  OUT EFI_BOOT_MODE                    *BootMode
  )
/*++

Routine Description:

  This service enables PEIMs to ascertain the present value of the boot mode.  

Arguments:

  PeiServices    - The PEI core services table.
  BootMode       - A pointer to contain the value of the boot mode. 

Returns:

  EFI_SUCCESS           - The boot mode was returned successfully.
  EFI_INVALID_PARAMETER - BootMode is NULL.

--*/
;

EFI_STATUS
EFIAPI
PeiSetBootMode (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN EFI_BOOT_MODE                     BootMode
  )
/*++

Routine Description:

  This service enables PEIMs to update the boot mode variable.    

Arguments:

  PeiServices    - The PEI core services table.
  BootMode       - The value of the boot mode to set.

Returns:

  EFI_SUCCESS    - The value was successfully updated

--*/
;

//
// Security support functions
//
VOID
InitializeSecurityServices (
  IN PEI_CORE_INSTANCE  *PrivateData,
  IN PEI_CORE_INSTANCE  *OldCoreData
  )
/*++

Routine Description:

  Initialize the security services.

Arguments:

  PeiServices - The PEI core services table.
  OldCoreData - Pointer to the old core data.
                NULL if being run in non-permament memory mode.
Returns:

  None

--*/
;

EFI_STATUS
VerifyFv (
  IN EFI_PEI_FV_HANDLE    VolumeHandle
  )
/*++

Routine Description:

  Provide a callout to the OEM FV verification service.

Arguments:

  VolumeHandle       - Pointer to the FV under investigation.

Returns:

  Status - EFI_SUCCESS

--*/
;


EFI_STATUS
VerifyPeim (
  IN PEI_CORE_INSTANCE      *PrivateData,
  IN EFI_PEI_FV_HANDLE      VolumeHandle,
  IN EFI_PEI_FILE_HANDLE    FileHandle
  )
/*++

Routine Description:

  Provide a callout to the security verification service.

Arguments:

  PeiServices          - The PEI core services table.
  CurrentPeimAddress   - Pointer to the Firmware File under investigation.

Returns:

  EFI_SUCCESS             - Image is OK
  EFI_SECURITY_VIOLATION  - Image is illegal

--*/
;


EFI_STATUS
EFIAPI
PeiGetHobList (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN OUT VOID                          **HobList
  )
/*++

Routine Description:

  Gets the pointer to the HOB List.

Arguments:

  PeiServices - The PEI core services table.
  HobList     - Pointer to the HOB List.

Returns:

  EFI_SUCCESS                 - Get the pointer of HOB List
  EFI_NOT_AVAILABLE_YET       - the HOB List is not yet published
  EFI_INVALID_PARAMETER       - HobList is NULL (in debug mode)
            
--*/
;

EFI_STATUS
EFIAPI
PeiCreateHob (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN UINT16                            Type,
  IN UINT16                            Length,
  IN OUT VOID                          **Hob
  )
/*++

Routine Description:

  Add a new HOB to the HOB List.

Arguments:

  PeiServices - The PEI core services table.
  Type        - Type of the new HOB.
  Length      - Length of the new HOB to allocate.
  Hob         - Pointer to the new HOB.

Returns:

  Status  - EFI_SUCCESS
          - EFI_INVALID_PARAMETER if Hob is NULL
          - EFI_NOT_AVAILABLE_YET if HobList is still not available.
          - EFI_OUT_OF_RESOURCES if there is no more memory to grow the Hoblist.
            
--*/
;

EFI_STATUS
PeiCoreBuildHobHandoffInfoTable (
  IN EFI_BOOT_MODE         BootMode,
  IN EFI_PHYSICAL_ADDRESS  MemoryBegin,
  IN UINT64                MemoryLength
  )
/*++

Routine Description:

  Builds a Handoff Information Table HOB

Arguments:

  BootMode      - Current Bootmode
  MemoryBegin   - Start Memory Address.
  MemoryLength  - Length of Memory.

Returns:

  EFI_SUCCESS

--*/
;


//
// FFS Fw Volume support functions
//

EFI_STATUS
EFIAPI 
PeiFfsFindNextVolume (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN UINTN                    Instance,
  IN OUT EFI_PEI_FV_HANDLE    *VolumeHandle
  )
/*++

Routine Description:

  Search the next FV Volume.

Arguments:
  PeiServices  - Pointer to the PEI Core Services Table.
  Instance     - The Fv Volume Instance.
  VolumeHandle - Pointer to the current Fv Volume to search.

Returns:
  EFI_INVALID_PARAMETER  - VolumeHandle is NULL.
  EFI_NOT_FOUND          - No FV Volume is found.
  EFI_SUCCESS            - The next FV Volume is found.
  
--*/  
;

EFI_STATUS
EFIAPI 
PeiFfsFindNextFile (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_FV_FILETYPE                SearchType,
  IN CONST EFI_PEI_FV_HANDLE        FvHandle,
  IN OUT EFI_PEI_FILE_HANDLE        *FileHandle
  )
/*++

Routine Description:

  Given the input FvHandle, search for the next matching type file in the FV Volume.

Arguments:
  PeiServices  - Pointer to the PEI Core Services Table.
  SearchType   - Filter to find only file of this type.
  FvHandle     - Pointer to the current FV to search.  
  FileHandle   - Pointer to the file matching SearchType in FwVolHeader.
                - NULL if file not found
Returns:
  EFI_STATUS
  
--*/
;

EFI_STATUS
EFIAPI 
PeiFfsFindSectionData (
  IN CONST  EFI_PEI_SERVICES    **PeiServices,
  IN EFI_SECTION_TYPE           SectionType,
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  OUT VOID                      **SectionData
  )
/*++

Routine Description:
    Given the input file pointer, search for the next matching section in the
    FFS volume.

Arguments:
    PeiServices - Pointer to the PEI Core Services Table.
    SearchType  - Filter to find only sections of this type.
    FileHandle  - Pointer to the current file to search.
    SectionData - Pointer to the Section matching SectionType in FfsFileHeader.
                - NULL if section not found

Returns:
    EFI_NOT_FOUND - No files matching the search criteria were found
    EFI_SUCCESS

--*/
;

//
// Memory support functions
//
VOID
InitializeMemoryServices (
  IN PEI_CORE_INSTANCE           *PrivateData,
  IN CONST EFI_SEC_PEI_HAND_OFF  *SecCoreData,
  IN PEI_CORE_INSTANCE           *OldCoreData
  )
/*++

Routine Description:

  Initialize the memory services.

Arguments:

  PeiServices          - The PEI core services table.
  PeiStartupDescriptor - Information and services provided by SEC phase.
  OldCoreData          - Pointer to the PEI Core data.
                         NULL if being run in non-permament memory mode.

Returns:

  None

--*/
;

EFI_STATUS
EFIAPI
PeiInstallPeiMemory (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PHYSICAL_ADDRESS       MemoryBegin,
  IN UINT64                     MemoryLength
  )
/*++

Routine Description:

  Install the permanent memory is now available.
  Creates HOB (PHIT and Stack).

Arguments:

  PeiServices   - The PEI core services table.
  MemoryBegin   - Start of memory address.
  MemoryLength  - Length of memory.

Returns:

  Status  - EFI_SUCCESS
            
--*/
;

EFI_STATUS
EFIAPI
PeiAllocatePages (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_MEMORY_TYPE            MemoryType,
  IN UINTN                      Pages,
  IN OUT EFI_PHYSICAL_ADDRESS   * Memory
  )
/*++

Routine Description:

  Memory allocation service on permanent memory, 
  not usable prior to the memory installation.

Arguments:

  PeiServices - The PEI core services table.
  Type        - Type of allocation.
  MemoryType  - Type of memory to allocate.
  Pages       - Number of pages to allocate.
  Memory      - Pointer of memory allocated.

Returns:

  Status - EFI_SUCCESS              The allocation was successful
           EFI_INVALID_PARAMETER    Only AllocateAnyAddress is supported.
           EFI_NOT_AVAILABLE_YET    Called with permanent memory not available
           EFI_OUT_OF_RESOURCES     There is not enough HOB heap to satisfy the requirement
                                    to allocate the number of pages.

--*/
;

EFI_STATUS
EFIAPI
PeiAllocatePool (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN UINTN                           Size,
  OUT VOID                           **Buffer
  )
/*++

Routine Description:

  Memory allocation service on the CAR.  

Arguments:

  PeiServices - The PEI core services table.

  Size        - Amount of memory required

  Buffer      - Address of pointer to the buffer

Returns:

  Status - EFI_SUCCESS              The allocation was successful
           EFI_OUT_OF_RESOURCES     There is not enough heap to satisfy the requirement
                                    to allocate the requested size.
                                    
--*/
;

VOID
EFIAPI
PeiCoreCopyMem (
  IN VOID                       *Destination,
  IN VOID                       *Source,
  IN UINTN                      Length
  )
/*++

Routine Description:

  Copy Length bytes from Source to Destination.

Arguments:

  Destination - Target of copy

  Source      - Place to copy from

  Length      - Number of bytes to copy

Returns:

  None

--*/
;

VOID
EFIAPI
PeiCoreSetMem (
  IN VOID                       *Buffer,
  IN UINTN                      Size,
  IN UINT8                      Value
  )
/*++

Routine Description:

  Set Size bytes at Buffer address with Value

Arguments:

  Buffer      - Target of set 

  Size        - Amount of memory to set

  Value       - Value to place in memory

Returns:

  None

--*/
;

EFI_STATUS
PeiLoadImage (
  IN     EFI_PEI_SERVICES             **PeiServices,
  IN     EFI_PEI_FILE_HANDLE          FileHandle,
  OUT    EFI_PHYSICAL_ADDRESS         *EntryPoint,
  OUT    UINT32                       *AuthenticationState
  )
/*++

Routine Description:

  Routine for load image file.

Arguments:

  PeiServices          - The PEI core services table.
  FileHandle           - Pointer to the FFS file header of the image.
  EntryPoint           - Pointer to entry point of specified image file for output.
  AuthenticationState  - Pointer to attestation authentication state of image.

Returns:

  Status - EFI_SUCCESS    - Image is successfully loaded.
           EFI_NOT_FOUND  - Fail to locate necessary PPI
           Others         - Fail to load file.
  
--*/    
;

EFI_STATUS
EFIAPI
PeiReportStatusCode (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_STATUS_CODE_TYPE           Type,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       * CallerId OPTIONAL,
  IN EFI_STATUS_CODE_DATA           * Data OPTIONAL
  )
/*++

Routine Description:

  Core version of the Status Code reporter

Arguments:

  PeiServices - The PEI core services table.
  
  CodeType    - Type of Status Code.
  
  Value       - Value to output for Status Code.
  
  Instance    - Instance Number of this status code.
  
  CallerId    - ID of the caller of this status code.
  
  Data        - Optional data associated with this status code.

Returns:

  Status  - EFI_SUCCESS             if status code is successfully reported
          - EFI_NOT_AVAILABLE_YET   if StatusCodePpi has not been installed

--*/
;


EFI_STATUS
EFIAPI
PeiResetSystem (
  IN  EFI_PEI_SERVICES   **PeiServices
  )
/*++

Routine Description:

  Core version of the Reset System

Arguments:

  PeiServices - The PEI core services table.

Returns:

  Status  - EFI_NOT_AVAILABLE_YET. PPI not available yet.
          - EFI_DEVICE_ERROR.   Did not reset system.
          
  Otherwise, resets the system. 

--*/
;


VOID 
PeiInitializeFv (
  IN  PEI_CORE_INSTANCE           *PrivateData,
  IN CONST EFI_SEC_PEI_HAND_OFF   *SecCoreData
  )
/*++

Routine Description:

  Initialize PeiCore Fv List.

Arguments:
  PrivateData     - Pointer to PEI_CORE_INSTANCE.
  SecCoreData     - Pointer to EFI_SEC_PEI_HAND_OFF.

Returns:
  NONE  
  
--*/  
;


EFI_STATUS
EFIAPI
FirmwareVolmeInfoPpiNotifyCallback (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  )
/*++

Routine Description:

  Process Firmware Volum Information once FvInfoPPI install.

Arguments:

  PeiServices - General purpose services available to every PEIM.
    
Returns:

  Status -  EFI_SUCCESS if the interface could be successfully
            installed

--*/
;


EFI_STATUS
EFIAPI 
PeiFfsFindFileByName (
  IN  CONST EFI_GUID        *FileName,
  IN  EFI_PEI_FV_HANDLE     VolumeHandle,
  OUT EFI_PEI_FILE_HANDLE   *FileHandle
  )
/*++

Routine Description:

  Given the input VolumeHandle, search for the next matching name file.

Arguments:

  FileName      - File name to search.
  VolumeHandle  - The current FV to search.
  FileHandle    - Pointer to the file matching name in VolumeHandle.
                - NULL if file not found
Returns:
  EFI_STATUS
  
--*/  
;


EFI_STATUS
EFIAPI 
PeiFfsGetFileInfo (
  IN EFI_PEI_FILE_HANDLE  FileHandle,
  OUT EFI_FV_FILE_INFO    *FileInfo
  )
/*++

Routine Description:

  Collect information of given file.

Arguments:
  FileHandle   - The handle to file.
  FileInfo     - Pointer to the file information.

Returns:
  EFI_STATUS
  
--*/    
;

EFI_STATUS
EFIAPI 
PeiFfsGetVolumeInfo (
  IN EFI_PEI_FV_HANDLE  VolumeHandle,
  OUT EFI_FV_INFO       *VolumeInfo
  )
/*++

Routine Description:

  Collect information of given Fv Volume.

Arguments:
  VolumeHandle    - The handle to Fv Volume.
  VolumeInfo      - The pointer to volume information.
  
Returns:
  EFI_STATUS
  
--*/    
;


EFI_STATUS
EFIAPI
PeiRegisterForShadow (
  IN EFI_PEI_FILE_HANDLE       FileHandle
  )
/*++

Routine Description:

  This routine enable a PEIM to register itself to shadow when PEI Foundation
  discovery permanent memory.

Arguments:
  FileHandle  - File handle of a PEIM.
  
Returns:
  EFI_NOT_FOUND        - The file handle doesn't point to PEIM itself.
  EFI_ALREADY_STARTED  - Indicate that the PEIM has been registered itself.
  EFI_SUCCESS          - Successfully to register itself.

--*/  
;


EFI_STATUS
PeiFindFileEx (
  IN  CONST EFI_PEI_FV_HANDLE        FvHandle,
  IN  CONST EFI_GUID                 *FileName,   OPTIONAL
  IN        EFI_FV_FILETYPE          SearchType,
  IN OUT    EFI_PEI_FILE_HANDLE      *FileHandle,
  IN OUT    EFI_PEI_FV_HANDLE        *AprioriFile  OPTIONAL
  )
/*++

Routine Description:
    Given the input file pointer, search for the next matching file in the
    FFS volume as defined by SearchType. The search starts from FileHeader inside
    the Firmware Volume defined by FwVolHeader.

Arguments:
    PeiServices - Pointer to the PEI Core Services Table.
    SearchType - Filter to find only files of this type.
      Type EFI_FV_FILETYPE_ALL causes no filtering to be done.
    FwVolHeader - Pointer to the FV header of the volume to search.
      This parameter must point to a valid FFS volume.
    FileHeader  - Pointer to the current file from which to begin searching.
      This pointer will be updated upon return to reflect the file found.
    Flag        - Indicator for if this is for PEI Dispath search 
    
Returns:
    EFI_NOT_FOUND - No files matching the search criteria were found
    EFI_SUCCESS

--*/
;

VOID
InitializeImageServices (
  IN  PEI_CORE_INSTANCE   *PrivateData,
  IN  PEI_CORE_INSTANCE   *OldCoreData
  )
/*++

Routine Description:

  Regitser PeCoffLoader to PeiCore PrivateData. And install
  Pei Load File PPI.

Arguments:

  PrivateData    - Pointer to PEI_CORE_INSTANCE.
  OldCoreData    - Pointer to PEI_CORE_INSTANCE.

Returns:

  NONE.
  
--*/      
;

//*** AMI PORTING BEGIN ***//
//This function is used to update LoadedImage PPI information
//LoadedImage PPI is defined in PI 1.0 specification
VOID UpdatedLoadedImagePpi(
  IN EFI_PEI_SERVICES         **PeiServices,
  EFI_PHYSICAL_ADDRESS        ImageAddress,
  UINT64                      ImageSize,
  EFI_PEI_FILE_HANDLE         FileHandle
);

#include <AmiPeiLib.h>
//*** AMI PORTING END *****//    
#endif
