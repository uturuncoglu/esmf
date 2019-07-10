// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_Info2CDefGeneric.C"

// Info C-Fortran method implementation (body) file

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
#include "ESMCI_Info2.h"
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
#define ESMC_METHOD "ESMC_Info2GetR4()"
void ESMC_Info2GetR4(ESMCI::Info2 *info, char *key, float &value,
  int &rc, float *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = info->get<float>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetR8()"
void ESMC_Info2GetR8(ESMCI::Info2 *info, char *key, double &value,
  int &rc, double *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = info->get<double>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetI4()"
void ESMC_Info2GetI4(ESMCI::Info2 *info, char *key, int &value,
  int &rc, int *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = info->get<int>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetI8()"
void ESMC_Info2GetI8(ESMCI::Info2 *info, char *key, long int &value,
  int &rc, long int *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = info->get<long int>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetLG()"
void ESMC_Info2GetLG(ESMCI::Info2 *info, char *key, bool &value,
  int &rc, bool *def, int *index, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    value = info->get<bool>(localKey, rc, def, index, recursive);
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetArrayR4()"
void ESMC_Info2GetArrayR4(ESMCI::Info2 *info, char *key,
                               float *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = info->getPointer(localKey, rc, recursive);
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
#define ESMC_METHOD "ESMC_Info2GetArrayR8()"
void ESMC_Info2GetArrayR8(ESMCI::Info2 *info, char *key,
                               double *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = info->getPointer(localKey, rc, recursive);
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
#define ESMC_METHOD "ESMC_Info2GetArrayI4()"
void ESMC_Info2GetArrayI4(ESMCI::Info2 *info, char *key,
                               int *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = info->getPointer(localKey, rc, recursive);
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
#define ESMC_METHOD "ESMC_Info2GetArrayI8()"
void ESMC_Info2GetArrayI8(ESMCI::Info2 *info, char *key,
                               long int *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = info->getPointer(localKey, rc, recursive);
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
#define ESMC_METHOD "ESMC_Info2GetArrayLG()"
void ESMC_Info2GetArrayLG(ESMCI::Info2 *info, char *key,
                               bool *values, int &count, int &rc, bool recursive) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    const json *j = info->getPointer(localKey, rc, recursive);
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
#define ESMC_METHOD "ESMC_Info2SetR4()"
void ESMC_Info2SetR4(ESMCI::Info2 *info, char *key, float &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<float>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetR8()"
void ESMC_Info2SetR8(ESMCI::Info2 *info, char *key, double &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<double>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetI4()"
void ESMC_Info2SetI4(ESMCI::Info2 *info, char *key, int &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<int>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetI8()"
void ESMC_Info2SetI8(ESMCI::Info2 *info, char *key, long int &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<long int>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetLG()"
void ESMC_Info2SetLG(ESMCI::Info2 *info, char *key, bool &value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<bool>(localKey, value, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayR4()"
void ESMC_Info2SetArrayR4(ESMCI::Info2 *info, char *key,
                               float *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<float>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayR8()"
void ESMC_Info2SetArrayR8(ESMCI::Info2 *info, char *key,
                               double *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<double>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayI4()"
void ESMC_Info2SetArrayI4(ESMCI::Info2 *info, char *key,
                               int *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<int>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayI8()"
void ESMC_Info2SetArrayI8(ESMCI::Info2 *info, char *key,
                               long int *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<long int>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayLG()"
void ESMC_Info2SetArrayLG(ESMCI::Info2 *info, char *key,
                               bool *values, int &count, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<bool>(localKey, values, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

}  // extern "C"