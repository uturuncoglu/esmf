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
//-----------------------------------------------------------------------------
//

#ifndef ESMCI_TESTUTILATTRIBUTES_H
#define ESMCI_TESTUTILATTRIBUTES_H

//-----------------------------------------------------------------------------

#include "ESMCI_Metadata.h"
#include "ESMCI_Util.h"
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
//BOP
// !CLASS:
//
// !DESCRIPTION:
// The code in this file implements something useful to ESMF and description
// lines can carry over.
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

  DistGrid* createTestDistGrid(const Metadata& meta, int& rc);
  json createTestJSONMetadata(int& rc);
  void finalizeFailure(int& rc, char failMsg[], string msg);

} // namespace

#endif  // ifdef barrier
