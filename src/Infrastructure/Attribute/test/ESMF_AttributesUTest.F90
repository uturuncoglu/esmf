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

#define FILENAME "src/Infrastructure/Attribute/test/ESMF_AttributesUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!module ESMF_AttributesUTestMod
!
!  ! modules
!  use ESMF_TestMod     ! test methods
!  use ESMF
!
!  implicit none
!
!  private
!
!  public
!
!contains !--------------------------------------------------------------------
!
!end module

!==============================================================================
!==============================================================================
!==============================================================================

program ESMF_AttributesUTest

  !============================================================================
  !BOP
  ! !PROGRAM: ESMF_ArraySMMUTest -  Tests ArraySMM()
  !
  ! !DESCRIPTION:
  !
  !----------------------------------------------------------------------------
  ! !USES:
  use ESMF_TestMod     ! test methods
  use ESMF
  use ESMF_AttributesMod

  implicit none

  !----------------------------------------------------------------------------
  ! The following line turns the CVS identifier string into a printable variable.
  character(*), parameter :: version = &
    '$Id$'
  !----------------------------------------------------------------------------

  ! individual test failure message
  character(ESMF_MAXSTR) :: failMsg
  character(ESMF_MAXSTR) :: name
  character(ESMF_MAXSTR) :: key

  integer               :: rc, petCount, i
  integer, allocatable  :: petList(:)
  type(ESMF_VM)         :: vm
  type(ESMF_GridComp)   :: gcomp
  ! cumulative result: count failures; no failures equals "all pass"
  integer               :: result = 0

  type(ESMF_Attributes) :: attrs, attrs2

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  call ESMF_VMGetGlobal(vm, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_VMGet(vm, petCount=petCount, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesCreate/ESMF_AttributesDestroy"

  write(failMsg, *) "Did not create Attributes"
  rc = ESMF_FAILURE
  attrs = ESMF_AttributesCreate(rc)
  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  write(failMsg, *) "Did not destroy Attributes"
  rc = ESMF_FAILURE;
  call ESMF_AttributesDestroy(attrs, rc)
  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesSet"
  rc = ESMF_FAILURE

  write(failMsg, *) "Did not set key"
  key = "testKey"
  call ESMF_AttributesSet(key, rc)
  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

!  attrs2 = ESMF_AttributesCreate(rc)
!  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
!
!  call ESMF_AttributesDestroy(attrs2, rc)
!  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesUTest
