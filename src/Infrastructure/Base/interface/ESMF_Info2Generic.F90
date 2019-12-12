!------------------------------------------------------------------------------
! Get (Scalar) ----------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetR4()"
subroutine ESMF_Info2GetR4(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
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
end subroutine ESMF_Info2GetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetR8()"
subroutine ESMF_Info2GetR8(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
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
end subroutine ESMF_Info2GetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetI4()"
subroutine ESMF_Info2GetI4(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
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
end subroutine ESMF_Info2GetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetI8()"
subroutine ESMF_Info2GetI8(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
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
end subroutine ESMF_Info2GetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetLG()"
subroutine ESMF_Info2GetLG(info, key, value, default, idx, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
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
value = local_value  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_Info2GetLG

!------------------------------------------------------------------------------
! GetArray --------------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayR4()"
subroutine ESMF_Info2GetArrayR4(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayR4Allocated()"
subroutine ESMF_Info2GetArrayR4Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayR4Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayR8()"
subroutine ESMF_Info2GetArrayR8(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayR8Allocated()"
subroutine ESMF_Info2GetArrayR8Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayR8Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayI4()"
subroutine ESMF_Info2GetArrayI4(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayI4Allocated()"
subroutine ESMF_Info2GetArrayI4Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayI4Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayI8()"
subroutine ESMF_Info2GetArrayI8(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:),  allocatable, intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayI8Allocated()"
subroutine ESMF_Info2GetArrayI8Allocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayI8Allocated

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayLG()"
subroutine ESMF_Info2GetArrayLG(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  logical, dimension(:),  allocatable, intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayLG

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2GetArrayLGAllocated()"
subroutine ESMF_Info2GetArrayLGAllocated(info, key, values, itemcount, attnestflag, rc)
  type(ESMF_Info2), intent(in) :: info
  character(len=*), intent(in) :: key
  logical, dimension(:), intent(out) :: values
  integer, intent(out) :: itemcount !tdk:todo: make this optional
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
  call ESMF_Info2Inquire(info, key=key, size=itemcount, attnestflag=attnestflag, rc=localrc)
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
end subroutine ESMF_Info2GetArrayLGAllocated

!------------------------------------------------------------------------------
! Set (Scalar) ----------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetR4()"
subroutine ESMF_Info2SetR4(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetR8()"
subroutine ESMF_Info2SetR8(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetI4()"
subroutine ESMF_Info2SetI4(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetI8()"
subroutine ESMF_Info2SetI8(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetCH()"
subroutine ESMF_Info2SetCH(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  character(len=*), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetLG()"
subroutine ESMF_Info2SetLG(info, key, value, force, idx, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
  character(len=*), intent(in) :: key
  logical, intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  character(len=*), intent(in), optional :: pkey
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force, local_value  integer(C_INT), target :: local_idx
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
end subroutine ESMF_Info2SetLG

!------------------------------------------------------------------------------
! SetArray --------------------------------------------------------------------
!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayR4()"
subroutine ESMF_Info2SetArrayR4(info, key, values, force, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayR8()"
subroutine ESMF_Info2SetArrayR8(info, key, values, force, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayI4()"
subroutine ESMF_Info2SetArrayI4(info, key, values, force, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayI8()"
subroutine ESMF_Info2SetArrayI8(info, key, values, force, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_Info2SetArrayLG()"
subroutine ESMF_Info2SetArrayLG(info, key, values, force, pkey, rc)
  type(ESMF_Info2), intent(inout) :: info
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
end subroutine ESMF_Info2SetArrayLG

