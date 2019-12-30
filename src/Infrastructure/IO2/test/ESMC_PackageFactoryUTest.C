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
#include <fstream>

#include "ESMC.h"
#include "ESMC_Test.h"
#include "ESMCI_Info.h"
#include "ESMCI_Macros.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"

using namespace ESMCI;
using namespace std;

//=============================================================================
//BOP
// !PROGRAM: ESMC_PackageFactoryUTest
//
// !DESCRIPTION: Test creating standard attribute packages
//
//EOP
//-----------------------------------------------------------------------------

void finalizeFailure(int& rc, char failMsg[], string msg) {
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testConstructor()"
void testConstructor(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
  PackageFactory pf;
  rc = ESMF_SUCCESS;
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "testGetOrCreateJSON()"
void testGetOrCreateJSON(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  PackageFactory pf;

  // Creating a package without a resolvable URI is an error ==================

  try {
    json willfail = pf.getOrCreateJSON("noexist", rc);
  } catch (esmf_attrs_error &e) {
    if (e.getReturnCode() != ESMC_RC_FILE_READ) {
      return finalizeFailure(rc, failMsg, "Did not catch file error");
    }
  }

  // Test a well-formed package ===============================================

  // Write a package to disk for testing
  json testpkg;
  testpkg["foo"] = "bar";
  string fn = "_esmf_test_pkg.json";
  std::ofstream f(fn);
  if (!f.good()) {
    return finalizeFailure(rc, failMsg, "Could not create test file");
  }
  f << testpkg;
  f.close();

  json pkg = pf.getOrCreateJSON("test_pkg", rc, fn);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU);

  // Remove the test file
  int crc = std::remove(fn.c_str());
  if (crc != 0) {
    return finalizeFailure(rc, failMsg, "Did not delete test file");
  }

  if (pkg["foo"] != "bar") {
    return finalizeFailure(rc, failMsg, "Package not created correctly");
  }

  json pkg2 = pf.getOrCreateJSON("test_pkg", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU);

  if (pkg2["foo"] != "bar") {
    return finalizeFailure(rc, failMsg, "Package not created correctly");
  }

  // Test trying to create a package with a parse error =======================

  json badpkg;
  string badfn = "_esmf_test_bad_pkg.json";
  std::ofstream bf(badfn);
  bf.close();

  try {
    json willfail2 = pf.getOrCreateJSON("blah", rc, badfn);
  } catch (esmf_attrs_error& e) {
    if (e.getReturnCode() != ESMC_RC_FILE_READ) {
      return finalizeFailure(rc, failMsg, "Did not catch parse error");
    }
  }

  rc = ESMF_SUCCESS;
  return;
};

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
  //tdk:TEST: enable testing for PackageFactory if this is used; turned off now
  //tdk:TEST:   because they do not work in parallel
  strcpy(name, "PackageFactory Constructor");
  testConstructor(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_disabled_UTest
  strcpy(name, "PackageFactory::getOrCreateJSON()");
  testGetOrCreateJSON(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
