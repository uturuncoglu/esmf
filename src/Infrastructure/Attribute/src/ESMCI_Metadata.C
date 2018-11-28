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
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <vector>
#include <iostream>

using namespace std;

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

#undef ESMC_METHOD
#define ESMC_METHOD "<DistGrid*> Metadata::createESMF()"
DistGrid* Metadata::createESMF(const vector<string>& dist_dims, int& rc) const {
 if (dist_dims.size() > 1) {
  string msg = "Only one distributed dimension allowed";
  ESMF_CHECKERR_STD("", ESMC_RC_ARG_BAD, msg, rc);
 }

 string key = "/dimensions/" + dist_dims[0] + "/size";
 auto dist_dim_size = this->get<int>(key, rc);
 ESMF_CHECKERR_STD("", rc, "Did not get distributed dimension", rc);

 cout << "(x) dist_dim_size= " << dist_dim_size << endl;

 vector<int> minIndex_vec = {1};
 InterArray<int> minIndex(minIndex_vec);

 vector<int> maxIndex_vec = {dist_dim_size};
 InterArray<int> maxIndex(maxIndex_vec);

 DistGrid* ret = ESMCI::DistGrid::create(&minIndex, &maxIndex, nullptr, nullptr,
                                         0, nullptr, nullptr, nullptr,
                                         nullptr, nullptr, nullptr, nullptr,
                                         &rc, ESMC_TYPEKIND_I4);

 ret->print();

};

}  // ESMCI
