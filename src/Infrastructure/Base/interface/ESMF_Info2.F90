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
#define ESMF_FILENAME "src/Infrastructure/Base/interface/ESMF_Info2.F90"
!==============================================================================

module ESMF_Info2Mod

!==============================================================================
!
! This file contains the Fortran wrapper code for the C++ implementation of
!  the Info2 class.
!
!------------------------------------------------------------------------------

! INCLUDES
#include "ESMF.h"

!==============================================================================
!BOPI
! !MODULE: ESMF_Info2Mod
!

!   Fortran API wrapper of C++ implemenation of Info2
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
!public :: ESMF_Info2

include "ESMF_Info2CDef.F90"
include "ESMF_Info2CDefGeneric.F90"

type ESMF_Info2
  type(C_PTR) :: ptr = C_NULL_PTR
end type ESMF_Info2

interface ESMF_Info2Create
  module procedure ESMF_Info2CreateEmpty
  module procedure ESMF_Info2CreateByKey
  module procedure ESMF_Info2CreateByParse
end interface ESMF_Info2Create

interface ESMF_Info2Get
  module procedure ESMF_Info2GetI4
  module procedure ESMF_Info2GetI8
  module procedure ESMF_Info2GetR4
  module procedure ESMF_Info2GetR8
  module procedure ESMF_Info2GetLG
  module procedure ESMF_Info2GetCH
  module procedure ESMF_Info2GetArrayI4
  module procedure ESMF_Info2GetArrayI8
  module procedure ESMF_Info2GetArrayR4
  module procedure ESMF_Info2GetArrayR8
  module procedure ESMF_Info2GetArrayLG
  module procedure ESMF_Info2GetArrayCH
end interface ESMF_Info2Get

interface ESMF_Info2Set
  module procedure ESMF_Info2SetI4
  module procedure ESMF_Info2SetI8
  module procedure ESMF_Info2SetR4
  module procedure ESMF_Info2SetR8
  module procedure ESMF_Info2SetLG
  module procedure ESMF_Info2SetCH
  module procedure ESMF_Info2SetINFO
  module procedure ESMF_Info2SetArrayI4
  module procedure ESMF_Info2SetArrayI8
  module procedure ESMF_Info2SetArrayR4
  module procedure ESMF_Info2SetArrayR8
  module procedure ESMF_Info2SetArrayLG
  module procedure ESMF_Info2SetArrayCH
end interface ESMF_Info2Set

interface operator(==)
  procedure ESMF_Info2Equal
end interface operator(==)
interface operator(/=)
  procedure ESMF_Info2NotEqual
end interface operator(/=)

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
character(*), parameter, private :: version = '$Id$'
!------------------------------------------------------------------------------

contains  !====================================================================

#include "ESMF_Info2Generic.F90"

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2BaseGet()"
function ESMF_Info2BaseGet(base) result(info)
  type(ESMF_Base), intent(in) :: base
  type(ESMF_Info2) :: info
  info%ptr = c_info_base_get(base%this%ptr)
end function ESMF_Info2BaseGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2PointerGet()"
function ESMF_Info2PointerGet(ptr) result(info)
  type(ESMF_Pointer), intent(in) :: ptr
  type(ESMF_Info2) :: info
  info%ptr = c_info_base_get(ptr%ptr)
