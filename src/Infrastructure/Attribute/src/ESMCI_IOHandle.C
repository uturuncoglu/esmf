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

#include <assert.h>
#include <vector>
#include <iostream>

#include <netcdf.h>
#include <pio.h>

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

#undef ESMC_METHOD
#define ESMC_METHOD "handlePIOReturnCode()"
void handlePIOReturnCode(const int& pio_rc, const string& pio_msg, int& rc) {
  if (pio_rc != 0) {
    string msg = "PIO Error Code: " + to_string(pio_rc) + " - " + pio_msg;
    ESMF_CHECKERR_STD("ESMC_RC_NETCDF_ERROR", ESMC_RC_NETCDF_ERROR, msg, rc);
  }
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::close()"
void IOHandle::close(int& rc) {
  rc = ESMF_FAILURE;
  int ncid = this->PIOArgs.at(PIOARG::NCID);
  int pio_rc = PIOc_closefile(ncid);
  handlePIOReturnCode(pio_rc, "Could not close with PIO", rc);
  this->PIOArgs.erase(this->PIOArgs.find(PIOARG::NCID));
  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::dodef()"
void IOHandle::dodef(int& rc) {
  rc = ESMF_FAILURE;
  int ncid = this->PIOArgs.at(PIOARG::NCID);

  json& smeta = this->meta.getStorageRefWritable();
  json& varmeta = smeta.at(K_VARS);
  json& varids = this->PIOArgs[PIOARG::VARIDS];
  int ndim;
  for (json::iterator it_var=varmeta.begin(); it_var!=varmeta.end(); it_var++) {
    cout<<"(x) varmeta.key="<<it_var.key()<<endl;
    auto it_varid = varids.find(it_var.key());
    if (it_varid == varids.end()) {
      cout<<"(x) varid not found"<<endl;
//      cout<<it_var.value().dump(2)<<endl;
      ndim = it_var.value()[K_DIMS].size();
      cout<<"(x) ndim="<<ndim<<endl;
      if (ndim > 0) {
        cout<<"(x) variable has dimensions"<<endl;
        json& dims = it_var.value()[K_DIMS];
        cout<<dims.dump(2)<<endl;
//        for (json::const_iterator it_dimnames=dims.cbegin(); it_dimnames!=dims.cend(); it_dimnames++) {
        for (auto& dimname : dims) {
          cout << "(x) dimname=" << dimname << endl;
        }
      } else {
        cout<<"(x) variable DOES NOT have dimensions"<<endl;
      }
    }
  }

//  int PIOc_def_dim(int ncid, const char *name, PIO_Offset len, int *idp);
//  handlePIOReturnCode(pio_rc, "Could not close with PIO", rc);

  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::enddef()"
void IOHandle::enddef(int& rc) {
  rc = ESMF_FAILURE;
  int ncid = this->PIOArgs.at(PIOARG::NCID);
  int pio_rc = PIOc_enddef(ncid);
  handlePIOReturnCode(pio_rc, "Could not enddef with PIO", rc);
  rc = ESMF_SUCCESS;
}

#undef ESMC_METHOD
#define ESMC_METHOD "finalize()"
void IOHandle::finalize(int& rc) {
  rc = ESMF_FAILURE;
  int iosysid = this->PIOArgs.at(PIOARG::IOSYSID);
  int pio_rc = PIOc_finalize(iosysid);
  handlePIOReturnCode(pio_rc, "Could not finalize PIO", rc);
  this->PIOArgs.erase(this->PIOArgs.find(PIOARG::IOSYSID));
  rc = ESMF_SUCCESS;
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
#define ESMC_METHOD "IOHandle::init()"
int IOHandle::init(int& rc) {
  rc = ESMF_FAILURE;

  int iosysid;
  const int io_proc_stride = 1;
  const int io_proc_start = 0;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  MPI_Comm comm = vm->getMpi_c();

//  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();
//  cout << localPet << " " << petCount;
  const int num_iotasks = petCount;

  int pio_rc = PIOc_Init_Intracomm(comm, num_iotasks, io_proc_stride,
    io_proc_start, PIO_REARR_SUBSET, &iosysid);
  handlePIOReturnCode(pio_rc, "Could not start PIO Intracomm", rc);

  this->PIOArgs[PIOARG::DIMIDS] = json::object();
  this->PIOArgs[PIOARG::IOSYSID] = iosysid;
  this->PIOArgs[PIOARG::VARIDS] = json::object();

  rc = ESMF_SUCCESS;
  return iosysid;
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::open()"
void IOHandle::open(int& rc) {
  rc = ESMF_FAILURE;

  const string filename = this->PIOArgs.value(PIOARG::FILENAME, "");
  if (filename == "") {
    ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
      "PIOArg 'filename' may not be empty", rc);
  }

  int iosysid;
  auto it_iosysid = this->PIOArgs.find(PIOARG::IOSYSID);
  if (it_iosysid == this->PIOArgs.end()) {
    iosysid = this->init(rc);
    ESMF_CHECKERR_STD("", rc, "Did not init", rc);
  } else {
    iosysid = it_iosysid.value();
  }

  int iotype = (int)(this->PIOArgs.value(PIOARG::IOTYPE, PIO_IOTYPE_NETCDF));
  int mode = this->PIOArgs.value(PIOARG::MODE, NC_WRITE);
  auto it_ncid = this->PIOArgs.find(PIOARG::NCID);
  int ncid;
  if (it_ncid == this->PIOArgs.end()) {
    int pio_rc = PIOc_createfile(iosysid, &ncid, &iotype, filename.c_str(),
      mode);
    handlePIOReturnCode(pio_rc, "Could not open filename: " + filename, rc);
    this->PIOArgs[PIOARG::NCID] = ncid;
  }

  rc = ESMF_SUCCESS;
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
