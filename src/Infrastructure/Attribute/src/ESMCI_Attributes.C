// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMCI_Attributes.C"

// Attribute method implementation (body) file

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
// tdk: DOC: add description
//
//-----------------------------------------------------------------------------

#include "ESMC.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "json.hpp"

#include <vector>
#include <iostream>

using namespace std;

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes"
Attributes::Attributes(void){};

#undef  ESMC_METHOD
#define ESMC_METHOD "~Attributes"
Attributes::~Attributes(void){};

#undef  ESMC_METHOD
#define ESMC_METHOD "~Attributes(json&)"
Attributes::Attributes(const json &storage){
  this->storage = storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "~Attributes(json&&)"
  Attributes::Attributes(json &&storage){
    this->storage = move(storage);
  };

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes(string&)"
Attributes::Attributes(const string &input, int &rc){
  rc = ESMF_FAILURE;
  try {
    this->storage = json::parse(input);
  } catch (json::parse_error &e) {
    ESMF_THROW_JSON(e, ESMC_RC_OBJ_NOT_CREATED, rc);
  }
  rc = ESMF_SUCCESS;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump (no indent)"
string Attributes::dump(int &rc) const{
  rc = ESMF_FAILURE;
  string ret = this->storage.dump();
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump (with indent)"
string Attributes::dump(int indent, int &rc) const{
  rc = ESMF_FAILURE;
  string ret = this->storage.dump(indent);
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::erase"
void Attributes::erase(const string &keyParent, const string &keyChild, int &rc){
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(keyParent, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  try {
    json &target = this->storage.at(jp);
    try {
      // Will throw out_of_range if key does not exist.
      json &found = target.at(keyChild);

      target.erase(keyChild);
    } catch (json::out_of_range& e) {
      ESMF_THROW_JSON(e, ESMC_RC_NOT_FOUND, rc);
    }
  } catch (json::out_of_range& e) {
      ESMF_THROW_JSON(e, ESMC_RC_NOT_FOUND, rc);
  }

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::formatKey"
json::json_pointer Attributes::formatKey(const string &key, int &rc) {
  rc = ESMF_FAILURE;
  string localKey;

  if (key[0] != '/') {
    localKey = '/' + key;
  } else {
    localKey = key;
  }

  if (localKey.find("///") != string::npos){
    string msg = "Triple forward slashes not allowed in key names";
    ESMF_CHECKERR_STD(ESMC_RC_ARG_BAD, msg, rc);
  }

  json::json_pointer jp(localKey);
  rc = ESMF_SUCCESS;

  return jp;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::get"
template <typename T, typename JT>
T Attributes::get(const string &key, int &rc) const{
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  try {
    T ret = this->storage.at(jp).get_ptr<JT>();
    rc = ESMF_SUCCESS;
    return ret;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, ESMC_RC_NOT_FOUND, rc);
  }
};
template const long int* const Attributes::get<const long int* const,
                const json::number_integer_t* const>(const string&, int&) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getStorageRef"
const json& Attributes::getStorageRef() const{
  return this->storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set"
bool Attributes::hasKey(const string &key, int &rc) const{
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  bool ret;
  try{
    this->storage.at(jp);
    ret = true;
  } catch (json::out_of_range& e) {
    ret = false;
  }
  rc = ESMF_SUCCESS;
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set"
template <typename T>
void Attributes::set(const string &key, T value, bool force, int &rc){
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD(rc, ESMCI_ERR_PASSTHRU, rc);

  if (!force){
    try {
      T result = this->storage.at(jp);
      string msg = "Attribute key \"" + key + "\" already in map and force=false.";
      ESMF_CHECKERR_STD(ESMC_RC_CANNOT_SET, msg, rc);
    }
    catch (json::out_of_range){
      // Key is not found in the map. Just pass on through.
      // See: https://github.com/nlohmann/json/issues/1194#issuecomment-413002974
    }
  }
  this->storage[jp] = value;
  rc = ESMF_SUCCESS;
  return;
};
template void Attributes::set<int>(const string&, int, bool, int&);

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::update"
void Attributes::update(const Attributes &attrs, int &rc){
  rc = ESMF_FAILURE;
  const json& r_j = attrs.getStorageRef();
  this->storage.update(r_j);
  rc = ESMF_SUCCESS;
  return;
};

}  // namespace
