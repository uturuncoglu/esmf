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

// Metadata implementation class inheriting from attributes

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include "ESMC.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_LocalArray.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <vector>
#include <iostream>

using namespace ESMCI::MKEY;
using namespace std;
using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

// Local function dependencies ================================================

#undef  ESMC_METHOD
#define ESMC_METHOD "createJSONPackage()"
json createJSONPackage(const string& pkgKey, int& rc) {
  rc = ESMF_FAILURE;

  json j;
  j[K_NAME] = json::value_t::null;  // Will be string
  j[K_UID] = json::value_t::null;  // Will be integer

  if (pkgKey == "ESMF:Metadata:Dimension") {
    // Will be int or potentially null if unlimited
    j[K_SIZE] = json::value_t::null;
    j[K_UNLIM] = json::value_t::null;  // Will be bool
  } else if (pkgKey == "ESMF:Metadata:Group") {
    j[K_VARS] = json::object();
    j[K_DIMS] = json::object();
    j[K_ATTRS] = json::object();
    j[K_GROUPS] = json::object();
    j[K_URI] = json::value_t::null;  // Will be string or rename null
  } else if (pkgKey == "ESMF:Metadata:Variable") {
    j[K_DTYPE] = json::value_t::null;  // Will be string
    j[K_DIMS] = json::array();  // Will append string dimension names
    j[K_ATTRS] = json::object();
  } else {
    string msg = "Package name not found: \'" + pkgKey + "\'";
    ESMF_CHECKERR_STD("ESMC_RC_NOT_FOUND", ESMF_RC_NOT_FOUND, msg, rc);
  }

  rc = ESMF_SUCCESS;
  return j;
}

#undef ESMC_METHOD
#define ESMC_METHOD "handleUnsupported"
void handleUnsupported(const json& j, const string& key, int& rc) {
  if (j.find(key) != j.end()) {
    string msg = "'" + key +"' not supported";
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
  } else {
    rc = ESMF_SUCCESS;
  }
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createArray()"
void Metadata::createArray(const DistGrid& distGrid, const json& jsonParms,
  int& rc) const {

  ESMC_R8 data[100][365];

  ESMC_TypeKind_Flag tk = ESMC_TYPEKIND_R8;
  int rank = 2;
  LocalArrayOrigin oflag = FROM_CPLUSPLUS;
  const int countsData[] = {100, 365};
  const int* counts = &countsData[0];
  void* base_addr = &data[0];
  CopyFlag docopy = DATA_REF;

//  LocalArray* la = ESMCI::LocalArray::create(tk, rank, &rc);
  LocalArray* la = ESMCI::LocalArray::create(tk, rank, counts, base_addr,
          docopy, &rc);
//  la->print();
//  la->validate();

//  LocalArray* larrayList[1];

//    return ret;

  };

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createDistGrid()"
DistGrid* Metadata::createDistGrid(const json& jsonParms, int& rc) const {

  vector<string> v_distDims = jsonParms.value("distDims", json::array());

  vector<ESMC_I4> v_minIndex = jsonParms.value("minIndex", json::array());
  vector<ESMC_I4> v_maxIndex = jsonParms.value("maxIndex", json::array());

  size_t v_distDims_size = v_distDims.size();
  if (v_minIndex.size() == 0) {
    v_minIndex.resize(v_distDims_size);
    for (auto ii=0; ii<v_distDims_size; ii++) {
      v_minIndex[ii] = 1;
    }
  }
  InterArray<ESMC_I4> minIndex(v_minIndex);

  if (v_maxIndex.size() == 0) {
    v_maxIndex.resize(v_distDims_size);
    for (auto ii=0; ii<v_distDims_size; ii++) {
      try {
        v_maxIndex[ii] = this->storage.at(K_DIMS).at(v_distDims[ii]).at(K_SIZE);
      }
      catch (json::out_of_range& e) {
        ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
      }
      catch (...) {
        ESMF_CHECKERR_STD("ESMF_FAILURE", ESMF_FAILURE,
          "Did not get max index", rc);
      }
    }
  }
  InterArray<ESMC_I4> maxIndex(v_maxIndex);

  handleUnsupported(jsonParms, "regDecomp", rc);
  auto regDecomp = nullptr;

  handleUnsupported(jsonParms, "decompflag", rc);
  auto decompflag = nullptr;

  handleUnsupported(jsonParms, "decompflagCount", rc);
  int decompflagCount = 0;

  handleUnsupported(jsonParms, "regDecompFirstExtra", rc);
  auto regDecompFirstExtra = nullptr;

  handleUnsupported(jsonParms, "regDecompLastExtra", rc);
  auto regDecompLastExtra = nullptr;

  handleUnsupported(jsonParms, "deLabelList", rc);
  auto deLabelList = nullptr;

  handleUnsupported(jsonParms, "indexflag", rc);
  auto indexflag = nullptr;

  handleUnsupported(jsonParms, "connectionList", rc);
  auto connectionList = nullptr;

  handleUnsupported(jsonParms, "delayout", rc);
  auto delayout = nullptr;

  handleUnsupported(jsonParms, "vm", rc);
  auto vm = nullptr;

  handleUnsupported(jsonParms, "indexTK", rc);
  ESMC_TypeKind_Flag indexTK = ESMF_NOKIND; //tdk:?: is this okay?

  DistGrid *ret = ESMCI::DistGrid::create(&minIndex, &maxIndex, regDecomp,
                                          decompflag, decompflagCount,
                                          regDecompFirstExtra,
                                          regDecompLastExtra, deLabelList,
                                          indexflag, connectionList, delayout,
                                          vm, &rc, indexTK);
  ESMF_CHECKERR_STD("", rc, "Did not create DistGrid", rc);

  return ret;

};

}  // ESMCI
