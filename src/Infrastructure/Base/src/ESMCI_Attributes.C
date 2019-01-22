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
//-----------------------------------------------------------------------------

//tdk:REMOVE: only required for tdk prints
#include <netcdf.h>
#include <pio.h>

#include "ESMC.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <assert.h>
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

// tdk logging functions ======================================================

//tdk:REMOVE
void tdklog(const string& msg) {
  string localmsg = "tdk: " + msg;
//  std::cout << msg << std::endl; //tdk:p: THIS IS INSIDE tdklog
  ESMC_LogWrite(localmsg.c_str(), ESMC_LOGMSG_INFO);
}

//tdk:REMOVE
void tdklog(const string& msg, const vector<string>& v) {
  for (size_t ii = 0; ii < v.size(); ii++) {
    tdklog(msg + "[" + to_string(ii) + "]=" + v[ii]);
  }
}

//tdk:REMOVE
void tdklog(const string& msg, const vector<ESMC_I4>& v) {
  for (size_t ii = 0; ii < v.size(); ii++) {
    tdklog(msg + "[" + to_string(ii) + "]=" + to_string(v[ii]));
  }
}

//tdk:REMOVE
void tdklog(const string& msg, const vector<PIO_Offset>& v) {
  for (size_t ii = 0; ii < v.size(); ii++) {
    tdklog(msg + "[" + to_string(ii) + "]=" + to_string(v[ii]));
  }
}

//tdk:REMOVE
void tdklog(const string& msg, const int* l, std::size_t size) {
  for (size_t ii = 0; ii < size; ii++) {
    tdklog(msg + "[" + to_string(ii) + "]=" + to_string(l[ii]));
  }
}

