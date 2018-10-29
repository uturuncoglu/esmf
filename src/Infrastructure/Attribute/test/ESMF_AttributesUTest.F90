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

  integer(ESMF_KIND_I4) :: value, actual

  type(ESMF_Attributes) :: attrs, attrs2, attrs3, attrs4

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
  ! Test basic set/get operation
  rc = ESMF_FAILURE

  attrs2 = ESMF_AttributesCreate(rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  write(name, *) "ESMF_AttributesSet"
  write(failMsg, *) "Did not set key"
  key = "testKey"
  actual = 333
  call ESMF_AttributesSet(attrs2, key, actual, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  write(name, *) "ESMF_AttributesGet"
  write(failMsg, *) "Did not get key"
  call ESMF_AttributesGet(attrs2, key, value, rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((value == actual), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs2, rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  ! Test we can overload the value if force is true
  rc = ESMF_FAILURE

  attrs3 = ESMF_AttributesCreate(rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  write(name, *) "ESMF_AttributesSet Force Flag"
  write(failMsg, *) "Could not overload value"
  key = "testKey"
  actual = 333

  call ESMF_AttributesSet(attrs3, "foobar", 123, force=.true., rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs3, "foobar", 123, force=.true., rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs3, rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  ! Test creating and destroying a bunch of attributes
  rc = ESMF_FAILURE
  write(name, *) "ESMF_Attributes Create+Destroy Loop"
  write(failMsg, *) "Failure during loop test"

  do i=1, 100000
    attrs4 = ESMF_AttributesCreate(rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_AttributesSet(attrs4, "foobar", 123, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_AttributesDestroy(attrs4, rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  end do

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesUTest
