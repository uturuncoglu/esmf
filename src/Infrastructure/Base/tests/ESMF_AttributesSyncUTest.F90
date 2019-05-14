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

#define FILENAME "src/Infrastructure/Attribute/test/ESMF_AttributesSyncUTest.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================
!==============================================================================
!==============================================================================

program ESMF_AttributesSyncUTest

  !============================================================================
  !BOP
  ! !PROGRAM: ESMF_AttributesUTest - Test general Attributes usage
  !
  ! !DESCRIPTION:
  !
  !----------------------------------------------------------------------------
  ! !USES:
  use ESMF_TestMod     ! test methods
  use ESMF_UtilTypesMod     ! ESMF utility types
  use ESMF
  use ESMF_AttributesMod
  use ESMF_AttributesSyncMod

  implicit none

  !----------------------------------------------------------------------------
  ! The following line turns the CVS identifier string into a printable variable.
  character(*), parameter :: version = '$Id$'
  !----------------------------------------------------------------------------

  ! individual test failure message
  character(ESMF_MAXSTR) :: failMsg
  character(ESMF_MAXSTR) :: name
  ! cumulative result: count failures; no failures equals "all pass"
  integer               :: result = 0, count
  integer :: rc, petCount, localPet, n
  type(ESMF_VM) :: vm

  type(ESMF_Array) :: arr, arr2
  type(ESMF_ArrayBundle) :: ab
  type(ESMF_Pointer) :: eptr
  type(ESMF_Attributes) :: attrsp, desired_attrs
  type(ESMF_Field) :: field, field2, field3, field4
  type(ESMF_FieldBundle) :: fb
  type(ESMF_DistGrid) :: distgrid, distgrid1d
  type(ESMF_Grid) :: grid
  type(ESMF_LocStream) :: locstream
  type(ESMF_RouteHandle) :: rh
  type(ESMF_State) :: state, nested_state, nested_state2
  type(ESMF_Inquire) :: einq, desired_einq, ainq
  integer :: rootPet=0
  integer(ESMF_KIND_I8), dimension(:), allocatable :: bases

  !----------------------------------------------------------------------------
  call ESMF_TestStart(ESMF_SRCLINE, rc=rc)  ! calls ESMF_Initialize() internally
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  !----------------------------------------------------------------------------

  call ESMF_VMGetGlobal(vm, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  call ESMF_VMGet(vm, petCount=petCount, localPet=localPet, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  !NEX_UTest
  write(name, *) "ESMF_AttributesReconcile"
  write(failMsg, *) "Did not reconcile successfully"
  rc = ESMF_FAILURE

  distgrid = ESMF_DistGridCreate(minIndex=(/1,1/),maxIndex=(/10,10/), rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  grid = ESMF_GridCreate(distgrid=distgrid, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  distgrid1d = ESMF_DistGridCreate(minIndex=(/1/),maxIndex=(/10/), rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  locstream = ESMF_LocStreamCreate(distgrid=distgrid1d, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  field = ESMF_FieldCreate(grid, ESMF_TYPEKIND_I8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  field2 = ESMF_FieldCreate(grid, ESMF_TYPEKIND_I8, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  field3 = ESMF_FieldCreate(locstream, ESMF_TYPEKIND_I4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  fb = ESMF_FieldBundleCreate(fieldList=(/field,field2/), name="mr_field_bundle", &
   rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  arr = ESMF_ArrayCreate(distgrid, ESMF_TYPEKIND_I4, rc=rc, name="iarr_i4")
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  arr2 = ESMF_ArrayCreate(distgrid, ESMF_TYPEKIND_I8, rc=rc, name="iarr_i8")
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  ab = ESMF_ArrayBundleCreate(arrayList=(/arr,arr2/), name="my_arrays", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  rh = ESMF_RouteHandleCreate(rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
 call ESMF_RouteHandleSet(rh, name="dummy", rc=rc)
 if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------
  nested_state2 = ESMF_StateCreate(fieldbundleList=(/fb/), name="nested_state2", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  nested_state = ESMF_StateCreate(arrayList=(/arr, arr2/), routehandleList=(/rh/), &
   nestedStateList=(/nested_state2/), rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  state = ESMF_StateCreate(arrayList=(/arr,arr2/), arrayBundleList=(/ab/), fieldList=(/field3/), &
   fieldbundleList=(/fb/), nestedStateList=(/nested_state/), rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  !----------------------------------------------------------------------------

  if (localPet == rootPet) then
    ! attrsp = ESMF_AttributesBaseGet(state%statep%base)
    ! ainq = ainq%Create(createInfo=.false.)
    ! call ainq%Update(state, "")
    ! attrsp = ainq%GetCurrentInfo()
    attrsp = ainq%GetInfo(state)
    call ESMF_AttributesSet(attrsp, "fvarname", "state")

    attrsp = ainq%GetInfo(nested_state2)
    ! attrsp = ESMF_AttributesBaseGet(nested_state2%statep%base)
    call ESMF_AttributesSet(attrsp, "fvarname", "nested_state2")

    ! attrsp = ESMF_AttributesBaseGet(field%ftypep%base)
    attrsp = ainq%GetInfo(field)
    call ESMF_AttributesSet(attrsp, "fvarname", "field")

    attrsp = ainq%GetInfo(field2)
    ! attrsp = ESMF_AttributesBaseGet(field2%ftypep%base)
    call ESMF_AttributesSet(attrsp, "fvarname", "field2")

    ! attrsp = ESMF_AttributesBaseGet(field3%ftypep%base)
    attrsp = ainq%GetInfo(field3)
    call ESMF_AttributesSet(attrsp, "fvarname", "field3")

    ! call ESMF_ArrayGetThis(arr, eptr)
    ! attrsp = ESMF_AttributesPointerGet(eptr)
    attrsp = ainq%GetInfo(arr)
    call ESMF_AttributesSet(attrsp, "fvarname", "arr")

    ! call ESMF_ArrayBundleGetThis(ab, eptr)
    ! attrsp = ESMF_AttributesPointerGet(eptr)
    attrsp = ainq%GetInfo(ab)
    call ESMF_AttributesSet(attrsp, "fvarname", "ab")

    attrsp = ainq%GetInfo(locstream)
    ! attrsp = ESMF_AttributesBaseGet(locstream%lstypep%base)
    call ESMF_AttributesSet(attrsp, "fvarname", "locstream")
  end if

  if (localPet == 0) then
    desired_einq = desired_einq%Create(addObjectInfo=.true.)
    call desired_einq%Update(state, "")
    desired_attrs = desired_einq%info
  else
    desired_attrs = ESMF_AttributesCreate()
  end if
  call ESMF_AttributesBroadcast(desired_attrs, 0)

  call ESMF_AttributesStateSync(state, rootPet, rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)

  einq = einq%Create(addObjectInfo=.true., rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  call einq%Update(state, "", rc=rc)
  if (rc /= ESMF_SUCCESS) call ESMF_Finalize(endflag=ESMF_END_ABORT)
  ! call einq%Print() !tdk:p

  call ESMF_Test((einq%info == desired_attrs), name, failMsg, result, ESMF_SRCLINE)

  call einq%Destroy()
  if (localPet == 0) then
    call desired_einq%Destroy()
  else
    call ESMF_AttributesDestroy(desired_attrs)
  end if
  call ESMF_StateDestroy(state)
  call ESMF_StateDestroy(nested_state)
  call ESMF_StateDestroy(nested_state2)
  call ESMF_ArrayBundleDestroy(ab)
  call ESMF_ArrayDestroy(arr)
  call ESMF_ArrayDestroy(arr2)
  call ESMF_FieldBundleDestroy(fb)
  call ESMF_FieldDestroy(field)
  call ESMF_FieldDestroy(field2)
  call ESMF_FieldDestroy(field3)
  call ESMF_GridDestroy(grid)
  call ESMF_LocStreamDestroy(locstream)
  call ESMF_DistGridDestroy(distgrid)
  call ESMF_DistGridDestroy(distgrid1d)
  call ESMF_RouteHandleDestroy(rh)
  !----------------------------------------------------------------------------

  !----------------------------------------------------------------------------
  call ESMF_TestEnd(ESMF_SRCLINE) ! calls ESMF_Finalize() internally
  !----------------------------------------------------------------------------

end program ESMF_AttributesSyncUTest
