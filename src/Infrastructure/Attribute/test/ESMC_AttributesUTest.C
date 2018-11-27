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
//=============================================================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "ESMC.h"
#include "ESMC_Test.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"

using namespace ESMCI;
using namespace std;

typedef const long int* const attr_int_ptr_t;
typedef const json::number_integer_t* const json_int_ptr_t;

//=============================================================================
//BOP
// !PROGRAM: ESMC_AttributesUTest - Internal Attribute JSON functionality
//
// !DESCRIPTION: Test Attributes class
//
//EOP
//-----------------------------------------------------------------------------

void finalizeFailure(int& rc, char failMsg[], string msg) {
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testBroadcastAttributes()"
void testBroadcastAttributes(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  Attributes attrs;

  int rootPet;
  // Use a non-zero root pet for parallel testing
  if (petCount == 1) {
    rootPet = 0;
  } else {
    rootPet = 1;
  }

  int desired = 5;
  if (localPet == rootPet) {
    attrs.set("foo", desired, false, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  }

  ESMCI::broadcastAttributes(&attrs, rootPet, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  int actual = attrs.get<int>("foo", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (actual != desired) {
    return finalizeFailure(rc, failMsg, "Value not broadcast");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testConstructor()"
void testConstructor(int& rc, char failMsg[]) {
  Attributes attrs;

  // Test constructing from a JSON object instance creates a copy.
  json root;
  int desired = 5;
  root["foo"] = desired;
  Attributes a(root);
  root["foo"] = 10;

  auto actual = a.getPointer<attr_int_ptr_t, json_int_ptr_t>("/foo", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual != desired){
    return finalizeFailure(rc, failMsg, "JSON object changed value");
  }

  root.clear();
  auto actual2 = a.getPointer<attr_int_ptr_t, json_int_ptr_t>("/foo", rc);
  if (*actual2 != desired){
    return finalizeFailure(rc, failMsg, "Clear removed desired value");
  }

  //---------------------------------------------------------------------------
  // Test move constructor

  json src;
  src["foo"] = 112;

  long int *srcPtr = src.at("foo").get_ptr<json::number_integer_t *>();

  Attributes dst(move(src));

  if (!src.is_null()){
    return finalizeFailure(rc, failMsg, "JSON object not moved");
  }

  auto actual3 = dst.getPointer<attr_int_ptr_t , json_int_ptr_t>("foo", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual3 != 112) {
    return finalizeFailure(rc, failMsg, "Value bad after move");
  }
  if (&*actual3 != &*srcPtr) {
    return finalizeFailure(rc, failMsg, "Pointer addresses not equal after move");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testCreateJSONPackage()"
void testCreateJSONPackage(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  string pkgKey = "ESMF:Metadata:Group";
  json jattrs = createJSONPackage(pkgKey, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  //---------------------------------------------------------------------------
  // Test an unsupported key

  bool failed = true;
  string badPkgKey = "Does:Not:Exist";
  try {
    json noattrs = createJSONPackage(badPkgKey, rc);
  }
  catch (esmf_attrs_error &err) {
    if (err.getReturnCode() == ESMF_RC_NOT_FOUND) {
      failed = false;
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Package should not be returned");
  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testGet()"
void testGet(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  attrs.set("target", 50, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  auto actual = attrs.get<int>("target", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (actual != 50) {
    return finalizeFailure(rc, failMsg, "Could not get target");
  }

  auto actual_ptr = attrs.getPointer<attr_int_ptr_t,json_int_ptr_t>("target", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  std::size_t addr1 = (std::size_t)&actual;
  std::size_t addr2 = (std::size_t)&*actual_ptr;
  if (addr1 == addr2) {
    return finalizeFailure(rc, failMsg, "Addresses should not be equal");
  }

  try {
    attrs.get<int>("blah", rc);
    return finalizeFailure(rc, failMsg, "Error not raised for missing key");
  } catch (esmf_attrs_error& err) {
    if (err.getReturnCode() != ESMC_RC_NOT_FOUND) {
      return finalizeFailure(rc, failMsg, "Wrong error return code");
    }
  }

  // Test get with a default value ============================================

  Attributes attrs2;
  rc = ESMF_FAILURE;
  int def = 3000;
  auto actual2 = attrs2.get<int>("blah-dee-blah", rc, &def);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (actual2 != def) {
    return finalizeFailure(rc, failMsg, "Did not get default value");
  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testHasKey()"
void testHasKey(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  attrs.set("/neverEver", 13, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  bool actual = attrs.hasKey("/hello", rc, true);
  if (actual){
    return finalizeFailure(rc, failMsg, "Key is not present");
  }

  bool actual2 = attrs.hasKey("/neverEver", rc, true);
  if (!actual2){
    return finalizeFailure(rc, failMsg, "Key is present");
  }

  // Test not using a JSON pointer.
  bool actual3 = attrs.hasKey("neverEver", rc);
  if (!actual3){
    return finalizeFailure(rc, failMsg, "Key is present with non-pointer");
  }

  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testErase()"
void testErase(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  string key = "/something/nested";
  attrs.set(key, 10, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  attrs.erase("/something", "nested", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  const json &storage = attrs.getStorageRef();
  const json &actual = storage["something"];
  if (actual.find("nested") != actual.end()){
    return finalizeFailure(rc, failMsg, "Nested item not deleted");
  }

  //---------------------------------------------------------------------------
  // Test errors handled with bad key combinations and erase.

  rc = ESMF_FAILURE;
  bool failed = true;
  try {
    attrs.erase("/nothing", "nested", rc);
    failed = true;
  }
  catch (esmf_attrs_error &err) {
    if (rc == ESMC_RC_NOT_FOUND){
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
  }
  catch (esmf_attrs_error &err){
    if (rc == ESMC_RC_NOT_FOUND){
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
#define ESMC_METHOD "testSetGet()"
void testSetGet(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  // Test setting a single value ==============================================

  int value = 10;
  string key = "theKey";
  attrs.set(key, value, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  const json& storage = attrs.getStorageRef();

  if (storage["theKey"] != value){
    return finalizeFailure(rc, failMsg, "Did not set key correctly");
  }

  rc = ESMF_FAILURE;
  attr_int_ptr_t actual = attrs.getPointer<attr_int_ptr_t, json_int_ptr_t>(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual != value){
    return finalizeFailure(rc, failMsg, "Did not get pointer key correctly");
  }

  //---------------------------------------------------------------------------

  int value2 = 33;
  string keyp = "/root/group1/group2";
  rc = ESMF_FAILURE;
  attrs.set(keyp, value2, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (storage["root"]["group1"]["group2"] != value2){
    return finalizeFailure(rc, failMsg, "Did not set nested key correctly");
  }

  rc = ESMF_FAILURE;
  auto actual2 = attrs.getPointer<attr_int_ptr_t, json_int_ptr_t>(keyp, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (*actual2 != value2){
    return finalizeFailure(rc, failMsg, "Did not get nested key correctly");
  }

  //---------------------------------------------------------------------------

  key = "/twiceSet";
  attrs.set(key, 10, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  value = 12;
  attrs.set(key, value, true, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (*attrs.getPointer<attr_int_ptr_t, json_int_ptr_t>(key, rc) != value){
    return finalizeFailure(rc, failMsg, "Did not overload existing key correctly");
  }

  // Test with string data type ===============================================

  key = "hello";
  string value3 = "world";
  attrs.set(key, value3, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  string actual3 = attrs.get<string>(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (actual3 != value3) {
    return finalizeFailure(rc, failMsg, "Did not get string value");
  }

  // Test using a JSON Pointer with an array ==================================

  std::vector<int> c_vector {1, 2, 3, 4};
  json ja;
  json j_vec(c_vector);
  ja["foo"] = j_vec;
  Attributes attrsvec(ja);

  auto actual4 = attrsvec.get<int>("/foo/2", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (actual4 != c_vector[2]) {
    return finalizeFailure(rc, failMsg, "Did not get array element value");
  }

  // Test with an array pointer ===============================================

  int c_int_arr[4] = {1, 2, 3, 4};
  int count = 4;

  Attributes attrs2;

  attrs2.set("the-key", c_int_arr, count, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  json::array_t apref = attrs2.getStorageRef()["the-key"];

  for (auto ii=0; ii<count; ii++) {
    if (apref[ii] != c_int_arr[ii]) {
      return finalizeFailure(rc, failMsg, "Element array not equal");
    }
  }

  // Test modifying internal storage ==========================================

  Attributes mstore;
  json& jstore = mstore.getStorageRefWritable();
  jstore["i am an int"] = 111;
  if (mstore.get<int>("i am an int", rc) != 111) {
    return finalizeFailure(rc, failMsg, "Did not modify internal storage");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testSetGetErrorHandling()"
void testSetGetErrorHandling(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  //---------------------------------------------------------------------------
  // Test the ESMF attributes exception

  esmf_attrs_error ae("ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, "a message");
  const char *actual = ae.what();
  string desired = "Error/Return Code 541 (ESMC_RC_NOT_FOUND) - a message";
  if (actual != desired || ae.getReturnCode() != ESMC_RC_NOT_FOUND) {
    return finalizeFailure(rc, failMsg, "Error string output not correct");
  }

  //---------------------------------------------------------------------------
  // Test trying to get a value that is not in the map or is the wrong type
  // will error.

  Attributes attrs;

  bool failed = true;
  string key = "/theKey";
  try {
    auto actual = attrs.getPointer<attr_int_ptr_t, json_int_ptr_t>(key, rc);
  }
  catch (esmf_attrs_error& err) {
    if (rc == ESMC_RC_NOT_FOUND){
      failed = false;
    }
  }

  // Test is expected to fail as we have not added anything at this key.
  if (failed){
    return finalizeFailure(rc, failMsg, "Return code not compliant with get error");
  }

  //---------------------------------------------------------------------------
  // Test setting force to false will error out if the map has already been
  // created.

  string key2 = "/theKey2";
  attrs.set(key2, 111, false, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  failed = true;
  try {
    attrs.set(key2, 222, false, rc);
  }
  catch (esmf_attrs_error &err) {
    if (rc == ESMC_RC_CANNOT_SET) {
      failed = false;
    }
  }
  if (failed){
    return finalizeFailure(rc, failMsg, "Error not handled with existing key");
  }

  //---------------------------------------------------------------------------
  // Test a malformed key

  failed = true;
  string key3 = "///key";
  try {
    attrs.set(key3, 111, false, rc);
  }
  catch (esmf_attrs_error &err) {
    if (rc == ESMC_RC_ARG_BAD && err.getReturnCode() == ESMC_RC_ARG_BAD) {
      failed = false;
    }
  }
  if (failed){
    return finalizeFailure(rc, failMsg, "Key is not parseable");
  }

  //---------------------------------------------------------------------------

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testUpdate()"
void testUpdate(int& rc, char failMsg[]) {
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
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  const json desired = R"( {"color": "blue", "price": 17.99, "speed": 100} )"_json;

  if (lhs != desired){
    return finalizeFailure(rc, failMsg, "Storage not updated");
  }

  return;
};

int main(void) {

  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  strcpy(failMsg, "Did not return ESMF_SUCCESS");  // Default fail message

  //---------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Constructors");
  testConstructor(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes erase()");
  testErase(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes set() & getPointer()");
  testSetGet(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------
  
  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Set/Get Error Handling");
  testSetGetErrorHandling(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes update()");
  testUpdate(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes hasKey()");
  testHasKey(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes createJSONPackage()");
  testCreateJSONPackage(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes get()");
  testGet(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes broadcastAttributes()");
  testBroadcastAttributes(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
