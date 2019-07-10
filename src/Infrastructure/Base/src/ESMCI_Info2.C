// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMCI_Info2.C"

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
#include "ESMCI_Info2.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

//-----------------------------------------------------------------------------
// Helper Functions -----------------------------------------------------------
//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "alignOffset()"
void alignOffset(int &offset) {
  int nbytes = offset % 8;
  if (nbytes!=0) offset += (8 - nbytes);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "is_attpack()"
bool is_attpack(const json &j) {
  bool ret = false;
  if (j.is_object() && j.size()==1 && j.cbegin().value().is_object()) {
    ret = true;
  }
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "get_attpack_count()"
std::size_t get_attpack_count(const json &j) {
  // Test: test_get_attpack_count
  assert(j.is_object());
  std::size_t ret = 0;
  for (json::const_iterator it=j.cbegin(); it!=j.cend(); it++) {
    if (is_attpack(it.value())) {ret++;}
  }
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "update_json_count()"
void update_json_count(std::size_t &count, std::size_t &count_total, const json &j, bool recursive) {
  // Test: test_update_json_count
  // Notes: if recursive, then attpacks and any other nested objects will be traversed
  //  - "count" is only single-valued info
  //  - "countTotal" includes objects and arrays
  assert(j.is_object());
  std::size_t ctr = 0;
  for (json::const_iterator it=j.cbegin(); it!=j.cend(); it++) {
    if (is_attpack(it.value())) {
      if (ctr>0 && !recursive) {break;}
      update_json_count(count, count_total, it.value().cbegin().value(), recursive);
    } else {
      count_total++;
      if (!it.value().is_structured()) {
        count++;
      }
      if (it.value().is_object() && recursive) {
        update_json_count(count, count_total, it.value(), recursive);
      }
    }
    ctr++;
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "update_json_pointer(<const>)"
void update_json_pointer(const json &j, json const **jdp, const json::json_pointer &key, bool recursive) {
  // Test: test_update_json_pointer
  // Notes:
  // Throws: json::out_of_range when key not found
  try {
    *jdp = &(j.at(key));
  } catch (json::out_of_range &e) {
    if (recursive) {
      for (json::const_iterator it=j.cbegin(); it!=j.cend(); it++) {
        if (it.value().is_object()) {
          update_json_pointer(it.value(), jdp, key, true);
        }
      }
    }
    if (!*jdp) {
      throw(e);
    }
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "update_json_pointer(<non-const>)"
void update_json_pointer(json &j, json **jdp, const json::json_pointer &key, bool recursive) {
  // Test: test_update_json_pointer (for const overload)
  // Notes:
  // Throws: json::out_of_range when key not found
  try {
    *jdp = &(j.at(key));
  } catch (json::out_of_range &e) {
    if (recursive) {
      for (json::iterator it=j.begin(); it!=j.end(); it++) {
        if (it.value().is_object()) {
          update_json_pointer(it.value(), jdp, key, true);
        }
      }
    }
    if (!*jdp) {
      throw(e);
    }
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "handleHasKey"
bool handleHasKey(const Info2* info, key_t& key, int& rc) {
  // Exceptions:  ESMCI::esmf_info_error

  bool has_key;
  try {
    has_key = info->hasKey(key, rc, true);
  }
  catch (ESMCI::esmf_info_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
  if (has_key) {
    std::string msg = "Attribute key \'" + key + "\' already in map and force=false.";
    ESMF_CHECKERR_STD("ESMC_RC_CANNOT_SET", ESMC_RC_CANNOT_SET, msg, rc);
  }
  return has_key;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "handleJSONTypeCheck()"
void handleJSONTypeCheck(key_t &key, const json &src, const json &dst, int &rc) {
  if (!src.is_null() && src.type() != dst.type()) {
    std::string errmsg = "Types not equivalent. The key is: " + key;
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, errmsg, rc);
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "isIn(<string,vector>)"
bool isIn(key_t& target, const std::vector<std::string>& container) {
  auto it = std::find(container.cbegin(), container.cend(), target);
  return !(it == container.cend());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "isIn(<string vector,vector>)"
bool isIn(const std::vector<std::string>& target, const std::vector<std::string>& container) {
  std::size_t count = 0;
  std::size_t required = target.size();
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

#undef  ESMC_METHOD
#define ESMC_METHOD "isIn(<string,json>)"
bool isIn(key_t& target, const json& j) {
  if (j.is_null()) {
    return false;
  } else {
    return j.find(target) != j.end();
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "json_type_to_esmf_typekind()"
ESMC_TypeKind_Flag json_type_to_esmf_typekind(const json &j) noexcept {
  json jj;
  jj["foo"] = ESMC_TYPEKIND_I1;
  ESMC_TypeKind_Flag esmf_type;
  if (j.type() == json::value_t::null) {
    esmf_type = ESMF_NOKIND;
  } else if (j.type() == json::value_t::boolean) {
    esmf_type = ESMC_TYPEKIND_LOGICAL;
  } else if (j.type() == json::value_t::number_integer) {
    esmf_type = ESMC_TYPEKIND_I8;
  } else if (j.type() == json::value_t::number_unsigned) {
    esmf_type = ESMC_TYPEKIND_I8;
  } else if (j.type() == json::value_t::number_float) {
    esmf_type = ESMC_TYPEKIND_R8;
  } else if (j.type() == json::value_t::object) {
    esmf_type = ESMF_NOKIND;
  } else if (j.type() == json::value_t::array) {
    esmf_type = ESMF_NOKIND;
  } else if (j.type() == json::value_t::string) {
    esmf_type = ESMC_TYPEKIND_CHARACTER;
  } else {
    assert(false);
  }
  return esmf_type;
}

//-----------------------------------------------------------------------------
// Info Implementations -------------------------------------------------------
//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2(json&)"
Info2::Info2(const json& storage){
  this->storage = storage;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2(json&&)"
Info2::Info2(json&& storage){
  if (!storage.is_object()) {
    std::string msg = "Can only create Info from JSON value_t::object_t types";
    int rc;
    ESMF_CHECKERR_STD("ESMC_RC_OBJ_NOT_CREATED", ESMC_RC_OBJ_NOT_CREATED, msg, rc);
  }
  this->storage = move(storage);
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2(string&)"
Info2::Info2(key_t& input, int& rc) {
  // Exceptions: ESMCI::esmf_info_error
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
#define ESMC_METHOD "Info2::dump(int &rc)"
std::string Info2::dump(int& rc) const {
  // Exceptions: ESMCI::esmf_info_error

  std::string ret;
  try {
    ret = this->dump(0, rc);
  } catch (ESMCI::esmf_info_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::dump(int indent, int &rc)"
std::string Info2::dump(int indent, int& rc) const {
  // Exceptions: ESMCI::esmf_info_error

  rc = ESMF_FAILURE;
  std::string ret;
  try {
    ret = this->getStorageRef().dump(indent);
  } catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_INCOMP", ESMC_RC_ARG_INCOMP, rc);
  }
  rc = ESMF_SUCCESS;
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::erase()"
void Info2::erase(key_t &keyParent, key_t &keyChild, int &rc) {
  // Exceptions: ESMCI::esmf_info_error

  rc = ESMF_FAILURE;
  try {
    json::json_pointer jp = this->formatKey(keyParent, rc);
    try {
      json &target = this->getStorageRefWritable().at(jp);
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
  catch (ESMCI::esmf_info_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
  this->dirty = true;
  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::formatKey()"
json::json_pointer Info2::formatKey(key_t& key, int& rc) {
  // Exceptions:  ESMCI:esmf_info_error

  rc = ESMF_FAILURE;
  std::string localKey;

  if (key != "" && key[0] != '/') {
    localKey = '/' + key;
  } else {
    localKey = key;
  }

  if (localKey.find("///") != std::string::npos){
    std::string msg = "Triple forward slashes not allowed in key names";
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
#define ESMC_METHOD "Info2::get()"
template <typename T>
T Info2::get(key_t &key, int &rc, const T *def, const int *index, bool recursive, std::string *ikey) const {
  // Exceptions:  ESMCI:esmf_info_error

  rc = ESMF_FAILURE;
  T ret;
  try {
    json::json_pointer jpath = this->formatKey(key, rc);
    try {
      json const *jp = nullptr;
      update_json_pointer(this->getStorageRef(), &jp, jpath, recursive);
      assert(jp);
      if (index) {
        if (jp->is_array()) {
          json::array_t const *jarr = jp->get_ptr < json::array_t const * > ();
          try {
            ret = jarr->at(*index);
          }
          catch (std::out_of_range &e) {
            ESMF_CHECKERR_STD("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE,
                              e.what(), rc)
          }
        } else if (jp->is_object()) {
          if (*index >= (int)jp->size()) {
            std::string msg = "'index' greater than object count";
            ESMF_CHECKERR_STD("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE,
                              msg, rc)
          }
          int ctr = 0;
          for (json::const_iterator it=jp->cbegin(); it!=jp->cend(); it++) {
            if (ctr == *index) {
              ret = it.value();
              if (ikey) {
                *ikey = it.key();
              }
              break;
            } else {
              ctr++;
            }
          }
        } else {
          std::string msg = "'index' only supported for JSON arrays or objects";
          ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
        }
      } else {
        ret = *jp;
      }
    } catch (json::out_of_range &e) {
      if (def) {
        ret = *def;
      } else {
        ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc)
      }
    }
  }
  ESMF_CATCH_INFO
  return ret;
}
template float Info2::get(key_t&, int&, const float*, const int*, bool, std::string*) const;
template double Info2::get(key_t&, int&, const double*, const int*, bool, std::string*) const;
template int Info2::get(key_t&, int&, const int*, const int*, bool, std::string*) const;
template long int Info2::get(key_t&, int&, const long int*, const int*, bool, std::string*) const;
template bool Info2::get(key_t&, int&, const bool*, const int*, bool, std::string*) const;
template std::string Info2::get(key_t&, int&, const std::string*, const int*, bool, std::string*) const;
template json Info2::get(key_t&, int&, const json*, const int*, bool, std::string*) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::get() vector<T>"
template <typename T>
std::vector<T> Info2::getvec(key_t& key, int& rc, bool recursive) const {
  std::vector<T> ret;
  try {
    json const *j;
    try {
      j = this->getPointer(key, rc, recursive);
    }
    ESMF_CATCH_PASSTHRU
    ret = j->get<std::vector<T>>();
  }
  ESMF_CATCH_INFO
  return ret;
};
template std::vector<std::string> Info2::getvec(key_t&, int&, bool) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::getPointer()"
json const * Info2::getPointer(key_t& key, int& rc, bool recursive) const {
  // Exceptions:  ESMCI:esmf_info_error

  rc = ESMF_FAILURE;
  json const *ret = nullptr;
  try {
    json::json_pointer jpath = this->formatKey(key, rc);
    try {
      update_json_pointer(this->getStorageRef(), &ret, jpath, recursive);
      assert(ret);
    }
    ESMF_CATCH_JSON
  }
  ESMF_CATCH_INFO
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::hasKey()"
bool Info2::hasKey(key_t& key, int& rc, bool isptr) const {
  // Exceptions:  ESMCI::esmf_info_error
  // isptr is optional

  rc = ESMF_FAILURE;
  bool ret;
  const json &storage = this->getStorageRef();
  if (isptr) {
    // Use JSON pointer syntax. This is slower than just attempting to find
    // the key. JSON pointers do not work with find. See: https://github.com/nlohmann/json/issues/1182#issuecomment-409708389
    // for an explanation.
    try {
      json::json_pointer jp = this->formatKey(key, rc);
      try {
        storage.at(jp);
        ret = true;
      }
      catch (json::out_of_range& e) {
        ret = false;
      }
    }
    catch (ESMCI::esmf_info_error &e) {
      ESMF_HANDLE_PASSTHRU(e);
    }

  } else {
    // This is faster because it avoids exceptions. However, it does not work
    // with JSON pointers.
    ret = !(storage.find(key) == storage.end());
  }
  rc = ESMF_SUCCESS;
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::hasKey()"
bool Info2::hasKey(const json::json_pointer &jp, int& rc) const {
  // Exceptions:  ESMCI::esmf_info_error
  rc = ESMF_FAILURE;
  bool ret;
  const json &storage = this->getStorageRef();
  // Use JSON pointer syntax. This is slower than just attempting to find
  // the key. JSON pointers do not work with find. See: https://github.com/nlohmann/json/issues/1182#issuecomment-409708389
  // for an explanation.
  try {
    try {
      storage.at(jp);
      ret = true;
    }
    catch (json::out_of_range& e) {
      ret = false;
    }
  }
  ESMF_CATCH_PASSTHRU
  rc = ESMF_SUCCESS;
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::inquire()"
json Info2::inquire(key_t &key, int &rc, bool recursive, const int *idx) const {
  // Test: testInquire
  // Notes: if key="" then the base storage object is used; recursive=true is
  //  equivalent to turning on the attnestflag
  rc = ESMF_FAILURE;
  json j = json::object();
  try {
    j["isDirty"] = this->isDirty();
    j["key"] = json::value_t::null;
    const json *sp = &(this->getStorageRef());
    std::size_t attPackCount = 0;
    if (key!="") {
      json::json_pointer jp = this->formatKey(key, rc);
      update_json_pointer(this->getStorageRef(), &sp, jp, recursive);
    } else {
      attPackCount = get_attpack_count(*sp);
    }
    if (idx) {
      if (sp->is_array()) {
        try {
          sp = &(sp->at(*idx));
        }
        catch (std::out_of_range &e) {
          ESMF_CHECKERR_STD("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE,
                            e.what(), rc)
        }
      } else if (sp->is_object()) {
        if (*idx >= (int)sp->size()) {
          std::string msg = "'idx' greater than object count";
          ESMF_CHECKERR_STD("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE,
                            msg, rc)
        }
        int ctr = 0;
        for (json::const_iterator it=sp->cbegin(); it!=sp->cend(); it++) {
          if (ctr == *idx) {
            sp = &(it.value());
            j["key"] = it.key();
            break;
          } else {
            ctr++;
          }
        }
      } else {
        std::string msg = "'idx' only supported for JSON arrays or objects";
        ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
      }
    }
    const json &sk = *sp;
    j["ESMC_TypeKind_Flag"] = json_type_to_esmf_typekind(sk);
    std::size_t count = 0;
    std::size_t count_total = 0;
    if (!sk.is_array() && recursive) {
      update_json_count(count, count_total, sk, true);
    } else {
      count = sk.size();
    }
    j["count"] = count;
    j["countTotal"] = count_total;
    j["attPackCount"] = attPackCount;
    std::string json_typename;
    bool is_array = false;
    if (sk.is_array()) {
      is_array = true;
      const json &e = sk[0];
      json_typename = e.type_name();
    } else {
      json_typename = sk.type_name();
    }
    j["jsonType"] = json_typename;
    j["isArray"] = is_array;
  }
  ESMF_CATCH_INFO
  rc = ESMF_SUCCESS;
  return j;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::parse()"
void Info2::parse(key_t& input, int& rc) {
  // Exceptions:  ESMCI:esmf_info_error

  rc = ESMF_FAILURE;
  try {
    this->getStorageRefWritable() = json::parse(input);
  }
  ESMF_CATCH_JSON
  rc = ESMF_SUCCESS;
  return;
}

//tdk: ORDER
#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::deserialize()"
void Info2::deserialize(char *buffer, int *offset, int &rc) {
  // Exceptions:  ESMCI:esmf_info_error
  rc = ESMF_FAILURE;
  alignOffset(*offset);
  // Act like an integer to get the string length.
  int *ibuffer = reinterpret_cast<int*>(buffer);
  // Get the serialized string length from the buffer start.
  int length = ibuffer[*offset];
  // Move 4 bytes to the start of the string actual.
  (*offset) += 4;
  std::string infobuffer(&(buffer[*offset]), length);
  try {
    this->parse(infobuffer, rc);
  }
  catch (esmf_info_error &e) {
    ESMF_HANDLE_PASSTHRU(e);
  }
  (*offset) += length;
  alignOffset(*offset);
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::get_isoc()"
void Info2::get_isoc(ESMCI::ESMC_ISOCType ictype, void *ret, char* key,
  int& rc, void* def) const {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    if (ictype == ESMCI::C_INT) {
      *(reinterpret_cast<int *>(ret)) = this->get<int>(localKey, rc,
        reinterpret_cast<int *>(def));
    } else if (ictype == ESMCI::C_LONG) {
      *(reinterpret_cast<long int *>(ret)) = this->get<long int>(localKey, rc,
        reinterpret_cast<long int *>(def));
    } else if (ictype == ESMCI::C_FLOAT) {
      *(reinterpret_cast<float *>(ret)) = this->get<float>(localKey, rc,
        reinterpret_cast<float *>(def));
    } else if (ictype == ESMCI::C_DOUBLE) {
      *(reinterpret_cast<double *>(ret)) = this->get<double>(localKey, rc,
        reinterpret_cast<double *>(def));
    } else if (ictype == ESMCI::C_CHAR) {
      // Reinterpret casts from void to character types
      char *local_ret = reinterpret_cast<char *>(ret);
      char *local_def = reinterpret_cast<char *>(def);
      // String pointer used to define the default value if present
      std::string *local_def_str_ptr;
      // String object that holds the default if present
      std::string local_def_str;
      if (local_def) {
        // Set the default pointer to the string object created from the char
        // array from Fortran
        local_def_str = std::string(local_def);
        local_def_str_ptr = &local_def_str;
      } else {
        local_def_str_ptr = nullptr;
      }
      std::string as_str;
      try {
        as_str = this->get<std::string>(localKey, rc, local_def_str_ptr);
      }
      catch (ESMCI::esmf_info_error &exc_esmf) {
        ESMF_HANDLE_PASSTHRU(exc_esmf);
      }
      // Transfer the string characters into the Fortran character array using
      // spaces to fill the Fortran array if we are past the max string length.
      for (std::size_t ii = 0; ii < ESMF_MAXSTR; ++ii) {
        if (ii < as_str.size()) {
          local_ret[ii] = as_str[ii];
        } else {
          local_ret[ii] = ' ';
        }
      }
    } else {
      std::string msg = "ESMC_ISOCType not supported: " + std::to_string(ictype);
      ESMF_CHECKERR_STD("ESMC_RC_NOT_IMPL", ESMC_RC_NOT_IMPL, msg, rc);
    }
  }
  catch (ESMCI::esmf_info_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::isSetNull()"
bool Info2::isSetNull(key_t &key, int &rc) const {
  rc = ESMF_FAILURE;
  bool ret;
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    try {
      ret = !(this->getStorageRef().at(jp).is_null());
    }
    ESMF_CATCH_JSON
  }
  catch (ESMCI::esmf_info_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::serialize()"
void Info2::serialize(char *buffer, int *length, int *offset,
  ESMC_InquireFlag inquireflag, int& rc) {
  // Exceptions:  ESMCI:esmf_info_error
  rc = ESMF_FAILURE;
  std::string infobuffer;
  try {
    infobuffer = this->dump(rc);
  }
  catch (ESMCI::esmf_info_error &exc_esmf) {
    ESMF_HANDLE_PASSTHRU(exc_esmf);
  }
  // Adjust the buffer length to account for string attribute representation.
  int n = (int)infobuffer.length();
  *length += n;
  // Need 32 bits (4 bytes) to store the length of the string buffer for a
  // later deserialize.
  *length += 4;
  // If this is not an inquire operation, transfer the string info dump
  // into the serialization buffer. Update the offset in the process.
  if (inquireflag != ESMF_INQUIREONLY) {
    alignOffset(*offset);
    int *ibuffer = reinterpret_cast<int*>(buffer);
    ibuffer[*offset] = n;
    (*offset) += 4;
    for (int ii=0; ii<n; ++ii) {
      buffer[*offset] = infobuffer[ii];
      (*offset)++;
    }
    alignOffset(*offset);
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::set(<Info2>)"
void Info2::set(key_t &key, const ESMCI::Info2 &info, bool force,
  int &rc) {
  //tdk:todo: type-safe overload?
  //tdk:test: in c++
  rc = ESMF_FAILURE;
  if (!force) {
    try {
      bool has_key = handleHasKey(this, key, rc);
    }
    catch (ESMCI::esmf_info_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
  }
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    this->getStorageRefWritable()[jp] = info.getStorageRef();
  }
  ESMF_CATCH_INFO;
  this->dirty = true;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::set(<null>)"
void Info2::set(key_t &key, bool force, int &rc) {
  rc = ESMF_FAILURE;
  if (!force) {
    try {
      bool has_key = handleHasKey(this, key, rc);
    }
    catch (ESMCI::esmf_info_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
  }
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    this->getStorageRefWritable()[jp] = json::value_t::null;
  }
  ESMF_CATCH_INFO
  this->dirty = true;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::set(<scalar>)"
template <typename T>
void Info2::set(key_t &key, T value, bool force, int &rc, const int *index) {
  // Exceptions:  ESMCI:esmf_info_error
  rc = ESMF_FAILURE;
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    bool has_key = false;
    json jvalue = value;
    json &wstore = this->getStorageRefWritable();
    if (!index) {
      try {
        has_key = this->hasKey(jp, rc);
      }
      ESMF_CATCH_PASSTHRU
      if (has_key && !force) {
        std::string msg = "Attribute key \'" + key + "\' already in map and "
                                                     "force=false.";
        ESMF_CHECKERR_STD("ESMC_RC_CANNOT_SET", ESMC_RC_CANNOT_SET, msg, rc);
      }
    }
    if (index) {
      json::array_t *arr_ptr = wstore.at(jp).get_ptr<json::array_t *>();
      try {
        json jat = arr_ptr->at(*index);
        try {
          handleJSONTypeCheck(key, jat, jvalue, rc);
        }
        ESMF_CATCH_PASSTHRU
        arr_ptr->at(*index) = jvalue;
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE,
                          std::string(exc.what()), rc);
      }
    } else {
      if (has_key) {
        json jat = wstore.at(jp);
        try {
          handleJSONTypeCheck(key, jat, jvalue, rc);
        }
        ESMF_CATCH_PASSTHRU
      }
      wstore[jp] = jvalue;
    }
  }
  ESMF_CATCH_INFO
  this->dirty = true;
};
template void Info2::set<float>(key_t&, float, bool, int&, const int*);
template void Info2::set<double>(key_t&, double, bool, int&, const int*);
template void Info2::set<int>(key_t&, int, bool, int&, const int*);
template void Info2::set<long int>(key_t&, long int, bool, int&, const int*);
template void Info2::set<std::string>(key_t&, std::string, bool, int&, const int*);
template void Info2::set<bool>(key_t&, bool, bool, int&, const int*);

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::set(<array>)"
template <typename T>
void Info2::set(key_t &key, T *values, int count, bool force, int &rc) {
  // Exceptions:  ESMCI:esmf_info_error
  rc = ESMF_FAILURE;
  if (count <= 0) {
    std::string msg = "Count must be >= 1";
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
  }
  if (!force) {
    try {
      bool has_key = handleHasKey(this, key, rc);
    }
    catch (ESMCI::esmf_info_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
  }
  try {
    json::json_pointer jp = this->formatKey(key, rc);
    this->getStorageRefWritable()[jp] = json::array();
    //tdk:OPTIMIZE: can this "at" be removed?
    json::array_t *arr_ptr = this->getStorageRefWritable().at(jp).get_ptr<json::array_t *>();
    if (values) {
      // If values are not null, transfer said values into the JSON array.
      arr_ptr->reserve(count);
      for (auto ii = 0; ii < count; ii++) {
        arr_ptr->push_back(values[ii]);
      }
    } else {
      // If there are no values provided, reserve the space for future
      // setting by index.
      arr_ptr->resize(count);
    }
  }
  ESMF_CATCH_INFO;
  this->dirty = true;
  rc = ESMF_SUCCESS;
};
template void Info2::set<float>(key_t&, float*, int, bool, int&);
template void Info2::set<double>(key_t&, double*, int, bool, int&);
template void Info2::set<int>(key_t&, int*, int, bool, int&);
template void Info2::set<long int>(key_t&, long int*, int, bool, int&);
template void Info2::set<bool>(key_t&, bool*, int, bool, int&);
template void Info2::set<std::vector<std::string>>(key_t&, std::vector<std::string>*, int, bool, int&);

#undef  ESMC_METHOD
#define ESMC_METHOD "Info2::update()"
void Info2::update(const Info2 &info, int &rc) {
  rc = ESMF_FAILURE;
  const json& r_j = info.getStorageRef();
  try {
    this->getStorageRefWritable().update(r_j);
  }
  ESMF_CATCH_JSON;
  this->dirty = true;
  rc = ESMF_SUCCESS;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Print"
//BOPI
// !IROUTINE:  Info::ESMC_Print - Print the {\tt Info} contents
//
// !INTERFACE:
  int Info2::ESMC_Print(
//
// !RETURN VALUE:
//    {\tt ESMF\_SUCCESS} or error code on failure.
//
// !ARGUMENTS:
    bool tofile,            // stream to stdout or file
    const char *filename,   // filename
    bool append) const {    // append or start new
//
// !DESCRIPTION:
//     Print the contents of an {\tt Attribute} object
//
//EOPI
    int strsize=4*ESMF_MAXSTR;
    char msgbuf[strsize];
    std::ofstream fp;
    int rc = ESMF_SUCCESS;

    if (tofile) {
      sprintf(msgbuf, "%s\n", filename);
      // open file for writing and append to previous contents
      if (append)
        fp.open(msgbuf, std::ofstream::out | std::ofstream::app);
        // open file for writing and throw away previous contents
      else
        fp.open(msgbuf, std::ofstream::out | std::ofstream::trunc);
    }

    if (tofile)
      fp << this->dump(2, rc);
    else
      printf("%s", this->dump(2, rc).c_str());

    if (tofile)
      fp.close();
    else
      fflush (stdout);

    return rc;

  }  // end ESMC_Print

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "InfoView::copy(<scalar>)"
template<typename T>
T InfoView::copy(void) {
  return this->get_ivref();
}
template bool InfoView::copy<bool>();
template int InfoView::copy<int>();
template long int InfoView::copy<long int>();
template float InfoView::copy<float>();
template double InfoView::copy<double>();
template std::string InfoView::copy<std::string>();
template json InfoView::copy<json>();

#undef  ESMC_METHOD
#define ESMC_METHOD "InfoView::copy(<list>)"
template<typename T>
void InfoView::copy(T target, const int &count) {
  // Throws: esmf_info_error
  json::array_t *jarr = nullptr;
  this->update_ptr(&jarr);
  if (count==(int)jarr->size()) {
    std::string msg = "JSON array size not equal to count";
    throw(esmf_info_error("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE, msg));
  }
  for (int ii = 0; ii < count; ++ii) {
    target[ii] = jarr->at(ii);
  }
}
template void InfoView::copy<bool*>(bool*, const int &count);
template void InfoView::copy<int*>(int*, const int &count);
template void InfoView::copy<long int*>(long int*, const int &count);
template void InfoView::copy<float*>(float*, const int &count);
template void InfoView::copy<double*>(double*, const int &count);
template void InfoView::copy<std::vector<std::string>>(std::vector<std::string>, const int &count);

#undef  ESMC_METHOD
#define ESMC_METHOD "InfoView::update_ptr()"
void InfoView::update_ptr(bool **ptr) {
  // Test: test_infoview_update_ptr
  if (!this->storage->is_boolean()) {
    std::string msg = "JSON boolean type expected";
    throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg));
  }
  *ptr = &(*this->storage->get_ptr<json::boolean_t*>());
}
void InfoView::update_ptr(long int **ptr) {
  if (!this->storage->is_number_integer()) {
    std::string msg = "JSON integer type expected";
    throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg));
  }
  *ptr = &(*this->storage->get_ptr<json::number_integer_t*>());
}
void InfoView::update_ptr(double **ptr) {
  if (!this->storage->is_number_float()) {
    std::string msg = "JSON float type expected";
    throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg));
  }
  *ptr = &(*this->storage->get_ptr<json::number_float_t*>());
}
void InfoView::update_ptr(std::string **ptr) {
  if (!this->storage->is_string()) {
    std::string msg = "JSON string type expected";
    throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg));
  }
  *ptr = &(*this->storage->get_ptr<json::string_t*>());
}
void InfoView::update_ptr(json::array_t **ptr) {
  if (!this->storage->is_array()) {
    std::string msg = "JSON array type expected";
    throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg));
  }
  *ptr = &(*this->storage->get_ptr<json::array_t*>());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "InfoView::update_storage_ptr()"
void InfoView::update_storage_ptr(const json::json_pointer *key, const int *idx, bool recursive) {
  // Throws: json::out_of_range when key not found
  //         std::out_of_range when index not found
  //         esmf_info_error if "idx" and target is not an array
  json *lj = nullptr;
  if (key) {
    update_json_pointer(*this->storage, &lj, *key, recursive);
  }
  if (idx) {
    if (!lj) {
      lj = this->storage;
    }
    if (!lj->is_array()) {
      throw(esmf_info_error("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
        "Attempted to index into non-array type"));
    }
    lj = &(lj->get_ptr<json::array_t*>()->at(*idx));
  }
  this->storage = &(*lj);
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "esmf_info_error::esmf_info_error()"
esmf_info_error::esmf_info_error (key_t &code_name, int rc, key_t &msg) {
  assert(rc != ESMF_SUCCESS);
  std::string the_msg;
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
json PackageFactory::getOrCreateJSON(key_t& key, int& rc,
                                     key_t& uri) {
  rc = ESMF_FAILURE;
  try {
    json ret = this->cache.at(key);
    rc = ESMF_SUCCESS;
    return ret;
  } catch (json::out_of_range& e) {
    std::string localuri;
    if (uri == "") {
      localuri = this->uris.value(key, uri);
    } else {
      localuri = uri;
    }
    std::ifstream i(localuri, std::ifstream::in);
    if (!i.good()){
      std::string errmsg = "File location is bad for key '" + key + "': " + localuri;
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
#define ESMC_METHOD "broadcastInfo()"
void broadcastInfo(ESMCI::Info2* info, int rootPet, int& rc) {
  // Exceptions:  ESMCI:esmf_info_error

  rc = ESMF_FAILURE;
  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);
  int localPet = vm->getLocalPet();
  std::size_t target_size = 0;  // Size of serialized info storage
  std::string target;  // Serialize storage buffer
  if (localPet == rootPet) {
    // If this is the root, serialize the info storage to std::string
    try {
      target = info->dump(rc);
    }
    catch (ESMCI::esmf_info_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
    target_size = target.size();
  }
  // Broadcast size of the string buffer holding the serialized info.
  // Used for allocating destination string buffers on receiving PETs.
  rc = vm->broadcast(&target_size, sizeof(target_size), rootPet);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  std::string target_received(target_size, '\0');  // Allocate receive buffer
  if (localPet == rootPet) {
    // If this is root, just move the data to the receive buffer with no copy.
    target_received = move(target);
  }
  // Broadcast the string buffer
  rc = vm->broadcast(&target_received[0], target_size, rootPet);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  if (localPet != rootPet) {
    // If not root, then parse the incoming string buffer into attribute storage.
    try {
      info->parse(target_received, rc);
    }
    catch (ESMCI::esmf_info_error &exc_esmf) {
      ESMF_HANDLE_PASSTHRU(exc_esmf);
    }
  }
  return;
}

}  // namespace ESMCI
