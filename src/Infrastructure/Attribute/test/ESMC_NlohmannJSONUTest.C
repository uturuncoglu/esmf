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
//==============================================================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

// ESMF header
#include "ESMC.h"

// ESMF Test header
#include "ESMC_Test.h"

// JSON header
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//==============================================================================
//BOP
// !PROGRAM: ESMC_NlohmannJSONUTest - Test the nlohmann/json for Modern C++
//           library
//
// !DESCRIPTION: Test inclusion of JSON header file and creating a basic object
//
//EOP
//------------------------------------------------------------------------------

int main(void){

  // Test variables
  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  //----------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Basic JSON Map Creation");
  strcpy(failMsg, "Did not return ESMF_SUCCESS");

  json root;

  root["ESMF"] = json::object();

  root["ESMF"]["General"] = json::object();
  root["ESMF"]["Extended"] = json::object();

  // Reference the ESMF/Extended JSON object using keys
  root["ESMF"]["Extended"]["long_name"] = "foobar";

  // Reference the ESMF/General JSON object then add something to it
  json &general  = root["ESMF"]["General"];
  general["what"] = "has been added";

  std::cout << root.dump(4) << std::endl;

  rc = ESMF_SUCCESS

  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);

  //----------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //----------------------------------------------------------------------------

  return 0;
}
