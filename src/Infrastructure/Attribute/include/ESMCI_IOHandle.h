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

#ifndef ESMCI_IOHANDLE_H
#define ESMCI_IOHANDLE_H

#include <fstream>
#include <vector>

#include <netcdf.h>
#include <pio.h>

#include "ESMCI_Array.h"
#include "ESMCI_Attributes.h"
#include "ESMCI_DistGrid.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_Util.h"
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.
using namespace std;

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  IOHandle
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

enum ESMC_RWMode {ESMC_RWMODE_READ, ESMC_RWMODE_WRITE};

namespace PIOARG {
  const string CLOBBER = "clobber";
  const string DIMIDS = "dimids";
  const string FILENAME = "filename";
  const string FRAMES = "frames";
  const string IOIDS = "ioids";
  const string IOSYSID = "iosysid";
  const string IOTYPE = "iotype";
  const string MAPLENS = "maplens";
  const string MODE = "mode";
  const string NCID = "ncid";
  const string VARIDS = "varids";
}

namespace PIODEF {
  const bool CLOBBER = false;
  const int IOTYPE = PIO_IOTYPE_NETCDF;
  const int MODE_READ = NC_NOWRITE;
  const int MODE_WRITE = NC_WRITE;
  const int REARRANGER = PIO_REARR_SUBSET;
}

//-----------------------------------------------------------------------------

class IOHandle {

public:
  ESMCI::Metadata meta;
  json PIOArgs = json::object();

private:
  const int to_remove_reserve_size = 10;
  vector<string> to_remove_on_close;

  void readOrWrite(ESMC_RWMode rwmode, const Array& arr, int& rc);
  int init(int& rc);  //tdk:TODO: init should return void like all other io functions
  void initPIODecomp(const Array& arr, int& rc);

public:
//  IOHandle(void);
  IOHandle(void) = default;  // Default constructor
//  ~IOHandle(void);
  ~IOHandle(void) = default; // Default destructor
  IOHandle(IOHandle&&) = delete; // Move constructor
  IOHandle(const IOHandle&) = delete; // Copy constructor
  IOHandle&operator=(const IOHandle&) = delete; // Copy assignment
  IOHandle&operator=(IOHandle&&) = delete; // Move assignment

  ESMCI::Metadata &getMetadata() { return this->meta; }
  json& getPIOArgs() { return this->PIOArgs; }
  void readMetadata(int& rc);
  //tdk:TODO: copy option for setMetadata
  void setMetadata(ESMCI::Metadata &&meta);

  //tdk:TODO: consider add const qualifier to all io operations is possible; a lot of them modify pioargs
  void close(int& rc);
  void dodef(int& rc);
  void enddef(int& rc);
  void finalize(int& rc);
  void open(int& rc);
  void read(const Array& arr, int& rc);
  void write(const Array& arr, int& rc);

};

//-----------------------------------------------------------------------------

} // namespace

// Fortran interface functions
extern "C" {

}  // extern "C"

#endif  // ifdef barrier
