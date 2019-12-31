! $Id$
!
! Earth System Modeling Framework
! Copyright 2002-2018, University Corporation for Atmospheric Research,
! Massachusetts Institute of Technology, Geophysical Fluid Dynamics
! Laboratory, University of Michigan, National Centers for Environmental
! Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
! NASA Goddard Space Flight Center.
! Licensed under the University of Illinois-NCSA License.
!
!==============================================================================
#define ESMF_FILENAME "src/Infrastructure/Base/interface/ESMF_Info.F90"
!==============================================================================

module ESMF_InfoMod

!==============================================================================
!
! This file contains the Fortran wrapper code for the C++ implementation of
!  the Info class.
!
!------------------------------------------------------------------------------

! INCLUDES
#include "ESMF.h"

!==============================================================================
!BOPI
! !MODULE: ESMF_InfoMod
!

!   Fortran API wrapper of C++ implemenation of Info
!
!------------------------------------------------------------------------------

! !USES:
use ESMF_UtilTypesMod     ! ESMF utility types
use ESMF_InitMacrosMod    ! ESMF initializer macros
use ESMF_BaseMod          ! ESMF base class
use ESMF_LogErrMod        ! ESMF error handling
use ESMF_VMMod
use iso_c_binding

implicit none

!private
!public :: ESMF_Info

!tdk:todo: need to specify public interfaces

include "ESMF_InfoCDef.F90"
include "ESMF_InfoCDefGeneric.F90"

type ESMF_Info
  type(C_PTR) :: ptr = C_NULL_PTR
end type ESMF_Info

interface ESMF_InfoCreate
  module procedure ESMF_InfoCreateEmpty
  module procedure ESMF_InfoCreateByKey
  module procedure ESMF_InfoCreateByParse
end interface ESMF_InfoCreate

!tdk:todo: rename GetArray* to GetList*
interface ESMF_InfoGet
  module procedure ESMF_InfoGetI4
  module procedure ESMF_InfoGetI8
  module procedure ESMF_InfoGetR4
  module procedure ESMF_InfoGetR8
  module procedure ESMF_InfoGetLG
  module procedure ESMF_InfoGetCH
  module procedure ESMF_InfoGetArrayI4
  module procedure ESMF_InfoGetArrayI8
  module procedure ESMF_InfoGetArrayR4
  module procedure ESMF_InfoGetArrayR8
  module procedure ESMF_InfoGetArrayLG
  module procedure ESMF_InfoGetArrayCH
end interface ESMF_InfoGet

interface ESMF_InfoGetListAllocated
  module procedure ESMF_InfoGetArrayI4Allocated
  module procedure ESMF_InfoGetArrayI8Allocated
  module procedure ESMF_InfoGetArrayR4Allocated
  module procedure ESMF_InfoGetArrayR8Allocated
  module procedure ESMF_InfoGetArrayLGAllocated
  module procedure ESMF_InfoGetArrayCHAllocated
end interface ESMF_InfoGetListAllocated

interface ESMF_InfoSet
  module procedure ESMF_InfoSetI4
  module procedure ESMF_InfoSetI8
  module procedure ESMF_InfoSetR4
  module procedure ESMF_InfoSetR8
  module procedure ESMF_InfoSetLG
  module procedure ESMF_InfoSetCH
  module procedure ESMF_InfoSetINFO
  module procedure ESMF_InfoSetArrayI4
  module procedure ESMF_InfoSetArrayI8
  module procedure ESMF_InfoSetArrayR4
  module procedure ESMF_InfoSetArrayR8
  module procedure ESMF_InfoSetArrayLG
  module procedure ESMF_InfoSetArrayCH
end interface ESMF_InfoSet

interface operator(==)
  procedure ESMF_InfoEqual
end interface operator(==)
interface operator(/=)
  procedure ESMF_InfoNotEqual
end interface operator(/=)

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
character(*), parameter, private :: version = '$Id$'
!------------------------------------------------------------------------------

contains  !====================================================================

#include "ESMF_InfoGeneric.F90"

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoBaseGet()"
function ESMF_InfoBaseGet(base) result(info)
  type(ESMF_Base), intent(in) :: base
  type(ESMF_Info) :: info
  info%ptr = c_info_base_get(base%this%ptr)
end function ESMF_InfoBaseGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoPointerGet()"
function ESMF_InfoPointerGet(ptr) result(info)
  type(ESMF_Pointer), intent(in) :: ptr
  type(ESMF_Info) :: info
  info%ptr = c_info_base_get(ptr%ptr)
