interface
  function c_create_attributes(rc) bind(C, name="create_attributes")
    use iso_c_binding
    implicit none
    integer(C_INT), intent(inout) :: rc
    type(C_PTR) :: c_create_attributes
  end function c_create_attributes
end interface
