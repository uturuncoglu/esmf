// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMCI_IOHandle.C"

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include <vector>
#include <iostream>

#include "ESMC.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_IOHandle.h"
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

// Local function dependencies ================================================

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::close()"
void IOHandle::close(int& rc) {
  rc = ESMF_FAILURE;
//  pio_rc = PIOc_closefile(ncid);
//  if (pio_rc != 0){
//    printf("PIO_TEST:ERROR: %d", pio_rc);
//    ret = 1;
//  }
  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::enddef()"
void IOHandle::enddef(int& rc) {
  rc = ESMF_FAILURE;
//  pio_rc = PIOc_def_dim(ncid, "test_dim", 7, &dimid);
//  if (pio_rc != 0){
//    printf("PIO_TEST:ERROR: %d", pio_rc);
//    ret = 1;
//  }
  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "finalize()"
void finalize(int& rc) {
  rc = ESMF_FAILURE;
//  pio_rc = PIOc_finalize(iosysid);
  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::getOrCreateDimension"
int IOHandle::getOrCreateDimension(int &rc) {
  rc = ESMF_FAILURE;

  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::getOrCreateGroup()"
int IOHandle::getOrCreateGroup(int& rc) {
  rc = ESMF_FAILURE;

//  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::getOrCreateVariable()"
int IOHandle::getOrCreateVariable(int& rc) {
  rc = ESMF_FAILURE;

  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHO "IOHandle(void)"
void IOHandle::init(int& rc) {
  rc = ESMF_FAILURE;
  //tdk:FIX: handle pio error
//  pio_rc = PIOc_Init_Intracomm(MPI_COMM_WORLD, num_iotasks, io_proc_stride, io_proc_start, PIO_REARR_SUBSET, &iosysid);
//  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::open()"
void IOHandle::open(int& rc) {
  rc = ESMF_FAILURE;
//  pio_rc = PIOc_createfile(iosysid, &ncid, &iotype, "_pio-testing_created_file.nc_", 0x0000);
//  if (pio_rc != 0){
//    printf("PIO_TEST:ERROR: %d", pio_rc);
//    ret = 1;
//  }
  //  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::write()"
void IOHandle::write(const Array& arr, int& rc) {
  rc = ESMF_FAILURE;

  vector<string> jargs = {"filename"};
  vector<string> jkwargs = {"clobber", "fileOnly", "mode"};

  // meta.update
  // init
  // open
  // getOrCreateGroup
  // getOrCreateDimension
  // getOrCreateVariable
  // enddef
  // insert
  // close
  // finalize

  //  rc = ESMF_SUCCESS;
}

}  // ESMCI
