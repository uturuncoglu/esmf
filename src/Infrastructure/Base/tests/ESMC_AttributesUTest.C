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

  ESMCI::VM *vm = nullptr;
  try {
    vm = ESMCI::VM::getCurrent(&rc);
  }
  ESMF_CATCH_PASSTHRU

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
  try {
      attrs.set("foo", desired, false, rc);
  }
  ESMF_CATCH_PASSTHRU
  }

  try {
    ESMCI::broadcastAttributes(&attrs, rootPet, rc);
  }
  ESMF_CATCH_PASSTHRU

  try {
    int actual = attrs.get<int>("foo", rc);
    if (actual != desired) {
      return finalizeFailure(rc, failMsg, "Value not broadcast");
    }
  }
  ESMF_CATCH_PASSTHRU
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

  try {
    auto actual = a.getPointer("/foo", rc);
    if (*actual != desired){
      return finalizeFailure(rc, failMsg, "JSON object changed value");
    }
  }
  ESMF_CATCH_PASSTHRU

  root.clear();
  try {
    auto actual2 = a.getPointer("/foo", rc);
    if (*actual2 != desired){
      return finalizeFailure(rc, failMsg, "Clear removed desired value");
    }
  }
  ESMF_CATCH_PASSTHRU

  //---------------------------------------------------------------------------
  // Test move constructor

  json src;
  src["foo"] = 112;

  long int *srcPtr = src.at("foo").get_ptr<json::number_integer_t *>();

  Attributes dst(move(src));

  if (!src.is_null()){
    return finalizeFailure(rc, failMsg, "JSON object not moved");
  }

  try {
    const long int *actual3 = dst.getPointer("foo", rc)->get_ptr<const long int *>();
    if (*actual3 != 112) {
      return finalizeFailure(rc, failMsg, "Value bad after move");
    }
    if (&*actual3 != &*srcPtr) {
      return finalizeFailure(rc, failMsg, "Pointer addresses not equal after move");
    }
  }
  ESMF_CATCH_PASSTHRU
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testGet()"
void testGet(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  try {
    attrs.set("target", 50, false, rc);
  }
  ESMF_CATCH_PASSTHRU

  try {
    auto actual = attrs.get<int>("target", rc);

    if (actual != 50) {
      return finalizeFailure(rc, failMsg, "Could not get target");
    }

    auto actual_ptr = attrs.getPointer("target", rc);

    std::size_t addr1 = (std::size_t)&actual;
    std::size_t addr2 = (std::size_t)&*actual_ptr;
    if (addr1 == addr2) {
      return finalizeFailure(rc, failMsg, "Addresses should not be equal");
    }
  }
  ESMF_CATCH_PASSTHRU

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
  try {
    auto actual2 = attrs2.get<int>("blah-dee-blah", rc, &def);
    if (actual2 != def) {
      return finalizeFailure(rc, failMsg, "Did not get default value");
    }
  }
  ESMF_CATCH_PASSTHRU
}