end function ESMF_InfoPointerGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoCopy()"
function ESMF_InfoCopy(info, rc) result(info_copy)
  type(ESMF_Info), intent(in) :: info
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info_copy

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info_copy%ptr = c_info_copy(info%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoCopy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoCreateEmpty()"
function ESMF_InfoCreateEmpty(rc) result(info)
  integer, intent(inout), optional :: rc
  integer :: localrc
  type(ESMF_Info) :: info

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create(localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoCreateEmpty

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoCreateByKey()"
function ESMF_InfoCreateByKey(srcInfo, key, rc) result(info)
  type(ESMF_Info), intent(in) :: srcInfo
  character(len=*), intent(in) :: key
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create_by_key(srcInfo%ptr, trim(key)//C_NULL_CHAR, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoCreateByKey

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoCreateByParse()"
function ESMF_InfoCreateByParse(payload, rc) result(info)
  character(len=*), intent(in) :: payload
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create_by_parse(trim(payload)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoCreateByParse

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoBroadcast()"
subroutine ESMF_InfoBroadcast(info, rootPet, rc)
  type(ESMF_Info), intent(inout) :: info
  integer, intent(in) :: rootPet
  integer, intent(inout), optional :: rc
  integer :: localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_FAILURE

  call c_info_broadcast(info%ptr, rootPet, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoBroadcast

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoDestroy()"
subroutine ESMF_InfoDestroy(info, rc)
  type(ESMF_Info), intent(inout) :: info
  integer, intent(inout), optional :: rc
  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_destroy(info%ptr, localrc)
  info%ptr = C_NULL_PTR
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoDestroy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoDump()"
function ESMF_InfoDump(info, key, rc) result(output)
  type(ESMF_Info), intent(in) :: info
  character(*), intent(in), optional :: key
  integer, intent(inout), optional :: rc
  character(:), allocatable :: output

  character(:), allocatable :: l_key
  integer :: dump_length, localrc

  localrc = ESMF_RC_NOT_IMPL
  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  if (present(key)) then
    l_key = key
  else
    l_key = ""
  endif

  call c_info_dump_len(info%ptr, dump_length, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT)) return

  allocate(character(dump_length)::output)

  call c_info_dump(info%ptr, output, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoDump

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoEqual()"
function ESMF_InfoEqual(lhs, rhs) result(is_equal)
  type(ESMF_Info), intent(in) :: lhs
  type(ESMF_Info), intent(in) :: rhs
  logical :: is_equal

  integer :: localrc
  logical(C_BOOL) :: local_is_equal

  is_equal = .false.
  call c_info_is_equal(lhs%ptr, rhs%ptr, local_is_equal, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT)) return

  is_equal = local_is_equal
end function ESMF_InfoEqual

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoNotEqual()"
function ESMF_InfoNotEqual(lhs, rhs) result(is_equal)
  type(ESMF_Info), intent(in) :: lhs
  type(ESMF_Info), intent(in) :: rhs
  logical :: is_equal
  is_equal = .not. ESMF_InfoEqual(lhs, rhs)
end function ESMF_InfoNotEqual

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoRemove()"
subroutine ESMF_InfoRemove(info, keyParent, keyChild, attnestflag, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: keyParent
  character(len=*), intent(in), optional :: keyChild
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  character(:), allocatable :: localkeyChild
  logical(C_BOOL) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = .false.

  if (present(keyChild)) then
    localkeyChild = keyChild
  else
    localkeyChild = ""
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = .true.
  end if

  call c_info_erase(info%ptr, trim(keyParent)//C_NULL_CHAR, &
                     trim(localkeyChild)//C_NULL_CHAR, recursive, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoRemove

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoInquire()"
subroutine ESMF_InfoInquire(info, key, size, attrCount, attrCountTotal, jsonType, &
  isArray, isDirty, attPackCount, attPackCountTotal, attnestflag, idx, typekind, &
  ikey, isPresent, isStructured, isNull, attrCompliance, rc)

  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in), optional :: key
  integer(C_INT), intent(out), optional :: size
  integer(C_INT), intent(out), optional :: attrCount
  integer(C_INT), intent(out), optional :: attrCountTotal
  character(len=*), intent(out), optional :: jsonType
  logical, intent(out), optional :: isArray
  logical, intent(out), optional :: isDirty
  integer(C_INT), intent(out), optional :: attPackCount
  integer(C_INT), intent(out), optional :: attPackCountTotal
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer(C_INT), intent(in), optional :: idx
  type(ESMF_TypeKind_Flag), intent(out), optional :: typekind
  character(len=*), intent(out), optional :: ikey
  logical, intent(out), optional :: isPresent
  logical, intent(out), optional :: isStructured
  logical, intent(out), optional :: isNull
  logical, intent(in), optional :: attrCompliance
  integer, intent(inout), optional :: rc

  integer :: localrc, esmc_typekind, local_size
  type(ESMF_Info) :: inq, inq2
  character(:), allocatable :: local_key
  integer :: recursive, local_attrCompliance
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  if (present(rc)) rc = ESMF_FAILURE
  localrc = ESMF_FAILURE
  recursive = 0 !false
  local_attrCompliance = 0 !false

  if (present(key)) then
    local_key = trim(key)
  else
    local_key = ""
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attrCompliance)) then
    if (attrCompliance) local_attrCompliance = 1 !true
  end if

  if (present(isPresent)) then
    if (LEN(key) > 0) then
      isPresent = ESMF_InfoIsPresent(info, local_key, isPointer=.true., &
        attnestflag=attnestflag, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    else
      isPresent = .true.
    end if
  end if

  inq = ESMF_InfoCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_inquire(info%ptr, inq%ptr, local_key//C_NULL_CHAR, recursive, &
   local_idx_ptr, local_attrCompliance, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(size)) then
      call ESMF_InfoGet(inq, "size", size, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attrCount)) then
    call ESMF_InfoGet(inq, "attrCount", attrCount, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attrCountTotal)) then
      call ESMF_InfoGet(inq, "attrCountTotal", attrCountTotal, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(jsonType)) then
    call ESMF_InfoGet(inq, "jsonType", jsonType, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isArray)) then
    call ESMF_InfoGet(inq, "isArray", isArray, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isDirty)) then
    call ESMF_InfoGet(inq, "isDirty", isDirty, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attPackCount)) then
    call ESMF_InfoGet(inq, "attPackCount", attPackCount, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attPackCountTotal)) then
      call ESMF_InfoGet(inq, "attPackCountTotal", attPackCountTotal, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(typekind)) then
    call ESMF_InfoGet(inq, "ESMC_TypeKind_Flag", esmc_typekind, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    typekind = ESMF_TypeKind_Flag(esmc_typekind)
  end if
  if (present(ikey)) then
    call ESMF_InfoGet(inq, "key", ikey, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(idx)) then
    if (present(isPresent)) then
      if (isPresent) then
          inq2 = ESMF_InfoCreate(rc=localrc)
          if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

          call c_info_inquire(info%ptr, inq2%ptr, local_key//C_NULL_CHAR, recursive, &
            C_NULL_PTR, local_attrCompliance, localrc)
          if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

          call ESMF_InfoGet(inq2, "size", local_size, rc=localrc)
          if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

          if (idx > local_size) then
            isPresent = .false.
          end if

          call ESMF_InfoDestroy(inq2, rc=localrc)
          if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
       end if
    end if
  end if
  if (present(isStructured)) then
    call ESMF_InfoGet(inq, "isStructured", isStructured, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isNull)) then
    call ESMF_InfoGet(inq, "isNull", isNull, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  call ESMF_InfoDestroy(inq, rc=rc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoInquire

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoIsPresent()"
function ESMF_InfoIsPresent(info, key, attnestflag, isPointer, rc) result(is_present)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  logical, intent(in), optional :: isPointer
  integer, intent(inout), optional :: rc
  logical :: is_present

  logical :: local_isPointer
  integer :: localrc
  integer(C_INT) :: isPointer_forC
  integer(C_INT) :: local_is_present
  integer(C_INT) :: recursive

  is_present = .false.
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false
  local_is_present = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if
  if (present(isPointer)) then
    local_isPointer = isPointer
  else
    local_isPointer = .false.
  end if

  if (local_isPointer) then
    isPointer_forC = 1 !true
  else
    isPointer_forC = 0 !false
  end if

  call c_info_is_present(info%ptr, trim(key)//C_NULL_CHAR, local_is_present, &
    localrc, recursive, isPointer_forC)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (local_is_present == 1) then
    is_present = .true.
  else
    is_present = .false.
  end if

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoIsPresent

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoIsSet()"
function ESMF_InfoIsSet(info, key, rc) result(is_set)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer, intent(inout), optional :: rc
  logical :: is_set

  integer :: localrc
  integer(C_INT) :: is_set_c

  is_set = .false.
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_is_set(info%ptr, trim(key)//C_NULL_CHAR, is_set_c, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (is_set_c == 1) then
    is_set = .true.
  end if

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoIsSet

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoPrint()"
subroutine ESMF_InfoPrint(info, indent, rc)
  type(ESMF_Info), intent(in) :: info
  integer, intent(in), optional :: indent
  integer, intent(inout), optional :: rc

  integer :: localrc, localindent

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(indent)) then
    localindent = indent
  else
    localindent = 4
  end if

  call c_info_print(info%ptr, localindent, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoPrint

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoUpdate()"
subroutine ESMF_InfoUpdate(lhs, rhs, rc)
  type(ESMF_Info), intent(inout) :: lhs
  type(ESMF_Info), intent(in) :: rhs
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_update(lhs%ptr, rhs%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoUpdate

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoReadJSON()"
function ESMF_InfoReadJSON(filename, rc) result(info_r)
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info_r

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info_r = ESMF_InfoCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  call c_info_read_json(info_r%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_InfoReadJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoWriteJSON()"
subroutine ESMF_InfoWriteJSON(info, filename, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_write_json(info%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoWriteJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetCH()"
subroutine ESMF_InfoGetCH(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), intent(inout), target :: value
  character(len=*), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, vlen
  integer(C_INT), target :: local_idx
  character(len=ESMF_MAXSTR), target :: local_default
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  recursive = 0 !false
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = .false.

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  vlen = LEN(value)
  call c_info_get_CH(info%ptr, trim(key)//C_NULL_CHAR, value, vlen, &
    localrc, local_default_ptr, local_idx_ptr, recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayCH()"
subroutine ESMF_InfoGetArrayCH(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(out) :: values
  integer(C_INT), intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, ii
  logical :: is_array

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_InfoInquire(info, key=trim(key)//C_NULL_CHAR, size=itemcount, &
    attnestflag=attnestflag, isArray=is_array, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (.not. is_array) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_WRONGTYPE, &
      msg="Array requested but type in JSON storage is not an array. Key is: "//TRIM(key), &
      ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  do ii=1,itemcount
    call ESMF_InfoGetCH(info, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayCHAllocated()"
subroutine ESMF_InfoGetArrayCHAllocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), intent(out) :: values
  integer(C_INT), intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, ii
  character(len=ESMF_MAXSTR) :: logmsg
  logical :: is_array

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_InfoInquire(info, key=trim(key)//C_NULL_CHAR, size=itemcount, &
    attnestflag=attnestflag, isArray=is_array, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (.not. is_array) then
      if (ESMF_LogFoundError(ESMF_RC_ATTR_WRONGTYPE, &
        msg="Array requested but type in JSON storage is not an array. Key is: "//TRIM(key), &
        ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  if (itemcount /= SIZE(values)) then
    write(logmsg, *) "itemcount=", itemcount
    call ESMF_LogWrite(logmsg, ESMF_LOGMSG_ERROR)
    write(logmsg, *) "SIZE(values)=", SIZE(values)
    call ESMF_LogWrite(logmsg, ESMF_LOGMSG_ERROR)
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  do ii=1,itemcount
    call ESMF_InfoGetCH(info, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayCHAllocated

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetNULL()"
subroutine ESMF_InfoSetNULL(info, key, force, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if

  call c_info_set_NULL(info%ptr, trim(key)//C_NULL_CHAR, local_force, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetNULL

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetINFO()"
subroutine ESMF_InfoSetINFO(info, key, value, force, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  type(ESMF_Info), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if

  call c_info_set_INFO(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value%ptr, &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetINFO

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayCH()"
subroutine ESMF_InfoSetArrayCH(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc, ii
  integer(C_INT) :: idx
  logical(C_BOOL) :: local_force
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if

  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  ! Allocate storage in C
  call c_info_set_array_CH(info%ptr, trim(key)//C_NULL_CHAR, &
    SIZE(values), local_force, localrc, local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Set each character element in the underlying store
  do ii=1,SIZE(values)
    call ESMF_InfoSetCH(info, key, values(ii), idx=ii, pkey=local_pkey, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayCH

end module ESMF_InfoMod  !===============================================
