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

type ESMF_Attributes
  private
  type(C_PTR) :: ptr
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

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
character(*), parameter, private :: version = '$Id$'
!------------------------------------------------------------------------------

contains  !====================================================================

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
#define ESMF_METHOD "ESMF_AttributesErase()"
subroutine ESMF_AttributesErase(attrs, keyParent, keyChild, rc)
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
end subroutine ESMF_AttributesErase

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesInquire()"
subroutine ESMF_AttributesInquire(attrs, key, count, jsonType, isArray, isDirty, rc)
  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in), optional :: key
  integer(C_INT), intent(inout), optional :: count
  character(len=*), intent(inout), optional :: jsonType
  logical, intent(inout), optional :: isArray
  logical, intent(inout), optional :: isDirty
  integer, intent(inout), optional :: rc

  integer :: localrc
  type(ESMF_Attributes) :: inq
  character(len=ESMF_MAXSTR) :: local_key

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(key)) then
    local_key = key
  else
    local_key = ""
  end if

  inq = ESMF_AttributesCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  call c_attrs_inquire(attrs%ptr, inq%ptr, trim(local_key)//C_NULL_CHAR, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(count)) then
    call ESMF_AttributesGet(inq, "count", count, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return
  end if
  if (present(jsonType)) then
    call ESMF_AttributesGet(inq, "jsonType", jsonType, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return
  end if
  if (present(isArray)) then
    call ESMF_AttributesGet(inq, "isArray", isArray, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return
  end if
  if (present(isDirty)) then
    call ESMF_AttributesGet(inq, "isDirty", isDirty, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return
  end if

  call ESMF_AttributesDestroy(inq, rc=rc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

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
#define ESMF_METHOD "ESMF_AttributesGetR4()"
subroutine ESMF_AttributesGetR4(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(inout) :: value
  real(ESMF_KIND_R4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_FLOAT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
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

  ! Call C ####################################################################

  call c_attrs_get_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetR8()"
subroutine ESMF_AttributesGetR8(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(inout) :: value
  real(ESMF_KIND_R8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_DOUBLE), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
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

  ! Call C ####################################################################

  call c_attrs_get_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetI4()"
subroutine ESMF_AttributesGetI4(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(inout) :: value
  integer(ESMF_KIND_I4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
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

  ! Call C ####################################################################

  call c_attrs_get_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetI8()"
subroutine ESMF_AttributesGetI8(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(inout) :: value
  integer(ESMF_KIND_I8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_LONG), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
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

  ! Call C ####################################################################

  call c_attrs_get_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetLG()"
subroutine ESMF_AttributesGetLG(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(inout) :: value
  logical, intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL), target :: local_default
  logical(C_BOOL) :: local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
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

  ! Call C ####################################################################

  call c_attrs_get_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  value = local_value
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetLG

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetCH()"
subroutine ESMF_AttributesGetCH(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), intent(inout), target :: value
  character(len=*), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc, vlen
  integer(C_INT), target :: local_idx
  character(len=ESMF_MAXSTR), target :: local_default
  type(C_PTR) :: local_default_ptr, local_idx_ptr

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

  ! Call C ####################################################################

  vlen = LEN(value)
  call c_attrs_get_CH(attrs%ptr, trim(key)//C_NULL_CHAR, value, vlen, &
    localrc, local_default_ptr, local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetCH

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayR4()"
subroutine ESMF_AttributesGetArrayR4(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayR8()"
subroutine ESMF_AttributesGetArrayR8(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayI4()"
subroutine ESMF_AttributesGetArrayI4(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayI8()"
subroutine ESMF_AttributesGetArrayI8(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayLG()"
subroutine ESMF_AttributesGetArrayLG(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(local_values(nelements))
  nelements_only = 0
  call c_attrs_get_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  allocate(values(nelements))
  do ii=1,SIZE(values)
    values(ii) = local_values(ii)
  enddo
  deallocate(local_values)
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayLG

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayCH()"
subroutine ESMF_AttributesGetArrayCH(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), allocatable, intent(inout) :: values
  integer(C_INT), target, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, ii

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  call ESMF_AttributesInquire(attrs, key=trim(key)//C_NULL_CHAR, count=nelements, &
                              rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  do ii=1,nelements
    call ESMF_AttributesGetCH(attrs, key, values(ii), idx=ii, rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayCH

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
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetNULL

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetR4()"
subroutine ESMF_AttributesSetR4(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  call c_attrs_set_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetR8()"
subroutine ESMF_AttributesSetR8(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  call c_attrs_set_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetI4()"
subroutine ESMF_AttributesSetI4(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  call c_attrs_set_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetI8()"
subroutine ESMF_AttributesSetI8(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  call c_attrs_set_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetCH()"
subroutine ESMF_AttributesSetCH(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  call c_attrs_set_CH(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    trim(value)//C_NULL_CHAR, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetLG()"
subroutine ESMF_AttributesSetLG(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force, local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if

  local_value = value
  call c_attrs_set_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetLG

!------------------------------------------------------------------------------

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

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayR4()"
subroutine ESMF_AttributesSetArrayR4(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), intent(in) :: values
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
  call c_attrs_set_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayR8()"
subroutine ESMF_AttributesSetArrayR8(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), intent(in) :: values
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
  call c_attrs_set_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayI4()"
subroutine ESMF_AttributesSetArrayI4(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(in) :: values
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
  call c_attrs_set_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayI8()"
subroutine ESMF_AttributesSetArrayI8(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), intent(in) :: values
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
  call c_attrs_set_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayLG()"
subroutine ESMF_AttributesSetArrayLG(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  
  allocate(local_values(SIZE(values)))
  do ii=1,SIZE(values)
    local_values(ii) = values(ii)
  enddo
  call c_attrs_set_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  deallocate(local_values)
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayLG

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