
#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetR4()"
subroutine ESMF_AttributesGetR4(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(inout) :: value
  real(ESMF_KIND_R4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_FLOAT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  ! Call C ####################################################################

  call c_attrs_get_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetR8()"
subroutine ESMF_AttributesGetR8(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(inout) :: value
  real(ESMF_KIND_R8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  real(C_DOUBLE), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  ! Call C ####################################################################

  call c_attrs_get_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetI4()"
subroutine ESMF_AttributesGetI4(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(inout) :: value
  integer(ESMF_KIND_I4), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_INT), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  ! Call C ####################################################################

  call c_attrs_get_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetI8()"
subroutine ESMF_AttributesGetI8(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(inout) :: value
  integer(ESMF_KIND_I8), intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  integer(C_LONG), target :: local_default
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  ! Call C ####################################################################

  call c_attrs_get_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetLG()"
subroutine ESMF_AttributesGetLG(attrs, key, value, default, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(inout) :: value
  logical, intent(in), optional :: default
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL), target :: local_default
  logical(C_BOOL) :: local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_default_ptr, local_idx_ptr

  ! Set up local return code
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

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

  ! Call C ####################################################################

  call c_attrs_get_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    localrc, &
    local_default_ptr, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  value = local_value
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetLG


!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayR4()"
subroutine ESMF_AttributesGetArrayR4(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayR8()"
subroutine ESMF_AttributesGetArrayR8(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayI4()"
subroutine ESMF_AttributesGetArrayI4(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayI8()"
subroutine ESMF_AttributesGetArrayI8(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(values(nelements))
  nelements_only = 0
  call c_attrs_get_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesGetArrayLG()"
subroutine ESMF_AttributesGetArrayLG(attrs, key, values, nelements, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, dimension(:), allocatable, intent(inout) :: values
  integer, intent(inout) :: nelements
  integer, intent(inout), optional :: rc

  integer :: localrc, nelements_only
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  ! Get the array size from the attributes store
  nelements_only = 1
  call c_attrs_get_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return

  ! Allocate the outgoing storage array and call into C to fill the array
  allocate(local_values(nelements))
  nelements_only = 0
  call c_attrs_get_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    nelements, &
    nelements_only, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  allocate(values(nelements))
  do ii=1,SIZE(values)
    values(ii) = local_values(ii)
  enddo
  deallocate(local_values)
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesGetArrayLG

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetR4()"
subroutine ESMF_AttributesSetR4(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  call c_attrs_set_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetR8()"
subroutine ESMF_AttributesSetR8(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  call c_attrs_set_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetI4()"
subroutine ESMF_AttributesSetI4(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  call c_attrs_set_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetI8()"
subroutine ESMF_AttributesSetI8(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  call c_attrs_set_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetCH()"
subroutine ESMF_AttributesSetCH(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  character(len=*), intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  call c_attrs_set_CH(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    trim(value)//C_NULL_CHAR, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetCH

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetLG()"
subroutine ESMF_AttributesSetLG(attrs, key, value, force, idx, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, intent(in) :: value
  logical, intent(in), optional :: force
  integer, intent(in), optional :: idx
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force, local_value
  integer(C_INT), target :: local_idx
  type(C_PTR) :: local_idx_ptr

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

  local_value = value
  call c_attrs_set_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_value, &
    local_force, &
    localrc, &
    local_idx_ptr)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
      rcToReturn=rc)) return

  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetLG

!------------------------------------------------------------------------------

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayR4()"
subroutine ESMF_AttributesSetArrayR4(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R4), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  call c_attrs_set_array_R4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayR4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayR8()"
subroutine ESMF_AttributesSetArrayR8(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  real(ESMF_KIND_R8), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  call c_attrs_set_array_R8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayR8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayI4()"
subroutine ESMF_AttributesSetArrayI4(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I4), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  call c_attrs_set_array_I4(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayI4

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayI8()"
subroutine ESMF_AttributesSetArrayI8(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  integer(ESMF_KIND_I8), dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  call c_attrs_set_array_I8(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayI8

#undef  ESMF_METHOD
#define ESMF_METHOD "ESMF_AttributesSetArrayLG()"
subroutine ESMF_AttributesSetArrayLG(attrs, key, values, force, rc)
  type(ESMF_Attributes), intent(inout) :: attrs
  character(len=*), intent(in) :: key
  logical, dimension(:), intent(in) :: values
  logical, intent(in), optional :: force
  integer, intent(inout), optional :: rc

  integer :: localrc
  logical(C_BOOL) :: local_force
  integer :: ii
  logical(C_BOOL), dimension(:), allocatable :: local_values
  
  localrc = ESMF_FAILURE
  if (present(rc)) rc = ESMF_FAILURE

  if (present(force)) then
    local_force = force
  else
    local_force = .true.
  end if
  
  allocate(local_values(SIZE(values)))
  do ii=1,SIZE(values)
    local_values(ii) = values(ii)
  enddo
  call c_attrs_set_array_LG(&
    attrs%ptr, &
    trim(key)//C_NULL_CHAR, &
    local_values, &
    SIZE(values), &
    local_force, &
    localrc)
  if (ESMF_LogFoundError(localrc, ESMF_ERR_PASSTHRU, ESMF_CONTEXT, &
    rcToReturn=rc)) return
  deallocate(local_values)
  if (present(rc)) rc = ESMF_SUCCESS
end subroutine ESMF_AttributesSetArrayLG
