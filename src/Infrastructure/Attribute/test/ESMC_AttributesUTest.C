// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2016, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//
//==============================================================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ESMF header
#include "ESMC.h"

// ESMF Test header
#include "ESMC_Test.h"

// Attributes header
#include "ESMCI_Attributes.h"

//==============================================================================
//BOP
// !PROGRAM: ESMC_AttributesUTest - Internal Attribute JSON functionality
//
// !DESCRIPTION: Test Attributes class
//
//EOP
//------------------------------------------------------------------------------

int testConstructor(){
  ESMCI::Attributes attrs;
  return ESMF_SUCCESS;
};

int main(void){

  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  //----------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Constructor");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");
  rc = testConstructor();
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
};
