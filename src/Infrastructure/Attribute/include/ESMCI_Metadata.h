// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//
// ESMF Attributes C++ include file
//
//-----------------------------------------------------------------------------
//

#ifndef ESMCI_METADATA_H
#define ESMCI_METADATA_H

//-----------------------------------------------------------------------------

#include <fstream>
#include "json.hpp"
#include <vector>

#include "ESMCI_Array.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_DistGrid.h"
#include "ESMCI_Util.h"

using json = nlohmann::json;  // Convenience rename for JSON namespace.
using namespace std;

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  Metadata
//
// !DESCRIPTION:
// The code in this file implements the Metadata defined type and methods.
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

//-----------------------------------------------------------------------------

namespace MKEY {
  const string K_ATTRS = "attrs";
  const string K_AXIS = "axis";
  const string K_BNDS = "bounds";
  const string K_CALENDAR = "calendar";
  const string K_DIMS = "dims";
  const string K_DTYPE = "dtype";
  const string K_GROUPS = "groups";
  const string K_UNITS = "units";
  const string K_UNLIM = "is_unlimited";
  const string K_NAME = "name";
  const string K_SIZE = "size";
  const string K_UID = "uid";
  const string K_URI = "uri";
  const string K_VARS = "vars";
}

//-----------------------------------------------------------------------------

json createJSONPackage(const string& pkgKey, int& rc);

//-----------------------------------------------------------------------------

class Metadata : public Attributes {

using Attributes::Attributes; // Required to inherit constructors

private:
  unsigned int dimCreateCounter = 0;

  virtual void init(void);

public:
  void add(const ESMCI::Array& arr, const json& jsonParms, int& rc);

  ESMCI::Array* createArray(DistGrid& distGrid, const json& jsonParms,
    int& rc) const;
  DistGrid* createDistGrid(const json& jsonParms, int& rc) const;

  json& getVariable(const string& name, int& rc) {

  }

};

//-----------------------------------------------------------------------------

} // namespace

// Fortran interface functions
extern "C" {

}  // extern "C"

#endif  // ifdef barrier
