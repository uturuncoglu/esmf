// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2016, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//
//=============================================================================

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <iostream>

#include "ESMC.h"
#include "ESMC_Test.h"
#include "ESMCI_Array.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"

using namespace ESMCI;
using namespace ESMCI::MKEY;  // Contains key vnames for metadata dictionary
using namespace std;

//=============================================================================
//BOP
// !PROGRAM: ESMC_MetadataUTest
//
// !DESCRIPTION:
//
//EOP
//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "createTestDistGrid()"
DistGrid* createTestDistGrid(const Metadata& meta, int& rc) {
  json jsonParms;
  jsonParms["distDims"] = {"dim_lon", "dim_lat"};
  DistGrid* dist_grid = meta.createDistGrid(jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "DistGrid creation failed", rc);
  return dist_grid;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "createTestJSONMetadata()"
json createTestJSONMetadata(int& rc) {

  // Create a metadata object mimicking CF-Grid metadata ======================
  //tdk:TEST: add bounds for spatial/time

  json root = createJSONPackage("ESMF:Metadata:Group", rc);
  ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

  root[K_ATTRS]["Convention"] = "~CF-1.x";

//  vector <string> dimnames = {"dim_lon", "dim_lat", "dim_time", "dim_level",
//                              "dim_realization"};
  vector<string> dimnames = {"dim_realization", "dim_time", "dim_level",
                             "dim_lat", "dim_lon"};
//  vector<long int> sizes = {360, 180, 4, 20, 3};
  vector<long int> sizes = {3, 20, 4, 180, 360};
  auto ctr = 0;
  for (auto name : dimnames) {
    root[K_DIMS][name] = createJSONPackage("ESMF:Metadata:Dimension",
                                           rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root[K_DIMS][name][K_NAME] = name;
    root[K_DIMS][name][K_SIZE] = sizes[ctr];
    root[K_DIMS][name][K_UNLIM] = false;

    ctr++;
  }

  // Time and level are the unlimited dimensions
  root[K_DIMS]["dim_time"][K_UNLIM] = true;
  root[K_DIMS]["dim_level"][K_UNLIM] = true;

  vector <string> varnames = {"the_xc", "the_yc", "the_time", "the_level",
                              "the_realization", "foo", "dimensionless"};
  for (auto name : varnames) {
    root[K_VARS][name] = createJSONPackage("ESMF:Metadata:Variable", rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root[K_VARS][name][K_NAME] = name;
    root[K_VARS][name][K_DTYPE] = "double";
  }

  // Level has integer meters
  root[K_VARS]["the_level"][K_DTYPE] = "int";

  // Add the "data" variable which holds the things we care about in a data file
  root[K_VARS]["foo"][K_DIMS] =
    json::array({"dim_realization", "dim_time", "dim_level", "dim_lat", "dim_lon"});
  root[K_VARS]["foo"][K_DTYPE] = "double";
  root[K_VARS]["foo"][K_ATTRS]["grid_mapping_name"] = "latitude_longitude";

  root[K_VARS]["the_xc"][K_DIMS].push_back("dim_lon");
  root[K_VARS]["the_yc"][K_DIMS].push_back("dim_lat");
  root[K_VARS]["the_time"][K_DIMS].push_back("dim_time");
  root[K_VARS]["the_level"][K_DIMS].push_back("dim_level");
  root[K_VARS]["the_realization"][K_DIMS].push_back("dim_realization");

  root[K_VARS]["the_xc"][K_ATTRS][K_AXIS] = "X";
  root[K_VARS]["the_xc"][K_ATTRS][K_UNITS] = "degrees_east";

  root[K_VARS]["the_yc"][K_ATTRS][K_AXIS] = "Y";
  root[K_VARS]["the_yc"][K_ATTRS][K_UNITS] = "degrees_north";

  root[K_VARS]["the_time"][K_ATTRS][K_AXIS] = "T";
  root[K_VARS]["the_time"][K_ATTRS][K_UNITS] = "days since 1900-01-01";
  root[K_VARS]["the_time"][K_ATTRS][K_CALENDAR] = "standard";

  root[K_VARS]["the_level"][K_ATTRS][K_AXIS] = "Z";
  root[K_VARS]["the_level"][K_ATTRS][K_UNITS] = "meters";

  return root;
};

void finalizeFailure(int& rc, char failMsg[], string msg) {
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testCreateArray()"
void testCreateArray(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  bool failed = true;

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  // Test creation with a DistGrid ============================================

  DistGrid* distgrid = createTestDistGrid(meta, rc);

  json jsonParms = {{"distDims", {"dim_lon", "dim_lat"}},
                    {"variableName", "foo"}};

  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

  const json& smeta = meta.getStorageRef();
  int rank_desired = smeta[K_VARS]["foo"][K_DIMS].size();
  int rank_actual = arr->getRank();
  if (rank_desired != rank_actual) {
    return finalizeFailure(rc, failMsg, "Array is wrong rank");
  }

  string name_actual(arr->getName());
  if (name_actual != "foo") {
    return finalizeFailure(rc, failMsg, "Array has the wrong name");
  }

  rc = ESMCI::Array::destroy(&arr);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying array", rc);

  // Test no variable name provided ===========================================

  json badParms = json::object();
  try {
    ESMCI::Array *arr2 = meta.createArray(*distgrid, badParms, rc);
    failed = true;
  }
  catch (esmf_attrs_error& e) {
    if (rc != ESMC_RC_ARG_BAD) {
      failed = true;
    } else {
      failed = false;
    }
  }

  if (failed) {
    return finalizeFailure(rc, failMsg, "Did not handle missing parameter");
  }

  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying distgrid", rc);

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testCreateDistGrid()"
void testCreateDistGrid(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

//  cout << root.dump(2) << endl; //tdk:p

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  // Test creating with distributed dimension names ===========================

  //tdk:TEST: test error handling with missing required parameter
  //tdk:TEST: test error handling with unsupported parameter
  //tdk:TEST: test error handling with unsupported parameter type
  //tdk:TEST: test error handling with extra parameters

  DistGrid* dist_grid2 = createTestDistGrid(meta, rc);

  ESMC_I8 const *ecount = dist_grid2->getElementCountPTile();
  if (*ecount != 64800) {
    return finalizeFailure(rc, failMsg, "Wrong number of elements");
  }

  rc = ESMCI::DistGrid::destroy(&dist_grid2);
  ESMF_CHECKERR_STD("", rc, "Did not destroy dist grid", rc);

  // Test with an empty parameters definition =================================

  json jsonParms2 = json::object();
  try {
    DistGrid *dist_grid3 = meta.createDistGrid(jsonParms2, rc);
    return finalizeFailure(rc, failMsg, "Somehow created DistGrid");
  }
  catch (ESMCI::esmf_attrs_error) {
    if (rc != ESMC_RC_PTR_NULL) {
      return finalizeFailure(rc, failMsg, "Did not get expected return code");
    }
  }
  rc = ESMF_SUCCESS;

  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testCreateJSONPackage()"
void testCreateJSONPackage(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  string pkgKey = "ESMF:Metadata:Group";
  json jattrs = createJSONPackage(pkgKey, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  // Test an unsupported key ==================================================

  bool failed = true;
  string badPkgKey = "Does:Not:Exist";
  try {
    json noattrs = createJSONPackage(badPkgKey, rc);
  }
  catch (esmf_attrs_error &err) {
    if (err.getReturnCode() == ESMF_RC_NOT_FOUND) {
      failed = false;
    }
  }
  if (failed) {
    return finalizeFailure(rc, failMsg, "Package should not be returned");
  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testGetArrayShape()"
void testGetArrayShape(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  DistGrid* distgrid = createTestDistGrid(meta, rc);

  json jsonParms = {{"distDims", {"dim_lon", "dim_lat"}},
                    {"variableName", "foo"}};

  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

  auto shp = getArrayShape(*arr, rc);
  ESMF_CHECKERR_STD("", rc, "Array get shape failed", rc);

  auto actual = meta.getVariableShape("foo", rc);
  ESMF_CHECKERR_STD("", rc, "Did not get variable shape", rc);

//  for (auto e : shp) {cout << e << endl;}
//  for (auto e : actual) {cout << e << endl;}

  for (std::size_t ii=0; ii<shp.size(); ii++) {
    if (shp[ii] != actual[ii]) {
      return finalizeFailure(rc, failMsg, "Shapes not equal");
    }
  }

  rc = ESMCI::Array::destroy(&arr);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying array", rc);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying distgrid", rc);

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testUpdate()"
void testUpdate(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  DistGrid* distgrid = createTestDistGrid(meta, rc);

  json jsonParms = {{"distDims", {"dim_lon", "dim_lat"}},
                    {"variableName", "foo"}};

  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

  // Essentially an identity operation. The array is being added to the metadata
  // that created it. Nothing should change in the metadata object.
  auto mref = meta.getStorageRef();
  vector<string> dimnames = mref[K_VARS]["foo"][K_DIMS];
  json desired = mref;  // Copy original for comparison.
  meta.update(*arr, &dimnames, rc);
  ESMF_CHECKERR_STD("", rc, "Update with array failed", rc);

  if (desired != meta.getStorageRef()) {
    return finalizeFailure(rc, failMsg, "Metadata should be equal");
  }

  // Test creating dimensions when the array is added =========================

  meta.update(*arr, nullptr, rc);
  ESMF_CHECKERR_STD("", rc, "Update with array failed", rc);

  if (desired == meta.getStorageRef()) {
    return finalizeFailure(rc, failMsg, "Metadata should not be equal");
  }

  rc = ESMCI::Array::destroy(&arr);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying array", rc);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying distgrid", rc);

  rc = ESMF_SUCCESS;
  return;
}

//-----------------------------------------------------------------------------

int main(void) {

  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  strcpy(failMsg, "Did not return ESMF_SUCCESS");  // Default fail message

  //---------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Metadata::createArray()");
  testCreateArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Metadata::createDistGrid()");
  testCreateDistGrid(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Metadata::update(<Array>)");
  testUpdate(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "createJSONPackage()");
  testCreateJSONPackage(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "getArrayShape()");
  testGetArrayShape(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
