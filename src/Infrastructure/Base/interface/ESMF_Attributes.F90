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
#define ESMF_FILENAME "src/Infrastructure/Attribute/interface/ESMF_Attributes.F90"
!==============================================================================

module ESMF_AttributesMod

!==============================================================================
!
! This file contains the Fortran wrapper code for the C++ implementation of
!  the Attributes class.
!
!------------------------------------------------------------------------------

! INCLUDES
#include "ESMF.h"

!==============================================================================
!BOPI
! !MODULE: ESMF_AttributesMod
!

!   Fortran API wrapper of C++ implemenation of Attributes
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
!public :: ESMF_Attributes

include "ESMF_AttributesCDef.F90"
include "ESMF_AttributesCDefGeneric.F90"

type ESMF_Attributes
  type(C_PTR) :: ptr = C_NULL_PTR
end type ESMF_Attributes

interface ESMF_AttributesCreate
  module procedure ESMF_AttributesCreateEmpty
  module procedure ESMF_AttributesCreateByKey
  module procedure ESMF_AttributesCreateByParse
end interface ESMF_AttributesCreate

interface ESMF_AttributesGet
  module procedure ESMF_AttributesGetI4
  module procedure ESMF_AttributesGetI8
  module procedure ESMF_AttributesGetR4
  module procedure ESMF_AttributesGetR8
  module procedure ESMF_AttributesGetLG
  module procedure ESMF_AttributesGetCH
  module procedure ESMF_AttributesGetArrayI4
  module procedure ESMF_AttributesGetArrayI8
  module procedure ESMF_AttributesGetArrayR4
  module procedure ESMF_AttributesGetArrayR8
  module procedure ESMF_AttributesGetArrayLG
  module procedure ESMF_AttributesGetArrayCH
end interface ESMF_AttributesGet

interface ESMF_AttributesSet
  module procedure ESMF_AttributesSetI4
  module procedure ESMF_AttributesSetI8
  module procedure ESMF_AttributesSetR4
  module procedure ESMF_AttributesSetR8
  module procedure ESMF_AttributesSetLG
  module procedure ESMF_AttributesSetCH
  module procedure ESMF_AttributesSetATTRS
  module procedure ESMF_AttributesSetArrayI4
  module procedure ESMF_AttributesSetArrayI8
  module procedure ESMF_AttributesSetArrayR4
  module procedure ESMF_AttributesSetArrayR8
  module procedure ESMF_AttributesSetArrayLG
  module procedure ESMF_AttributesSetArrayCH
end interface ESMF_AttributesSet

interface operator(==)
  procedure ESMF_AttributesEqual
end interface operator(==)
interface operator(/=)
  procedure ESMF_AttributesNotEqual
end interface operator(/=)

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
character(*), parameter, private :: version = '$Id$'
!------------------------------------------------------------------------------

contains  !====================================================================

#include "ESMF_AttributesGeneric.F90"

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesBaseGet()"
function ESMF_AttributesBaseGet(base) result(attrs)
  type(ESMF_Base), intent(in) :: base
  type(ESMF_Attributes) :: attrs
  attrs%ptr = c_attrs_base_get(base%this%ptr)
end function ESMF_AttributesBaseGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesPointerGet()"
function ESMF_AttributesPointerGet(ptr) result(attrs)
  type(ESMF_Pointer), intent(in) :: ptr
  type(ESMF_Attributes) :: attrs
  attrs%ptr = c_attrs_base_get(ptr%ptr)
