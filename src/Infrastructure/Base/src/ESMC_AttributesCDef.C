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

#include <iostream>
#include <vector>

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

//-----------------------------------------------------------------------------

#undef ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_VOID()"
void ESMC_AttributesGet_VOID(ESMCI::ESMC_ISOCType ictype, void *ret,
                             ESMCI::Attributes* attrs, char* key, int& rc,
                             void* def) {
  attrs->get_isoc(ictype, ret, key, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_C_FLOAT()"
float ESMC_AttributesGet_C_FLOAT(ESMCI::Attributes* attrs, char* key, int& rc, float* def) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  float ret = attrs->get<float>(localKey, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_C_DOUBLE()"
double ESMC_AttributesGet_C_DOUBLE(ESMCI::Attributes* attrs, char* key, int& rc, double* def) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  double ret = attrs->get<double>(localKey, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_C_INT()"
void ESMC_AttributesGet_C_INT(ESMCI::Attributes* attrs, char* key, int &value, int& rc,
  int* def) {
  ESMC_AttributesGet_VOID(ESMCI::ESMC_ISOCType::C_INT, &value, attrs, key, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_C_LONG()"
long int ESMC_AttributesGet_C_LONG(ESMCI::Attributes* attrs, char* key, int& rc, long int* def) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  long int ret = attrs->get<long int>(localKey, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc))
    throw(rc);
  return ret;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGet_C_CHAR()"
void ESMC_AttributesGet_C_CHAR(ESMCI::Attributes* attrs, char* key, char* value,
  int& rc, char* def) {
  ESMC_AttributesGet_VOID(ESMCI::ESMC_ISOCType::C_CHAR, value, attrs, key, rc, def);
  if (ESMC_LogDefault.MsgFoundError(rc, "Get failed", ESMC_CONTEXT, &rc)) throw(rc);
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArray_C_FLOAT()"
void ESMC_AttributesGetArray_C_FLOAT(ESMCI::Attributes* attrs, char* key,
                                     float* values, int& count, int& count_only, int& rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  const std::vector<json>* const ap = attrs->getPointer<const std::vector<json>* const,
    const json::array_t* const>(localKey, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not get array pointer",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  count = (int)ap->size();
  if (count_only == 0) {
    for (int ii=0; ii<count; ii++) {
      values[ii] = ap[0][ii];
    }
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArray_C_DOUBLE()"
void ESMC_AttributesGetArray_C_DOUBLE(ESMCI::Attributes* attrs, char* key,
                                      double* values, int& count, int& count_only, int& rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  const std::vector<json>* const ap = attrs->getPointer<const std::vector<json>* const,
    const json::array_t* const>(localKey, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not get array pointer",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  count = (int)ap->size();
  if (count_only == 0) {
    for (int ii=0; ii<count; ii++) {
      values[ii] = ap[0][ii];
    }
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArray_C_INT()"
void ESMC_AttributesGetArray_C_INT(ESMCI::Attributes* attrs, char* key,
                                   int* values, int& count, int& count_only, int& rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  const std::vector<json>* const ap = attrs->getPointer<const std::vector<json>* const,
    const json::array_t* const>(localKey, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not get array pointer",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  count = (int)ap->size();
  if (count_only == 0) {
    for (int ii=0; ii<count; ii++) {
      values[ii] = ap[0][ii];
    }
  }
  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArray_C_LONG()"
void ESMC_AttributesGetArray_C_LONG(ESMCI::Attributes* attrs, char* key,
                                    long int* values, int& count, int& count_only, int& rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  const std::vector<json>* const ap = attrs->getPointer<const std::vector<json>* const,
    const json::array_t* const>(localKey, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Did not get array pointer",
                                    ESMC_CONTEXT, &rc)) throw(rc);
  count = (int)ap->size();
  if (count_only == 0) {
    for (int ii=0; ii<count; ii++) {
      values[ii] = ap[0][ii];
    }
  }
  rc = ESMF_SUCCESS;
  return;
}

//-----------------------------------------------------------------------------

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
#define ESMC_METHOD "ESMC_AttributesPrint()"
void ESMC_AttributesPrint(ESMCI::Attributes* attrs, int& indent, int& rc) {
  rc = ESMF_FAILURE;
  std::cout << attrs->dump(indent, rc) << std::endl;
  if (ESMC_LogDefault.MsgFoundError(rc, "Dump failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet_C_FLOAT()"
void ESMC_AttributesSet_C_FLOAT(ESMCI::Attributes* attrs, char* key, float &value,
                                int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set<float>(localKey, value, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed",  ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet_C_DOUBLE()"
void ESMC_AttributesSet_C_DOUBLE(ESMCI::Attributes* attrs, char* key, double &value,
                                 int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set<double>(localKey, value, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed",  ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet_C_INT()"
void ESMC_AttributesSet_C_INT(ESMCI::Attributes* attrs, char* key, int &value,
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
#define ESMC_METHOD "ESMC_AttributesSet_C_LONG()"
void ESMC_AttributesSet_C_LONG(ESMCI::Attributes* attrs, char* key, long int &value,
                               int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  attrs->set<long int>(localKey, value, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed",  ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSet_C_CHAR()"
void ESMC_AttributesSet_C_CHAR(ESMCI::Attributes* attrs, char* key, const char* value,
                               int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;

//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }

  std::string localKey(key);
  std::string localValue(value);
  attrs->set<std::string>(localKey, localValue, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed",  ESMC_CONTEXT, &rc))
    throw(rc);
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArray_C_FLOAT()"
void ESMC_AttributesSetArray_C_FLOAT(ESMCI::Attributes* attrs, char* key,
                                     float* values, int& count, int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;
//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }
  std::string localKey(key);
  attrs->set<float>(localKey, values, count, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArray_C_DOUBLE()"
void ESMC_AttributesSetArray_C_DOUBLE(ESMCI::Attributes* attrs, char* key,
                                      double* values, int& count, int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;
//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }
  std::string localKey(key);
  attrs->set<double>(localKey, values, count, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArray_C_INT()"
void ESMC_AttributesSetArray_C_INT(ESMCI::Attributes* attrs, char* key,
                                   int* values, int& count, int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;
//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }
  std::string localKey(key);
  attrs->set<int>(localKey, values, count, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArray_C_LONG()"
void ESMC_AttributesSetArray_C_LONG(ESMCI::Attributes* attrs, char* key,
                                    long int* values, int& count, int& force, int& rc) {
  rc = ESMF_FAILURE;
  bool localforce = force;
//  if (force == 1) {
//    localforce = true;
//  } else {
//    localforce = false;
//  }
  std::string localKey(key);
  attrs->set<long int>(localKey, values, count, localforce, rc);
  if (ESMC_LogDefault.MsgFoundError(rc, "Set failed", ESMC_CONTEXT, &rc))
    throw(rc);
}

}  // extern "C"
