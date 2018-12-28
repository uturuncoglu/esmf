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
#include "ESMCI_ArrayBundle.h"
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

// Use the same type as PIO for dimension lengths.
typedef int dimsize_t;

namespace ESMFARG {
  const string DISTDIMS = "distDims";
  const string MININDEX = "minIndex";
  const string MAXINDEX = "maxIndex";
  const string VARIABLENAME = "variableName";
}

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
vector<vector<dimsize_t>> getArrayBounds(const Array& arr,
  const ESMC_IndexFlag& idxFlag, int& rc);
vector<dimsize_t> getArrayShape(const Array& arr,
  const ESMC_IndexFlag& idxFlag, int& rc);

//-----------------------------------------------------------------------------

class Metadata : public Attributes {

using Attributes::Attributes; // Required to inherit constructors

private:
  virtual void init(void);

public:
  // Since other constructors are inherited and this is the only
  // specialization, it needs to be explicitly defined to ensure its init
  // method is called.
  Metadata(void) {this->init();}
  ~Metadata(void) = default;  // Overload the destructor so we clean-up nicely.

  ESMCI::Array* createArray(DistGrid& distGrid, const json& jsonParms,
    int& rc) const;
  ESMCI::ArrayBundle* createArrayBundle(DistGrid& distGrid, const json& jsonParms,
    int& rc) const;
  DistGrid* createDistGrid(const json& jsonParms, int& rc) const;
  json getDimensionSizes(int& rc) const;
  dimsize_t getDimensionSize(const string& name, int& rc) const;
  json& getOrCreateVariable(const string& name, int& rc);
  vector<dimsize_t> getVariableShape(const string& name, int& rc) const;
  bool hasVariable(const string& name) const;
  bool isUnlimited(const string& name) const;
  void update(const ESMCI::Array& arr, const vector<string>* dimnames, int& rc);
};

//-----------------------------------------------------------------------------

} // namespace

// Fortran interface functions
extern "C" {

}  // extern "C"

#endif  // ifdef barrier
