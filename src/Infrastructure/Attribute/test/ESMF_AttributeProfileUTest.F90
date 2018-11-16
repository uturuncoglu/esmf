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

#define FILENAME "src/Infrastructure/Attribute/test/ESMF_AttributeProfileUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================
!==============================================================================
!==============================================================================

program ESMF_AttributesUTest

  !============================================================================
  !BOP
  ! !PROGRAM: ESMF_AttributeProfileUTest -  Profiles Attribute code
  !
  ! !DESCRIPTION:
  !
  !----------------------------------------------------------------------------
  ! !USES:
  use ESMF_TestMod     ! test methods
  use ESMF

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

  character(ESMF_MAXSTR):: key
  integer               :: rc, ii, idx, rsize
  integer, dimension(:), allocatable :: seed
  real                  :: r
  integer(ESMF_KIND_I4) :: value
  type(ESMF_AttPack)    :: attrs
  integer, parameter    :: nkeys = 1000
  integer, parameter    :: ntests = 100000

  type(ESMF_LocStream)  :: ls
  character(ESMF_MAXSTR):: attrList(nkeys)

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  ls = ESMF_LocStreamCreate(42, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  ! Test creating and destroying a bunch of attributes
  rc = ESMF_FAILURE
  write(name, *) "ESMF_Attribute Profile Loop"
  write(failMsg, *) "Failure during profile loop test"

  do ii=1, nkeys
    write(attrList(ii), *) ii
  enddo

  call ESMF_AttributeAdd(ls, "ESMF", "General", attrList, &
                         count=nkeys, attpack=attrs, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  ! Set nkeys count of attributes key/value pairs.
  do ii=1, nkeys
    write(key, *) ii

    call ESMF_TraceRegionEnter("ESMF_Attribute::Set", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_AttributeSet(ls, key, ii, attrs, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("ESMF_Attribute::Set", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

  ! For the ntests test count, retrieve the attribute value.
  call random_seed(size=rsize)
  allocate(seed(rsize))
  seed(:) = 0
  call random_seed(put=seed)

  do ii=1, ntests
    call random_number(r)
    idx = ceiling(r*nkeys)

    write(key, *) idx

    call ESMF_TraceRegionEnter("ESMF_Attribute::Get", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_AttributeGet(ls, key, attrs, value, rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

    call ESMF_TraceRegionExit("ESMF_Attribute::Get", rc=rc)
    if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  end do

!  call ESMF_AttributesPrint(attrs)

  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_LocStreamDestroy(ls, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  deallocate(seed)

  call ESMF_Test((rc == ESMF_SUCCESS), name, failMsg, result, ESMF_SRCLINE)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesUTest
