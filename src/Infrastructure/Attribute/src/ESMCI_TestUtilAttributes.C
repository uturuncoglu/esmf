// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMCI_TestUtilAttributes.C"

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include <netcdf.h>  //tdk:TODO: why do i not include PIO only?

#include "ESMC.h"
#include "ESMCI_DistGrid.h"
#include "ESMCI_TestUtilAttributes.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

using namespace ESMCI::MKEY;
using namespace std;
using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {
  
#undef  ESMC_METHOD
#define ESMC_METHOD "createTestDistGrid()"
DistGrid* createTestDistGrid(const Metadata& meta, int& rc) {
  json jsonParms = createTestDistDims();
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
                             "dim_lat", "dim_lon", "dim_other"};
//  vector<long int> sizes = {360, 180, 4, 20, 3};
  vector<long int> sizes = {3, 20, 4, 180, 360, 5};
  auto ctr = 0;
  for (auto name : dimnames) {
    root[K_DIMS][name] = createJSONPackage("ESMF:Metadata:Dimension", rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root[K_DIMS][name][K_NAME] = name;
    root[K_DIMS][name][K_SIZE] = sizes[ctr];

    ctr++;
  }

  // Time is the unlimited dimension.
  root[K_DIMS]["dim_time"][K_UNLIM] = true;

  vector <string> varnames = {"the_xc", "the_yc", "the_time", "the_level",
                              "the_realization", "foo", "dimensionless",
                              "simple_3D"};
  for (auto name : varnames) {
    root[K_VARS][name] = createJSONPackage("ESMF:Metadata:Variable", rc);
    ESMF_CHECKERR_STD("", rc, "Package creation failed", rc);

    root[K_VARS][name][K_NAME] = name;
    root[K_VARS][name][K_DTYPE] = NC_DOUBLE;
  }

  // Level has integer meters
  root[K_VARS]["the_level"][K_DTYPE] = NC_INT;

  // Add the "data" variable which holds the things we care about in a data file
  root[K_VARS]["foo"][K_DIMS] =
    json::array({"dim_time", "dim_realization", "dim_level", "dim_lat", "dim_lon"});
  root[K_VARS]["foo"][K_ATTRS]["grid_mapping_name"] = "latitude_longitude";

  root[K_VARS]["the_xc"][K_DIMS].push_back("dim_lon");
  root[K_VARS]["the_yc"][K_DIMS].push_back("dim_lat");
  root[K_VARS]["the_time"][K_DIMS].push_back("dim_time");
  root[K_VARS]["the_level"][K_DIMS].push_back("dim_level");
  root[K_VARS]["the_realization"][K_DIMS].push_back("dim_realization");

  root[K_VARS]["the_xc"][K_ATTRS][K_AXIS] = "X";
  root[K_VARS]["the_xc"][K_ATTRS][K_UNITS] = "degrees_east";
  root[K_VARS]["the_xc"][K_ATTRS]["meridian"] = 181.1;

  root[K_VARS]["the_yc"][K_ATTRS][K_AXIS] = "Y";
  root[K_VARS]["the_yc"][K_ATTRS][K_UNITS] = "degrees_north";
  root[K_VARS]["the_yc"][K_ATTRS]["modifier_int"] = 400;

  root[K_VARS]["the_time"][K_ATTRS][K_AXIS] = "T";
  root[K_VARS]["the_time"][K_ATTRS][K_UNITS] = "days since 1900-01-01";
  root[K_VARS]["the_time"][K_ATTRS][K_CALENDAR] = "standard";

  root[K_VARS]["the_level"][K_ATTRS][K_AXIS] = "Z";
  root[K_VARS]["the_level"][K_ATTRS][K_UNITS] = "meters";

  root[K_VARS]["simple_3D"][K_ATTRS]["desc"] = "a test 3d variable";
  root[K_VARS]["simple_3D"][K_DIMS] = json::array({"dim_realization", "dim_other", "dim_level"});

  return root;
}

#undef ESMC_METHOD
#define ESMC_METHOD "createTestDistDims()"
json createTestDistDims() {
  json j;
  j[ESMFARG::DISTDIMS] = json::array({"dim_lon", "dim_lat"});
  return j;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "finalizeFailure()"
void finalizeFailure(int& rc, char failMsg[], string msg) {
  rc = ESMF_FAILURE;
  strcpy(failMsg, msg.c_str());
  return;
};

}  // ESMCI
