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

// Attribute method implementation (body) file

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
// tdk: DOC: add description
//
//-----------------------------------------------------------------------------

// ESMF header
#include "ESMC.h"

#include "ESMCI_Attributes.h"
#include "json.hpp"

#include <vector>
#include <iostream>

using json = nlohmann::json;  // Convenience rename for JSON namespace.

using std::string;
using std::vector;

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

Attributes::Attributes(void){
  this->storage = json::object();
};

Attributes::~Attributes(void){};

const json& Attributes::getStorageRef(){
  return this->storage;
};

template <typename T>
T Attributes::get(string key, int &rc){
  json::json_pointer jp(key);
  T ret;
  ret = this->storage.value(jp, NULL);
  rc = ESMF_SUCCESS;
  return ret;
};
template int Attributes::get<int>(string, int&);

template <typename T>
int Attributes::set(string key, T value){
  json::json_pointer jp(key);
  this->storage[jp] = value;
  return ESMF_SUCCESS;
};
template int Attributes::set<int>(string, int);

}  // namespace
