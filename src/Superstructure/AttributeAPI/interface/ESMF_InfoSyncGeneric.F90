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