end function ESMF_AttributesPointerGet

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesCopy()"
function ESMF_AttributesCopy(attrs, rc) result(attrs_copy)
  type(ESMF_Attributes), intent(in) :: attrs
  integer, intent(inout), optional :: rc
  type(ESMF_Attributes) :: attrs_copy

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs_copy%ptr = c_attrs_copy(attrs%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesCopy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesCreateEmpty()"
function ESMF_AttributesCreateEmpty(rc) result(attrs)
  integer, intent(inout), optional :: rc
  integer :: localrc
  type(ESMF_Attributes) :: attrs

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs%ptr = c_attrs_create(localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesCreateEmpty

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesCreateByKey()"
function ESMF_AttributesCreateByKey(srcAttrs, key, rc) result(attrs)
  type(ESMF_Attributes), intent(in) :: srcAttrs
  character(len=*), intent(in) :: key
  integer, intent(inout), optional :: rc
  type(ESMF_Attributes) :: attrs

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs%ptr = c_attrs_create_by_key(srcAttrs%ptr, trim(key)//C_NULL_CHAR, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesCreateByKey

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesCreateByParse()"
function ESMF_AttributesCreateByParse(payload, rc) result(attrs)
  character(len=*), intent(in) :: payload
  integer, intent(inout), optional :: rc
  type(ESMF_Attributes) :: attrs

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs%ptr = c_attrs_create_by_parse(trim(payload)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesCreateByParse

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesBroadcast()"
subroutine ESMF_AttributesBroadcast(attrs, rootPet, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  integer, intent(in) :: rootPet
  integer, intent(inout), optional :: rc
  integer :: localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_FAILURE

  call c_attrs_broadcast(attrs%ptr, rootPet, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesBroadcast

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesDestroy()"
subroutine ESMF_AttributesDestroy(attrs, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  integer, intent(inout), optional :: rc
  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_attrs_destroy(attrs%ptr, localrc)
  attrs%ptr = C_NULL_PTR
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesDestroy

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesEqual()"
function ESMF_AttributesEqual(lhs, rhs) result(is_equal)
  type(ESMF_Attributes), intent(in) :: lhs
  type(ESMF_Attributes), intent(in) :: rhs
  logical :: is_equal

  integer :: localrc
  logical(C_BOOL) :: local_is_equal

  is_equal = .false.
  call c_attrs_is_equal(lhs%ptr, rhs%ptr, local_is_equal, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT)) return

  is_equal = local_is_equal
end function ESMF_AttributesEqual

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesNotEqual()"
function ESMF_AttributesNotEqual(lhs, rhs) result(is_equal)
  type(ESMF_Attributes), intent(in) :: lhs
  type(ESMF_Attributes), intent(in) :: rhs
  logical :: is_equal
  is_equal = .not. ESMF_AttributesEqual(lhs, rhs)
end function ESMF_AttributesNotEqual

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesRemove()"
subroutine ESMF_AttributesRemove(attrs, keyParent, keyChild, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: keyParent
  character(len=*), intent(in), optional :: keyChild
  integer, intent(inout), optional :: rc

  integer :: localrc
  character(len=ESMF_MAXSTR) :: localkeyChild

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(keyChild)) then
    localkeyChild = keyChild
  else
    localkeyChild = ""
  end if

  call c_attrs_erase(attrs%ptr, trim(keyParent)//C_NULL_CHAR, &
                     trim(localkeyChild)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesRemove

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesInquire()"
subroutine ESMF_AttributesInquire(attrs, key, count, countTotal, jsonType, isArray, &
  isDirty, attPackCount, attnestflag, idx, typekind, rc)

  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in), optional :: key
  integer(C_INT), intent(inout), optional :: count
  integer(C_INT), intent(inout), optional :: countTotal
  character(len=*), intent(inout), optional :: jsonType
  logical, intent(inout), optional :: isArray
  logical, intent(inout), optional :: isDirty
  integer(C_INT), intent(inout), optional :: attPackCount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer(C_INT), intent(in), optional :: idx
  type(ESMF_TypeKind_Flag), optional :: typekind
  integer, intent(inout), optional :: rc

  integer :: localrc, esmc_typekind
  type(ESMF_Attributes) :: inq
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

  inq = ESMF_AttributesCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_attrs_inquire(attrs%ptr, inq%ptr, local_key//C_NULL_CHAR, recursive, &
   local_idx_ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(count)) then
    call ESMF_AttributesGet(inq, "count", count, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(jsonType)) then
    call ESMF_AttributesGet(inq, "jsonType", jsonType, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isArray)) then
    call ESMF_AttributesGet(inq, "isArray", isArray, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(isDirty)) then
    call ESMF_AttributesGet(inq, "isDirty", isDirty, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(attPackCount)) then
    call ESMF_AttributesGet(inq, "attPackCount", attPackCount, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(countTotal)) then
    call ESMF_AttributesGet(inq, "countTotal", countTotal, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if
  if (present(typekind)) then
    call ESMF_AttributesGet(inq, "ESMC_TypeKind_Flag", esmc_typekind, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    typekind = ESMF_TypeKind_Flag(esmc_typekind)
  end if

  call ESMF_AttributesDestroy(inq, rc=rc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesInquire

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesIsPresent()"
function ESMF_AttributesIsPresent(attrs, key, isPointer, rc) result(is_present)
  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(in), optional :: isPointer
  integer, intent(inout), optional :: rc
  logical :: is_present

  logical :: local_isPointer
  integer :: localrc
  integer(C_INT) :: isPointer_forC
  logical(C_BOOL) :: local_is_present

  is_present = .false.
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  call c_attrs_is_present(attrs%ptr, trim(key)//C_NULL_CHAR, local_is_present, &
    localrc, isPointer_forC)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  is_present = local_is_present

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesIsPresent

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesIsSet()"
function ESMF_AttributesIsSet(attrs, key, rc) result(is_set)
  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in) :: key
  integer, intent(inout), optional :: rc
  logical :: is_set

  integer :: localrc
  integer(C_INT) :: is_set_c

  is_set = .false.
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_attrs_is_set(attrs%ptr, trim(key)//C_NULL_CHAR, is_set_c, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (is_set_c == 1) then
    is_set = .true.
  end if

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesIsSet

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesPrint()"
subroutine ESMF_AttributesPrint(attrs, indent, rc)
  type(ESMF_Attributes), intent(in) :: attrs
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

  call c_attrs_print(attrs%ptr, localindent, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesPrint

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesUpdate()"
subroutine ESMF_AttributesUpdate(lhs, rhs, rc)
  type(ESMF_Attributes), intent(inout) :: lhs
  type(ESMF_Attributes), intent(in) :: rhs
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_attrs_update(lhs%ptr, rhs%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesUpdate

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesReadJSON()"
function ESMF_AttributesReadJSON(filename, rc) result(attrs_r)
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc
  type(ESMF_Attributes) :: attrs_r

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs_r = ESMF_AttributesCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  call c_attrs_read_json(attrs_r%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesReadJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesWriteJSON()"
subroutine ESMF_AttributesWriteJSON(attrs, filename, rc)
  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in) :: filename
  integer, intent(inout), optional :: rc

  integer :: localrc

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  call c_attrs_write_json(attrs%ptr, trim(filename)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesWriteJSON

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetCH()"
subroutine ESMF_AttributesGetCH(attrs, key, value, default, idx, attnestflag, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
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
  call c_attrs_get_CH(attrs%ptr, trim(key)//C_NULL_CHAR, value, vlen, &
    localrc, local_default_ptr, local_idx_ptr, recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayCH()"
subroutine ESMF_AttributesGetArrayCH(attrs, key, values, nelements, attnestflag, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(out) :: values
  integer(C_INT), target, intent(inout) :: nelements
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, ii

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_AttributesInquire(attrs, key=trim(key)//C_NULL_CHAR, count=nelements, &
    attnestflag=attnestflag, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  if (.not. allocated(values)) allocate(values(nelements))
  do ii=1,nelements
    call ESMF_AttributesGetCH(attrs, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayCHAlloc()"
subroutine ESMF_AttributesGetArrayCHAlloc(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(out) :: values
  integer(C_INT), target, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, ii

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_AttributesInquire(attrs, key=trim(key)//C_NULL_CHAR, count=nelements, &
                              rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  do ii=1,nelements
    call ESMF_AttributesGetCH(attrs, key, values(ii), idx=ii, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayCHAlloc

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetNULL()"
subroutine ESMF_AttributesSetNULL(attrs, key, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
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

  call c_attrs_set_NULL(attrs%ptr, trim(key)//C_NULL_CHAR, local_force, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetNULL

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetATTRS()"
subroutine ESMF_AttributesSetATTRS(attrs, key, value, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  type(ESMF_Attributes), intent(in) :: value
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

  call c_attrs_set_ATTRS(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value%ptr, &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetATTRS

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayCH()"
subroutine ESMF_AttributesSetArrayCH(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
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
  call c_attrs_set_array_CH(attrs%ptr, trim(key)//C_NULL_CHAR, &
    SIZE(values), local_force, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Set each character element in the underlying store
  do ii=1,SIZE(values)
    call ESMF_AttributesSetCH(attrs, key, values(ii), idx=ii, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayCH

end module ESMF_AttributesMod  !===============================================
