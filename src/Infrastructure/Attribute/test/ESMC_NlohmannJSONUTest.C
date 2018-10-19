// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
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

// ESMF header
#include "ESMC.h"

// ESMF Test header
#include "ESMC_Test.h"

// JSON header
#include "json.hpp"

using namespace std;
using json = nlohmann::json;  // Convenience rename for JSON namespace

//==============================================================================
//BOP
// !PROGRAM: ESMC_NlohmannJSONUTest - Test the nlohmann/json for Modern C++
//           library
//
// !DESCRIPTION: Test inclusion of JSON header file and creating a basic object
//
//EOP
//------------------------------------------------------------------------------

class MockAttributesNoCopy
{
  public:
    json *storage;

    MockAttributesNoCopy(json &storage){
      this->storage = &storage;
    };

    string dump() const{
      return this->storage->dump(2);
    }

    json* getPointer() const{
      return this->storage;
    }
};

const long int * const runGetPointer(const string & key, const json & j){
  return j.at(key).get_ptr<const json::number_integer_t* const>();
}

int main(void){

  // Test variables
  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  //----------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Basic JSON Map Creation");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");

  json root;

  root["ESMF"] = json::object();

  root["ESMF"]["General"] = json::object();
  root["ESMF"]["Extended"] = json::object();

  // Reference the ESMF/Extended JSON object using keys
  root["ESMF"]["Extended"]["long_name"] = "foobar";

  // Reference the ESMF/General JSON object then add something to it
  json &general = root["ESMF"]["General"];
  general["what"] = "has been added";

  // Use a JSON Pointer
  json::json_pointer jp("/foo/bar/nest");
  root[jp] = "a deep nest";

  // Add an element then erase it
  json toClear = {{"one", 1}, {"two", 2}, {"three", 2.9}};
  root["toClear"] = toClear;
//  std::cout << root.dump(4) << std::endl;
  root.erase("toClear");

  ESMC_Test(true, name, failMsg, &result, __FILE__, __LINE__, 0);

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Reference and pointer access using the JSON API");
  bool failed = false;

  json j;
  int desired = 123;
  string key = "theNumber";
  j[key] = desired;

  int const & the_ref1 = j[key].get_ref<const json::number_integer_t&>();
  int const & the_ref2 = j[key].get_ref<const json::number_integer_t&>();
  if (the_ref1 != the_ref2){
    strcpy(failMsg, "Values not equal for references");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }
  if (&the_ref1 == &the_ref2){
    strcpy(failMsg, "Addresses are equal for references");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  auto ptr = j[key].get_ptr<const json::number_integer_t* const>();
  if (*ptr != desired){
    strcpy(failMsg, "Pointer value is not the desired value");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  const long int * ptr2 = runGetPointer(key, j);
  if (*ptr != *ptr2){
    strcpy(failMsg, "Pointer values are not equal");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }
  if (&*ptr != &*ptr2){
    strcpy(failMsg, "Pointer addresses are not equal");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  if (!failed){
    ESMC_Test(true, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Mock attributes constructor");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");

  json j2;
  MockAttributesNoCopy mattrs(j2);
  string key2 = "something";
  string value2 = "nothing";
  j2[key2] = value2;

  json *ref = mattrs.getPointer();
  string actual2 = ref->at(key2);

  failed = false;
  if (actual2 != value2){
    strcpy(failMsg, "Value not added to target JSON object");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }
  if (&j2 != &*ref){
    strcpy(failMsg, "Address target not equivalent");
    failed = true;
    ESMC_Test(false, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  if (!failed){
    ESMC_Test(true, name, failMsg, &result, __FILE__, __LINE__, 0);
  }

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
}
