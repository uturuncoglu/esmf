// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMCI_Metadata.C"

// Metadata implementation class inheriting from attributes

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include <netcdf.h>

#include "ESMC.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_LocalArray.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

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
    j[K_UNLIM] = false;
  } else if (pkgKey == "ESMF:Metadata:Group") {
    j[K_VARS] = json::object();
    j[K_DIMS] = json::object();
    j[K_ATTRS] = json::object();
    j[K_GROUPS] = json::object();
    j[K_URI] = json::value_t::null;  // Will be string or rename null
  } else if (pkgKey == "ESMF:Metadata:Variable") {
    j[K_DTYPE] = json::value_t::null;  // Will be nc_type
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
#define ESMC_METHOD "getArrayBounds()"
vector<vector<dimsize_t>> getArrayBounds(const Array& arr,
  const ESMC_IndexFlag& idxFlag, int& rc) {
  // Notes:
  //  * Zero-basis, [bounds), >= and <
  //  * ret[ii][0] = Lower Bound
  //  * ret[ii][1] = Upper Bound

  rc = ESMF_FAILURE;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  //---------------------------------------------------------------------------

  DistGrid* distgrid = arr.getDistGrid();
  int rank = arr.getRank();

  const int* arr2dg_map = arr.getArrayToDistGridMap(); // F-order & indexing
//  tdklog("getArrayBounds arr2dg_map", arr2dg_map, rank);

  const int* undistLBound = arr.getUndistLBound(); // F-order & indexing
  const int* undistUBound = arr.getUndistUBound(); // F-order & indexing
//  tdklog("getArrayBounds undistLBound", undistLBound, rank);
//  tdklog("getArrayBounds undistUBound", undistUBound, rank);

  int const* minIndex;
  int const* maxIndex;
  if (idxFlag == ESMC_INDEX_GLOBAL) {
//    tdklog("getArrayBounds using ESMC_INDEX_GLOBAL");
    minIndex = distgrid->getMinIndexPDimPTile();
    maxIndex = distgrid->getMaxIndexPDimPTile();
  } else if (idxFlag == ESMC_INDEX_DELOCAL) {
//    tdklog("getArrayBounds using ESMC_INDEX_DELOCAL");
    minIndex = distgrid->getMinIndexPDimPDe(localPet, nullptr);
    maxIndex = distgrid->getMaxIndexPDimPDe(localPet, nullptr);
//    tdklog("ArrayCreate:minIndex", minIndex, 1);
//    tdklog("ArrayCreate:maxIndex", maxIndex, 1);
  } else {
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
                      "Index flag not supported: " + to_string(idxFlag), rc);
  }
//  tdklog("getArrayBounds minIndex", minIndex, rank);
//  tdklog("getArrayBounds maxIndex", maxIndex, rank);

//  vector<dimsize_t> ret(rank, 0);
  vector<vector<dimsize_t>> ret;
  ret.resize(rank);
//  size_t dgct.r = 0;
  size_t udctr = 0;
  for (auto ii=0; ii<rank; ++ii) {
    vector<dimsize_t> curr(2, std::numeric_limits<dimsize_t>::min());
    if (arr2dg_map[ii] == 0) {
      // Dimension is undistributed
//      ret[ii] = undistUBound[ii] - undistLBound[ii] + 1;
      curr[0] = undistLBound[udctr] - 1;
      curr[1] = undistUBound[udctr];
      ++udctr;
    } else {
      // Dimension is distributed
//      ret[ii] = maxIndex[arr2dg_map[ii]-1] - minIndex[arr2dg_map[ii]-1] + 1;
      curr[0] = minIndex[arr2dg_map[ii]-1] - 1;
      curr[1] = maxIndex[arr2dg_map[ii]-1];
//      ++dgctr;
    }
//    tdklog("getArrayBounds curr", curr);
    assert(curr[0] >= 0);
    assert(curr[1] >= curr[0]);
    ret[ii] = curr;
  }

  rc = ESMF_SUCCESS;
  return ret;

  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "getArrayShape()"
vector<dimsize_t> getArrayShape(const Array& arr,
  const ESMC_IndexFlag& idxFlag, int& rc) {
  // Notes:
  //   * Returns a C-order vector.
  //   * Distributed dimension sizes should always come from DistGrid.
  //tdk:TEST: multiple DEs, multiple PETs
  rc = ESMF_FAILURE;

  vector<vector<dimsize_t>> bnds = getArrayBounds(arr, idxFlag, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  vector<dimsize_t> ret(bnds.size(), 0);
  for (dimsize_t ii = 0; ii < ret.size(); ++ii) {
    ret[ii] = bnds[ii][1] - bnds[ii][0];
  }

//  tdklog("getArrayShape ret", ret);

//  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
//  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);
//
//  int localPet = vm->getLocalPet();
//  int petCount = vm->getPetCount();
//
//  //---------------------------------------------------------------------------
//
//  DistGrid* distgrid = arr.getDistGrid();
//  int rank = arr.getRank();
//
//  const int* arr2dg_map = arr.getArrayToDistGridMap(); // F-order & indexing
////  const int* dg2arr_map = arr.getDistGridToArrayMap(); // F-order & indexing
//
//  const int* undistLBound = arr.getUndistLBound(); // F-order & indexing
//  const int* undistUBound = arr.getUndistUBound(); // F-order & indexing
//  int dg_dim_count = distgrid->getDimCount(); tdklog("getArrayShape undistUBound=", undistUBound, rank-dg_dim_count);
////  const int* totalLBound = arr.getTotalLBound();
////  const int* totalUBound = arr.getTotalUBound();
//
//  int const* minIndex;
//  int const* maxIndex;
//  if (idxFlag == ESMC_INDEX_GLOBAL) {
//    minIndex = distgrid->getMinIndexPDimPTile();
//    maxIndex = distgrid->getMaxIndexPDimPTile();
//  } else if (idxFlag == ESMC_INDEX_DELOCAL) {
//    minIndex = distgrid->getMinIndexPDimPDe(localPet, nullptr);
//    maxIndex = distgrid->getMaxIndexPDimPDe(localPet, nullptr);
//    tdklog("ArrayCreate:minIndex", minIndex, 1);
//    tdklog("ArrayCreate:maxIndex", maxIndex, 1);
//  } else {
//    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
//      "Index flag not supported: " + to_string(idxFlag), rc);
//  }
//
//  vector<dimsize_t> ret(rank, 0);
//  for (auto ii=0; ii<rank; ii++) {
//    if (arr2dg_map[ii] == 0) {
//      // Dimension is undistributed
//      ret[ii] = undistUBound[ii] - undistLBound[ii] + 1;
//    } else {
//      // Dimension is distributed
//      ret[ii] = maxIndex[arr2dg_map[ii]-1] - minIndex[arr2dg_map[ii]-1] + 1;
//    }
//  }

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
ESMC_TypeKind_Flag getESMFTypeKind(const nc_type xtype, int& rc) {
  ESMC_TypeKind_Flag ret;
  if (xtype == NC_DOUBLE) {
    ret = ESMC_TYPEKIND_R8;
  } else if (xtype == NC_INT) {
    ret = ESMC_TYPEKIND_I4;
  } else {
    string msg = "The type '" + to_string(xtype) + "' is not supported";
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
#define ESMC_METHOD "sizeFromShape()"
dimsize_t sizeFromShape(const vector<dimsize_t>& shp) {
  dimsize_t ret;
  if (shp.size() == 0) {
    ret = 0;
  } else {
    ret = 1;
    for (const auto& s: shp) {
      ret *= s;
    }
  }
  return ret;
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata(void)"
void Metadata::init(void) {
  int rc = ESMF_FAILURE;
  this->storage = createJSONPackage("ESMF:Metadata:Group", rc);
}

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::update(<Array>)"
void Metadata::update(const ESMCI::Array& arr, const vector<string>* dimnames,
  int& rc) {
  //tdk:ORDER
  //tdk:TODO: standard try/catch

  // Notes:
  //  * dimnames needs to be F-order

  // Notes:
  //  * "dimnames" comes in as F-Order but is stored as C-Order. It uses F-Order because the Array storage uses a Fortran ordering!
  rc = ESMF_FAILURE;

  const ESMC_TypeKind_Flag tk = arr.getTypekind();
  nc_type dtype;
  if (tk == ESMC_TYPEKIND_R8) {
    dtype = NC_DOUBLE;
  } else {
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
      "TypeKind not supported through update: " + to_string(tk) , rc);
  }

  string name(arr.getName());
  int rank = arr.getRank();

  json& var_meta = this->getOrCreateVariable(name, rc);
  ESMF_CHECKERR_STD("", rc, "Did not get variable metadata", rc);

  auto arrshp = getArrayShape(arr, ESMC_INDEX_GLOBAL, rc);
  ESMF_CHECKERR_STD("", rc, "Did not get array shape", rc);
  tdklog("Metadata::update arrshp", arrshp);


  if (dimnames) {
    json dimsizes = this->getDimensionSizes(rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

//    vector<string> ldimnames(dimnames->size(), "");
//    std::reverse_copy(dimnames->begin(), dimnames->end(), ldimnames.begin());
    const vector<string>& ldimnames = *dimnames;
    assert((int)ldimnames.size() == rank);

    for (auto ii=0; ii<rank; ii++) {
      // If the dimension name exists in the dimension sizes pulled from the
      // metadata, confirm it has the correct size.
      if (isIn(ldimnames[ii], dimsizes)) {
        if (dimsizes[ldimnames[ii]] != arrshp[ii]) {
          tdklog("Metadata::update dimsizes=" + dimsizes.dump());
          auto msg = "Provided dimension names have sizes in current storage that "
                     "conflict with the array size.";
          ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
        }
      } else {
        // Create the dimension as it does not yet exist.
        //tdk:TODO: need a create dimension for json
        this->storage[K_DIMS][ldimnames[ii]] = createJSONPackage("ESMF:Metadata:Dimension", rc);
        ESMF_CHECKERR_STD("", rc, "Did not create dimension package", rc);

        this->storage[K_DIMS][ldimnames[ii]][K_NAME] = ldimnames[ii];
        this->storage[K_DIMS][ldimnames[ii]][K_SIZE] = arrshp[ii];
      }
    }

    var_meta[K_DIMS] = ldimnames;
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

  var_meta[K_NAME] = name;
  var_meta[K_DTYPE] = dtype;

  assert(!this->storage[K_VARS][name][K_NAME].is_null());
  assert(!this->storage[K_VARS][name][K_DTYPE].is_null());

  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createArray()"
Array* Metadata::createArray(DistGrid& distgrid, const json& jsonParms,
  int& rc) const {
  //tdk:TODO: attributes on array bundle object!
  //tdk:TODO: standard try/catch
  //tdk:TODO: fix all warnings
  string variableName;
  try {
    variableName = jsonParms.at(ESMFARG::VARIABLENAME);
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
    // Get array name from internal metadata.
    const json& var_meta = this->storage.at(K_VARS).at(variableName);

    // Get the dimension names for the array. We need to remove any zero-length
    // unlimited dimensions unless we are always creating unlimited dimensions.
    vector<string> orig_dim_names = var_meta.at(K_DIMS);
    vector<string> dim_names;
    dim_names.reserve(orig_dim_names.size());
    json dimsizes = this->getDimensionSizes(rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    for (const auto& dn : orig_dim_names) {
      if (dimsizes.at(dn) != 0) {
        dim_names.push_back(dn);
      }
    }
    tdklog("metadata::createarray dim_names",dim_names);
    tdklog("metadata::createarray dimsizes="+dimsizes.dump());

    // Get the distributed dimension names having a zero-length array as default.
    vector<string> v_distDims = jsonParms.value(ESMFARG::DISTDIMS,
      json::array());
    int rank = dim_names.size();
    // Convert the metadata data type to an ESMF TypeKind.
    ESMC_TypeKind_Flag tk = getESMFTypeKind(var_meta[K_DTYPE], rc);
    ESMF_CHECKERR_STD("", rc, "Did not get TypeKind", rc);

    ArraySpec arrayspec;
    arrayspec.set(rank, tk);

    // Distributed dimensions =================================================

    // Map distributed dimensions to their index in the ESMF Array
    vector<ESMC_I4> v_distgridToArrayMap(v_distDims.size(), -999);
    int ii = 0;
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

    // Unsupported parameters =================================================

    auto computationalEdgeLWidthArg = nullptr;
    auto computationalEdgeUWidthArg = nullptr;
    auto computationalLWidthArg = nullptr;
    auto computationalUWidthArg = nullptr;
    auto totalLWidthArg = nullptr;
    auto totalUWidthArg = nullptr;
    ESMC_IndexFlag* indexflag = nullptr;
    auto distLBoundArg = nullptr;

    // Undistributed dimensions ===============================================

    // Undistributed dimension count
    int n_undist = dim_names.size() - v_distDims.size();
    assert(n_undist >= 0);

    // Undistributed lower bounds
    //tdk:TODO: undistriubted lower bound may be non-zero
    vector<ESMC_I4> v_undistLBoundArg(n_undist, 1);
    InterArray<ESMC_I4> undistLBoundArg(v_undistLBoundArg);
//  auto undistLBoundArg = nullptr;

    // Undistributed upper bounds
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

    // Array creation =========================================================

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
#define ESMC_METHOD "Metadata::createArrayBundle()"
ArrayBundle* Metadata::createArrayBundle(DistGrid& distgrid, vector<Array*>& arrayList,
  const json& jsonParms, int& rc) const {
  //tdk:TODO: attributes on array bundle object!
  rc = ESMF_FAILURE;
  try {
    vector<string> varname_filter = jsonParms.value(ESMFARG::VARIABLENAME, json::array());
    const json& jmeta = this->getStorageRef();
    const json& varmeta = jmeta.at(K_VARS);
    vector<string> varnames;
    varnames.reserve(varmeta.size());
    vector<string> distdims = jsonParms.value(ESMFARG::DISTDIMS, json::array());
    for (json::const_iterator it=varmeta.cbegin(); it!=varmeta.cend(); it++) {
      string curr_varname = it.key();
      if (varname_filter.size() == 0 || varname_filter.size() > 0 && isIn(curr_varname, varname_filter)) {
        // Only load arrays from disk that share distributed dimensions with
        // the distgrid.
        const vector<string> dims = varmeta.at(curr_varname).at(K_DIMS);
        if (isIn(distdims, dims)) {
          varnames.push_back(curr_varname);
        }
      }
    }
    tdklog("metadata::createarraybundle varnames", varnames);
    int nvars = varnames.size();
    arrayList.resize(nvars);
    json local_arrParms = jsonParms;
    for (auto ii = 0; ii < varnames.size(); ii++) {
      local_arrParms[ESMFARG::VARIABLENAME] = varnames[ii];
      tdklog("metadata::createarraybundle current varname="+varnames[ii]);
      arrayList[ii] = this->createArray(distgrid, local_arrParms, rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    }
    ArrayBundle* arrb = ArrayBundle::create(arrayList.data(), nvars, false, false,
      &rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    return arrb;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
    throw;
  }
  catch (...) {
    ESMF_CHECKERR_STD("", rc, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "Metadata::createDistGrid()"
DistGrid* Metadata::createDistGrid(const json& jsonParms, int& rc) const {
  // Notes:
  //   * DistGrid dimension names must be in the DistGrid dimension creation order
  //   * DISTDIMS uses F-order
  //tdk:TODO: standard try/catch
  vector<string> unsupported = {"regDecomp", "decompflag", "decompflagCount",
    "regDecompFirstExtra", "regDecompLastExtra", "deLabelList", "indexflag",
    "connectionList", "delayout", "vm", "indexTK"};
  handleUnsupported(jsonParms, unsupported, rc);

  vector<string> v_distDims = jsonParms.value(ESMFARG::DISTDIMS,
    json::array());

  vector<ESMC_I4> v_minIndex = jsonParms.value(ESMFARG::MININDEX,
    json::array());
  vector<ESMC_I4> v_maxIndex = jsonParms.value(ESMFARG::MAXINDEX,
    json::array());

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

//  ret->print(); //tdk:p

  return ret;

}

#undef ESMC_METHOD
#define ESMC_METHOD "getDimensionSizes()"
json Metadata::getDimensionSizes(int& rc) const {
  //tdk:TODO: add standard try/catch
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
dimsize_t Metadata::getDimensionSize(const string& name, int& rc) const {
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
vector<dimsize_t> Metadata::getVariableShape(const string& name, int& rc) const {
  //tdk:TEST: dimensionless variable
  rc = ESMF_FAILURE;

  try {
    const json::array_t* dims = this->storage.at(K_VARS).at(name).at(K_DIMS).get_ptr<const json::array_t*>();
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
bool Metadata::hasVariable(const string& name) const {
  const json& vars_meta = this->storage[K_VARS];
  return vars_meta.find(name) != vars_meta.end();
}

#undef ESMC_METHOD
#define ESMC_METHOD "isUnlimited()"
bool Metadata::isUnlimited(const string& name) const {
  return this->storage.at(K_DIMS).at(name).at(K_UNLIM);
}

}  // ESMCI
