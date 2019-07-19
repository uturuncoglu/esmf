interface

  function c_info_base_get(base_address) bind(C, name="ESMC_BaseGetInfo2")
    use iso_c_binding
    implicit none
    integer(C_LONG), intent(in) :: base_address
    type(C_PTR) :: c_info_base_get
  end function c_info_base_get

  !=============================================================================

  function c_info_copy(info, rc) bind(C, name="ESMC_Info2Copy")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_info_copy
  end function c_info_copy

  !=============================================================================

  function c_info_create(rc) bind(C, name="ESMC_Info2Create")
    use iso_c_binding
    implicit none
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_info_create
  end function c_info_create

  function c_info_create_by_key(srcInfo, key, rc) bind(C, name="ESMC_Info2CreateByKey")
    use iso_c_binding
    implicit none
    type(C_PTR), value, intent(in) :: srcInfo
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_info_create_by_key
  end function c_info_create_by_key

  function c_info_create_by_parse(payload, rc) bind(C, name="ESMC_Info2CreateByParse")
    use iso_c_binding
    implicit none
    character(C_CHAR), intent(in) :: payload(*)
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_info_create_by_parse
  end function c_info_create_by_parse

  !=============================================================================

  subroutine c_info_broadcast(info, rootPet, rc) bind(C, name="ESMC_Info2Broadcast")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    integer(C_INT), intent(in) :: rootPet
    integer(C_INT), intent(inout) :: rc
  end subroutine

  !=============================================================================

  subroutine c_info_destroy(info, rc) bind(C, name="ESMC_Info2Destroy")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_destroy

  !=============================================================================

  subroutine c_info_erase(info, keyParent, keyChild, recursive, rc) bind(C, name="ESMC_Info2Erase")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: keyParent(*)
    character(C_CHAR), intent(in) :: keyChild(*)
    logical(C_BOOL), intent(in) :: recursive
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_erase

  !=============================================================================

  subroutine c_info_inquire(info, inq, key, recursive, idx, rc) bind(C, name="ESMC_Info2Inquire")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    type(C_PTR), value :: inq
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(in) :: recursive
    type(C_PTR), value :: idx
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_inquire

  !=============================================================================

  subroutine c_info_is_equal(lhs, rhs, is_equal, localrc) bind (C, name="ESMC_Info2IsEqual")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: lhs
    type(C_PTR), value :: rhs
    logical(C_BOOL), intent(inout) :: is_equal
    integer(C_INT), intent(inout) :: localrc
  end subroutine c_info_is_equal

  !=============================================================================

  subroutine c_info_is_present(info, key, res, rc, recursive, isptr) bind(C, name="ESMC_Info2IsPresent")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(inout) :: res
    integer(C_INT), intent(inout) :: rc
    logical(C_BOOL), intent(in) :: recursive
    integer(C_INT), intent(in) :: isptr
  end subroutine c_info_is_present

  !=============================================================================

  subroutine c_info_is_set(info, key, is_set_c, rc) bind(C, name="ESMC_Info2IsSet")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(inout) :: is_set_c
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_is_set

  !=============================================================================

  subroutine c_info_print(info, indent, rc) bind(C, name="ESMC_Info2Print")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    integer(C_INT), intent(in) :: indent
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_print

  !=============================================================================

  subroutine c_info_update(lhs, rhs, rc) bind(C, name="ESMC_Info2Update")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: lhs
    type(C_PTR), value :: rhs
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_update

  !=============================================================================

  subroutine c_info_base_sync(inqstate, rootPet, rc) bind(C, name="ESMC_Info2BaseSync")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: inqstate
    integer(C_INT), intent(in) :: rootPet
    integer(C_INT), intent(inout) :: rc
  end subroutine

  !=============================================================================

  subroutine c_info_read_json(info, filename, rc) bind(C, name="ESMC_Info2ReadJSON")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: filename(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_read_json

  subroutine c_info_write_json(info, filename, rc) bind(C, name="ESMC_Info2WriteJSON")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: filename(*)
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_write_json

  !=============================================================================

!  subroutine c_info_get_attpack(infop, attpackp, convention, purpose, rc) bind(C, name="ESMC_InfoGetAttPack")
!    use iso_c_binding
!    implicit none
!    type(C_PTR), value :: infop
!    type(C_PTR), value :: attpackp
!    character(C_CHAR), intent(in) :: convention(*)
!    character(C_CHAR), intent(in) :: purpose(*)
!    integer(C_INT), intent(inout) :: rc
!  end subroutine c_info_get_attpack

  !=============================================================================

  subroutine c_info_get_CH(info, key, value, vlen, rc, default, idx, recursive) bind(C, name="ESMC_Info2GetCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(C_CHAR), intent(in) :: key(*)
    character(kind=C_CHAR, len=1), dimension(vlen), intent(inout) :: value
    integer(C_INT) :: vlen
    integer(C_INT), intent(inout) :: rc
    type(C_PTR), value :: default, idx
    logical(C_BOOL), intent(in) :: recursive
  end subroutine c_info_get_CH

  !=============================================================================

  subroutine c_info_set_INFO(info, key, value, force, rc) bind(C, name="ESMC_Info2SetINFO")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    type(C_PTR), value :: value
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_set_INFO

  subroutine c_info_set_NULL(info, key, force, rc) bind(C, name="ESMC_Info2SetNULL")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_set_NULL

  subroutine c_info_set_array_CH(info, key, nelements, force, rc) bind(C, name="ESMC_Info2SetArrayCH")
    use iso_c_binding
    implicit none
    type(C_PTR), value :: info
    character(kind=C_CHAR), intent(in) :: key(*)
    integer(C_INT), intent(in) :: nelements
    logical(C_BOOL), intent(in) :: force
    integer(C_INT), intent(inout) :: rc
  end subroutine c_info_set_array_CH

end interface
