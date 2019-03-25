// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_AttributesCDef.C"

// Attributes C-Fortran method implementation (body) file

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include "ESMC.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <iostream>
#include <vector>

using json = nlohmann::json;

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

extern "C" {

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCopy()"
ESMCI::Attributes* ESMC_AttributesCopy(ESMCI::Attributes *attrs, int &rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes(attrs->getStorageRef());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreate()"
ESMCI::Attributes* ESMC_AttributesCreate(int& rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreateByKey()"
ESMCI::Attributes* ESMC_AttributesCreateByKey(ESMCI::Attributes *srcAttrs,
  char* key, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Attributes *attrs;
  try {
    std::string local_key(key);
    json new_storage = srcAttrs->get<json>(local_key, rc);
    attrs = new ESMCI::Attributes(std::move(new_storage));
  }
  ESMF_CATCH_ISOC;
  return attrs;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreateByParse()"
ESMCI::Attributes* ESMC_AttributesCreateByParse(char *payload, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Attributes *attrs;
  try {
    std::string local_payload(payload);
    attrs = new ESMCI::Attributes(payload, rc);
  }
  ESMF_CATCH_ISOC;
  return attrs;
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
  try {
    if (localkeyChild == "") {
      attrs->erase(localkeyChild, localkeyParent, rc);
    } else {
      attrs->erase(localkeyParent, localkeyChild, rc);
    }
  }
  ESMF_CATCH_ISOC;
}

#undef ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesInquire()"
void ESMC_AttributesInquire(ESMCI::Attributes *attrs, char *key, int &rc, int *count) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    if (count) {
      const std::vector <json> *value = attrs->getPointer<const std::vector<json>* const,
        const json::array_t* const>(localKey, rc);
      *count = value->size();
    }
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsPresent()"
void ESMC_AttributesIsPresent(ESMCI::Attributes *attrs, char *key, int &res, int &rc,
        int &isptr) {
  std::string local_key(key);
  try {
    res = attrs->hasKey(local_key, rc, isptr);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsSet()"
void ESMC_AttributesIsSet(ESMCI::Attributes *attrs, char *key, int &isSet, int &rc) {
  std::string local_key(key);
  try {
    isSet = attrs->isSetNull(local_key, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesPrint()"
void ESMC_AttributesPrint(ESMCI::Attributes *attrs, int &indent, int &rc) {
  rc = ESMF_FAILURE;
  try {
    std::cout << attrs->dump(indent, rc) << std::endl;
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesReadJSON()"
void ESMC_AttributesReadJSON(ESMCI::Attributes *attrs, char *filename,
                             int &rc) {
  rc = ESMF_FAILURE;
  std::string filename2(filename);
  try {
    std::ifstream i(filename2, std::ifstream::in);
    if (!i.good()){
      std::string errmsg = "File location not working: " + filename2;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, errmsg, rc);
    }
    json j;
    try {
      i >> j;
    } catch (json::parse_error& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, rc);
    }
    i.close();
    json &out = attrs->getStorageRefWritable();
    out = move(j);
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesWriteJSON()"
void ESMC_AttributesWriteJSON(ESMCI::Attributes *attrs, char *filename,
  int &rc) {
  rc = ESMF_FAILURE;
  std::string filename2(filename);
  try {
    std::ofstream file;
    file.open(filename2);
    if (!file.is_open()) {
      std::string errmsg = "Error opening output file: " + filename2;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_OPEN", ESMC_RC_FILE_OPEN, errmsg, rc);
    }
    file << attrs->getStorageRef();
    file.close();
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetR4()"
void ESMC_AttributesGetR4(ESMCI::Attributes *attrs, char *key, float &value,
  int &rc, float *def, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<float>(localKey, rc, def, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetR8()"
void ESMC_AttributesGetR8(ESMCI::Attributes *attrs, char *key, double &value,
  int &rc, double *def, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<double>(localKey, rc, def, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetI4()"
void ESMC_AttributesGetI4(ESMCI::Attributes *attrs, char *key, int &value,
  int &rc, int *def, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<int>(localKey, rc, def, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetI8()"
void ESMC_AttributesGetI8(ESMCI::Attributes *attrs, char *key, long int &value,
  int &rc, long int *def, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<long int>(localKey, rc, def, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetLG()"
void ESMC_AttributesGetLG(ESMCI::Attributes *attrs, char *key, bool &value,
  int &rc, bool *def, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<bool>(localKey, rc, def, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetCH()"
void ESMC_AttributesGetCH(ESMCI::Attributes* attrs, char *key, char *value,
  int &vlen, int &rc, char *def, int *index) {
  // String pointer used to define the default value if present
  std::string *def_str_ptr;
  // String object that holds the default if present
  std::string def_str;
  if (def) {
    // Set the default pointer to the string object created from the char
    // array from Fortran
    def_str = std::string(def);
    def_str_ptr = &def_str;
  } else {
    def_str_ptr = nullptr;
  }
  std::string as_str;
  std::string local_key(key);
  try {
    as_str = attrs->get<std::string>(local_key, rc, def_str_ptr, index);
  }
  ESMF_CATCH_ISOC;
  // Transfer the string characters into the Fortran character array using
  // spaces to fill the Fortran array if we are past the max string length.
  for (int ii = 0; ii < vlen; ++ii) {
    if (ii < (int)as_str.size()) {
      value[ii] = as_str[ii];
    } else {
      value[ii] = ' ';
    }
  }
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayR4()"
void ESMC_AttributesGetArrayR4(ESMCI::Attributes *attrs, char *key,
                               float *values, int &count, int &count_only,
                               int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    ESMCI::vecjson_t ap = attrs->getPointer<ESMCI::vecjson_t,
                                            ESMCI::arrjson_t>(localKey, rc);
    count = (int)ap->size();
    // Only fill the outgoing array if we are not getting the count only
    if (count_only == 0) {
      for (int ii=0; ii<count; ii++) {
        values[ii] = ap[0][ii];
      }
    }
  }
  ESMF_CATCH_ISOC;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayR8()"
void ESMC_AttributesGetArrayR8(ESMCI::Attributes *attrs, char *key,
                               double *values, int &count, int &count_only,
                               int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    ESMCI::vecjson_t ap = attrs->getPointer<ESMCI::vecjson_t,
                                            ESMCI::arrjson_t>(localKey, rc);
    count = (int)ap->size();
    // Only fill the outgoing array if we are not getting the count only
    if (count_only == 0) {
      for (int ii=0; ii<count; ii++) {
        values[ii] = ap[0][ii];
      }
    }
  }
  ESMF_CATCH_ISOC;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayI4()"
void ESMC_AttributesGetArrayI4(ESMCI::Attributes *attrs, char *key,
                               int *values, int &count, int &count_only,
                               int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    ESMCI::vecjson_t ap = attrs->getPointer<ESMCI::vecjson_t,
                                            ESMCI::arrjson_t>(localKey, rc);
    count = (int)ap->size();
    // Only fill the outgoing array if we are not getting the count only
    if (count_only == 0) {
      for (int ii=0; ii<count; ii++) {
        values[ii] = ap[0][ii];
      }
    }
  }
  ESMF_CATCH_ISOC;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayI8()"
void ESMC_AttributesGetArrayI8(ESMCI::Attributes *attrs, char *key,
                               long int *values, int &count, int &count_only,
                               int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    ESMCI::vecjson_t ap = attrs->getPointer<ESMCI::vecjson_t,
                                            ESMCI::arrjson_t>(localKey, rc);
    count = (int)ap->size();
    // Only fill the outgoing array if we are not getting the count only
    if (count_only == 0) {
      for (int ii=0; ii<count; ii++) {
        values[ii] = ap[0][ii];
      }
    }
  }
  ESMF_CATCH_ISOC;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayLG()"
void ESMC_AttributesGetArrayLG(ESMCI::Attributes *attrs, char *key,
                               bool *values, int &count, int &count_only,
                               int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    ESMCI::vecjson_t ap = attrs->getPointer<ESMCI::vecjson_t,
                                            ESMCI::arrjson_t>(localKey, rc);
    count = (int)ap->size();
    // Only fill the outgoing array if we are not getting the count only
    if (count_only == 0) {
      for (int ii=0; ii<count; ii++) {
        values[ii] = ap[0][ii];
      }
    }
  }
  ESMF_CATCH_ISOC;
  return;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetR4()"
void ESMC_AttributesSetR4(ESMCI::Attributes *attrs, char *key, float &value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<float>(localKey, value, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetR8()"
void ESMC_AttributesSetR8(ESMCI::Attributes *attrs, char *key, double &value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<double>(localKey, value, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetI4()"
void ESMC_AttributesSetI4(ESMCI::Attributes *attrs, char *key, int &value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<int>(localKey, value, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetI8()"
void ESMC_AttributesSetI8(ESMCI::Attributes *attrs, char *key, long int &value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<long int>(localKey, value, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetLG()"
void ESMC_AttributesSetLG(ESMCI::Attributes *attrs, char *key, bool &value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<bool>(localKey, value, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetCH()"
void ESMC_AttributesSetCH(ESMCI::Attributes *attrs, char *key, char *value,
                              int &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  std::string localValue(value);
  try {
    attrs->set<std::string>(localKey, localValue, localforce, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetATTRS()"
void ESMC_AttributesSetATTRS(ESMCI::Attributes *attrs, char *key,
  ESMCI::Attributes *value, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;  //tdk:todo: use C_BOOL type to avoid this logic
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set(localKey, *value, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetNULL()"
void ESMC_AttributesSetNULL(ESMCI::Attributes *attrs, char *key, int &force,
  int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set(localKey, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayR4()"
void ESMC_AttributesSetArrayR4(ESMCI::Attributes *attrs, char *key,
                               float *values, int &count, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<float>(localKey, values, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayR8()"
void ESMC_AttributesSetArrayR8(ESMCI::Attributes *attrs, char *key,
                               double *values, int &count, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<double>(localKey, values, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayI4()"
void ESMC_AttributesSetArrayI4(ESMCI::Attributes *attrs, char *key,
                               int *values, int &count, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<int>(localKey, values, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayI8()"
void ESMC_AttributesSetArrayI8(ESMCI::Attributes *attrs, char *key,
                               long int *values, int &count, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<long int>(localKey, values, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayLG()"
void ESMC_AttributesSetArrayLG(ESMCI::Attributes *attrs, char *key,
                               bool *values, int &count, int &force, int &rc) {
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<bool>(localKey, values, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayCH()"
void ESMC_AttributesSetArrayCH(ESMCI::Attributes *attrs, char *key, int &count,
  int &force, int &rc) {
  // Notes:
  //  * Only allocates storage. Does not actually insert anything!
  rc = ESMF_FAILURE;
  bool localforce;
  if (force == 1) {
    localforce = true;
  } else {
    localforce = false;
  }
  std::string localKey(key);
  try {
    attrs->set<std::vector<std::string>>(localKey, nullptr, count, localforce, rc);
  }
  ESMF_CATCH_ISOC;
}

}  // extern "C"