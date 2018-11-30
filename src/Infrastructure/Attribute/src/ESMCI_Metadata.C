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
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

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

#undef ESMC_METHOD
#define ESMC_METHOD "<DistGrid*> Metadata::createESMF(<JSON Parms>)"
DistGrid* Metadata::createESMF(const json& jsonParms, int& rc) const {

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
      string key = "/dims/" + v_distDims[ii] + "/size";
      v_maxIndex[ii] = this->get<ESMC_I4>(key, rc);
      ESMF_CHECKERR_STD("", rc, "Did not get distributed dimension", rc);
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
