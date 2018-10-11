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

void checkESMFReturnCode(int &rc){
  assert(rc == ESMF_SUCCESS);
  return;
};

int testConstructor(){
  int rc = ESMF_FAILURE;
  Attributes attrs;
  rc = ESMF_SUCCESS;
  return rc;
};

int testSetGet(){
  int rc = ESMF_FAILURE;

  Attributes attrs;

  int value = 10;
  string key = "/theKey";
  rc = ESMF_FAILURE;
  attrs.set(key, value, rc);
  checkESMFReturnCode(rc);

  const json& storage = attrs.getStorageRef();

  if (storage["theKey"] != value){
    rc = ESMF_FAILURE;
    return rc;
  }

  rc = ESMF_FAILURE;
  int actual = attrs.get<int>(key, rc);
  checkESMFReturnCode(rc);

  if (actual != value){
    rc = ESMF_FAILURE;
    return rc;
  }

  //----------------------------------------------------------------------------

  int value2 = 33;
  string keyp = "/root/group1/group2";
  rc = ESMF_FAILURE;
  attrs.set(keyp, value2, rc);
  checkESMFReturnCode(rc);

  if (storage["root"]["group1"]["group2"] != value2){
    rc = ESMF_FAILURE;
    return rc;
  }

  rc = ESMF_FAILURE;
  actual = attrs.get<int>(keyp, rc);
  checkESMFReturnCode(rc);

  if (actual != value2){
    rc = ESMF_FAILURE;
    return rc;
  }

  //----------------------------------------------------------------------------

  key = "/twiceSet";
  attrs.set(key, 10, rc);
  checkESMFReturnCode(rc);
  value = 12;
  attrs.set(key, value, rc);
  checkESMFReturnCode(rc);

  if (attrs.get<int>(key, rc) != value){
    rc = ESMF_FAILURE;
    return rc;
  }

  std::cout << storage.dump(2) << std::endl;

  return rc;
};

int testSetGetErrorHandling(){
  int rc = ESMF_FAILURE;

  Attributes attrs;

  string key = "/theKey";
  int actual = attrs.get<int>(key, rc);

  // Test is expected to fail as we have not added anything at this key.
  if (rc != ESMC_RC_ATTR_WRONGTYPE){
    rc = ESMF_FAILURE;
    return rc;
  }

  rc = ESMF_SUCCESS;
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
  //NEX_UTest
  strcpy(name, "Attributes SetGet Error Handling");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");
  rc = testSetGetErrorHandling();
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
};
