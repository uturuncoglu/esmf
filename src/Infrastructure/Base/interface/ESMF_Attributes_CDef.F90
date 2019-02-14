interface
  function c_attrs_create(rc) bind(C, name="ESMC_AttributesCreate")
    use iso_c_binding
    implicit none
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_create
  end function c_attrs_create

  !=============================================================================

  subroutine c_attrs_destroy(attrs, rc) bind(C, name="ESMC_AttributesDestroy")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_destroy

  !=============================================================================

  subroutine c_attrs_erase(attrs, keyParent, keyChild, rc) bind(C, name="ESMC_AttributesErase")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: keyParent(*)
    character(C_CHAR), intent(in) :: keyChild(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_erase

  !=============================================================================

  function c_attrs_is_present(attrs, key, isptr, rc) bind(C, name="ESMC_AttributesIsPresent")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: isptr
    integer(C_INT), intent(inout) :: rc
    integer(C_INT) :: c_attrs_is_present
  end function c_attrs_is_present

  !=============================================================================

  subroutine c_attrs_print(attrs, indent, rc) bind(C, name="ESMC_AttributesPrint")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(in) :: indent
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_print

  !=============================================================================

  subroutine c_attrs_get_C_FLOAT(attrs, key, value, rc, default) bind(C, name="ESMC_AttributesGet_C_FLOAT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
  end subroutine c_attrs_get_C_FLOAT

  function c_attrs_get_C_DOUBLE(attrs, key, rc, default) bind(C, name="ESMC_AttributesGet_C_DOUBLE")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
    real(C_DOUBLE) :: c_attrs_get_C_DOUBLE
  end function c_attrs_get_C_DOUBLE

  subroutine c_attrs_get_C_INT(attrs, key, value, rc, default) bind(C, name="ESMC_AttributesGet_C_INT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
  end subroutine c_attrs_get_C_INT

  function c_attrs_get_C_LONG(attrs, key, rc, default) bind(C, name="ESMC_AttributesGet_C_LONG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
    integer(C_LONG) :: c_attrs_get_C_LONG
  end function c_attrs_get_C_LONG

  subroutine c_attrs_get_C_CHAR(attrs, key, value, vlen, rc, default) bind(C, name="ESMC_AttributesGet_C_CHAR")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    character(kind=C_CHAR, len=1), dimension(vlen), intent(inout) :: value
    integer(C_INT) :: vlen
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
  end subroutine c_attrs_get_C_CHAR

  !=============================================================================

  subroutine c_attrs_get_array_C_FLOAT(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray_C_FLOAT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array_C_FLOAT

  subroutine c_attrs_get_array_C_DOUBLE(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray_C_DOUBLE")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array_C_DOUBLE

  subroutine c_attrs_get_array_C_INT(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray_C_INT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array_C_INT

  subroutine c_attrs_get_array_C_LONG(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray_C_LONG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array_C_LONG

  subroutine c_attrs_get_array_C_CHAR(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray_C_CHAR")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    character(C_CHAR), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array_C_CHAR

  !=============================================================================

  subroutine c_attrs_set_C_FLOAT(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet_C_FLOAT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_C_FLOAT

  subroutine c_attrs_set_C_DOUBLE(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet_C_DOUBLE")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_C_DOUBLE

  subroutine c_attrs_set_C_INT(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet_C_INT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_C_INT

  subroutine c_attrs_set_C_LONG(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet_C_LONG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_C_LONG

  subroutine c_attrs_set_C_CHAR(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet_C_CHAR")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    character(C_CHAR), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_C_CHAR

  !=============================================================================

  subroutine c_attrs_set_array_C_FLOAT(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray_C_FLOAT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_C_FLOAT

  subroutine c_attrs_set_array_C_DOUBLE(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray_C_DOUBLE")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_C_DOUBLE

  subroutine c_attrs_set_array_C_INT(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray_C_INT")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_C_INT

  subroutine c_attrs_set_array_C_LONG(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray_C_LONG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_C_LONG

  subroutine c_attrs_set_array_C_CHAR(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray_C_CHAR")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    character(C_CHAR), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_C_CHAR

  !=============================================================================

end interface
