interface

!===============================================================================
! c_info_get ===================================================================
!===============================================================================

  subroutine c_info_get_R4(info, key, value, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(out) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_R4

  subroutine c_info_get_R8(info, key, value, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(out) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_R8

  subroutine c_info_get_I4(info, key, value, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(out) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_I4

  subroutine c_info_get_I8(info, key, value, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(out) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_I8

  subroutine c_info_get_LG(info, key, value, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(out) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_LG

  subroutine c_info_get_array_R4(info, key, value, itemcount, rc, recursive) bind(C, name="ESMC_Info2GetArrayR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(out) :: value
    integer(C_INT), intent(out) :: itemcount
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_array_R4

  subroutine c_info_get_array_R8(info, key, value, itemcount, rc, recursive) bind(C, name="ESMC_Info2GetArrayR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(out) :: value
    integer(C_INT), intent(out) :: itemcount
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_array_R8

  subroutine c_info_get_array_I4(info, key, value, itemcount, rc, recursive) bind(C, name="ESMC_Info2GetArrayI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(out) :: value
    integer(C_INT), intent(out) :: itemcount
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_array_I4

  subroutine c_info_get_array_I8(info, key, value, itemcount, rc, recursive) bind(C, name="ESMC_Info2GetArrayI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(out) :: value
    integer(C_INT), intent(out) :: itemcount
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_array_I8

  subroutine c_info_get_array_LG(info, key, value, itemcount, rc, recursive) bind(C, name="ESMC_Info2GetArrayLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), dimension(*), intent(out) :: value
    integer(C_INT), intent(out) :: itemcount
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: recursive
  end subroutine c_info_get_array_LG

!===============================================================================
! c_info_set ===================================================================
!===============================================================================

  subroutine c_info_set_R4(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_R4

  subroutine c_info_set_R8(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_R8

  subroutine c_info_set_I4(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_I4

  subroutine c_info_set_I8(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_I8

  subroutine c_info_set_CH(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    character(C_CHAR), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_CH

  subroutine c_info_set_LG(info, key, value, force, rc, idx, pkey) bind(C, name="ESMC_Info2SetLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_LG

  subroutine c_info_set_array_R4(info, key, value, itemcount, force, rc, pkey) bind(C, name="ESMC_Info2SetArrayR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(in) :: value
    integer(C_INT), intent(in) :: itemcount
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_array_R4

  subroutine c_info_set_array_R8(info, key, value, itemcount, force, rc, pkey) bind(C, name="ESMC_Info2SetArrayR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(in) :: value
    integer(C_INT), intent(in) :: itemcount
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_array_R8

  subroutine c_info_set_array_I4(info, key, value, itemcount, force, rc, pkey) bind(C, name="ESMC_Info2SetArrayI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(in) :: value
    integer(C_INT), intent(in) :: itemcount
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_array_I4

  subroutine c_info_set_array_I8(info, key, value, itemcount, force, rc, pkey) bind(C, name="ESMC_Info2SetArrayI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(in) :: value
    integer(C_INT), intent(in) :: itemcount
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_array_I8


  subroutine c_info_set_array_LG(info, key, value, itemcount, force, rc, pkey) bind(C, name="ESMC_Info2SetArrayLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), dimension(*), intent(in) :: value
    integer(C_INT), intent(in) :: itemcount
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    character(kind=C_CHAR), intent(in) :: pkey(*)
  end subroutine c_info_set_array_LG

end interface