#undef ESMC_METHOD
#define ESMC_METHOD "testSetGetIndex()"
void testSetGetIndex(int& rc, char failMsg[]) {
  //tdk:order
  rc = ESMF_FAILURE;
  Attributes attrs;
  int n = 25;
  double values[n];
  for (int ii = 0; ii < n; ++ii) {
    values[ii] = (double)ii / (double)n;
  }
  std::string key = "the-key";
  try {
    attrs.set<double>(key, nullptr, n, false, rc);
    for (int ii = 0; ii < n; ++ii) {
      attrs.set<double>(key, values[ii], false, rc, &ii);
    }
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
  double diff;
  double actual;
  for (int ii = 0; ii < n; ++ii) {
    try {
      actual = attrs.get<double>(key, rc, nullptr, &ii);
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
    diff = std::abs(values[ii] - actual);
    if (diff >= 1e-16) {
      return finalizeFailure(rc, failMsg, "Values are not equal");
    }
  }

  // Test vector std::out_of_range is handled =================================

  int jj = 1000;
  try {
    actual = attrs.get<double>(key, rc, nullptr, &jj);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    if (exc_esmf.getReturnCode() != ESMC_RC_ARG_OUTOFRANGE) {
      return finalizeFailure(rc, failMsg, "Did not handle out_of_range");
    } else {
      rc = ESMF_SUCCESS;
    }
  }

  // Test a key has to exist when using an index ==============================

  std::string not_there = "not_there";
  int noidx = 5;
  try {
    attrs.set<int>(not_there, 111, false, rc, &noidx);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    if (exc_esmf.getReturnCode() == ESMC_RC_NOT_FOUND) {
      rc = ESMF_SUCCESS;
    } else {
      return finalizeFailure(rc, failMsg, "Key must exist with an index");
    }
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testHasKey()"
void testHasKey(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  try {
    attrs.set("/neverEver", 13, false, rc);
  }
  ESMF_CATCH_PASSTHRU

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
  try {
    attrs.set(key, 10, false, rc);
  }
  ESMF_CATCH_PASSTHRU

  try {
    attrs.erase("/something", "nested", rc);
  }
  ESMF_CATCH_PASSTHRU

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
#define ESMC_METHOD "testFormatKey()"
void testFormatKey(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  Attributes attrs;
  bool has_key;
  try {
    has_key = attrs.hasKey("/foo/~", rc, true);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    if (exc_esmf.getReturnCode() != ESMC_RC_ARG_BAD) {
      return finalizeFailure(rc, failMsg, "Did not handle JSON parse error");
    }
  }
  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "test_get_attpack_count()"
void test_get_attpack_count(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  json j;
  j["ESMF"]["General"]["foo"] = 5;
  j["ESMF"]["General"]["fool"] = 55;
  j["NUOPC"]["General"]["foo2"] = 3;
  j["NUOPC"]["General"]["fool2"] = 33;
  std::size_t c = get_attpack_count(j);
  if (c!=2) {return finalizeFailure(rc, failMsg, "AttPack count incorrect");}

  json j2;
  j2["what"] = "nothing";
  std::size_t c2 = get_attpack_count(j2);
  if (c2!=0) {return finalizeFailure(rc, failMsg, "AttPack count incorrect");}
  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "test_update_json_count()"
void test_update_json_count(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  json j;
  j["ESMF"]["General"]["foo"] = 5;
  j["ESMF"]["General"]["fool"] = 55;
  j["NUOPC"]["General"]["foo2"] = 3;
  j["NUOPC"]["General"]["fool2"] = 33;

  std::size_t c = 0;
  std::size_t ct = 0;
  update_json_count(c, ct, j, false);
  if (c!=2) {return finalizeFailure(rc, failMsg, "Recursive count incorrect");}

  c = 0;
  ct = 0;
  update_json_count(c, ct, j, true);
  if (c!=4) {return finalizeFailure(rc, failMsg, "Recursive count incorrect");}

  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testSetGet()"
void testSetGet(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  Attributes attrs;

  // Test setting a single value ==============================================

  int value = 10;
  string key = "theKey";
  try {
    attrs.set(key, value, false, rc);
  }
  ESMF_CATCH_PASSTHRU

  const json& storage = attrs.getStorageRef();

  if (storage["theKey"] != value){
    return finalizeFailure(rc, failMsg, "Did not set key correctly");
  }

  rc = ESMF_FAILURE;
  try {
    auto actual = attrs.getPointer(key, rc);

    if (*actual != value){
      return finalizeFailure(rc, failMsg, "Did not get pointer key correctly");
    }
  }
  ESMF_CATCH_PASSTHRU

  //---------------------------------------------------------------------------

  int value2 = 33;
  string keyp = "/root/group1/group2";
  rc = ESMF_FAILURE;
try {
      attrs.set(keyp, value2, false, rc);
  }
ESMF_CATCH_PASSTHRU

  if (storage["root"]["group1"]["group2"] != value2){
    return finalizeFailure(rc, failMsg, "Did not set nested key correctly");
  }

  rc = ESMF_FAILURE;
  try {
    auto actual2 = attrs.getPointer(keyp, rc);
    if (*actual2 != value2){
      return finalizeFailure(rc, failMsg, "Did not get nested key correctly");
    }
  }
  ESMF_CATCH_PASSTHRU

  //---------------------------------------------------------------------------

  key = "/twiceSet";
  try {
    attrs.set(key, 10, false, rc);
  }
  ESMF_CATCH_PASSTHRU

  value = 12;
  try {
    attrs.set(key, value, true, rc);
  }
  ESMF_CATCH_PASSTHRU

  if (*attrs.getPointer(key, rc) != value){
    return finalizeFailure(rc, failMsg, "Did not overload existing key correctly");
  }

  // Test with string data type ===============================================

  key = "hello";
  string value3 = "world";
  try {
    attrs.set(key, value3, false, rc);
  }
  ESMF_CATCH_PASSTHRU

  try {
    string actual3 = attrs.get<string>(key, rc);
    if (actual3 != value3) {
      return finalizeFailure(rc, failMsg, "Did not get string value");
    }
  }
  ESMF_CATCH_PASSTHRU

  // Test using a JSON Pointer with an array ==================================

  std::vector<int> c_vector {1, 2, 3, 4};
  json ja;
  json j_vec(c_vector);
  ja["foo"] = j_vec;
  Attributes attrsvec(ja);

  try {
    auto actual4 = attrsvec.get<int>("/foo/2", rc);
    if (actual4 != c_vector[2]) {
      return finalizeFailure(rc, failMsg, "Did not get array element value");
    }
  }
  ESMF_CATCH_PASSTHRU

  // Test with an array pointer ===============================================

  int c_int_arr[4] = {1, 2, 3, 4};
  int count = 4;

  Attributes attrs2;

  try {
    attrs2.set("the-key", c_int_arr, count, false, rc);
  }
  ESMF_CATCH_PASSTHRU

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
    auto actual = attrs.getPointer(key, rc);
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
  try {
    attrs.set(key2, 111, false, rc);
  }
  ESMF_CATCH_PASSTHRU

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
#define ESMC_METHOD "testDumpLength()"
void testDumpLength(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  Attributes attrs;
  std::string attrbuff;
  try {
    attrbuff = attrs.dump(rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  std::size_t sf = attrbuff.length();
  try {
    attrs.set("the_int", 50, false, rc);
    attrbuff = attrs.dump(rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  std::size_t sf2 = attrbuff.length();
  if (sf2 < sf) {
    return finalizeFailure(rc, failMsg, "Length too small with int");
  }
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testSerializeDeserialize()"
void testSerializeDeserialize(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  Attributes attrs;
  try {
    attrs.set("foo", 16, false, rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  char *null_buffer = nullptr;
  int inquire_length = 0;
  int offset = 0;
  try {
    attrs.serialize(null_buffer, &inquire_length, &offset, ESMF_INQUIREONLY, rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  if (offset != 0) {
    return finalizeFailure(rc, failMsg, "Should not have adjusted offset");
  }
  char buffer[inquire_length];
  int length = 0;
  try {
    attrs.serialize(buffer, &length, &offset, ESMF_NOINQUIRE, rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  int aligned_length = length;
  alignOffset(aligned_length);
  if (offset != aligned_length) {
    return finalizeFailure(rc, failMsg, "Offset and length should be equal");
  }
  Attributes deattrs;
  int deoffset = 0;
  try {
    deattrs.deserialize(buffer, &deoffset, rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  if (deoffset != offset) {
    return finalizeFailure(rc, failMsg, "Deserialize offset incorrect");
  }
  if (attrs.getStorageRef() != deattrs.getStorageRef()) {
    return finalizeFailure(rc, failMsg, "Storage not equal");
  }
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testInquire()"
void testInquire(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  json j;
  j["ESMF"]["General"]["n"] = 100;
  j["ESMF"]["General"]["x"] = 1000;
  j["NUOPC"]["General"]["a"] = 111;
  j["NUOPC"]["General"]["b"] = 1111;
  ESMCI::Attributes info(std::move(j));
  json inq;
  try {
    inq = info.inquire("", rc);
  }
  ESMF_CATCH_PASSTHRU
  if(inq.at("attPackCount")!=2) {
    return finalizeFailure(rc, failMsg, "Wrong inquire count");
  }

  try {
    inq = info.inquire("/ESMF/General", rc);
  }
  ESMF_CATCH_PASSTHRU
  if(inq.at("count")!=2) {
    return finalizeFailure(rc, failMsg, "Wrong inquire count with key");
  }

  try {
    inq = info.inquire("", rc, true);
  }
  ESMF_CATCH_PASSTHRU
  if(inq.at("count")!=4) {
    return finalizeFailure(rc, failMsg, "Wrong inquire count with recursive");
  }

  info.getStorageRefWritable()["NUOPC"]["General"]["foobar"] = {3, 4, 5};
  try {
    int idx = 1;
    inq = info.inquire("/NUOPC/General/foobar", rc, false, &idx);
  }
  ESMF_CATCH_PASSTHRU
  if(inq.at("jsonType")!="number") {
    return finalizeFailure(rc, failMsg, "Wrong inquire count with recursive");
  }
}

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
  try {
    update_target_attrs.update(used_to_update_attrs, rc);
  }
  ESMF_CATCH_PASSTHRU

  const json desired = R"( {"color": "blue", "price": 17.99, "speed": 100} )"_json;

  if (lhs != desired){
    return finalizeFailure(rc, failMsg, "Storage not updated");
  }

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "test_update_json_pointer()"
void test_update_json_pointer(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  bool failed = false;

  json j;
  j["foo"] = 5;
  j["nest"]["twice"]["foobar"] = "bingo";
  j["nest"]["twice"]["an_array"] = {44, 55, 66};
  j["nest"]["once"]["once_again"]["deeper"] = 77;

  //---------------------------------------------------------------------------

  json const *jdp = nullptr;
  json::json_pointer key("/wonderbar");
  bool recursive = false;
  try {
    update_json_pointer(j, &jdp, key, recursive);
    failed = true;
  } catch (json::out_of_range &e) {
    if (jdp) {
      return finalizeFailure(rc, failMsg, "Pointer should be null with missing key");
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Did not catch exception");
  }

  //---------------------------------------------------------------------------

  jdp = nullptr;
  json::json_pointer key2("/deeper");
  recursive = true;
  update_json_pointer(j, &jdp, key2, recursive);
  if (&*jdp!=&j["nest"]["once"]["once_again"]["deeper"]) {
    return finalizeFailure(rc, failMsg, "Pointer not updated with recursive");
  }

  //---------------------------------------------------------------------------

  jdp = nullptr;
  json::json_pointer key21("/not there");
  recursive = true;
  try {
    update_json_pointer(j, &jdp, key21, recursive);
    failed = true;
  } catch (json::out_of_range &e) {
    if (jdp) {
      return finalizeFailure(rc, failMsg, "Pointer should be null with missing key & recursive");
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Did not catch exception");
  }

  //---------------------------------------------------------------------------

  jdp = nullptr;
  json::json_pointer key3("/nest/twice/an_array");
  recursive = true;
  update_json_pointer(j, &jdp, key3, recursive);
  if (&*jdp!=&j["nest"]["twice"]["an_array"]) {
    return finalizeFailure(rc, failMsg, "Pointer not updated to array");
  }

  rc = ESMF_SUCCESS;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "test_infoview_update_ptr()"
void test_infoview_update_ptr(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  bool failed = false;

  json j = {{"a", 5}, {"b", 6}, {"c", {"rainbow", 555, 66.6}}};
  InfoView iview(j);

  json::json_pointer jp("/a");
  iview.update_storage_ptr(&jp, nullptr, false);
  long int *jip = nullptr;
  iview.update_ptr(&jip);
  int newi = 9999;
  *jip = newi;
  if (j["a"]!=newi) {
    return finalizeFailure(rc, failMsg, "update_ptr failed");
  }

  json::json_pointer jp2("/c");
  iview.update_storage_ptr(j);
  int idx = 2;
  iview.update_storage_ptr(&jp2, &idx, false);
  double *jdp = nullptr;
  iview.update_ptr(&jdp);
  *jdp = 77.99;
  if (j["c"].at(2)!=77.99) {
    return finalizeFailure(rc, failMsg, "update_ptr failed");
  }

  json::json_pointer jp3("/foobar");
  try {
    iview.update_storage_ptr(&jp3, nullptr, true);
    return finalizeFailure(rc, failMsg, "did not hit JSON out of range error");
  }
  catch (json::out_of_range &e) {}

  try {
    int idx = 55;
    iview.update_storage_ptr(j);
    iview.update_storage_ptr(&jp2, &idx, false);
    return finalizeFailure(rc, failMsg, "did not hit std out of range error");
  }
  catch (std::out_of_range &e) {}

  try {
    iview.update_storage_ptr(j);
    iview.update_storage_ptr(&jp, &idx, true);
    return finalizeFailure(rc, failMsg, "did not hit type error with idx");
  }
  catch (ESMCI::esmf_attrs_error &e) {}

  iview.update_storage_ptr(j);
  json::json_pointer jp4("/b");
  iview.update_storage_ptr(&jp4, nullptr, true);
  double *incompatp = nullptr;
  try {
    iview.update_ptr(&incompatp);
    return finalizeFailure(rc, failMsg, "did not hit type error with update_ptr");
  }
  catch (ESMCI::esmf_attrs_error &e) {}

  rc = ESMF_SUCCESS;
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
  //NEX_UTest
  strcpy(name, "Attributes formatKey()");
  testFormatKey(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Dump Length");
  testDumpLength(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Serialize/Deserialize");
  testSerializeDeserialize(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes Set/Get with an Index");
  testSetGetIndex(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes counting AttPacks");
  test_get_attpack_count(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes recursive counting");
  test_update_json_count(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes inquire");
  testInquire(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Attributes update_json_pointer");
  test_update_json_pointer(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "InfoView update_ptr");
  test_infoview_update_ptr(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
