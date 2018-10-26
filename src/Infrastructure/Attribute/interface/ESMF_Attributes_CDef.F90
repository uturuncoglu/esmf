interface
  function c_create_attributes() bind(C, name="create_attributes")
    use iso_c_binding
    implicit none
    type(C_PTR) :: c_create_attributes
  end function c_create_attributes
end interface