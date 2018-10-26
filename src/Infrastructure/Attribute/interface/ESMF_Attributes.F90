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

end module ESMF_AttributesMod  !===============================================
