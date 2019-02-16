// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
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

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

#undef ESMC_METHOD
#define ESMC_METHOD "alignOffset()"
void alignOffset(int &offset) {
  int nbytes = offset % 8;
  if (nbytes!=0) offset += (8 - nbytes);
}

#undef ESMC_METHOD
#define ESMC_METHOD "handleHasKey"
bool handleHasKey(const Attributes* attrs, key_t& key, int& rc) {
  // Exceptions:  ESMCI::esmf_attrs_error

  bool has_key;
  try {
    has_key = attrs->hasKey(key, rc, true);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
  if (has_key) {
    std::string msg = "Attribute key \'" + key + "\' already in map and force=false.";
    ESMF_CHECKERR_STD("ESMC_RC_CANNOT_SET", ESMC_RC_CANNOT_SET, msg, rc);
  }
  return has_key;
}

#undef ESMC_METHOD
#define ESMC_METHOD "isIn(<string,vector>)"
bool isIn(key_t& target, const std::vector<std::string>& container) {
  auto it = std::find(container.cbegin(), container.cend(), target);
  return !(it == container.cend());
}

#undef ESMC_METHOD
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

#undef ESMC_METHOD
#define ESMC_METHOD "isIn(<string,json>)"
bool isIn(key_t& target, const json& j) {
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
Attributes::Attributes(key_t& input, int& rc) {
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
std::string Attributes::dump(int& rc) const {
  // Exceptions: ESMCI::esmf_attrs_error

  std::string ret;
  try {
    ret = this->dump(0, rc);
  } catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CATCH_PASSTHRU(e);
  }
  return ret;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::dump(int indent, int &rc)"
std::string Attributes::dump(int indent, int& rc) const {
  // Exceptions: ESMCI::esmf_attrs_error

  rc = ESMF_FAILURE;
  std::string ret;
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
void Attributes::erase(key_t& keyParent, key_t& keyChild, int& rc) {
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
json::json_pointer Attributes::formatKey(key_t& key, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error

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
#define ESMC_METHOD "Attributes::get()"
template <typename T>
T Attributes::get(key_t &key, int &rc, T *def, int *index) const {
  // Exceptions:  ESMCI:esmf_attrs_error

  rc = ESMF_FAILURE;
  T ret;
  if (index) {
    try {
      const std::vector<json> *const vj = this->getPointer
        <const std::vector<json>* const, const json::array_t* const>
        (key, rc);
      try {
        ret = vj[0].at(*index);
      }
      catch (std::out_of_range &e) {
        ESMF_CHECKERR_STD("ESMC_RC_ARG_OUTOFRANGE", ESMC_RC_ARG_OUTOFRANGE, e.what(), rc)
      }
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_CATCH_PASSTHRU(exc_esmf);
    }
  } else {
    try {
      json::json_pointer jp = this->formatKey(key, rc);
      if (def) {
        ret = this->storage.value(jp, *def);
      } else {
        try {
          ret = this->storage.at(jp);
        }
        catch (json::out_of_range &e) {
          ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
        }
        catch (json::type_error &e) {
          ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
        }
      }
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_CATCH_PASSTHRU(exc_esmf);
    }
  }
  return ret;
}
template float Attributes::get(key_t&, int&, float*, int*) const;
template double Attributes::get(key_t&, int&, double*, int*) const;
template int Attributes::get(key_t&, int&, int*, int*) const;
template long int Attributes::get(key_t&, int&, long int*, int*) const;
template std::string Attributes::get(key_t&, int&, std::string*, int*) const;

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::getPointer()"
template <typename T, typename JT>
T Attributes::getPointer(key_t& key, int& rc) const {
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
//          const json::number_float_t* const>(key_t&, int&) const;
template const double* const Attributes::getPointer<const double* const,
        const json::number_float_t* const>(key_t&, int&) const;
//  template const int* const Attributes::getPointer<const int* const,
//          const json::number_integer_t* const>(key_t&, int&) const;
template const long int* const Attributes::getPointer<const long int* const,
        const json::number_integer_t* const>(key_t&, int&) const;
template key_t* const Attributes::getPointer<key_t* const,
        const json::string_t* const>(key_t&, int&) const;
template const std::vector<json>* const Attributes::getPointer<const std::vector<json>*
        const, const json::array_t* const>(key_t&, int&) const;

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
bool Attributes::hasKey(key_t& key, int& rc, bool isptr) const {
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
void Attributes::parse(key_t& input, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error

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

//tdk: ORDER
#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::deserialize()"
void Attributes::deserialize(char *buffer, int *offset, int &rc) {
  // Exceptions:  ESMCI:esmf_attrs_error
  rc = ESMF_FAILURE;
  alignOffset(*offset);
  // Act like an integer to get the string length.
  int *ibuffer = reinterpret_cast<int*>(buffer);
  // Get the serialized string length from the buffer start.
  int length = ibuffer[*offset];
  // Move 4 bytes to the start of the string actual.
  (*offset) += 4;
  std::string attrsbuffer(&(buffer[*offset]), length);
  try {
    this->parse(attrsbuffer, rc);
  }
  catch (esmf_attrs_error &e) {
    ESMF_CATCH_PASSTHRU(e);
  }
  (*offset) += length;
  alignOffset(*offset);
  return;
}

#undef ESMC_METHOD
#define ESMC_METHOD "Attributes::get_isoc()"
void Attributes::get_isoc(ESMCI::ESMC_ISOCType ictype, void *ret, char* key,
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
      catch (ESMCI::esmf_attrs_error &exc_esmf) {
        ESMF_CATCH_PASSTHRU(exc_esmf);
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
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::serialize()"
void Attributes::serialize(char *buffer, int *length, int *offset,
  ESMC_InquireFlag inquireflag, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error
  rc = ESMF_FAILURE;
  std::string attrbuffer;
  try {
    attrbuffer = this->dump(rc);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
  // Adjust the buffer length to account for string attribute representation.
  int n = (int)attrbuffer.length();
  *length += n;
  // Need 32 bits (4 bytes) to store the length of the string buffer for a
  // later deserialize.
  *length += 4;
  // If this is not an inquire operation, transfer the string attributes dump
  // into the serialization buffer. Update the offset in the process.
  if (inquireflag != ESMF_INQUIREONLY) {
    alignOffset(*offset);
    int *ibuffer = reinterpret_cast<int*>(buffer);
    ibuffer[*offset] = n;
    (*offset) += 4;
    for (int ii=0; ii<n; ++ii) {
      buffer[*offset] = attrbuffer[ii];
      (*offset)++;
    }
    alignOffset(*offset);
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set()"
template <typename T>
void Attributes::set(key_t& key, T value, bool force, int& rc) {
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
template void Attributes::set<float>(key_t&, float, bool, int&);
template void Attributes::set<double>(key_t&, double, bool, int&);
template void Attributes::set<int>(key_t&, int, bool, int&);
template void Attributes::set<long int>(key_t&, long int, bool, int&);
template void Attributes::set<std::string>(key_t&, std::string, bool, int&);

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::set(<array>)"
template <typename T>
void Attributes::set(key_t& key, T *values, int count, bool force, int& rc,
  int *index) {
  // Exceptions:  ESMCI:esmf_attrs_error

  rc = ESMF_FAILURE;
  if (count <= 0) {
    std::string msg = "Count must be >= 1";
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
  }
  if (index) {
    if (count != 1) {
      std::string msg = "When using an index, the count must be 1";
      ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
    }
    try {
      json::json_pointer jp = this->formatKey(key, rc);
      try {
        json::array_t *arr_ptr = this->storage.at(jp).get_ptr<json::array_t *>();
        arr_ptr[0][*index] = values[0];
      }
      catch (json::out_of_range &e) {
        ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
      }
      catch (json::type_error &e) {
        ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
      }
    }
    catch (esmf_attrs_error &e) {
      ESMF_CATCH_PASSTHRU(e);
    }
  } else {
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
        this->storage[jp] = json::array();
        //tdk:OPTIMIZE: can this "at" be removed?
        json::array_t *arr_ptr = this->storage.at(jp).get_ptr<json::array_t *>();
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
    catch (...) {
      ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
    }
  }
  rc = ESMF_SUCCESS;
  return;
};
template void Attributes::set<float>(key_t&, float*, int, bool, int&, int*);
template void Attributes::set<double>(key_t&, double*, int, bool, int&, int*);
template void Attributes::set<int>(key_t&, int*, int, bool, int&, int*);
template void Attributes::set<long int>(key_t&, long int*, int, bool, int&, int*);
//template void Attributes::set<std::vector<std::string>>(key_t& std::vector<std::string>, int&, bool, int&);

#undef  ESMC_METHOD
#define ESMC_METHOD "Attributes::update()"
void Attributes::update(const Attributes &attrs, int &rc) {
  rc = ESMF_FAILURE;
  const json& r_j = attrs.getStorageRef();
  try {
    this->storage.update(r_j);
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  rc = ESMF_SUCCESS;
  return;
};

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "esmf_attrs_error::esmf_attrs_error()"
esmf_attrs_error::esmf_attrs_error (key_t& code_name, int rc,
                                    key_t& msg) {
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
#define ESMC_METHOD "broadcastAttributes()"
void broadcastAttributes(ESMCI::Attributes* attrs, int rootPet, int& rc) {
  // Exceptions:  ESMCI:esmf_attrs_error

  rc = ESMF_FAILURE;
  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);
  int localPet = vm->getLocalPet();
  std::size_t target_size = 0;  // Size of serialized attributes storage
  std::string target;  // Serialize storage buffer
  if (localPet == rootPet) {
    // If this is the root, serialize the attributes storage to std::string
    try {
      target = attrs->dump(rc);
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_CATCH_PASSTHRU(exc_esmf);
    }
    target_size = target.size();
  }
  // Broadcast size of the string buffer holding the serialized attributes.
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
      attrs->parse(target_received, rc);
    }
    catch (ESMCI::esmf_attrs_error &exc_esmf) {
      ESMF_CATCH_PASSTHRU(exc_esmf);
    }
  }
  return;
}

}  // namespace ESMCI
