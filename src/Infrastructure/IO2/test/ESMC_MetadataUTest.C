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
#include "ESMCI_ArrayBundle.h"
#include "ESMCI_Info.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_TestUtilAttributes.h"
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
#define ESMC_METHOD "testCreateArray()"
void testCreateArray(int& rc, char failMsg[]) {
  //tdk:TEST: creation with no distributed dimensions
  //tdk:TODO: should we create empty arrays? essentially just attribute carriers
  //tdk:TEST: create an array and distrid in one call to create array
  rc = ESMF_FAILURE;
  bool failed = true;

  json root = createTestJSONMetadata(rc);
  ESMF_CHECKERR_STD("", rc, "Did not create JSON Metadata", rc);

  Metadata meta(move(root));

  // Test creation with a DistGrid ============================================

  DistGrid* distgrid = createTestDistGrid(meta, rc);

  json jsonParms = createTestDistDims();
  jsonParms[ESMFARG::VARIABLENAME] = "foo";
  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

  // Test Array rank
  const json& smeta = meta.getStorageRef();
  int rank_desired = smeta[K_VARS]["foo"][K_DIMS].size();
  int rank_actual = arr->getRank();
  if (rank_desired != rank_actual) {
    return finalizeFailure(rc, failMsg, "Array is wrong rank");
  }
  // Test Array name
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
  catch (esmf_info_error& e) {
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
  catch (ESMCI::esmf_info_error &exc) {
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
  catch (esmf_info_error &err) {
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

  auto shp = getArrayShape(*arr, ESMC_INDEX_GLOBAL, rc);
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

#undef  ESMC_METHOD
#define ESMC_METHOD "testCreateArrayBundle()"
void testCreateArrayBundle(int& rc, char failMsg[]) {
  //tdk:TEST: with limiting list of variable names
  rc = ESMF_FAILURE;

//  const vector<string> distDims = {"dim_lat", "dim_lon"};
  const vector<string> distDims;

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  DistGrid* distgrid = createTestDistGrid(meta, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  json arrParms;
  arrParms[ESMFARG::DISTDIMS] = distDims;
  arrParms[ESMFARG::VARIABLENAME] = {"the_xc", "the_yc", "foo", "simple_3D"};

  vector<ESMCI::Array*> arrayList;
  ESMCI::ArrayBundle* arrb = meta.createArrayBundle(*distgrid, arrayList, arrParms,
    rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  rc = ESMCI::ArrayBundle::destroy(&arrb);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  for (ESMCI::Array* arr : arrayList) {
    rc = ESMCI::Array::destroy(&arr);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  }
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

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
  //NEX_disabled_UTest
  strcpy(name, "Metadata::createArray()");
  testCreateArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_disabled_UTest
  strcpy(name, "Metadata::createDistGrid()");
  testCreateDistGrid(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_disabled_UTest
  strcpy(name, "Metadata::update(<Array>)");
  testUpdate(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_disabled_UTest
  strcpy(name, "createJSONPackage()");
  testCreateJSONPackage(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_disabled_UTest
  strcpy(name, "getArrayShape()");
  testGetArrayShape(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

//  //---------------------------------------------------------------------------
//  //NEX_disabled_UTest
//  strcpy(name, "createArrayBundle()");
//  testCreateArrayBundle(rc, failMsg);
//  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
//  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
