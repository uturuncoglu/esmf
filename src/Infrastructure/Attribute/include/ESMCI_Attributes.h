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

#ifndef ESMCI_ATTRIBUTES_H
#define ESMCI_ATTRIBUTES_H

//-----------------------------------------------------------------------------

#include <vector>

#include "ESMCI_Util.h"
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.
using std::string;

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  Attributes
//
// !DESCRIPTION:
// The code in this file implements the Attributes defined type
// and methods.
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

class Attributes
{
 private:
    std::string attrName;  // Inline to reduce memory thrashing
    json storage;  // JSON object store

    // Prevent accidental copying
    Attributes(const Attributes&);

 public:
    Attributes(void);
    ~Attributes(void);
    Attributes(json &storage);

    void erase(string key, string keyChild, int &rc);

    const json& getStorageRef(void);

    template <typename T>
    T get(string key, int &rc);

    template <typename T>
    void set(string key, T value, bool force, int &rc);

};
} // namespace

// Fortran interface functions
extern "C" {
}

#endif  // ifdef barrier
