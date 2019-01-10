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

include "ESMF_Attributes_CDef.F90"

type ESMF_Attributes
  private
  type(C_PTR) :: ptr
end type ESMF_Attributes

!interface ESMF_Attributes
!  procedure ESMF_AttributesCreate
!end interface ESMF_Attributes

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
character(*), parameter, private :: version = '$Id$'
!------------------------------------------------------------------------------

contains  !====================================================================

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesCreate()"
function ESMF_AttributesCreate(rc) result(attrs)
  implicit none

  integer, intent(inout), optional :: rc
  integer :: localrc
  type(ESMF_Attributes) :: attrs

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  attrs%ptr = c_attrs_create(localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
                         rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesCreate

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesDestroy()"
subroutine ESMF_AttributesDestroy(attrs, rc)
  implicit none

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
  implicit none

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
#define ESMF_METHOD "ESMF_AttributesGet()"
subroutine ESMF_AttributesGet(attrs, key, value, default, rc)
  implicit none

  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(inout) :: value
  integer, intent(in), optional :: default
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT), target :: localdefault
  type(C_PTR) :: localdefault_ptr

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(default)) then
    localdefault = default
    localdefault_ptr = C_LOC(localdefault)
  else
    localdefault_ptr = C_NULL_PTR
  end if

  value = c_attrs_get(attrs%ptr, trim(key)//C_NULL_CHAR, localrc, localdefault_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGet

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArray()"
subroutine ESMF_AttributesGetArray(attrs, key, values, count, rc)
  ! Notes:
  !    * Default does not really make sense for getting a JSON array. This
  !      argument is intentionally left out.
  implicit none

  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: count
  integer, intent(inout), optional :: rc

  integer :: localrc, count_only

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  count_only = 1
  call c_attrs_get_array(attrs%ptr, trim(key)//C_NULL_CHAR, values, count, count_only, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  allocate(values(count))

  count_only = 0
  call c_attrs_get_array(attrs%ptr, trim(key)//C_NULL_CHAR, values, count, count_only, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArray

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesIsPresent()"
function ESMF_AttributesIsPresent(attrs, key, isPointer, rc) result(is_present)
  implicit none

  type(ESMF_Attributes), intent(in) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(in), optional :: isPointer
  integer, intent(inout), optional :: rc
  logical :: is_present

  logical :: local_isPointer
  integer :: localrc
  integer(C_INT) :: isPointer_forC

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

  !tdk:FIX: this conversion throws a compiler warning.
  is_present = c_attrs_is_present(attrs%ptr, trim(key)//C_NULL_CHAR, localrc, &
    isPointer_forC)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end function ESMF_AttributesIsPresent

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesPrint()"
subroutine ESMF_AttributesPrint(attrs, indent, rc)
  implicit none

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
#define ESMF_METHOD "ESMF_AttributesSet()"
subroutine ESMF_AttributesSet(attrs, key, value, force, rc)
  implicit none

  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: localforce

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  localforce = 1
  if (present(force)) then
    if (force .eqv. .false.) then
      localforce = 0
    end if
  end if

  call c_attrs_set(attrs%ptr, trim(key)//C_NULL_CHAR, value, localforce, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSet

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArray()"
subroutine ESMF_AttributesSetArray(attrs, key, values, force, rc)
  implicit none

  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: localforce

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  localforce = 1
  if (present(force)) then
    if (force .eqv. .false.) then
      localforce = 0
    end if
  end if

  call c_attrs_set_array(attrs%ptr, trim(key)//C_NULL_CHAR, values, &
      size(values), localforce, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArray

end module ESMF_AttributesMod  !===============================================