//tdk:REMOVE
void tdklog(const string& msg, PIO_Offset* l, std::size_t size) {
  for (size_t ii = 0; ii < size; ii++) {
    tdklog(msg + "[" + to_string(ii) + "]=" + to_string(l[ii]));
  }
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "handleHasKey"
bool handleHasKey(const Attributes* attrs, const string& key, int& rc) {
  // Exceptions:  ESMCI::esmf_attrs_error
  bool has_key;
  try {
    has_key = attrs->hasKey(key, rc, true);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }

  if (has_key) {
    string msg = "Attribute key \'" + key + "\' already in map and force=false.";
    ESMF_CHECKERR_STD("ESMC_RC_CANNOT_SET", ESMC_RC_CANNOT_SET, msg, rc);
  }

  return has_key;
}

#undef ESMC_METHOD
#define ESMC_METHOD "isIn(<string,vector>)"
bool isIn(const string& target, const vector<string>& container) {
  auto it = std::find(container.cbegin(), container.cend(), target);
  return !(it == container.cend());
}

#undef ESMC_METHOD
#define ESMC_METHOD "isIn(<string vector,vector>)"
bool isIn(const vector<string>& target, const vector<string>& container) {
  size_t count = 0;
  size_t required = target.size();
  bool ret;
  if (target.size() == 0) {
    ret = true;
  } else {
    ret = false;
  }
  if (!ret) {
    for (const auto &t : target) {
      if (isIn(t, container)) {
        ++count;
      }
      if (count == required) {
        ret = true;
        break;
      }
    }
  }
  return ret;
}

#undef ESMC_METHOD
#define ESMC_METHOD "isIn(<string,json>)"
bool isIn(const string& target, const json& j) {
  if (j.is_null()) {
    return false;
  } else {
    return j.find(target) != j.end();
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes(json&)"
Attributes::Attributes(const json& storage){
  this->storage = storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes(json&&)"
Attributes::Attributes(json&& storage){
  this->storage = move(storage);
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes(string&)"
Attributes::Attributes(const string& input, int& rc) {
  // Exceptions: ESMCI::esmf_attrs_error
  // tdk:FEAT: use the parse method on the object!
  rc = ESMF_FAILURE;
  try {
    this->storage = json::parse(input);
  }
  catch (json::parse_error& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_OBJ_NOT_CREATED", ESMC_RC_OBJ_NOT_CREATED, rc);
  }
  rc = ESMF_SUCCESS;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump(int &rc)"
string Attributes::dump(int& rc) const {
  // Exceptions: ESMCI::esmf_attrs_error
  string ret;
  try {
    ret = this->dump(0, rc);
  } catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CATCH_PASSTHRU(e);
  }
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump(int indent, int &rc)"
string Attributes::dump(int indent, int& rc) const {
  // Exceptions: ESMCI::esmf_attrs_error
  rc = ESMF_FAILURE;
  string ret;
  try {
    ret = this->storage.dump(indent);
  } catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_INCOMP", ESMC_RC_ARG_INCOMP, rc);
  }
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::erase()"
void Attributes::erase(const string& keyParent, const string& keyChild, int& rc) {
  // Exceptions: ESMCI::esmf_attrs_error
  rc = ESMF_FAILURE;
  try {
    json::json_pointer jp = this->formatKey(keyParent, rc);
    try {
      json &target = this->storage.at(jp);
      json &found = target.at(keyChild);
      target.erase(keyChild);
    }
    catch (json::out_of_range &e) {
      ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
    }
    catch (json::type_error& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
    }
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::formatKey()"
json::json_pointer Attributes::formatKey(const string& key, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error
  rc = ESMF_FAILURE;
  string localKey;

  if (key != "" && key[0] != '/') {
    localKey = '/' + key;
  } else {
    localKey = key;
  }

  if (localKey.find("///") != string::npos){
    string msg = "Triple forward slashes not allowed in key names";
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
  }

  try {
    json::json_pointer jp(localKey);
    rc = ESMF_SUCCESS;
    return jp;
  }
  catch (json::parse_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::get()"
template <typename T>
T Attributes::get(const string& key, int& rc, T* def) const {
  // Exceptions:  ESMCI:esmf_attrs_error
  rc = ESMF_FAILURE;
  T ret;
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    if (def) {
      ret = this->storage.value(jp, *def);
    } else {
      try {
        ret = this->storage.at(jp);
      }
      catch (json::out_of_range& e) {
        ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
      }
      catch (json::type_error& e) {
        ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
      }
    }
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
  return ret;
}
template float Attributes::get(const string&, int&, float*) const;
template double Attributes::get(const string&, int&, double*) const;
template int Attributes::get(const string&, int&, int*) const;
template long int Attributes::get(const string&, int&, long int*) const;
template string Attributes::get(const string&, int&, string*) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getPointer()"
template <typename T, typename JT>
T Attributes::getPointer(const string& key, int& rc) const {
  // Exceptions:  ESMCI:esmf_attrs_error
  rc = ESMF_FAILURE;
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    try {
      T ret = this->storage.at(jp).get_ptr<JT>();
      rc = ESMF_SUCCESS;
      return ret;
    }
    catch (json::out_of_range& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
    }
    catch (json::type_error& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
    }
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
};
//  template const float* const Attributes::getPointer<const float* const,
//          const json::number_float_t* const>(const string&, int&) const;
template const double* const Attributes::getPointer<const double* const,
        const json::number_float_t* const>(const string&, int&) const;
//  template const int* const Attributes::getPointer<const int* const,
//          const json::number_integer_t* const>(const string&, int&) const;
template const long int* const Attributes::getPointer<const long int* const,
        const json::number_integer_t* const>(const string&, int&) const;
template const string* const Attributes::getPointer<const string* const,
        const json::string_t* const>(const string&, int&) const;
template const vector<json>* const Attributes::getPointer<const vector<json>*
        const, const json::array_t* const>(const string&, int&) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getStorageRef()"
const json& Attributes::getStorageRef() const{
  return this->storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getStorageRefWritable()"
json& Attributes::getStorageRefWritable() {
  return this->storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::hasKey()"
bool Attributes::hasKey(const string& key, int& rc, bool isptr) const {
  // Exceptions:  ESMCI::esmf_attrs_error
  // isptr is optional

  rc = ESMF_FAILURE;
  bool ret;
  if (isptr) {
    // Use JSON pointer syntax. This is slower than just attempting to find
    // the key. JSON pointers do not work with find. See: https://github.com/nlohmann/json/issues/1182#issuecomment-409708389
    // for an explanation.
    try {
      json::json_pointer jp = this->formatKey(key, rc);
      try {
        this->storage.at(jp);
        ret = true;
      }
      catch (json::out_of_range& e) {
        ret = false;
      }
    }
    catch (ESMCI::esmf_attrs_error &e) {
      ESMF_CATCH_PASSTHRU(e);
    }

  } else {
    // This is faster because it avoids exceptions. However, it does not work
    // with JSON pointers.
    ret = !(this->storage.find(key) == this->storage.end());
  }
  rc = ESMF_SUCCESS;
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::parse()"
void Attributes::parse(const string& input, int& rc) {
  rc = ESMF_FAILURE;
  try {
    this->storage = json::parse(input);
  }
  catch (json::parse_error& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_OBJ_NOT_CREATED", ESMC_RC_OBJ_NOT_CREATED, rc);
  }
  catch (json::type_error& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set()"
template <typename T>
void Attributes::set(const string& key, T value, bool force, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error

  rc = ESMF_FAILURE;
  if (!force) {
    try {
      bool has_key = handleHasKey(this, key, rc);
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_CATCH_PASSTHRU(exc_esmf);
    }
  }
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    try {
      this->storage[jp] = value;
    }
    catch (json::out_of_range &e) {
      ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
    }
    catch (json::type_error &e) {
      ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
    }
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
  rc = ESMF_SUCCESS;
  return;
};
template void Attributes::set<float>(const string&, float, bool, int&);
template void Attributes::set<double>(const string&, double, bool, int&);
template void Attributes::set<int>(const string&, int, bool, int&);
template void Attributes::set<long int>(const string&, long int, bool, int&);
template void Attributes::set<string>(const string&, string, bool, int&);

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set(<array>)"
void Attributes::set(const string& key, int values[], int& count, bool force, int& rc) {
  rc = ESMF_FAILURE;

  if (!force) {
    bool has_key = handleHasKey(this, key, rc);
  }

  json::json_pointer jp = this->formatKey(key, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  this->storage[jp] = json::array();
  //tdk:OPTIMIZE: can this "at" be removed?
  json::array_t* arr_ptr = this->storage.at(jp).get_ptr<json::array_t*>();
  arr_ptr->reserve(count);

  for (auto ii=0; ii<count; ii++) {
    arr_ptr->push_back(values[ii]);
  }

  rc = ESMF_SUCCESS;
  return;
};

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
esmf_attrs_error::esmf_attrs_error (const string& code_name, int rc,
                                    const string& msg) {
  assert(rc != ESMF_SUCCESS);
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
#define ESMC_METHOD "PackageFactory::getOrCreateJSON()"
json PackageFactory::getOrCreateJSON(const string& key, int& rc,
                                     const string& uri) {
  rc = ESMF_FAILURE;
  try {
    json ret = this->cache.at(key);
    rc = ESMF_SUCCESS;
    return ret;
  } catch (json::out_of_range& e) {
    string localuri;
    if (uri == "") {
      localuri = this->uris.value(key, uri);
    } else {
      localuri = uri;
    }
    std::ifstream i(localuri, std::ifstream::in);
    if (!i.good()){
      string errmsg = "File location is bad for key '" + key + "': " + localuri;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, errmsg, rc);
    }
    json j;
    try {
      i >> j;
    } catch (json::parse_error& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, rc);
    }
    i.close();
    this->cache[key] = move(j);
    return this->getOrCreateJSON(key, rc);
  }
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "broadcastAttributes()"
void broadcastAttributes(ESMCI::Attributes* attrs, int rootPet, int& rc) {
  rc = ESMF_FAILURE;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  int localPet = vm->getLocalPet();

  size_t target_size = 0;  // Size of serialized attributes storage
  string target;  // Serialize storage buffer
  if (localPet == rootPet) {
    // If this is the root, serialize the attributes storage to string
    target = attrs->dump(rc);
    ESMF_CHECKERR_STD("", rc, "Could not dump attributes", rc);

    target_size = target.size();
  }

  // Broadcast size of the string buffer holding the serialized attributes.
  // Used for allocating destination string buffers on receiving PETs.
  rc = vm->broadcast(&target_size, sizeof(target_size), rootPet);
  ESMF_CHECKERR_STD("", rc, "Did not broadcast string size", rc);

  string target_received(target_size, '\0');  // Allocate receive buffer
  if (localPet == rootPet) {
    // If this is root, just move the data to the receive buffer with no copy.
    target_received = move(target);
  }

  // Broadcast the string buffer
  rc = vm->broadcast(&target_received[0], target_size, rootPet);
  ESMF_CHECKERR_STD("", rc, "Did not broadcast string", rc);

  if (localPet != rootPet) {
    // If not root, then parse the incoming string buffer into attributes
    // storage.
    attrs->parse(target_received, rc);
    ESMF_CHECKERR_STD("", rc, "Did not parse string", rc);
  }

  return;
}

}  // namespace ESMCI

extern "C" {

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreate()"
ESMCI::Attributes* ESMC_AttributesCreate(int& rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesDestroy()"
void ESMC_AttributesDestroy(ESMCI::Attributes* attrs, int& rc) {
  delete attrs;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesErase()"
void ESMC_AttributesErase(ESMCI::Attributes* attrs, char* keyParent,
                          char* keyChild, int& rc) {
  rc = ESMF_FAILURE;
  std::string localkeyParent(keyParent);
  std::string localkeyChild(keyChild);

  // This seems strange. This is the best method to delete from the Fortran
  // interface to avoid passing "" as the parent key when you want to delete
  // from the root. Otherwise a parent and child key are always required which
  // seems redundant.
  if (localkeyChild == "") {
    return attrs->erase(localkeyChild, localkeyParent, rc);
  } else {
    return attrs->erase(localkeyParent, localkeyChild, rc);
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet()"
int ESMC_AttributesGet(ESMCI::Attributes* attrs, char* key, int& rc, int* def) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  int ret = attrs->get<int>(localKey, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArray()"
void ESMC_AttributesGetArray(ESMCI::Attributes* attrs, char* key, int* values,
        int& count, int& count_only, int& rc) {
  rc = ESMF_FAILURE;

  std::string localKey(key);

  const vector<json>* const ap = attrs->getPointer<const vector<json>* const,
          const json::array_t* const>(localKey, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not get array pointer",
          ESMC_CONTEXT, &rc)) throw(rc);

  count = (int)ap->size();
  if (count_only == 0) {
    for (auto ii=0; ii<count; ii++) {
      values[ii] = ap[0][ii];
    }
  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsPresent()"
int ESMC_AttributesIsPresent(ESMCI::Attributes* attrs, char* key, int& rc,
        int& isptr) {
  string local_key(key);
  int ret = attrs->hasKey(local_key, rc, isptr);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not detect key presence",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesPrint()"
void ESMC_AttributesPrint(ESMCI::Attributes* attrs, int& indent, int& rc) {
  rc = ESMF_FAILURE;
  std::cout << attrs->dump(indent, rc) << std::endl;
  if (ESMC_LogDefault.MsgFoundError(rc, "Dump failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet()"
void ESMC_AttributesSet(ESMCI::Attributes* attrs, char* key, int& value,
                        int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set<int>(localKey, value, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed",  ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArray()"
void ESMC_AttributesSetArray(ESMCI::Attributes* attrs, char* key, int* values,
                        int& count, int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set(localKey, values, count, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

}  // extern "C"
