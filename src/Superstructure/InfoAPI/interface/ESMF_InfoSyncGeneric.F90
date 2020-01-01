#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncCplComp()"
subroutine ESMF_InfoSyncCplComp(target, rootPet, vm, rc)
  type(ESMF_CplComp), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncCplComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncGridComp()"
subroutine ESMF_InfoSyncGridComp(target, rootPet, vm, rc)
  type(ESMF_GridComp), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncGridComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncSciComp()"
subroutine ESMF_InfoSyncSciComp(target, rootPet, vm, rc)
  type(ESMF_SciComp), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncSciComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncField()"
subroutine ESMF_InfoSyncField(target, rootPet, vm, rc)
  type(ESMF_Field), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncField

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncFieldBundle()"
subroutine ESMF_InfoSyncFieldBundle(target, rootPet, vm, rc)
  type(ESMF_FieldBundle), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncFieldBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSyncState()"
subroutine ESMF_InfoSyncState(target, rootPet, vm, rc)
  type(ESMF_State), intent(inout) :: target
  integer, intent(in) :: rootPet
  type(ESMF_VM), intent(in) :: vm
  integer, intent(inout), optional :: rc

  type(ESMF_Inquire) :: einq
  integer :: localrc
  type(ESMF_Pointer) :: eptr

  if (present(rc)) rc = ESMF_RC_NOT_IMPL

  call ESMF_VMGetThis(vm, eptr, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  einq = einq%Create(addBaseAddress=.true., addObjectInfo=.false., rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Update(target, "", rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call c_info_base_sync(einq%info%ptr, rootPet, eptr%ptr, localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  call einq%Destroy(rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSyncState

!------------------------------------------------------------------------------
! GetHandle -------------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleArray()"
function ESMF_InfoGetHandleArray(target, rc) result(info)
  type(ESMF_Array), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleArray

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleArrayBundle()"
function ESMF_InfoGetHandleArrayBundle(target, rc) result(info)
  type(ESMF_ArrayBundle), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleArrayBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleCplComp()"
function ESMF_InfoGetHandleCplComp(target, rc) result(info)
  type(ESMF_CplComp), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleCplComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleGridComp()"
function ESMF_InfoGetHandleGridComp(target, rc) result(info)
  type(ESMF_GridComp), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleGridComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleSciComp()"
function ESMF_InfoGetHandleSciComp(target, rc) result(info)
  type(ESMF_SciComp), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleSciComp

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleDistGrid()"
function ESMF_InfoGetHandleDistGrid(target, rc) result(info)
  type(ESMF_DistGrid), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleDistGrid

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleField()"
function ESMF_InfoGetHandleField(target, rc) result(info)
  type(ESMF_Field), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleField

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleFieldBundle()"
function ESMF_InfoGetHandleFieldBundle(target, rc) result(info)
  type(ESMF_FieldBundle), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleFieldBundle

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleGrid()"
function ESMF_InfoGetHandleGrid(target, rc) result(info)
  type(ESMF_Grid), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleGrid

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleState()"
function ESMF_InfoGetHandleState(target, rc) result(info)
  type(ESMF_State), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleState

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetHandleLocStream()"
function ESMF_InfoGetHandleLocStream(target, rc) result(info)
  type(ESMF_LocStream), intent(in) :: target
  integer, intent(inout), optional :: rc
  type(ESMF_Info) :: info

  integer :: localrc
  type(ESMF_Inquire) :: einq

  info = einq%GetInfo(target, rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return
end function ESMF_InfoGetHandleLocStream
