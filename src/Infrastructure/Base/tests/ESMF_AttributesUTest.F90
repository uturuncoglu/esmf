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

!==============================================================================
!==============================================================================
!==============================================================================

program ESMF_AttributesUTest

  !============================================================================
  !BOP
  ! !PROGRAM: ESMF_AttributesUTest - Test general Attributes usage
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
  character(*), parameter :: version = '$Id$'
  !----------------------------------------------------------------------------

  ! individual test failure message
  character(ESMF_MAXSTR) :: failMsg
  character(ESMF_MAXSTR) :: name

  character(ESMF_MAXSTR) :: key, key_i8, key_r4, key_r8
  character(len=55) :: key_char, value_char, desired_char, def_desired_char, &
                       def_key_char
  character(len=33) :: def_value_char
  character(len=22) :: key_empty_char
  character(len=2) :: desired_empty_char, empty_value_char
  integer               :: rc, petCount, i
  integer, allocatable  :: petList(:)
  type(ESMF_VM)         :: vm
  type(ESMF_GridComp)   :: gcomp
  ! cumulative result: count failures; no failures equals "all pass"
  integer               :: result = 0

  integer(ESMF_KIND_I4) :: value, actual, actual2, actual3, arr_i4_get_count
  integer(ESMF_KIND_I8) :: desired_i8, value_i8
  real(ESMF_KIND_R4) :: desired_r4, value_r4
  real(ESMF_KIND_R8) :: desired_r8, value_r8
  integer(ESMF_KIND_I4), dimension(3) :: arr_i4
  integer(ESMF_KIND_I4), dimension(:), allocatable :: arr_i4_get
  type(ESMF_Attributes) :: attrs, attrs2, attrs3, attrs4, attrs5, attrs6, &
                           attrs7, attrs8, attrs9, attrs10, attrs_copy_src, &
                           attrs_copy_dst

  logical :: is_present, failed, is_set, is_present_copy_test

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
  write(name, *) "ESMF_AttributesCreate"
  write(failMsg, *) "Did not create Attributes"
  rc = ESMF_FAILURE

  attrs = ESMF_AttributesCreate(rc=rc)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesDestroy"
  write(failMsg, *) "Did not destroy Attributes"
  rc = ESMF_FAILURE;

  call ESMF_AttributesDestroy(attrs, rc=rc)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesSet"
  write(failMsg, *) "Did not set key"
  rc = ESMF_FAILURE

  attrs2 = ESMF_AttributesCreate(rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  key = "testKey"
  actual = 333
  call ESMF_AttributesSet(attrs2, key, actual, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  key_i8 = "testKeyI8"
  desired_i8 = 92233720
  call ESMF_AttributesSet(attrs2, key_i8, desired_i8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  key_r4 = "testKeyR4"
  desired_r4 = 333.0 + (1.0/3.0)
  call ESMF_AttributesSet(attrs2, key_r4, desired_r4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  key_r8 = "testKeyR8"
  desired_r8 = 1.797693e-10
  call ESMF_AttributesSet(attrs2, key_r8, desired_r8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  key_char = "testKeyChar"
  desired_char = "i am char"
  call ESMF_AttributesSet(attrs2, key_char, desired_char, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetI4"
  write(failMsg, *) "Did not get key"

  call ESMF_AttributesGet(attrs2, key, value, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((value == actual), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetI8"
  write(failMsg, *) "Did not get key"

  call ESMF_AttributesGet(attrs2, key_i8, value_i8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((value_i8 == desired_i8), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetR4"
  write(failMsg, *) "Did not get key"

  call ESMF_AttributesGet(attrs2, key_r4, value_r4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((ABS(value_r4 - desired_r4) < 1e-16), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetR8"
  write(failMsg, *) "Did not get key"

  call ESMF_AttributesGet(attrs2, key_r8, value_r8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((ABS(value_r8 - desired_r8) < 1e-16), name, failMsg, result, ESMF_SRCLINE)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetCH with Default"
  write(failMsg, *) "Did not get default key"

  def_desired_char = "this is default!"
  def_key_char = "does not exist in attrs"
  call ESMF_AttributesGet(attrs2, def_key_char, def_value_char, default=def_desired_char, &
    rc=rc)

  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  call ESMF_Test((def_value_char==def_desired_char), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesSet/Get Empty Character"
  write(failMsg, *) "Did not set/get empty string key"

  key_empty_char = "so empty inside"
  desired_empty_char = ""
  call ESMF_AttributesSet(attrs2, key_empty_char, desired_empty_char, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGet(attrs2, key_empty_char, empty_value_char, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((desired_empty_char==empty_value_char), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesGetCH"
  write(failMsg, *) "Did not get key"

  call ESMF_AttributesGet(attrs2, key_char, value_char, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((desired_char==value_char), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs2, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  ! Test we can overload the value if force is true
  write(name, *) "ESMF_AttributesSet Force Flag"
  write(failMsg, *) "Could not overload value"
  rc = ESMF_FAILURE

  attrs3 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

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
  !NEX_UTest
  ! Test get with a default value
  write(name, *) "ESMF_AttributesGet with Default Value"
  write(failMsg, *) "Did not get default value"
  rc = ESMF_FAILURE

  attrs5 = ESMF_AttributesCreate(rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGet(attrs5, "doesNotExist", actual2, rc=rc, default=5897)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((actual2 == 5897), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs5, rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesPrint"
  write(failMsg, *) "Print somehow not successful"

  rc = ESMF_FAILURE

  attrs6 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs6, "/i/am/nested", 111, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs6, "top-level", 222, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  print *, ""
  print *, "===== ESMF_AttributesPrint Test Start ====="
  print *, ""

  call ESMF_AttributesPrint(attrs6, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesPrint(attrs6, indent=1, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  print *, ""
  print *, "===== ESMF_AttributesPrint Test End ====="
  print *, ""

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs6, rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesErase Child From Parent"
  write(failMsg, *) "Child not erased from parent"

  rc = ESMF_FAILURE

  attrs7 = ESMF_AttributesCreate(rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs7, "this/is/erase/test", 111, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesErase(attrs7, "this/is/erase", keyChild="test", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGet(attrs7, "/this/is/erase/test", actual3, &
                          default=-999, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((actual3 == -999), name, failMsg, result, ESMF_SRCLINE)

!  call ESMF_AttributesPrint(attrs7, rc=rc)
!  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesErase Root"
  write(failMsg, *) "Did not erase from root"

  call ESMF_AttributesErase(attrs7, "this", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

!  call ESMF_AttributesPrint(attrs7, rc=rc)
!  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGet(attrs7, "this", actual3, default=-888, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((actual3 == -888), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs7, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesIsPresent False"
  write(failMsg, *) "Attribute key should not be present"

  attrs8 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_present = ESMF_AttributesIsPresent(attrs8, "this", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((.not. is_present), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesIsPresent True"
  write(failMsg, *) "Attribute key is actually present"

  attrs8 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs8, "this", 11, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_present = ESMF_AttributesIsPresent(attrs8, "this", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((is_present), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesIsPresent True w/ Pointer"
  write(failMsg, *) "Attribute key is present using pointer syntax"

  attrs8 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs8, "/this/is/nested", 11, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_present = ESMF_AttributesIsPresent(attrs8, "/this/is/nested", &
    isPointer=.true., rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((is_present), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_Attributes Array Setting/Getting"
  write(failMsg, *) "Array operations failed"
  failed = .false.

  arr_i4(1) = 123
  arr_i4(2) = 456
  arr_i4(3) = 789

  attrs9 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs9, "the-key", arr_i4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesGet(attrs9, "the-key", arr_i4_get, arr_i4_get_count, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  do i=1, 3
    if (arr_i4(i) /= arr_i4_get(i)) then
      failed = .true.
    end if
  end do

  deallocate(arr_i4_get)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)

  call ESMF_AttributesDestroy(attrs9, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesSetNULL and ESMF_AttributesIsSet"
  write(failMsg, *) "Did not return ESMF_SUCCESS"
  failed = .false.

  attrs10 = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs10, "is-the-null", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_set = ESMF_AttributesIsSet(attrs10, "is-the-null", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  if (is_set) then
    failed = .true.
  end if

  call ESMF_AttributesSet(attrs10, "is-the-null", 5, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_set = ESMF_AttributesIsSet(attrs10, "is-the-null", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  if (.not. is_set) then
    failed = .true.
  end if

!  call ESMF_AttributesPrint(attrs10, rc=rc) !tdk:p

  call ESMF_AttributesDestroy(attrs10, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((.not. failed), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesCopy"
  write(failMsg, *) "Did not copy Attributes"
  rc = ESMF_FAILURE

  attrs_copy_src = ESMF_AttributesCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  attrs_copy_dst = ESMF_AttributesCopy(attrs_copy_src, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_AttributesSet(attrs_copy_dst, "a-key", 22, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  is_present_copy_test = ESMF_AttributesIsPresent(attrs_copy_src, "a-key", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_Test((is_present_copy_test .eqv. .false.), name, failMsg, result, ESMF_SRCLINE)

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesUTest
