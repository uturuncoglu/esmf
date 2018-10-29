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
#define ESMC_METHOD "Attributes()"
Attributes::Attributes(void){};

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
    ESMF_THROW_JSON(e, "ESMC_RC_OBJ_NOT_CREATED", ESMC_RC_OBJ_NOT_CREATED, rc);
  }
  rc = ESMF_SUCCESS;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump(int &rc)"
string Attributes::dump(int &rc) const{
  rc = ESMF_FAILURE;
  string ret = this->storage.dump();
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump(int indent, int &rc)"
string Attributes::dump(int indent, int &rc) const{
  rc = ESMF_FAILURE;
  string ret = this->storage.dump(indent);
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::erase()"
void Attributes::erase(const string &keyParent, const string &keyChild, int &rc){
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(keyParent, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  try {
    json &target = this->storage.at(jp);
    try {
      // Will throw out_of_range if key does not exist.
      json &found = target.at(keyChild);

      target.erase(keyChild);
    } catch (json::out_of_range& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
    }
  } catch (json::out_of_range& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::formatKey()"
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
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
  }

  json::json_pointer jp(localKey);
  rc = ESMF_SUCCESS;

  return jp;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::get()"
template <typename T>
T Attributes::get(const string &key, int &rc) const {
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  try {
    T ret = this->storage.at(jp);
    rc = ESMF_SUCCESS;
    return ret;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
}
template int Attributes::get(const string&, int&) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getPointer()"
template <typename T, typename JT>
T Attributes::getPointer(const string &key, int &rc) const {
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  try {
    T ret = this->storage.at(jp).get_ptr<JT>();
    rc = ESMF_SUCCESS;
    return ret;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
};
template const long int* const Attributes::getPointer<const long int* const,
                const json::number_integer_t* const>(const string&, int&) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getStorageRef()"
const json& Attributes::getStorageRef() const{
  return this->storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::hasKey()"
bool Attributes::hasKey(const string &key, int &rc) const{
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

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
#define ESMC_METHOD "Attributes::set()"
template <typename T>
void Attributes::set(const string &key, T value, bool force, int &rc) {
  rc = ESMF_FAILURE;

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if (!force) {
    try {
      T result = this->storage.at(jp);
      string msg = "Attribute key \'" + key + "\' already in map and force=false.";
      ESMF_CHECKERR_STD("ESMC_RC_CANNOT_SET", ESMC_RC_CANNOT_SET, msg, rc);
    }
    catch (json::out_of_range) {
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
#define ESMC_METHOD "Attributes::update()"
void Attributes::update(const Attributes &attrs, int &rc) {
  rc = ESMF_FAILURE;
  const json& r_j = attrs.getStorageRef();
  this->storage.update(r_j);
  rc = ESMF_SUCCESS;
  return;
};

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "esmf_attrs_error::esmf_attrs_error()"
esmf_attrs_error::esmf_attrs_error (const string &code_name, int rc,
                                    const string &msg) {
  string the_msg;
  if (code_name != "") {
    the_msg = "Error/Return Code " + std::to_string(rc) + " (" + \
                     code_name + ") - " + msg;
  } else {
    the_msg = "Error/Return Code " + std::to_string(rc) + " - " + msg;
  }
  this->msg = the_msg;
  this->rc = rc;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "createJSONPackage()"
json createJSONPackage(const string &pkgKey, int &rc) {
  rc = ESMF_FAILURE;

  json j;
  j["name"] = json::value_t::null;  // Will be string

  if (pkgKey == "ESMF:Metadata:Dimension") {
    j["size"] = json::value_t::null;  // Will be int or potentially null if unlimited
    j["is_unlimited"] = json::value_t::null;  // Will be bool
  } else if (pkgKey == "ESMF:Metadata:Group") {
    j["variables"] = json::object();
    j["dimensions"] = json::object();
    j["attrs"] = json::object();
    j["groups"] = json::object();
    j["uri"] = json::value_t::null;  // Will be string or rename null
  } else if (pkgKey == "ESMF:Metadata:Variable") {
    j["dtype"] = json::value_t::null;  // Will be string
    j["dimensions"] = json::array();  // Will append string dimension names
    j["attrs"] = json::object();
  } else {
    string msg = "Package name not found: \'" + pkgKey + "\'";
    ESMF_CHECKERR_STD("ESMC_RC_NOT_FOUND", ESMF_RC_NOT_FOUND, msg, rc);
  }

  rc = ESMF_SUCCESS;
  return j;
}

}  // namespace ESMCI

extern "C" {

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreate()"
ESMCI::Attributes* ESMC_AttributesCreate(int &rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesDestroy()"
void ESMC_AttributesDestroy(ESMCI::Attributes *attrs, int &rc) {
  delete attrs;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet()"
int ESMC_AttributesGet(ESMCI::Attributes *attrs, char *key, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  int ret = attrs->get<int>(localKey, rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet()"
void ESMC_AttributesSet(ESMCI::Attributes *attrs, char *key, int &value,
                        int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set<int>(localKey, value, localforce, rc);
}

}  // extern "C"
