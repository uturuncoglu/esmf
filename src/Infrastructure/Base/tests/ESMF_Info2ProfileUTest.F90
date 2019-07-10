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

#define FILENAME "src/Infrastructure/Base/test/ESMF_Info2ProfileUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================
!==============================================================================
!==============================================================================

program ESMF_Info2ProfileUTest

  !============================================================================
  !BOP
  ! !PROGRAM: ESMF_Info2ProfileUTest -  Profiles Info2 code
  !
  ! !DESCRIPTION:
  !
  !----------------------------------------------------------------------------
  ! !USES:
  use ESMF_TestMod     ! test methods
  use ESMF
  use ESMF_Info2Mod

  implicit none

  !----------------------------------------------------------------------------
  ! The following line turns the CVS identifier string into a printable variable.
  character(*), parameter :: version = '$Id$'
  !----------------------------------------------------------------------------

  ! individual test failure message
  character(ESMF_MAXSTR) :: failMsg
  character(ESMF_MAXSTR) :: name
  ! cumulative result: count failures; no failures equals "all pass"
  integer               :: result = 0

  character(ESMF_MAXSTR) :: key
  integer               :: rc, ii, idx, rsize
  integer, allocatable, dimension(:) :: seed
  real                  :: r
  integer(ESMF_KIND_I4) :: value
  type(ESMF_Info2) :: attrs, attrs2
  integer, parameter    :: nkeys = 1000
  integer, parameter    :: ntests = 100000
  logical :: is_present

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  ! Test setting and getting a bunch of attributes.

  rc = ESMF_FAILURE
  write(name, *) "ESMF_Info2 Profile Loop"
  write(failMsg, *) "Failure during profile loop test"

  attrs = ESMF_Info2Create(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------

  ! Set nkeys count of attributes key/value pairs.
  do ii=1, nkeys

    write(key, *) ii

    call ESMF_TraceRegionEnter("JSON_Info2::Set", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_Info2Set(attrs, adjustl(trim(key)), ii, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("JSON_Info2::Set", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

  !----------------------------------------------------------------------------

  ! For the ntests test count, retrieve the attribute value.
  call random_seed(size=rsize)
  allocate(seed(rsize))
  seed(:) = 0
  call random_seed(put=seed)

  do ii=1, ntests
    call random_number(r)
    idx = ceiling(r*nkeys)

    write(key, *) idx

    call ESMF_TraceRegionEnter("JSON_Info2::Get", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_Info2Get(attrs, adjustl(trim(key)), value, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("JSON_Info2::Get", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

  !----------------------------------------------------------------------------

!  call ESMF_Info2Print(attrs)

  deallocate(seed)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest

  ! Profile checking for attribute presence.

  write(name, *) "ESMF_Info2 Presence Check Profile Test"
  write(failMsg, *) "Failure during presence profile loop test"

  do ii=1, ntests

    call ESMF_TraceRegionEnter("JSON_Info2::IsPresent False", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    is_present = ESMF_Info2IsPresent(attrs, "this", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("JSON_Info2::IsPresent False", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

  do ii=1, ntests

    call ESMF_TraceRegionEnter("JSON_Info2::IsPresent True", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    is_present = ESMF_Info2IsPresent(attrs, "999", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("JSON_Info2::IsPresent True", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_Info2Destroy(attrs, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_Info2ProfileUTest
