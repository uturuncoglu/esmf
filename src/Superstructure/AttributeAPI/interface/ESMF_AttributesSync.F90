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

#define FILENAME "src/Superstructure/AttributeAPI/interface/ESMF_AttributesSync.F90"

#include "ESMF_Macros.inc"
#include "ESMF.h"

!==============================================================================
!==============================================================================
!==============================================================================

module ESMF_AttributesSyncMod

use ESMF_StateMod
use ESMF_StateItemMod
use ESMF_DistGridMod
use ESMF_FieldMod
use ESMF_FieldGetMod
use ESMF_FieldBundleMod
use ESMF_CompMod
use ESMF_GridCompMod
use ESMF_CplCompMod
use ESMF_SciCompMod
use ESMF_ArrayMod
use ESMF_ArrayBundleMod
use ESMF_AttributesMod
use ESMF_UtilTypesMod
use ESMF_GeomBaseMod
use ESMF_MeshMod
use ESMF_GridMod
use ESMF_XGridMod
use ESMF_XGridGetMod
use ESMF_LocStreamMod
use ESMF_RHandleMod
implicit none

type ESMF_Inquire
    type(ESMF_Attributes), pointer :: info
    logical :: addBaseAddress = .false.
    logical :: addObjectInfo = .false.
    logical :: is_initialized = .false.
contains
  procedure, private :: updateWithState, updateWithArray, updateWithArrayBundle, &
   updateWithField, updateWithFieldBundle, updateWithLocStream, updateWithGrid, &
   updateWithXGrid, updateWithMesh, updateWithRouteHandle, updateWithDistGrid, &
   updateWithGridComp, updateWithCplComp, updateWithSciComp
  generic :: Update => updateWithState, updateWithArray, updateWithArrayBundle, &
   updateWithField, updateWithFieldBundle, updateWithLocStream, updateWithGrid, &
   updateWithXGrid, updateWithMesh, updateWithRouteHandle, updateWithDistGrid, &
   updateWithGridComp, updateWithCplComp, updateWithSciComp
  procedure, private :: fillMembersState, fillMembersArrayBundle, fillMembersField, &
   fillMembersFieldBundle
  generic :: FillMembers => fillMembersState, fillMembersArrayBundle, fillMembersField, &
   fillMembersFieldBundle
  procedure, public, pass :: Destroy, Print
  procedure, public, nopass :: Create
  procedure, private :: updateGeneric
end type ESMF_Inquire

