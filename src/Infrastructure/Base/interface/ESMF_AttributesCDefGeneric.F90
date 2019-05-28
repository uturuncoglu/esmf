interface

  subroutine c_attrs_get_R4(attrs, key, value, rc, default, idx, recursive) bind(C, name="ESMC_AttributesGetR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_R4

  subroutine c_attrs_get_R8(attrs, key, value, rc, default, idx, recursive) bind(C, name="ESMC_AttributesGetR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_R8

  subroutine c_attrs_get_I4(attrs, key, value, rc, default, idx, recursive) bind(C, name="ESMC_AttributesGetI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_I4

  subroutine c_attrs_get_I8(attrs, key, value, rc, default, idx, recursive) bind(C, name="ESMC_AttributesGetI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_I8

  subroutine c_attrs_get_LG(attrs, key, value, rc, default, idx, recursive) bind(C, name="ESMC_AttributesGetLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_LG

  !=============================================================================

  subroutine c_attrs_get_array_R4(attrs, key, values, nelements, rc, recursive) bind(C, name="ESMC_AttributesGetArrayR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, rc
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_array_R4

  subroutine c_attrs_get_array_R8(attrs, key, values, nelements, rc, recursive) bind(C, name="ESMC_AttributesGetArrayR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, rc
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_array_R8

  subroutine c_attrs_get_array_I4(attrs, key, values, nelements, rc, recursive) bind(C, name="ESMC_AttributesGetArrayI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, rc
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_array_I4

  subroutine c_attrs_get_array_I8(attrs, key, values, nelements, rc, recursive) bind(C, name="ESMC_AttributesGetArrayI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, rc
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_array_I8

  subroutine c_attrs_get_array_LG(attrs, key, values, nelements, rc, recursive) bind(C, name="ESMC_AttributesGetArrayLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, rc
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_attrs_get_array_LG

  !=============================================================================

  subroutine c_attrs_set_R4(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_R4

  subroutine c_attrs_set_R8(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_R8

  subroutine c_attrs_set_I4(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_I4

  subroutine c_attrs_set_I8(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_I8

  subroutine c_attrs_set_CH(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    character(C_CHAR), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_CH

  subroutine c_attrs_set_LG(attrs, key, value, force, rc, idx) bind(C, name="ESMC_AttributesSetLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: idx
  end subroutine c_attrs_set_LG

  !=============================================================================

  subroutine c_attrs_set_array_R4(attrs, key, values, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_R4

  subroutine c_attrs_set_array_R8(attrs, key, values, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_R8

  subroutine c_attrs_set_array_I4(attrs, key, values, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_I4

  subroutine c_attrs_set_array_I8(attrs, key, values, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_I8

  subroutine c_attrs_set_array_LG(attrs, key, values, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_LG

end interface