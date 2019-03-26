interface

  function c_attrs_copy(attrs, rc) bind(C, name="ESMC_AttributesCopy")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_copy
  end function c_attrs_copy

  !=============================================================================

  function c_attrs_create(rc) bind(C, name="ESMC_AttributesCreate")
    use iso_c_binding
    implicit none
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_create
  end function c_attrs_create

  function c_attrs_create_by_key(srcAttrs, key, rc) bind(C, name="ESMC_AttributesCreateByKey")
    use iso_c_binding
    implicit none
    type(C_PTR), value, intent(in) :: srcAttrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_create_by_key
  end function c_attrs_create_by_key

  function c_attrs_create_by_parse(payload, rc) bind(C, name="ESMC_AttributesCreateByParse")
    use iso_c_binding
    implicit none
    character(C_CHAR), intent(in) :: payload(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_attrs_create_by_parse
  end function c_attrs_create_by_parse

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

  subroutine c_attrs_inquire(attrs, key, rc, nelements) bind(C, name="ESMC_AttributesInquire")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: nelements
  end subroutine c_attrs_inquire

  !=============================================================================

  subroutine c_attrs_is_present(attrs, key, res, rc, isptr) bind(C, name="ESMC_AttributesIsPresent")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(inout) :: res
    integer(C_INT), intent(inout) :: rc
    integer(C_INT), intent(in) :: isptr
  end subroutine c_attrs_is_present

  !=============================================================================

  subroutine c_attrs_is_set(attrs, key, is_set_c, rc) bind(C, name="ESMC_AttributesIsSet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: is_set_c
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_is_set

  !=============================================================================

  subroutine c_attrs_print(attrs, indent, rc) bind(C, name="ESMC_AttributesPrint")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    integer(C_INT), intent(in) :: indent
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_print

  !=============================================================================

  subroutine c_attrs_read_json(attrs, filename, rc) bind(C, name="ESMC_AttributesReadJSON")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: filename(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_read_json

  !=============================================================================

  subroutine c_attrs_write_json(attrs, filename, rc) bind(C, name="ESMC_AttributesWriteJSON")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: filename(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_write_json

  !=============================================================================

  subroutine c_attrs_get_R4(attrs, key, value, rc, default, idx) bind(C, name="ESMC_AttributesGetR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_R4

  subroutine c_attrs_get_R8(attrs, key, value, rc, default, idx) bind(C, name="ESMC_AttributesGetR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_R8

  subroutine c_attrs_get_I4(attrs, key, value, rc, default, idx) bind(C, name="ESMC_AttributesGetI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_I4

  subroutine c_attrs_get_I8(attrs, key, value, rc, default, idx) bind(C, name="ESMC_AttributesGetI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_I8

  subroutine c_attrs_get_LG(attrs, key, value, rc, default, idx) bind(C, name="ESMC_AttributesGetLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(inout) :: value
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_LG

  subroutine c_attrs_get_CH(attrs, key, value, vlen, rc, default, idx) bind(C, name="ESMC_AttributesGetCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    character(kind=C_CHAR, len=1), dimension(vlen), intent(inout) :: value
    integer(C_INT) :: vlen
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
  end subroutine c_attrs_get_CH

  !=============================================================================

  subroutine c_attrs_get_array_R4(attrs, key, values, nelements, nelements_only, rc) bind(C, name="ESMC_AttributesGetArrayR4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_FLOAT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, nelements_only, rc
  end subroutine c_attrs_get_array_R4

  subroutine c_attrs_get_array_R8(attrs, key, values, nelements, nelements_only, rc) bind(C, name="ESMC_AttributesGetArrayR8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    real(C_DOUBLE), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, nelements_only, rc
  end subroutine c_attrs_get_array_R8

  subroutine c_attrs_get_array_I4(attrs, key, values, nelements, nelements_only, rc) bind(C, name="ESMC_AttributesGetArrayI4")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, nelements_only, rc
  end subroutine c_attrs_get_array_I4

  subroutine c_attrs_get_array_I8(attrs, key, values, nelements, nelements_only, rc) bind(C, name="ESMC_AttributesGetArrayI8")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    integer(C_LONG), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, nelements_only, rc
  end subroutine c_attrs_get_array_I8

  subroutine c_attrs_get_array_LG(attrs, key, values, nelements, nelements_only, rc) bind(C, name="ESMC_AttributesGetArrayLG")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), dimension(*), intent(inout) :: values
    integer(C_INT), intent(inout) :: nelements, nelements_only, rc
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

  subroutine c_attrs_set_ATTRS(attrs, key, value, force, rc) bind(C, name="ESMC_AttributesSetATTRS")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    type(C_PTR), value :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_ATTRS

  subroutine c_attrs_set_NULL(attrs, key, force, rc) bind(C, name="ESMC_AttributesSetNULL")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_NULL

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

  subroutine c_attrs_set_array_CH(attrs, key, nelements, force, rc) bind(C, name="ESMC_AttributesSetArrayCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: attrs
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_attrs_set_array_CH
  !=============================================================================

end interface