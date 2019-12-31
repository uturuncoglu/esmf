!------------------------------------------------------------------------------
! Get (Scalar) ----------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetR4()"
subroutine ESMF_InfoGetR4(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(out) :: value
  real(ESMF_KIND_R4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_FLOAT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  call c_info_get_R4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetR8()"
subroutine ESMF_InfoGetR8(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(out) :: value
  real(ESMF_KIND_R8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_DOUBLE), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  call c_info_get_R8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetI4()"
subroutine ESMF_InfoGetI4(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(out) :: value
  integer(ESMF_KIND_I4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  call c_info_get_I4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetI8()"
subroutine ESMF_InfoGetI8(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(out) :: value
  integer(ESMF_KIND_I8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_LONG), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  call c_info_get_I8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetCH()"
subroutine ESMF_InfoGetCH(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), intent(out) :: value
  character(len=*), intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc, vlen
  character(:), allocatable, target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = trim(default)//C_NULL_CHAR
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  vlen = LEN(value)
  call c_info_get_CH(info%ptr, trim(key)//C_NULL_CHAR, value, vlen, &
    localrc, local_default_ptr, local_idx_ptr, recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetLG()"
subroutine ESMF_InfoGetLG(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  logical, intent(inout) :: value
  logical, intent(in), optional :: default
  integer, intent(in), optional :: idx
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL), target :: local_default
  logical(C_BOOL) :: local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr
  integer(C_INT) :: recursive

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  ! Handle optional arguments for C ###########################################

  if (present(default)) then
    local_default = default
    local_default_ptr = C_LOC(local_default)
  else
    local_default_ptr = C_NULL_PTR
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Call C ####################################################################

  call c_info_get_LG(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  value = local_value

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetLG

!------------------------------------------------------------------------------
! GetArray --------------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayR4()"
subroutine ESMF_InfoGetArrayR4(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  call c_info_get_array_R4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayR4Allocated()"
subroutine ESMF_InfoGetArrayR4Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  call c_info_get_array_R4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayR4Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayR8()"
subroutine ESMF_InfoGetArrayR8(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  call c_info_get_array_R8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayR8Allocated()"
subroutine ESMF_InfoGetArrayR8Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  call c_info_get_array_R8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayR8Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayI4()"
subroutine ESMF_InfoGetArrayI4(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  call c_info_get_array_I4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayI4Allocated()"
subroutine ESMF_InfoGetArrayI4Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  call c_info_get_array_I4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayI4Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayI8()"
subroutine ESMF_InfoGetArrayI8(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  call c_info_get_array_I8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayI8Allocated()"
subroutine ESMF_InfoGetArrayI8Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  call c_info_get_array_I8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayI8Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayCH()"
subroutine ESMF_InfoGetArrayCH(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive
  integer :: ii
  logical :: is_array

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    isArray=is_array, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (.not. is_array) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_WRONGTYPE, &
      msg="Array requested but type in JSON storage is not an array. Key is: "//TRIM(key), &
      ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  do ii=1,itemcount
    call ESMF_InfoGetCH(info, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayCHAllocated()"
subroutine ESMF_InfoGetArrayCHAllocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  character(len=*), dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive
  integer :: ii
  logical :: is_array

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    isArray=is_array, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (.not. is_array) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_WRONGTYPE, &
      msg="Array requested but type in JSON storage is not an array. Key is: "//TRIM(key), &
      ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  do ii=1,itemcount
    call ESMF_InfoGetCH(info, key, values(ii), idx=ii, attnestflag=attnestflag, &
      rc=localrc)
  enddo
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayCHAllocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayLG()"
subroutine ESMF_InfoGetArrayLG(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  logical, dimension(:),  allocatable, intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(itemcount))
  allocate(local_values(itemcount))
  call c_info_get_array_LG(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  do ii=1,SIZE(values)
    values(ii) = local_values(ii)
  enddo
  deallocate(local_values)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayLG

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoGetArrayLGAllocated()"
subroutine ESMF_InfoGetArrayLGAllocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info), intent(in) :: info
  character(len=*), intent(in) :: key
  logical, dimension(:), intent(out) :: values
  integer, intent(out), optional :: itemcount
  type(ESMF_AttNest_Flag), intent(in), optional :: attnestflag
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT) :: recursive
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE
  recursive = 0 !false

  if (present(attnestflag)) then
    if (attnestflag%value==ESMF_ATTNEST_ON%value) recursive = 1 !true
  end if

  ! Get the array size from the info store
  call ESMF_InfoInquire(info, key=key, size=itemcount, attnestflag=attnestflag, &
    rc=localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (itemcount /= SIZE(values)) then
    if (ESMF_LogFoundError(ESMF_RC_ATTR_ITEMSOFF, msg="values allocation size does not match size in Info storage", ESMF_CONTEXT, rcToReturn=rc)) return
  end if

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(local_values(itemcount))
  call c_info_get_array_LG(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    itemcount, &
    localrc, &
    recursive)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  do ii=1,SIZE(values)
    values(ii) = local_values(ii)
  enddo
  deallocate(local_values)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoGetArrayLGAllocated

!------------------------------------------------------------------------------
! Set (Scalar) ----------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetR4()"
subroutine ESMF_InfoSetR4(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_R4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetR8()"
subroutine ESMF_InfoSetR8(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_R8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetI4()"
subroutine ESMF_InfoSetI4(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_I4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetI8()"
subroutine ESMF_InfoSetI8(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_I8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetCH()"
subroutine ESMF_InfoSetCH(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_CH(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    trim(value)//C_NULL_CHAR, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetLG()"
subroutine ESMF_InfoSetLG(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  logical, intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force, local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr
  character(:), allocatable :: local_pkey

  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(idx)) then
    local_idx = idx - 1  ! Shift to C (zero-based) indexing
    local_idx_ptr = C_LOC(local_idx)
  else
    local_idx_ptr = C_NULL_PTR
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  local_value = value
  call c_info_set_LG(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    local_force, &
    localrc, &
    local_idx_ptr, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetLG

!------------------------------------------------------------------------------
! SetArray --------------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayR4()"
subroutine ESMF_InfoSetArrayR4(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  character(:), allocatable :: local_pkey
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_array_R4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayR8()"
subroutine ESMF_InfoSetArrayR8(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  character(:), allocatable :: local_pkey
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_array_R8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayI4()"
subroutine ESMF_InfoSetArrayI4(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  character(:), allocatable :: local_pkey
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_array_I4(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayI8()"
subroutine ESMF_InfoSetArrayI8(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  character(:), allocatable :: local_pkey
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  call c_info_set_array_I8(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_InfoSetArrayLG()"
subroutine ESMF_InfoSetArrayLG(info, key, values, force, pkey, rc)
  type(ESMF_Info), intent(inout) :: info
  character(len=*), intent(in) :: key
  logical, dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values
  character(:), allocatable :: local_pkey
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  if (present(pkey)) then
    local_pkey = TRIM(pkey)//C_NULL_CHAR
  else
    local_pkey = ""//C_NULL_CHAR
  end if

  allocate(local_values(SIZE(values)))
  do ii=1,SIZE(values)
    local_values(ii) = values(ii)
  enddo

  call c_info_set_array_LG(&
    info%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    SIZE(values), &
    local_force, &
    localrc, &
    local_pkey)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, rcToReturn=rc)) return

  deallocate(local_values)

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_InfoSetArrayLG