end function ESMF_Info2PointerGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Copy()"
function ESMF_Info2Copy(info, rc) result(info_copy)
  type(ESMF_Info2), intent(in) :: info
  integer, intent(inout), optional :: rc
  type(ESMF_Info2) :: info_copy

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info_copy%ptr = c_info_copy(info%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2Copy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2CreateEmpty()"
function ESMF_Info2CreateEmpty(rc) result(info)
  integer, intent(inout), optional :: rc
  integer :: localrc
  type(ESMF_Info2) :: info

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create(localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2CreateEmpty

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2CreateByKey()"
function ESMF_Info2CreateByKey(srcInfo, key, rc) result(info)
  type(ESMF_Info2), intent(in) :: srcInfo
  character(len=*), intent(in) :: key
  integer, intent(inout), optional :: rc
  type(ESMF_Info2) :: info

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create_by_key(srcInfo%ptr, trim(key)//C_NULL_CHAR, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2CreateByKey

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2CreateByParse()"
function ESMF_Info2CreateByParse(payload, rc) result(info)
  character(len=*), intent(in) :: payload
  integer, intent(inout), optional :: rc
  type(ESMF_Info2) :: info

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info%ptr = c_info_create_by_parse(trim(payload)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2CreateByParse

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Broadcast()"
subroutine ESMF_Info2Broadcast(info, rootPet, rc)
  type(ESMF_Info2), intent(inout) :: info
  integer, intent(in) :: rootPet
  integer, intent(inout), optional :: rc
  integer :: localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_FAILURE

  call c_info_broadcast(info%ptr, rootPet, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2Broadcast

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Destroy()"
subroutine ESMF_Info2Destroy(info, rc)
  type(ESMF_Info2), intent(inout) :: info
  integer, intent(inout), optional :: rc
  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_destroy(info%ptr, localrc)
  info%ptr = C_NULL_PTR
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2Destroy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Dump()"
function ESMF_Info2Dump(info, key, rc) result(output)
  type(ESMF_Info2), intent(in) :: info
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
end function ESMF_Info2Dump

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Equal()"
function ESMF_Info2Equal(lhs, rhs) result(is_equal)
  type(ESMF_Info2), intent(in) :: lhs
  type(ESMF_Info2), intent(in) :: rhs
  logical :: is_equal

  integer :: localrc
  logical(C_BOOL) :: local_is_equal

  is_equal = .false.
  call c_info_is_equal(lhs%ptr, rhs%ptr, local_is_equal, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT)) return

  is_equal = local_is_equal
end function ESMF_Info2Equal

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2NotEqual()"
function ESMF_Info2NotEqual(lhs, rhs) result(is_equal)
  type(ESMF_Info2), intent(in) :: lhs
  type(ESMF_Info2), intent(in) :: rhs
  logical :: is_equal
  is_equal = .not. ESMF_Info2Equal(lhs, rhs)
end function ESMF_Info2NotEqual

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Remove()"
subroutine ESMF_Info2Remove(info, keyParent, keyChild, attnestflag, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: keyParent
  character(len=*), intent(in), optional :: keyChild
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  character(len=ESMF_MAXSTR) :: localkeyChild !tdk:todo: change this to an allocated
  logical(C_BOOL) :: recursive=.false.

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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
end subroutine ESMF_Info2Remove

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Inquire()"
subroutine ESMF_Info2Inquire(info, key, count, countTotal, jsonType, isArray, &
  isDirty, attPackCount, attnestflag, idx, typekind, ikey, rc)

  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in), optional :: key
  integer(C_INT), intent(out), optional :: count
  integer(C_INT), intent(out), optional :: countTotal
  character(len=*), intent(out), optional :: jsonType
  logical, intent(out), optional :: isArray
  logical, intent(out), optional :: isDirty
  integer(C_INT), intent(out), optional :: attPackCount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer(C_INT), intent(in), optional :: idx
  type(ESMF_TypeKind_Flag), intent(out), optional :: typekind
  character(len=*), intent(out), optional :: ikey
  integer, intent(inout), optional :: rc

  integer :: localrc, esmc_typekind
  type(ESMF_Info2) :: inq
  character(:), allocatable :: local_key
  logical(C_BOOL) :: recursive=.false.
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(key)) then
    allocate(character(len(trim(key)))::local_key)
    local_key = trim(key)
  else
    allocate(character(0)::local_key)
    local_key = ""
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  inq = ESMF_Info2Create(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_inquire(info%ptr, inq%ptr, local_key//C_NULL_CHAR, recursive, &
   local_idx_ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(count)) then
    call ESMF_Info2Get(inq, "count", count, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(jsonType)) then
    call ESMF_Info2Get(inq, "jsonType", jsonType, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isArray)) then
    call ESMF_Info2Get(inq, "isArray", isArray, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isDirty)) then
    call ESMF_Info2Get(inq, "isDirty", isDirty, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attPackCount)) then
    call ESMF_Info2Get(inq, "attPackCount", attPackCount, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(countTotal)) then
    call ESMF_Info2Get(inq, "countTotal", countTotal, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(typekind)) then
    call ESMF_Info2Get(inq, "ESMC_TypeKind_Flag", esmc_typekind, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    typekind = ESMF_TypeKind_Flag(esmc_typekind)
  end if
  if (present(ikey)) then
    call ESMF_Info2Get(inq, "key", ikey, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  call ESMF_Info2Destroy(inq, rc=rc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2Inquire

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2IsPresent()"
function ESMF_Info2IsPresent(info, key, attnestflag, isPointer, rc) result(is_present)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  logical, intent(in), optional :: isPointer
  integer, intent(inout), optional :: rc
  logical :: is_present

  logical :: local_isPointer
  integer :: localrc
  integer(C_INT) :: isPointer_forC
  logical(C_BOOL) :: local_is_present
  logical(C_BOOL) :: recursive=.false.

  is_present = .false.
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = .true.
  end if
  if (present(isPointer)) then
    local_isPointer = isPointer
  else
    local_isPointer = .false.
  end if

  if (local_isPointer) then
    isPointer_forC = 1
  else
    isPointer_forC = 0
  end if

  call c_info_is_present(info%ptr, trim(key)//C_NULL_CHAR, local_is_present, &
    localrc, recursive, isPointer_forC)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  is_present = local_is_present

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2IsPresent

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2IsSet()"
function ESMF_Info2IsSet(info, key, rc) result(is_set)
  type(ESMF_Info2), intent(in) :: info
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
end function ESMF_Info2IsSet

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Print()"
subroutine ESMF_Info2Print(info, indent, rc)
  type(ESMF_Info2), intent(in) :: info
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
end subroutine ESMF_Info2Print

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2Update()"
subroutine ESMF_Info2Update(lhs, rhs, rc)
  type(ESMF_Info2), intent(inout) :: lhs
  type(ESMF_Info2), intent(in) :: rhs
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_update(lhs%ptr, rhs%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2Update

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2ReadJSON()"
function ESMF_Info2ReadJSON(filename, rc) result(info_r)
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc
  type(ESMF_Info2) :: info_r

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  info_r = ESMF_Info2Create(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  call c_info_read_json(info_r%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_Info2ReadJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2WriteJSON()"
subroutine ESMF_Info2WriteJSON(info, filename, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_info_write_json(info%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2WriteJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetCH()"
subroutine ESMF_Info2GetCH(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(inout) :: info
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
  logical(C_BOOL) :: recursive=.false.

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = .true.
  end if

  ! Call C ####################################################################

  vlen = LEN(value)
  call c_info_get_CH(info%ptr, trim(key)//C_NULL_CHAR, value, vlen, &
    localrc, local_default_ptr, local_idx_ptr, recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2GetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayCH()"
subroutine ESMF_Info2GetArrayCH(info, key, values, nelements, attnestflag, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(out) :: values
  integer(C_INT), target, intent(inout) :: nelements
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, ii

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_Info2Inquire(info, key=trim(key)//C_NULL_CHAR, count=nelements, &
    attnestflag=attnestflag, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  if (.not. allocated(values)) allocate(values(nelements))
  do ii=1,nelements
    call ESMF_Info2GetCH(info, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2GetArrayCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayCHAlloc()"
subroutine ESMF_Info2GetArrayCHAlloc(info, key, values, nelements, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(out) :: values
  integer(C_INT), target, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, ii

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_Info2Inquire(info, key=trim(key)//C_NULL_CHAR, count=nelements, &
                              rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  do ii=1,nelements
    call ESMF_Info2GetCH(info, key, values(ii), idx=ii, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2GetArrayCHAlloc

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetNULL()"
subroutine ESMF_Info2SetNULL(info, key, force, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetNULL

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetINFO()"
subroutine ESMF_Info2SetINFO(info, key, value, force, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  type(ESMF_Info2), intent(in) :: value
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
end subroutine ESMF_Info2SetINFO

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayCH()"
subroutine ESMF_Info2SetArrayCH(info, key, values, force, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc, ii
  integer(C_INT) :: idx
  logical(C_BOOL) :: local_force

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if

  ! Allocate storage in C
  call c_info_set_array_CH(info%ptr, trim(key)//C_NULL_CHAR, &
    SIZE(values), local_force, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Set each character element in the underlying store
  do ii=1,SIZE(values)
    call ESMF_Info2SetCH(info, key, values(ii), idx=ii, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2SetArrayCH

end module ESMF_Info2Mod  !===============================================
