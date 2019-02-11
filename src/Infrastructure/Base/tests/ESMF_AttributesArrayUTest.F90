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

#define FILENAME "src/Infrastructure/Attribute/test/ESMF_AttributesArrayUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================

program ESMF_AttributesArrayUTest
  use ESMF_TestMod
  use ESMF
  use ESMF_AttributesMod
  implicit none

  !----------------------------------------------------------------------------

  character(ESMF_MAXSTR) :: failMsg  ! Test failure message
  character(ESMF_MAXSTR) :: name  ! Test name
  integer               :: rc, petCount, i
  ! cumulative result: count failures; no failures equals "all pass"
  integer               :: result = 0
  real(ESMF_KIND_R4), parameter :: tol = 1e-16  ! Tolerance for real tests
  type(ESMF_Attributes) :: attrs
  logical :: failed

  real(ESMF_KIND_R4), dimension(3) :: arr_R4  ! Desired array values
  ! Actual array values retrieved from attributes
  real(ESMF_KIND_R4), dimension(:), allocatable :: arr_R4_get
  integer(ESMF_KIND_I4) :: arr_R4_get_count  ! Array element count

  real(ESMF_KIND_R8), dimension(3) :: arr_R8  ! Desired array values
  ! Actual array values retrieved from attributes
  real(ESMF_KIND_R8), dimension(:), allocatable :: arr_R8_get
  integer(ESMF_KIND_I4) :: arr_R8_get_count  ! Array element count

  integer(ESMF_KIND_I4), dimension(3) :: arr_I4  ! Desired array values
  ! Actual array values retrieved from attributes
  integer(ESMF_KIND_I4), dimension(:), allocatable :: arr_I4_get
  integer(ESMF_KIND_I4) :: arr_I4_get_count  ! Array element count

  integer(ESMF_KIND_I8), dimension(3) :: arr_I8  ! Desired array values
  ! Actual array values retrieved from attributes
  integer(ESMF_KIND_I8), dimension(:), allocatable :: arr_I8_get
  integer(ESMF_KIND_I4) :: arr_I8_get_count  ! Array element count

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  ! Create attributes object used by the array set/get tests
  attrs = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Attributes Array Setting/Getting R4"
  write(failMsg, *) "Attribute array operation failed"
  failed = .false.

  arr_R4(1:3) = (/ 1.0/3.0, 1.0/6.0, 1.0/12.0 /)

  call ESMF_AttributesSetArray(attrs, "the-key-R4", arr_R4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGetArray(attrs, "the-key-R4", arr_R4_get, &
    arr_R4_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (ABS(arr_R4(i) - arr_R4_get(i)) > tol) then
      failed = .true.
    end if
  end do

  deallocate(arr_R4_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Attributes Array Setting/Getting R8"
  write(failMsg, *) "Attribute array operation failed"
  failed = .false.

  arr_R8(1:3) = (/ 1.0/3.0, 1.0/6.0, 1.0/12.0 /)

  call ESMF_AttributesSetArray(attrs, "the-key-R8", arr_R8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGetArray(attrs, "the-key-R8", arr_R8_get, &
    arr_R8_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (ABS(arr_R8(i) - arr_R8_get(i)) > tol) then
      failed = .true.
    end if
  end do

  deallocate(arr_R8_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Attributes Array Setting/Getting I4"
  write(failMsg, *) "Attribute array operation failed"
  failed = .false.

  arr_I4(1:3) = (/ 123, 456, 789 /)

  call ESMF_AttributesSetArray(attrs, "the-key-I4", arr_I4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGetArray(attrs, "the-key-I4", arr_I4_get, &
    arr_I4_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (arr_I4(i) /= arr_I4_get(i)) then
      failed = .true.
    end if
  end do

  deallocate(arr_I4_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Attributes Array Setting/Getting I8"
  write(failMsg, *) "Attribute array operation failed"
  failed = .false.

  arr_I8(1:3) = (/ 123, 456, 789 /)

  call ESMF_AttributesSetArray(attrs, "the-key-I8", arr_I8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGetArray(attrs, "the-key-I8", arr_I8_get, &
    arr_I8_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (arr_I8(i) /= arr_I8_get(i)) then
      failed = .true.
    end if
  end do

  deallocate(arr_I8_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  ! Destroy the attributes object used by the array set/get tests
  call ESMF_AttributesDestroy(attrs, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesArrayUTest
