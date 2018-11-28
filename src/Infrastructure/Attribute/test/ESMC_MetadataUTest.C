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
#include "ESMCI_Attributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"

using namespace ESMCI;
using namespace std;

//=============================================================================
//BOP
// !PROGRAM: ESMC_MetadataUTest
//
// !DESCRIPTION:
//
//EOP
//-----------------------------------------------------------------------------

void finalizeFailure(int& rc, char failMsg[], string msg) {
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

#undef  ESMC_METHOD
#define ESMC_METHOD "test()"
void test(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  // Create a metadata object mimicking CF-Grid metadata ======================
  //tdk: replace duplicate strings
  //tdk: bounds for spatial/time

  json root = createJSONPackage("ESMF:Metadata:Group", rc);
  ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

  root["attrs"] = "~CF-1.x";

  vector<string> dimnames = {"dim_lon", "dim_lat", "dim_time", "dim_level",
                             "dim_realization"};
  vector<long int> sizes = {360, 180, 365, 100, 10};
  auto ctr = 0;
  for (auto name : dimnames) {
    root["dimensions"][name] = createJSONPackage("ESMF:Metadata:Dimension", rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root["dimensions"][name]["name"] = name;
    root["dimensions"][name]["size"] = sizes[ctr];
    root["dimensions"][name]["is_unlimited"] = false;

    ctr++;
  }

  // Time and level are the unlimited dimensions
  root["dimensions"]["dim_time"]["is_unlimited"] = true;
  root["dimensions"]["dim_level"]["is_unlimited"] = true;

  vector<string> varnames = {"the_xc", "the_yc", "the_time", "the_level",
                          "the_realization", "foo"};
  for (auto name : varnames) {
    root["variables"][name] = createJSONPackage("ESMF:Metadata:Variable", rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root["variables"][name]["name"] = name;
    root["variables"][name]["dtype"] = "double";
  }

  // Level has integer meters
  root["variables"]["the_level"]["dtype"] = "int";

  // Add the "data" variable which holds the things we care about in a data file
  root["variables"]["foo"]["dimensions"] =
          json::array({"the_realization", "the_time", "the_level", "the_yc",
                       "the_xc"});
  root["variables"]["foo"]["dtype"] = "double";
  root["variables"]["foo"]["attrs"]["grid_mapping_name"] = "latitude_longitude";

  root["variables"]["the_xc"]["dimensions"].push_back("dim_lon");
  root["variables"]["the_yc"]["dimensions"].push_back("dim_lat");
  root["variables"]["the_time"]["dimensions"].push_back("dim_time");
  root["variables"]["the_level"]["dimensions"].push_back("dim_level");
  root["variables"]["the_realization"]["dimensions"].push_back("dim_realization");

  root["variables"]["the_xc"]["attrs"]["axis"] = "X";
  root["variables"]["the_xc"]["attrs"]["units"] = "degrees_east";

  root["variables"]["the_yc"]["attrs"]["axis"] = "Y";
  root["variables"]["the_yc"]["attrs"]["units"] = "degrees_north";

  root["variables"]["the_time"]["attrs"]["axis"] = "T";
  root["variables"]["the_time"]["attrs"]["units"] = "days since 1900-01-01";
  root["variables"]["the_time"]["attrs"]["calendar"] = "standard";

  root["variables"]["the_level"]["attrs"]["axis"] = "Z";
  root["variables"]["the_level"]["attrs"]["units"] = "meters";

//  cout << root.dump(2) << endl; //tdk:p

  // ==========================================================================

  Metadata meta(move(root));
  vector<string> dist_dims = {"dim_lon"};
  DistGrid* dist_grid = meta.createESMF(dist_dims, rc);
  ESMF_CHECKERR_STD("", rc, "DistGrid creation failed", rc);

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
  //NEX_UTest
  strcpy(name, "Test general operations");
  test(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
