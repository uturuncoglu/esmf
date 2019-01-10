interface
  function c_attrs_create(rc) bind(C, name="ESMC_AttributesCreate")
    use iso_c_binding
    implicit none
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_create
  end function c_attrs_create

  subroutine c_attrs_destroy(attrs, rc) bind(C, name="ESMC_AttributesDestroy")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_destroy

  subroutine c_attrs_erase(attrs, keyParent, keyChild, rc) bind(C, name="ESMC_AttributesErase")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: keyParent(*)
    character(C_CHAR), intent(in) :: keyChild(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_erase

  function c_attrs_get(attrs, key, rc, default) bind(C, name="ESMC_AttributesGet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default
    integer(C_INT) :: c_attrs_get
  end function c_attrs_get

  subroutine c_attrs_get_array(attrs, key, values, count, count_only, rc) bind(C, name="ESMC_AttributesGetArray")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: count
    integer(C_INT), intent(inout) :: count_only
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_get_array

  function c_attrs_is_present(attrs, key, isptr, rc) bind(C, name="ESMC_AttributesIsPresent")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: isptr
    integer(C_INT), intent(inout) :: rc
    integer(C_INT) :: c_attrs_is_present
  end function c_attrs_is_present

  subroutine c_attrs_print(attrs, indent, rc) bind(C, name="ESMC_AttributesPrint")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(in) :: indent
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_print

  subroutine c_attrs_set(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: value
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set

  subroutine c_attrs_set_array(attrs, key, values, count, force, rc) bind(C, name="ESMC_AttributesSetArray")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(in) :: values
    integer(C_INT), intent(in) :: count
    integer(C_INT), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array
end interface
