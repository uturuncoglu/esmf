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

#undef  ESMC_METHOD
#define ESMC_METHOD "getArrayShape()"
vector<dimsize_t> getArrayShape(const Array& arr, int& rc) {
  // Notes:
  //   * Returns a C-order vector.
  //   * Distributed dimension sizes should always come from DistGrid.
  //tdk:TEST: multiple DEs, multiple PETs
  rc = ESMF_FAILURE;

  DistGrid* distgrid = arr.getDistGrid();
//  int dg_dim_count = distgrid->getDimCount();
  int rank = arr.getRank();

  const int* arr2dg_map = arr.getArrayToDistGridMap(); // F-order & indexing
//  const int* dg2arr_map = arr.getDistGridToArrayMap(); // F-order & indexing

//  const int* undistLBound = arr.getUndistLBound();
  const int* undistUBound = arr.getUndistUBound();
//  const int* totalLBound = arr.getTotalLBound();
//  const int* totalUBound = arr.getTotalUBound();

  int const* maxIndexPDimPTile = distgrid->getMaxIndexPDimPTile();

  vector<dimsize_t> ret(rank, 0);
  for (auto ii=rank-1; ii>=0; ii--) {
    if (arr2dg_map[ii] == 0) {
      ret[ii] = undistUBound[ii];
    } else {
      ret[ii] = maxIndexPDimPTile[arr2dg_map[ii]-1];
    }
  }

  rc = ESMF_SUCCESS;
  return ret;

//  for (auto ii=0; ii<dg_dim_count; ii++) {
//    cout << "(x) totalUBound=" << totalUBound[ii] << "; ii=" << ii << endl;
//  }
//  for (auto ii=0; ii<dg_dim_count; ii++) {
//    cout << "(x) maxIndexPDimPTile=" << maxIndexPDimPTile[ii] << "; ii=" << ii << endl;
//  }
//
//  for (auto ii=0; ii<(rank-dg_dim_count); ii++) {
//    cout << "(x) undistUBound=" << undistUBound[ii] << "; ii=" << ii << endl;
//  }
//  for (auto ii=0; ii<rank; ii++) {
//    cout << "(x) arr2dg_map=" << arr2dg_map[ii] << "; ii=" << ii << endl;
//  }
//  for (auto ii=0; ii<rank; ii++) {
//    cout << "(x) dg2arr_map=" << dg2arr_map[ii] << "; ii=" << ii << endl;
//  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "getESMFTypeKind"
ESMC_TypeKind_Flag getESMFTypeKind(const string& metaType, int& rc) {
  ESMC_TypeKind_Flag ret;
  if (metaType == "double") {
    ret = ESMC_TYPEKIND_R8;
  } else if (metaType == "int") {
    ret = ESMC_TYPEKIND_I4;
  } else {
    string msg = "The type '" + metaType + "' is not supported";
    ESMF_CHECKERR_STD("ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, msg, rc);
  }
  return ret;
}

#undef ESMC_METHOD
#define ESMC_METHOD "handleUnsupported"
void handleUnsupported(const json& j, const vector<string>& tokens, int& rc) {
  for (auto token : tokens) {
    if (j.find(token) != j.end()) {
      string msg = "Parameter not supported through JSON: " + token;
      ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
    } else {
      rc = ESMF_SUCCESS;
    }
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "isIn"
bool isIn(const string& target, const vector<string>& container) {
  auto it = std::find(container.cbegin(), container.cend(), target);
  return !(it == container.cend());
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHO "Metadata(void)"
void Metadata::init(void) {
  int rc = ESMF_FAILURE;
  this->storage = createJSONPackage("ESMF:Metadata:Group", rc);
}

#undef ESMC_METHOD
#define ESMC_METHOD "update(<Array>)"
void Metadata::update(const ESMCI::Array& arr, const vector<string>* dimnames,
  int& rc) {
  //tdk:ORDER
  rc = ESMF_FAILURE;

  string name(arr.getName());
  int rank = arr.getRank();

  json& var_meta = this->getOrCreateVariable(name, rc);
  ESMF_CHECKERR_STD("", rc, "Did not get variable metadata", rc);

  auto arrshp = getArrayShape(arr, rc);
  ESMF_CHECKERR_STD("", rc, "Did not get array shape", rc);

  if (dimnames) {
    json dimsizes = this->getDimensionSizes(rc);
    ESMF_CHECKERR_STD("", rc, "Did not get dimension sizes", rc);

    const vector<string>& r_dimnames = *dimnames;
    for (auto ii=0; ii<rank; ii++) {
      if (dimsizes[r_dimnames[ii]] != arrshp[ii]) {
        auto msg = "Provided dimension names have sizes in current storage that "
                   "conflict with the array size.";
        ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
      }
    }

    var_meta[K_DIMS] = *dimnames;
  } else {
    // Generate new dimensions based on the array shape.
      if (var_meta[K_DIMS].size() != 0) {
        var_meta[K_DIMS] = json::array();
      }
    json& dims = var_meta[K_DIMS];

    string newdimname;
    auto dim_ctr = 0;
    for (dimsize_t ii=0; ii<arrshp.size(); ii++) {
      //tdk:TODO: turn dimension prefix into constant or parameter
      newdimname = "esmf_" + name + "_dim" + to_string(dim_ctr);
      dims.push_back(newdimname);
      dim_ctr++;

      json& dimsmeta = this->storage[K_DIMS];
      dimsmeta[newdimname] = createJSONPackage("ESMF:Metadata:Dimension", rc);
      ESMF_CHECKERR_STD("", rc, "Did not create dimension package", rc);

      json& currdim = dimsmeta[newdimname];
      currdim[K_NAME] = newdimname;
      currdim[K_SIZE] = arrshp[ii];
    }
  }

  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createArray()"
Array* Metadata::createArray(DistGrid& distgrid, const json& jsonParms,
  int& rc) const {
  //tdk:TODO: attributes on array object!
  string variableName;
  try {
    variableName = jsonParms.at("variableName");
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }

  vector<string> unsupported = {"decompflagCount", "computationalEdgeUWidthArg",
    "computationalLWidthArg", "computationalUWidthArg", "totalLWidthArg",
    "totalUWidthArg", "indexflag", "undistLBoundArg", "undistUBoundArg",
    "distLBoundArg", "vm"};
  handleUnsupported(jsonParms, unsupported, rc);

  //---------------------------------------------------------------------------

  try {
    const json& var_meta = this->storage.at(K_VARS).at(variableName);
    vector<string> dim_names = var_meta.at(K_DIMS);
    vector<string> v_distDims = jsonParms.value("distDims", json::array());
    int rank = dim_names.size();
    ESMC_TypeKind_Flag tk = getESMFTypeKind(var_meta[K_DTYPE], rc);
    ESMF_CHECKERR_STD("", rc, "Did not get TypeKind", rc);

    ArraySpec arrayspec;
    arrayspec.set(rank, tk);

    vector<ESMC_I4> v_distgridToArrayMap(v_distDims.size(), -999);
    auto ii = 0;
    for (auto dist_dim_name : v_distDims) {
      auto it = std::find(dim_names.cbegin(), dim_names.cend(), dist_dim_name);
      if (it == dim_names.cend()) {
        auto msg = "Distributed dimension not found: " + dist_dim_name;
        ESMF_CHECKERR_STD("ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, msg, rc);
      } else {
        auto index = std::distance(dim_names.cbegin(), it);
        v_distgridToArrayMap[ii] = index + 1;  // Use Fortran indexing
        ii++;
      }
    }
    InterArray<ESMC_I4> distgridToArrayMap(v_distgridToArrayMap);
//  auto distgridToArrayMap = nullptr;

    auto computationalEdgeLWidthArg = nullptr;
    auto computationalEdgeUWidthArg = nullptr;
    auto computationalLWidthArg = nullptr;
    auto computationalUWidthArg = nullptr;
    auto totalLWidthArg = nullptr;
    auto totalUWidthArg = nullptr;
    ESMC_IndexFlag* indexflag = nullptr;
    auto distLBoundArg = nullptr;

    size_t n_undist = dim_names.size() - v_distDims.size();
    vector<ESMC_I4> v_undistLBoundArg(n_undist, 1);
    InterArray<ESMC_I4> undistLBoundArg(v_undistLBoundArg);
//  auto undistLBoundArg = nullptr;

    vector<ESMC_I4> v_undistUBoundArg(n_undist, -999);
    ii = 0;
    for (auto dim_name : dim_names) {
      if (!isIn(dim_name, v_distDims)) {
        v_undistUBoundArg[ii] = this->storage[K_DIMS][dim_name][K_SIZE];
        ii++;
      }
    }
    InterArray<ESMC_I4> undistUBoundArg(v_undistUBoundArg);
//  auto undistUBoundArg = nullptr;

    VM* vm = nullptr;

    ESMCI::Array* arr = ESMCI::Array::create(
      &arrayspec,
      &distgrid,
      &distgridToArrayMap,
      computationalEdgeLWidthArg,
      computationalEdgeUWidthArg,
      computationalLWidthArg,
      computationalUWidthArg,
      totalLWidthArg,
      totalUWidthArg,
      indexflag,
      distLBoundArg,
      &undistLBoundArg,
      &undistUBoundArg,
      &rc,
      vm);
    ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

    rc = arr->setName(variableName);
    ESMF_CHECKERR_STD("", rc, "Setting array name failed", rc);

    return arr;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createDistGrid()"
DistGrid* Metadata::createDistGrid(const json& jsonParms, int& rc) const {

  vector<string> unsupported = {"regDecomp", "decompflag", "decompflagCount",
    "regDecompFirstExtra", "regDecompLastExtra", "deLabelList", "indexflag",
    "connectionList", "delayout", "vm", "indexTK"};
  handleUnsupported(jsonParms, unsupported, rc);

  vector<string> v_distDims = jsonParms.value("distDims", json::array());

  vector<ESMC_I4> v_minIndex = jsonParms.value("minIndex", json::array());
  vector<ESMC_I4> v_maxIndex = jsonParms.value("maxIndex", json::array());

  size_t v_distDims_size = v_distDims.size();
  if (v_minIndex.size() == 0) {
    v_minIndex.resize(v_distDims_size);
    for (dimsize_t ii=0; ii<v_distDims_size; ii++) {
      v_minIndex[ii] = 1;
    }
  }
  InterArray<ESMC_I4> minIndex(v_minIndex);

  if (v_maxIndex.size() == 0) {
    v_maxIndex.resize(v_distDims_size);
    for (dimsize_t ii=0; ii<v_distDims_size; ii++) {
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

  auto regDecomp = nullptr;
  auto decompflag = nullptr;
  int decompflagCount = 0;
  auto regDecompFirstExtra = nullptr;
  auto regDecompLastExtra = nullptr;
  auto deLabelList = nullptr;
  auto indexflag = nullptr;
  auto connectionList = nullptr;
  auto delayout = nullptr;
  auto vm = nullptr;
  ESMC_TypeKind_Flag indexTK = ESMF_NOKIND; //tdk:?: is this okay?

  DistGrid *ret = ESMCI::DistGrid::create(
    &minIndex,
    &maxIndex,
    regDecomp,
    decompflag,
    decompflagCount,
    regDecompFirstExtra,
    regDecompLastExtra,
    deLabelList,
    indexflag,
    connectionList,
    delayout,
    vm,
    &rc,
    indexTK);
  ESMF_CHECKERR_STD("", rc, "Did not create DistGrid", rc);

  return ret;

}

#undef ESMC_METHOD
#define ESMC_METHOD "getDimensionSizes()"
json Metadata::getDimensionSizes(int& rc) {
  rc = ESMF_FAILURE;
  json ret;

  const json& dims = this->storage[K_DIMS];
  for (json::const_iterator it = dims.cbegin(); it!=dims.cend(); it++) {
    ret[it.key()] = this->storage[K_DIMS][it.key()][K_SIZE];
  }

  rc = ESMF_SUCCESS;
  return ret;
}

#undef ESMC_METHOD
#define ESMC_METHOD "getDimensionSize()"
dimsize_t Metadata::getDimensionSize(const string& name, int& rc) {
  try {
    dimsize_t ret = this->storage.at(K_DIMS).at(name).at(K_SIZE);
    rc = ESMF_SUCCESS;
    return ret;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "getOrCreateVariable()"
json& Metadata::getOrCreateVariable(const string& name, int& rc) {
  rc = ESMF_FAILURE;

  json& vars_meta = this->storage[K_VARS];
  auto it = vars_meta.find(name);
  if (it == vars_meta.end()) {
    vars_meta[name] = createJSONPackage("ESMF:Metadata:Variable", rc);
    ESMF_CHECKERR_STD("", rc, "Did not create variable package", rc);
  }
  json& ret = vars_meta[name];

  rc = ESMF_SUCCESS;
  return ret;
}

#undef ESMC_METHOD
#define ESMC_METHOD "getVariableShape()"
vector<dimsize_t> Metadata::getVariableShape(const string& name, int& rc) {
  //tdk:TEST: dimensionless variable
  rc = ESMF_FAILURE;

  try {
    json::array_t* dims = this->storage.at(K_VARS).at(name).at(K_DIMS).get_ptr<json::array_t*>();
    vector<dimsize_t> ret(dims->size(), 0);
    for (dimsize_t ii=0; ii<ret.size(); ii++) {
      ret[ii] = this->getDimensionSize(dims[0][ii], rc);
      ESMF_CHECKERR_STD("", rc, "Did not get dimension size", rc);
    }
    rc = ESMF_SUCCESS;
    return ret;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "hasVariable()"
bool Metadata::hasVariable(const string& name) {
  json& vars_meta = this->storage[K_VARS];
  return vars_meta.find(name) != vars_meta.end();
}

}  // ESMCI
