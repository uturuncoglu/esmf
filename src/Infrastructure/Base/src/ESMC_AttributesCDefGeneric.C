// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_AttributesCDefGeneric.C"

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
#include "ESMCI_Base.h"
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
#define ESMC_METHOD "ESMC_AttributesGetR4()"
void ESMC_AttributesGetR4(ESMCI::Attributes *attrs, char *key, float &value,
  int &rc, float *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<float>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetR8()"
void ESMC_AttributesGetR8(ESMCI::Attributes *attrs, char *key, double &value,
  int &rc, double *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<double>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetI4()"
void ESMC_AttributesGetI4(ESMCI::Attributes *attrs, char *key, int &value,
  int &rc, int *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<int>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetI8()"
void ESMC_AttributesGetI8(ESMCI::Attributes *attrs, char *key, long int &value,
  int &rc, long int *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<long int>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetLG()"
void ESMC_AttributesGetLG(ESMCI::Attributes *attrs, char *key, bool &value,
  int &rc, bool *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = attrs->get<bool>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayR4()"
void ESMC_AttributesGetArrayR4(ESMCI::Attributes *attrs, char *key,
                               float *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = attrs->getPointer(localKey, rc, recursive);
    const json::array_t *ap = j->get_ptr<const json::array_t *>();

    count = (int)ap->size();
    for (int ii=0; ii<count; ii++) {
      try {
        values[ii] = ap->at(ii);
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE, std::string(exc.what()), rc);
      }
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayR8()"
void ESMC_AttributesGetArrayR8(ESMCI::Attributes *attrs, char *key,
                               double *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = attrs->getPointer(localKey, rc, recursive);
    const json::array_t *ap = j->get_ptr<const json::array_t *>();

    count = (int)ap->size();
    for (int ii=0; ii<count; ii++) {
      try {
        values[ii] = ap->at(ii);
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE, std::string(exc.what()), rc);
      }
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayI4()"
void ESMC_AttributesGetArrayI4(ESMCI::Attributes *attrs, char *key,
                               int *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = attrs->getPointer(localKey, rc, recursive);
    const json::array_t *ap = j->get_ptr<const json::array_t *>();

    count = (int)ap->size();
    for (int ii=0; ii<count; ii++) {
      try {
        values[ii] = ap->at(ii);
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE, std::string(exc.what()), rc);
      }
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayI8()"
void ESMC_AttributesGetArrayI8(ESMCI::Attributes *attrs, char *key,
                               long int *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = attrs->getPointer(localKey, rc, recursive);
    const json::array_t *ap = j->get_ptr<const json::array_t *>();

    count = (int)ap->size();
    for (int ii=0; ii<count; ii++) {
      try {
        values[ii] = ap->at(ii);
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE, std::string(exc.what()), rc);
      }
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesGetArrayLG()"
void ESMC_AttributesGetArrayLG(ESMCI::Attributes *attrs, char *key,
                               bool *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = attrs->getPointer(localKey, rc, recursive);
    const json::array_t *ap = j->get_ptr<const json::array_t *>();

    count = (int)ap->size();
    for (int ii=0; ii<count; ii++) {
      try {
        values[ii] = ap->at(ii);
      }
      catch (std::out_of_range &exc) {
        ESMF_CHECKERR_STD("ESMF_RC_ARG_OUTOFRANGE", ESMF_RC_ARG_OUTOFRANGE, std::string(exc.what()), rc);
      }
    }
  }
  ESMF_CATCH_ISOC
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetR4()"
void ESMC_AttributesSetR4(ESMCI::Attributes *attrs, char *key, float &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<float>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetR8()"
void ESMC_AttributesSetR8(ESMCI::Attributes *attrs, char *key, double &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<double>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetI4()"
void ESMC_AttributesSetI4(ESMCI::Attributes *attrs, char *key, int &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<int>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetI8()"
void ESMC_AttributesSetI8(ESMCI::Attributes *attrs, char *key, long int &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<long int>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetLG()"
void ESMC_AttributesSetLG(ESMCI::Attributes *attrs, char *key, bool &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<bool>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayR4()"
void ESMC_AttributesSetArrayR4(ESMCI::Attributes *attrs, char *key,
                               float *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<float>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayR8()"
void ESMC_AttributesSetArrayR8(ESMCI::Attributes *attrs, char *key,
                               double *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<double>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayI4()"
void ESMC_AttributesSetArrayI4(ESMCI::Attributes *attrs, char *key,
                               int *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<int>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayI8()"
void ESMC_AttributesSetArrayI8(ESMCI::Attributes *attrs, char *key,
                               long int *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<long int>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayLG()"
void ESMC_AttributesSetArrayLG(ESMCI::Attributes *attrs, char *key,
                               bool *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<bool>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

}  // extern "C"