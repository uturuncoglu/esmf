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
#include "ESMCI_Util.h"

using namespace ESMCI;
using namespace std;

typedef const long int* const attr_int_ptr_t;
typedef const json::number_integer_t* const json_int_ptr_t;

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

  auto actual = a.get<attr_int_ptr_t, json_int_ptr_t>("/foo", rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual != desired){
    return finalizeFailure(rc, failMsg, "JSON object changed value");
  }

  root.clear();
  auto actual2 = a.get<attr_int_ptr_t, json_int_ptr_t>("/foo", rc);
  if (*actual2 != desired){
    return finalizeFailure(rc, failMsg, "Clear removed desired value");
  }

  //----------------------------------------------------------------------------
  // Test move constructor

  json src;
  src["foo"] = 112;

  long int *srcPtr = src.at("foo").get_ptr<json::number_integer_t *>();

  Attributes dst(move(src));

  if (!src.is_null()){
    return finalizeFailure(rc, failMsg, "JSON object not moved");
  }

  auto actual3 = dst.get<attr_int_ptr_t , json_int_ptr_t>("foo", rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual3 != 112) {
    return finalizeFailure(rc, failMsg, "Value bad after move");
  }
  if (&*actual3 != &*srcPtr) {
    return finalizeFailure(rc, failMsg, "Pointer addresses not equal after move");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testHasKey"
void testHasKey(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  attrs.set("/neverEver", 13, false, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, ESMCI_ERR_PASSTHRU, ESMC_CONTEXT,
                                    &rc)) return;

  bool actual = attrs.hasKey("/hello", rc);
  if (actual){
    return finalizeFailure(rc, failMsg, "Key is not present");
  }

  bool actual2 = attrs.hasKey("/neverEver", rc);
  if (!actual2){
    return finalizeFailure(rc, failMsg, "Key is present");
  }

  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testErase"
void testErase(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  string key = "/something/nested";
  attrs.set(key, 10, false, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  attrs.erase("/something", "nested", rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  const json &storage = attrs.getStorageRef();
  const json &actual = storage["something"];
  if (actual.find("nested") != actual.end()){
    return finalizeFailure(rc, failMsg, "Nested item not deleted");
  }

  //----------------------------------------------------------------------------
  // Test errors handled with bad key combinations and erase.

  rc = ESMF_FAILURE;
  bool failed = true;
  try {
    attrs.erase("/nothing", "nested", rc);
    failed = true;
  } catch (int err) {
    if (rc == ESMC_RC_NOT_FOUND && err == ESMC_RC_NOT_FOUND){
      failed = false;
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Error not handled for missing parent");
  }

  rc = ESMF_FAILURE;
  try {
    attrs.erase("/something", "underground", rc);
    failed = true;
  } catch (int err){
    if (rc == ESMC_RC_NOT_FOUND && err == ESMC_RC_NOT_FOUND){
      failed = false;
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Error not handled for missing child");
  }

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
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  const json& storage = attrs.getStorageRef();

  if (storage["theKey"] != value){
    return finalizeFailure(rc, failMsg, "Did not set key correctly");
  }

  rc = ESMF_FAILURE;
  attr_int_ptr_t actual = attrs.get<attr_int_ptr_t, json_int_ptr_t>(key, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual != value){
    return finalizeFailure(rc, failMsg, "Did not get key correctly");
  }

  //----------------------------------------------------------------------------

  int value2 = 33;
  string keyp = "/root/group1/group2";
  rc = ESMF_FAILURE;
  attrs.set(keyp, value2, false, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (storage["root"]["group1"]["group2"] != value2){
    return finalizeFailure(rc, failMsg, "Did not set nested key correctly");
  }

  rc = ESMF_FAILURE;
  auto actual2 = attrs.get<attr_int_ptr_t, json_int_ptr_t>(keyp, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual2 != value2){
    return finalizeFailure(rc, failMsg, "Did not get nested key correctly");
  }

  //----------------------------------------------------------------------------

  key = "/twiceSet";
  attrs.set(key, 10, false, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  value = 12;
  attrs.set(key, value, true, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (*attrs.get<attr_int_ptr_t, json_int_ptr_t>(key, rc) != value){
    return finalizeFailure(rc, failMsg, "Did not overload existing key correctly");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testSetGetErrorHandling"
void testSetGetErrorHandling(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  Attributes attrs;

  //----------------------------------------------------------------------------
  // Test trying to get a value that is not in the map or is the wrong type
  // will error.

  bool failed = true;
  string key = "/theKey";
  try {
    auto actual = attrs.get<attr_int_ptr_t, json_int_ptr_t>(key, rc);
  }
  catch (int err) {
    if (rc == ESMC_RC_NOT_FOUND && err == ESMC_RC_NOT_FOUND){
      failed = false;
    }
  }

  // Test is expected to fail as we have not added anything at this key.
  if (failed){
    return finalizeFailure(rc, failMsg, "Return code not compliant with get error");
  }

  //----------------------------------------------------------------------------
  // Test setting force to false will error out if the map has already been
  // created.

  string key2 = "/theKey2";
  attrs.set(key2, 111, false, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  failed = true;
  try {
    attrs.set(key2, 222, false, rc);
  }
  catch (int err) {
    if (rc == ESMC_RC_CANNOT_SET && err == ESMC_RC_CANNOT_SET) {
      failed = false;
    }
  }
  if (failed){
    return finalizeFailure(rc, failMsg, "Error not handled with existing key");
  }

  //----------------------------------------------------------------------------
  // Test a malformed key

  failed = true;
  string key3 = "//key";
  try {
    attrs.set(key3, 111, false, rc);
  }
  catch (int err) {
    if (rc == ESMC_RC_ARG_BAD && err == ESMC_RC_ARG_BAD){
      failed = false;
    }
  }
  if (failed){
    return finalizeFailure(rc, failMsg, "Key is not parseable");
  }

  //----------------------------------------------------------------------------

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testUpdate"
void testUpdate(int &rc, char failMsg[]){
  rc = ESMF_FAILURE;

  json update_target =  R"( {"color": "red", "price": 17.99} )"_json;
  Attributes update_target_attrs(update_target);

  json used_to_update = R"( {"color": "blue", "speed": 100} )"_json;
  Attributes used_to_update_attrs(used_to_update);

  const json& lhs = update_target_attrs.getStorageRef();
  const json& rhs = used_to_update_attrs.getStorageRef();

  if (lhs == rhs){
    return finalizeFailure(rc, failMsg, "Storage should not be equal");
  }

  rc = ESMF_FAILURE;
  update_target_attrs.update(used_to_update_attrs, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  const json desired = R"( {"color": "blue", "price": 17.99, "speed": 100} )"_json;

  if (lhs != desired){
    return finalizeFailure(rc, failMsg, "Storage not updated");
  }

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
  //NEX_UTest
  strcpy(name, "Attributes Update");
  testUpdate(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes hasKey");
  testHasKey(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
};
