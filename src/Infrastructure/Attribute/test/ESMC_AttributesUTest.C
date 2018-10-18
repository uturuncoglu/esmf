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

#include "ESMC.h"
#include "ESMC_Test.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_LogErr.h"

using namespace ESMCI;

//==============================================================================
//BOP
// !PROGRAM: ESMC_AttributesUTest - Internal Attribute JSON functionality
//
// !DESCRIPTION: Test Attributes class
//
//EOP
//------------------------------------------------------------------------------

void finalizeFailure(int &rc, char failMsg[], string msg){
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testConstructor"
void testConstructor(int &rc, char failMsg[]){
  Attributes attrs;

  // Test constructing from a JSON object instance creates a copy.
  json root;
  int desired = 5;
  root["foo"] = desired;
  Attributes a(root);
  root["foo"] = 10;

  int actual = a.get<int>("/foo", rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  if (actual != desired){
    return finalizeFailure(rc, failMsg, "JSON object changed value.");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testErase"
void testErase(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  string key = "/something/nested";
  attrs.set(key, 10, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;
  attrs.erase("/something", "nested", rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  const json &storage = attrs.getStorageRef();
  const json &actual = storage["something"];
  if (actual.find("nested") != actual.end()){
    return finalizeFailure(rc, failMsg, "Nested item not deleted");
  }

  //----------------------------------------------------------------------------
  // Test errors handled with bad key combinations and erase.

  rc = ESMF_FAILURE;
  attrs.erase("/nothing", "nested", rc);
  if (rc != ESMC_RC_NOT_FOUND) {
    return finalizeFailure(rc, failMsg, "Error not handled for missing parent");
  }

  rc = ESMF_FAILURE;
  attrs.erase("/something", "underground", rc);
  if (rc != ESMC_RC_NOT_FOUND) {
    return finalizeFailure(rc, failMsg, "Error not handled for missing child");
  }

//  std::cout << storage.dump(2) << std::endl;

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testSetGet"
void testSetGet(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  // ---------------------------------------------------------------------------
  // Test setting a single value.

  int value = 10;
  string key = "/theKey";
  attrs.set(key, value, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  const json& storage = attrs.getStorageRef();

  if (storage["theKey"] != value){
    return finalizeFailure(rc, failMsg, "Did not set key correctly");
  }

  rc = ESMF_FAILURE;
  int actual = attrs.get<int>(key, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  if (actual != value){
    return finalizeFailure(rc, failMsg, "Did not get key correctly");
  }

  //----------------------------------------------------------------------------

  int value2 = 33;
  string keyp = "/root/group1/group2";
  rc = ESMF_FAILURE;
  attrs.set(keyp, value2, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  if (storage["root"]["group1"]["group2"] != value2){
    return finalizeFailure(rc, failMsg, "Did not set nested key correctly");
  }

  rc = ESMF_FAILURE;
  actual = attrs.get<int>(keyp, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  if (actual != value2){
    return finalizeFailure(rc, failMsg, "Did not get nested key correctly");
  }

  //----------------------------------------------------------------------------

  key = "/twiceSet";
  attrs.set(key, 10, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;
  value = 12;
  attrs.set(key, value, true, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  if (attrs.get<int>(key, rc) != value){
    return finalizeFailure(rc, failMsg, "Did not overload existing key correctly");
  }

//  std::cout << storage.dump(2) << std::endl;

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testSetGetErrorHandling"
void testSetGetErrorHandling(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  // ---------------------------------------------------------------------------
  // Test trying to get a value that is not in the map or is the wrong type
  // will error.

  string key = "/theKey";
  int actual = attrs.get<int>(key, rc);

  // Test is expected to fail as we have not added anything at this key.
  if (rc != ESMC_RC_NOT_FOUND){
    return finalizeFailure(rc, failMsg, "Return code not compliant with get error");
  }

  // ---------------------------------------------------------------------------
  // Test setting force to false will error out if the map has already been
  // created.

  string key2 = "/theKey2";
  attrs.set(key2, 111, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;
  attrs.set(key2, 222, false, rc);
  if (rc != ESMC_RC_CANNOT_SET){
    return finalizeFailure(rc, failMsg, "Error not handled with existing key");
  }

  rc = ESMF_SUCCESS;
  return;
};

int main(void){

  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  strcpy(failMsg, "Did not return ESMF_SUCCESS");  // Default fail message

  //----------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Constructor");

  testConstructor(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Erase");
  testErase(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes SetGet");
  testSetGet(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes SetGet Error Handling");
  testSetGetErrorHandling(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
};
