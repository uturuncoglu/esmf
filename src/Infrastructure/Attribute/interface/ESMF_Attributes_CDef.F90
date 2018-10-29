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

  function c_attrs_get(attrs, key, rc) bind(C, name="ESMC_AttributesGet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    integer(C_INT) :: c_attrs_get
  end function c_attrs_get

  subroutine c_attrs_set(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set
end interface
