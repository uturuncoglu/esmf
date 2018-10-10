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
#include <iostream>
#include <assert.h>

// ESMF header
#include "ESMC.h"

// ESMF Test header
#include "ESMC_Test.h"

// Attributes header
#include "ESMCI_Attributes.h"

using namespace ESMCI;

//==============================================================================
//BOP
// !PROGRAM: ESMC_AttributesUTest - Internal Attribute JSON functionality
//
// !DESCRIPTION: Test Attributes class
//
//EOP
//------------------------------------------------------------------------------

int testConstructor(){
  int rc = ESMF_FAILURE;
  Attributes attrs;
  rc = ESMF_SUCCESS;
  return rc;
};

int testSetGet(){
  int rc = ESMF_FAILURE;

  Attributes attrs;

  typeKeyList kl;
  kl.push_back("theKey");

  int value = 10;
  rc = attrs.set("theKey", value);
  assert(rc == ESMF_SUCCESS);

  const json &storage = attrs.getStorageRef();

  if (storage["theKey"] != value) rc = ESMF_FAILURE;

  std::cout << storage.dump(2) << std::endl;
//  std::cout << storage["theKey"] << std::endl;


  return rc;
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
  //NEX_UTest
  strcpy(name, "Attributes SetGet");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");
  rc = testSetGet();
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
};
