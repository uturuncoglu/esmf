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
#define ESMC_METHOD "ESMC_AttributesIsPresent()"
int ESMC_AttributesIsPresent(ESMCI::Attributes* attrs, char* key, int& rc,
        int& isptr) {
  std::string local_key(key);
  int ret = attrs->hasKey(local_key, rc, isptr);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not detect key presence",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsSet()"
void ESMC_AttributesIsSet(ESMCI::Attributes *attrs, char *key, int &isSet, int &rc) {
  std::string local_key(key);
  try {
    isSet = attrs->isSetNull(local_key, rc);
  }
  catch (ESMCI::esmf_attrs_error &exc_esmf) {
    ESMF_CATCH_PASSTHRU(exc_esmf);
  }
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesPrint()"
void ESMC_AttributesPrint(ESMCI::Attributes* attrs, int& indent, int& rc) {
  rc = ESMF_FAILURE;
  std::cout << attrs->dump(indent, rc) << std::endl;
  if (ESMC_LogDefault.MsgFoundError(rc, "Dump failed", ESMC_CONTEXT, &rc))
    throw(rc);
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

}  // extern "C"