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

using json = nlohmann::json;  // Convenience rename for JSON namespace.

using std::string;
using std::vector;

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
#define ESMC_METHOD "Attributes(string&)"
Attributes::Attributes(const string &input, int &rc){
  rc = ESMF_FAILURE;
  try {
    this->storage = json::parse(input);
  } catch (json::parse_error &e) {
    ESMC_LogDefault.MsgFoundError(ESMC_RC_OBJ_NOT_CREATED, e.what(),
                                  ESMC_CONTEXT, &rc);
    return;
  }
  rc = ESMF_SUCCESS;
  };

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump (no indent)"
  string Attributes::dump(int &rc){
    rc = ESMF_FAILURE;
    string ret = this->storage.dump();
    rc = ESMF_SUCCESS;
    return ret;
  };

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump (with indent)"
string Attributes::dump(int indent, int &rc){
  rc = ESMF_FAILURE;
  string ret = this->storage.dump(indent);
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::erase"
void Attributes::erase(const string &keyParent, const string &keyChild, int &rc){
  rc = ESMF_FAILURE;

  json::json_pointer jp(keyParent);

  try {
    json &target = this->storage.at(jp);
    try {
      // Will throw out_of_range if key does not exist.
      json &found = target.at(keyChild);

      target.erase(keyChild);
    } catch (json::out_of_range& e) {
      ESMC_LogDefault.MsgFoundError(ESMC_RC_NOT_FOUND, e.what(),
                                    ESMC_CONTEXT, &rc);
      return;
    }
  } catch (json::out_of_range& e) {
      ESMC_LogDefault.MsgFoundError(ESMC_RC_NOT_FOUND, e.what(),
                                    ESMC_CONTEXT, &rc);
      return;
  }

  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getStorageRef"
const json& Attributes::getStorageRef() const{
  return this->storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::get"
template <typename T, typename JT>
T Attributes::get(const string &key, int &rc) const{
  rc = ESMF_FAILURE;
  json::json_pointer jp(key);
  try {
    T ret = this->storage.at(jp).get_ptr<JT>();
    rc = ESMF_SUCCESS;
    return ret;
  } catch (json::out_of_range& e) {
    ESMC_LogDefault.MsgFoundError(ESMC_RC_NOT_FOUND, e.what(),
                                  ESMC_CONTEXT, &rc);
    return 0;
  }
};
template const long int* const Attributes::get<const long int* const,
                const json::number_integer_t* const>(const string&, int&) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set"
template <typename T>
void Attributes::set(const string &key, T value, bool force, int &rc){
  rc = ESMF_FAILURE;
  json::json_pointer jp(key);
  if (!force){
    try {
      T result = this->storage.at(jp);
      string msg = "Attribute key \"" + key + "\" already in map and force=false.";
      ESMC_LogDefault.MsgFoundError(ESMC_RC_CANNOT_SET, msg, ESMC_CONTEXT, &rc);
      return;
    } catch (json::out_of_range){
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
