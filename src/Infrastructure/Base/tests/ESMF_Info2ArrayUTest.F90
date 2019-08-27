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

#define FILENAME "src/Infrastructure/Base/test/ESMF_Info2ArrayUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================

program ESMF_Info2ArrayUTest
  use ESMF_TestMod
  use ESMF
  use ESMF_Info2Mod
  implicit none

  !----------------------------------------------------------------------------

  character(ESMF_MAXSTR) :: failMsg  ! Test failure message
  character(ESMF_MAXSTR) :: name  ! Test name
  integer :: rc, petCount, i, charcount, logical_count
  ! cumulative result: count failures; no failures equals "all pass"
  integer :: result = 0
  real(ESMF_KIND_R4), parameter :: tol = 1e-16  ! Tolerance for real tests
  type(ESMF_Info2) :: info
  logical :: failed
  character(len=22), dimension(5) :: desired_char
  character(len=22), dimension(:), allocatable :: actual_char
  logical, dimension(5) :: desired_logical
  logical, dimension(:), allocatable :: actual_logical

  real(ESMF_KIND_R4), dimension(3) :: arr_R4  ! Desired array values
  ! Actual array values retrieved from info
  real(ESMF_KIND_R4), dimension(:), allocatable :: arr_R4_get
  real(ESMF_KIND_R4) :: value_R4_get
  integer(ESMF_KIND_I4) :: arr_R4_get_count  ! Array element count

  real(ESMF_KIND_R8), dimension(3) :: arr_R8  ! Desired array values
  ! Actual array values retrieved from info
  real(ESMF_KIND_R8), dimension(:), allocatable :: arr_R8_get
  real(ESMF_KIND_R8) :: value_R8_get
  integer(ESMF_KIND_I4) :: arr_R8_get_count  ! Array element count

  integer(ESMF_KIND_I4), dimension(3) :: arr_I4  ! Desired array values
  ! Actual array values retrieved from info
  integer(ESMF_KIND_I4), dimension(:), allocatable :: arr_I4_get
  integer(ESMF_KIND_I4) :: value_I4_get
  integer(ESMF_KIND_I4) :: arr_I4_get_count  ! Array element count

  integer(ESMF_KIND_I8), dimension(3) :: arr_I8  ! Desired array values
  ! Actual array values retrieved from info
  integer(ESMF_KIND_I8), dimension(:), allocatable :: arr_I8_get
  integer(ESMF_KIND_I8) :: value_I8_get
  integer(ESMF_KIND_I4) :: arr_I8_get_count  ! Array element count

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  ! Create info object used by the array set/get tests
  info = ESMF_Info2Create(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting R4"
  write(failMsg, *) "Comparison to array get failed for R4"
  failed = .false.

  arr_R4(1:3) = (/ 1.0/3.0, 1.0/6.0, 1.0/12.0 /)

  call ESMF_Info2Set(info, "the-key-R4", arr_R4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "the-key-R4", arr_R4_get, &
    arr_R4_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (ABS(arr_R4(i) - arr_R4_get(i)) > tol) then
      failed = .true.
      exit
    end if
  end do

  deallocate(arr_R4_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting R8"
  write(failMsg, *) "Comparison to array get failed for R8"
  failed = .false.

  arr_R8(1:3) = (/ 1.0/3.0, 1.0/6.0, 1.0/12.0 /)

  call ESMF_Info2Set(info, "the-key-R8", arr_R8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "the-key-R8", arr_R8_get, &
    arr_R8_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (ABS(arr_R8(i) - arr_R8_get(i)) > tol) then
      failed = .true.
      exit
    end if
  end do

  deallocate(arr_R8_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting I4"
  write(failMsg, *) "Comparison to array get failed for I4"
  failed = .false.

  arr_I4(1:3) = (/ 123, 456, 789 /)

  call ESMF_Info2Set(info, "the-key-I4", arr_I4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "the-key-I4", arr_I4_get, &
    arr_I4_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (arr_I4(i) /= arr_I4_get(i)) then
      failed = .true.
      exit
    end if
  end do

  deallocate(arr_I4_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting I8"
  write(failMsg, *) "Comparison to array get failed for I8"
  failed = .false.

  arr_I8(1:3) = (/ 123, 456, 789 /)

  call ESMF_Info2Set(info, "the-key-I8", arr_I8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "the-key-I8", arr_I8_get, &
    arr_I8_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (arr_I8(i) /= arr_I8_get(i)) then
      failed = .true.
      exit
    end if
  end do

  deallocate(arr_I8_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Index R4"
  write(failMsg, *) "Comparison to array index get failed for R4"
  failed = .false.

  do i=1, 3
    call ESMF_Info2Get(info, "the-key-R4", value_R4_get, idx=i, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
    if (ABS(arr_R4(i) - value_R4_get) > tol) then
      failed = .true.
    exit
    end if
  end do

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Index R8"
  write(failMsg, *) "Comparison to array index get failed for R8"
  failed = .false.

  do i=1, 3
    call ESMF_Info2Get(info, "the-key-R8", value_R8_get, idx=i, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
    if (ABS(arr_R8(i) - value_R8_get) > tol) then
      failed = .true.
    exit
    end if
  end do

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Index I4"
  write(failMsg, *) "Comparison to array index get failed for I4"
  failed = .false.

  do i=1, 3
    call ESMF_Info2Get(info, "the-key-I4", value_I4_get, idx=i, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
    if (arr_I4(i) /= value_I4_get) then
      failed = .true.
    exit
    end if
  end do

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Index I8"
  write(failMsg, *) "Comparison to array index get failed for I8"
  failed = .false.

  do i=1, 3
    call ESMF_Info2Get(info, "the-key-I8", value_I8_get, idx=i, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
    if (arr_I8(i) /= value_I8_get) then
      failed = .true.
    exit
    end if
  end do

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Logical"
  write(failMsg, *) "Info logical array operation failed"
  failed = .false.

  desired_logical(1) = .true.
  desired_logical(2) = .false.
  desired_logical(3) = .false.
  desired_logical(4) = .true.
  desired_logical(5) = .true.

  call ESMF_Info2Set(info, "logicalkey", desired_logical, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !call ESMF_Info2Print(info, rc=rc)
  !if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "logicalkey", actual_logical, logical_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  do i=1,logical_count
    if (desired_logical(i) .neqv. actual_logical(i)) then
      failed = .true.
      exit
    endif
  enddo
  deallocate(actual_logical)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Setting/Getting Character"
  write(failMsg, *) "Info character array operation failed"
  failed = .false.

  desired_char(1) = "my"
  desired_char(2) = "country"
  desired_char(3) = ""
  desired_char(4) = "sweet land"
  desired_char(5) = "of the liberty"

  call ESMF_Info2Set(info, "charkey", desired_char, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !call ESMF_Info2Print(info, rc=rc)
  !if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Info2Get(info, "charkey", actual_char, charcount, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1,charcount
    if (desired_char(i) /= actual_char(i)) then
      failed = .true.
      exit
    endif
  enddo

  deallocate(actual_char)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Bad Set-By-Index"
  write(failMsg, *) "Info set by index error not handled"
  failed = .false.

  call ESMF_Info2Set(info, "logicalkey", .false., idx=55, rc=rc)
  call ESMF_Test((rc/=ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Bad Set-By-Index Type"
  write(failMsg, *) "Info set by index error not handled"
  failed = .false.

  call ESMF_Info2Set(info, "logicalkey", 55, idx=1, rc=rc)
  call ESMF_Test((rc==ESMC_RC_OBJ_BAD), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Info2 Array Bad Key Overload Type"
  write(failMsg, *) "Info set error not handled"
  failed = .false.

  call ESMF_Info2Set(info, "charkey", 55, rc=rc)
  !if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  call ESMF_Test((rc==ESMC_RC_ARG_BAD), name, failMsg, result, ESMF_SRCLINE)

  !call ESMF_Info2Print(info, rc=rc)
  !----------------------------------------------------------------------------

  ! Destroy the info object used by the array set/get tests
  call ESMF_Info2Destroy(info, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_Info2ArrayUTest