contains

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%Create()"
function Create(addBaseAddress, addObjectInfo, rc) result(newinq)
  logical, intent(in), optional :: addBaseAddress
  logical, intent(in), optional :: addObjectInfo
  integer, intent(inout), optional :: rc
  type(ESMF_Inquire) :: newinq
  integer :: localrc=ESMF_FAILURE
  type(ESMF_Attributes), target, save :: info

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  if (present(addBaseAddress)) newinq%addBaseAddress = addBaseAddress
  if (present(addObjectInfo)) newinq%addObjectInfo = addObjectInfo
  nullify(newinq%info)
  info = ESMF_AttributesCreate(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  newinq%info => info
  newinq%is_initialized = .true.

  if (present(rc)) rc = ESMF_SUCCESS
end function Create

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%Destroy()"
subroutine Destroy(self, rc)
  class(ESMF_Inquire), intent(inout) :: self
  integer, intent(inout), optional :: rc
  integer :: localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_RC_NOT_IMPL
  if (self%is_initialized) then
    call ESMF_AttributesDestroy(self%info, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  endif
  nullify(self%info)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine Destroy

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%Print()"
subroutine Print(self, rc)
  class(ESMF_Inquire), intent(in) :: self
  integer, intent(inout), optional :: rc
  integer :: localrc=ESMF_FAILURE
  if (present(rc)) rc = ESMF_RC_NOT_IMPL
  call ESMF_AttributesPrint(self%info, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  if (present(rc)) rc = ESMF_FAILURE
end subroutine Print

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%fillMembersState()"
subroutine fillMembersState(self, state, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_State), intent(in) :: state
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc

  type(ESMF_Array) :: array
  type(ESMF_ArrayBundle) :: arraybundle
  type(ESMF_Field) :: field
  type(ESMF_FieldBundle) :: fieldbundle
  type(ESMF_RouteHandle) :: rh
  type(ESMF_State) :: state_nested
  type(ESMF_StateItem_Flag), dimension(:), allocatable :: stateTypes
  character(len=ESMF_MAXSTR), dimension(:), allocatable :: stateNames
  character(len=ESMF_MAXSTR) :: name
  integer :: ii, jj, itemCount, localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_StateGet(state, itemCount=itemCount, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  allocate(stateTypes(itemCount), stateNames(itemCount))
  call ESMF_StateGet(state, itemTypeList=stateTypes, itemNameList=stateNames, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  do ii=1,itemCount
    select case (stateTypes(ii)%ot)
    case(ESMF_STATEITEM_ARRAY%ot)
      call ESMF_StateGet(state, trim(stateNames(ii)), array, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
      call self%Update(array, root_key, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    case(ESMF_STATEITEM_ARRAYBUNDLE%ot)
      call ESMF_StateGet(state, trim(stateNames(ii)), arraybundle, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
      call self%Update(arraybundle, root_key, rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    case(ESMF_STATEITEM_STATE%ot)
     call ESMF_StateGet(state, trim(stateNames(ii)), state_nested, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(state_nested, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case(ESMF_STATEITEM_FIELD%ot)
     call ESMF_StateGet(state, trim(stateNames(ii)), field, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(field, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case(ESMF_STATEITEM_FIELDBUNDLE%ot)
     call ESMF_StateGet(state, trim(stateNames(ii)), fieldbundle, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(fieldbundle, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case(ESMF_STATEITEM_ROUTEHANDLE%ot)
     call ESMF_StateGet(state, trim(stateNames(ii)), rh, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(rh, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    case default
      if (ESMF_LogFoundError(ESMF_RC_OBJ_NOT_CREATED, msg="Object type not supported for Inquire", &
       ESMF_CONTEXT, rcToReturn=rc)) return
    end select
  end do
  deallocate(stateTypes, stateNames)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine fillMembersState

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateGeneric()"
subroutine updateGeneric(self, root_key, name, etype, base, base_is_valid, uname, rc)
  class(ESMF_Inquire), intent(inout) :: self
  character(*), intent(in) :: root_key
  character(*), intent(in) :: name
  character(*), intent(in) :: etype
  type(ESMF_Base), intent(in) :: base
  logical, intent(in), optional :: base_is_valid
  character(:), allocatable, optional :: uname
  integer, intent(inout), optional :: rc
  integer :: id_base
  character(:), allocatable :: c_id_base, l_uname

  character(:), allocatable :: local_root_key
  integer :: localrc=ESMF_FAILURE
  logical :: l_base_is_valid
  type(ESMF_Attributes) :: object_info

  if (.not. self%is_initialized) then
    if (ESMF_LogFoundError(ESMF_RC_OBJ_NOT_CREATED, msg="ESMF_Inquire is not initialized", &
     ESMF_CONTEXT, rcToReturn=rc)) return
  endif

  if (present(rc)) rc = ESMF_RC_NOT_IMPL
  if (present(base_is_valid)) then
    l_base_is_valid = base_is_valid
  else
    l_base_is_valid = .true.
  end if

  if (l_base_is_valid) then
    call ESMF_BaseGetId(base, id_base, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    call itoa(id_base, c_id_base)
    allocate(character(len(c_id_base)+len(trim(name))+1)::l_uname)
    l_uname = c_id_base//"-"//trim(name)
    deallocate(c_id_base)
  else
    allocate(character(len(trim(name)))::l_uname)
    l_uname = trim(name)
  end if

  allocate(character(len(trim(root_key))+len(l_uname)+1)::local_root_key)
  local_root_key = trim(root_key)//"/"//l_uname

  call ESMF_AttributesSet(self%info, local_root_key//"/esmf_type", etype, force=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call ESMF_AttributesSet(self%info, local_root_key//"/base_is_valid", l_base_is_valid, force=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call ESMF_AttributesSetNULL(self%info, local_root_key//"/members", force=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (self%addBaseAddress) then
    call ESMF_AttributesSet(self%info, local_root_key//"/base_address", base%this%ptr, force=.false., rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  if (l_base_is_valid) then
    call ESMF_AttributesSet(self%info, local_root_key//"/base_id", id_base, force=.false., rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  else
    call ESMF_AttributesSetNULL(self%info, local_root_key//"/base_id", force=.false., rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  if (self%addObjectInfo) then
    if (l_base_is_valid) then
      object_info = ESMF_AttributesBaseGet(base)
      call ESMF_AttributesSet(self%info, local_root_key//"/info", object_info, force=.false., rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    else
      call ESMF_AttributesSetNULL(self%info, local_root_key//"/info", force=.false., rc=localrc)
      if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
    end if
  end if

  if (present(uname)) then
    allocate(character(len(l_uname))::uname)
    uname = l_uname
  end if

  deallocate(local_root_key, l_uname)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateGeneric

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithArray()"
subroutine updateWithArray(self, array, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_Array), intent(in) :: array
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  type(ESMF_Base) :: newbase
  type(ESMF_Pointer) :: this

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_ArrayGetThis(array, this, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  newbase%this = this

  call ESMF_ArrayGet(array, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, "Array", newbase, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithArray

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithArrayBundle()"
subroutine updateWithArrayBundle(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_ArrayBundle), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  type(ESMF_Base) :: newbase
  type(ESMF_Pointer) :: this
  character(*), parameter :: etype = "ArrayBundle"
  character(:), allocatable :: uname

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_ArrayBundleGetThis(target, this, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  newbase%this = this

  call ESMF_ArrayBundleGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, newbase, uname=uname, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%FillMembers(target, root_key//"/"//uname//"/members", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  deallocate(uname)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithArrayBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithDistGrid()"
subroutine updateWithDistGrid(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_DistGrid), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  integer :: localrc
  type(ESMF_Base) :: newbase
  type(ESMF_Pointer) :: this
  character(*), parameter :: etype = "DistGrid"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_DistGridGetThis(target, this, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  newbase%this = this

  call self%updateGeneric(root_key, "__DistGrid__", etype, newbase, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithDistGrid

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithState()"
subroutine updateWithState(self, state, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_State), intent(in) :: state
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(:), allocatable :: uname

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_StateGet(state, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, "State", state%statep%base, uname=uname, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%FillMembers(state, root_key//"/"//uname//"/members", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  deallocate(uname)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithState

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithField()"
subroutine updateWithField(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_Field), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="Field"
  character(:), allocatable :: uname

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_FieldGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%ftypep%base, uname=uname, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%FillMembers(target, root_key//"/"//uname//"/members", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  deallocate(uname)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithField

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithGridComp()"
subroutine updateWithGridComp(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_GridComp), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="GridComp"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_GridCompGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%compp%base, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithGridComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithCplComp()"
subroutine updateWithCplComp(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_CplComp), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="CplComp"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_CplCompGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%compp%base, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithCplComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithSciComp()"
subroutine updateWithSciComp(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_SciComp), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="SciComp"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_SciCompGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%compp%base, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithSciComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithLocStream()"
subroutine updateWithLocStream(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_LocStream), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="LocStream"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_LocStreamGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%lstypep%base, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithLocStream

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithGrid()"
subroutine updateWithGrid(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_Grid), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="Grid"
  type(ESMF_Base) :: newbase

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_GridGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  newbase%this = target%this
  call self%updateGeneric(root_key, name, etype, newbase, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithGrid

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithXGrid()"
subroutine updateWithXGrid(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_XGrid), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="XGrid"

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_XGridGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%xgtypep%base, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithXGrid

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithMesh()"
subroutine updateWithMesh(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_Mesh), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="Mesh"
  type(ESMF_Base) :: newbase

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  ! call ESMF_MeshGet(target, name=name, rc=localrc)
  ! if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  newbase%this = -1
  call self%updateGeneric(root_key, "__invalid_mesh_base__", etype, newbase, base_is_valid=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithMesh

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithRouteHandle()"
subroutine updateWithRouteHandle(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_RouteHandle), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype="RouteHandle"
  type(ESMF_Base) :: newbase

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_RouteHandleGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call ESMF_RouteHandleGetThis(target, newbase%this, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, newbase, base_is_valid=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithRouteHandle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%updateWithFieldBundle()"
subroutine updateWithFieldBundle(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_FieldBundle), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  character(ESMF_MAXSTR) :: name
  integer :: localrc
  character(*), parameter :: etype = "FieldBundle"
  character(:), allocatable :: uname

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_FieldBundleGet(target, name=name, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%updateGeneric(root_key, name, etype, target%this%base, uname=uname, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call self%FillMembers(target, root_key//"/"//uname//"/members", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  deallocate(uname)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine updateWithFieldBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%fillMembersArrayBundle()"
subroutine fillMembersArrayBundle(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_ArrayBundle), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  type(ESMF_Array), dimension(:), allocatable :: targetList
  integer :: ii, targetCount, localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_ArrayBundleGet(target, arrayCount=targetCount, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  allocate(targetList(targetCount))

  call ESMF_ArrayBundleGet(target, arrayList=targetList, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  do ii=1,targetCount
    call self%Update(targetList(ii), root_key, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end do

  deallocate(targetList)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine fillMembersArrayBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%fillMembersFieldBundle()"
subroutine fillMembersFieldBundle(self, target, root_key, rc)
  class(ESMF_Inquire), intent(inout) :: self
  type(ESMF_FieldBundle), intent(in) :: target
  character(*), intent(in) :: root_key
  integer, intent(inout), optional :: rc
  type(ESMF_Field), dimension(:), allocatable :: targetList
  integer :: ii, targetCount, localrc=ESMF_FAILURE

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_FieldBundleGet(target, fieldCount=targetCount, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  allocate(targetList(targetCount))

  call ESMF_FieldBundleGet(target, fieldList=targetList, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  do ii=1,targetCount
    call self%Update(targetList(ii), root_key, rc=localrc)
    if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  end do

  deallocate(targetList)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine fillMembersFieldBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Inquire%fillMembersField()"
subroutine fillMembersField(self, field, root_key, rc)
 class(ESMF_Inquire), intent(inout) :: self
 type(ESMF_Field), intent(in) :: field
 character(*), intent(in) :: root_key
 integer, intent(inout), optional :: rc
 type(ESMF_GeomType_Flag) :: geomtype
 type(ESMF_Grid) :: grid
 type(ESMF_LocStream) :: locstream
 type(ESMF_XGrid) :: xgrid
 type(ESMf_Mesh) :: mesh
 integer :: localrc=ESMF_FAILURE

 if (present(rc)) rc = ESMF_RC_NOT_IMPL

 call ESMF_FieldGet(field, geomtype=geomtype, rc=localrc)
 if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

 select case (geomtype%type)
   case (ESMF_GEOMTYPE_GRID%type)
     call ESMF_FieldGet(field, grid=grid, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(grid, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case (ESMF_GEOMTYPE_LOCSTREAM%type)
     call ESMF_FieldGet(field, locstream=locstream, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(locstream, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case (ESMF_GEOMTYPE_XGRID%type)
     call ESMF_FieldGet(field, xgrid=xgrid, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(xgrid, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case (ESMF_GEOMTYPE_MESH%type)
     call ESMF_FieldGet(field, mesh=mesh, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
     call self%Update(mesh, root_key, rc=localrc)
     if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
   case default
     if (ESMF_LogFoundError(ESMF_RC_OBJ_NOT_CREATED, msg="Geometry type not supported for Inquire", &
      ESMF_CONTEXT, rcToReturn=rc)) return
 end select

 if (present(rc)) rc = ESMF_SUCCESS

end subroutine fillMembersField

subroutine itoa(i, res)
  ! https://stackoverflow.com/questions/1262695/convert-integers-to-strings-to-create-output-filenames-at-run-time
  character(:), allocatable :: res
  integer,intent(in) :: i
  character(:), allocatable :: tmp

  allocate(character(range(i)+2)::tmp)
  write(tmp,'(i0)') i
  allocate(character(len(trim(tmp)))::res)
  res = trim(tmp)
  deallocate(tmp)
end subroutine

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesStateSync()"
subroutine ESMF_AttributesStateSync(state, rootPet, rc)
  !tdk:doc
  type(ESMF_State), intent(inout) :: state
  integer, intent(in) :: rootPet
  type(ESMF_Inquire) :: einq
  integer, intent(inout), optional :: rc
  integer :: localrc

  if (present(rc)) rc = ESMF_RC_NOT_IMPL
  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  call einq%Update(state, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  call c_attrs_base_sync(einq%info%ptr, rootPet, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesStateSync

end module ESMF_AttributesSyncMod
