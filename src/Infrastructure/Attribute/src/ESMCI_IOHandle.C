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
#include <fstream>
#include <iostream>
#include <vector>

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

//#undef ESMC_METHOD
//#define ESMC_METHOD "getESMFSeqIndex()"
//vector<PIO_Offset> getESMFSeqIndex(const Array& arr, int& rc) {
//  rc = ESMF_FAILURE;
//  try {
//    vector<PIO_Offset> ret;
//    DELayout* delayout = arr.getDELayout();
//    int local_decount = delayout->getLocalDeCount();
//    assert(local_decount == 1);
//
//
//    for (int i = 0; i < local_decount; i++) {
//      // multi-dim loop object
//      ArrayElement arrayElement(&arr, i, false, true, false, true);
////      arrayElement.print();
////      arrayElement.setSkipDim(2);
//      // set up to skip over undistributed, i.e. tensor dimensions
////        const int *srcArrayToDistGridMap = srcArray->getArrayToDistGridMap();
////      for (int j = 0; j < arr->getRank(); j++) {
////          if (srcArrayToDistGridMap[j]==0) arrayElement.setSkipDim(j);
//        // fill in the factorIndexList
////        PIO_Offset adjust = -1;
//      while (arrayElement.isWithin()) {
////        tdklog("getesmfseqindex ctr="+to_string(ctr));
//        SeqIndex <ESMC_I4> seqIndex = arrayElement.getSequenceIndex<ESMC_I4>();
////        arrayElement.print();
////          if (adjust == -1) {
////            adjust = seqIndex.decompSeqIndex;
////          }
////          PIO_Offset adjusted = seqIndex.decompSeqIndex - adjust;
////          factorIndexList[2*jj] = factorIndexList[2*jj+1] =
////            seqIndex.decompSeqIndex;
////          tdklog(string(ESMC_METHOD) + " seqIndex.decompSeqIndex=" +to_string(adjusted));
////        tdklog("getesmfseqindex idx="+to_string(seqIndex.decompSeqIndex));
//        ret.push_back((PIO_Offset)seqIndex.decompSeqIndex);
////          ++jj; // increment counter
//        arrayElement.next();
////          ctr++;
//      } // end while over all exclusive elements
//
////      auto minv = std::min_element(ret.begin(), ret.end());
////      for (size_t ii=0; ii<ret.size(); ii++) {
////        ret[ii] = ret[ii] - *minv;
////      }
//
//    }
//    rc = ESMF_SUCCESS;
//    return ret;
//  }
//  catch (ESMCI::esmf_attrs_error) { throw; }
//  catch (...) { ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc); }
//}

#undef ESMC_METHOD
#define ESMC_METHOD "allb()"
bool allb(const vector<bool>& target) {
  bool ret = true;
  for (const auto &ii : target) {
    if (!ii) {
      ret = false;
      break;
    }
  }
  return ret;
}

#undef ESMC_METHOD
#define ESMC_METHOD "createPIOCompmap(<vector>)"
vector<PIO_Offset> createPIOCompmap(const vector<vector<dimsize_t>>& globalBounds,
                                    const vector<vector<dimsize_t>>& localBounds) {
  assert(globalBounds.size() == localBounds.size());
  size_t nvectors = globalBounds.size();
  assert(nvectors >= 1);

  // Holds the current index counter in tuple form
  vector<int> curr(nvectors, 0);

  // Get the number of global indices
  PIO_Offset nindices = 1;
  for (const auto& gbnds : globalBounds) {
    nindices *= gbnds.at(1) - gbnds.at(0);
  }
  assert(nindices > 0);

  // Will hold PIO offsets for the decomposition mapping
  vector<PIO_Offset> compmap;
  compmap.reserve(nindices);

  vector<bool> contains;
  contains.reserve(nvectors);

  // Loop over every possible global index
  for (PIO_Offset gidx = 0; gidx < nindices; ++gidx) {
    contains.resize(0);
    assert(contains.capacity() >= nvectors);

    // Loop over every vector container.
    for (size_t ctr = 0; ctr < nvectors; ++ctr) {
      // Identify if the tuple counter is in the local bounds.
      if (curr[ctr] >= localBounds[ctr].at(0) && curr[ctr] < localBounds[ctr].at(1)) {
        contains.push_back(true);
      } else {
        contains.push_back(false);
      }
    }
    assert(contains.size() == globalBounds.size());

    // Check if global index is in the decomposed portion of the global bounds
    // defined by the local bounds.
    if (allb(contains)) {
      compmap.push_back(gidx);
    }

    // Increment the tuple counters.
    for (PIO_Offset cii = nvectors - 1; cii >= 0; --cii) {
      if (curr[cii] < globalBounds[cii].at(1) - 1) {
        ++curr[cii];
        break;
      } else {
        curr[cii] = 0;
      }
    }

  }
  return compmap;
}

#undef ESMC_METHOD
#define ESMC_METHOD "createPIOCompmap(<Array>)"
vector<PIO_Offset> createPIOCompmap(const Array& arr, int& rc) {
  rc = ESMF_FAILURE;
  try {
    DELayout* delayout = arr.getDELayout();
    int local_decount = delayout->getLocalDeCount();
    assert(local_decount == 1);

    auto globalBounds = getArrayBounds(arr, ESMC_INDEX_GLOBAL, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    auto localBounds = getArrayBounds(arr, ESMC_INDEX_DELOCAL, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    vector<PIO_Offset> compmap = createPIOCompmap(globalBounds, localBounds);

    rc = ESMF_SUCCESS;
    return compmap;
  }
  catch (ESMCI::esmf_attrs_error) { throw; }
  catch (...) { ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc); }
}

#undef ESMC_METHOD
#define ESMC_METHOD "doesFileExist()"
bool doesFileExist(const string& filename) {
  std::ifstream ifile(filename.c_str());
  bool exists = (bool)ifile;
  ifile.close();
  return exists;
}

#undef ESMC_METHOD
#define ESMC_METHOD "handlePIOReturnCode()"
void handlePIOReturnCode(const int& pio_rc, const string& pio_msg, int& rc) {
  //tdk:TODO: can we avoid having PIO abort on error?
  if (pio_rc != 0) {
    string msg = "PIO Error Code: " + to_string(pio_rc) + " - " + pio_msg;
    ESMF_CHECKERR_STD("ESMC_RC_NETCDF_ERROR", ESMC_RC_NETCDF_ERROR, msg, rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "writePIOAttributes()"
void writePIOAttributes(const json& attrs, int ncid, int varid, int& rc) {
  rc = ESMF_FAILURE;

  try {
    int pio_rc;
    for (json::const_iterator it_attrs=attrs.cbegin(); it_attrs!=attrs.cend(); it_attrs++) {
      auto attrname = it_attrs.key().c_str();
      if (it_attrs.value().is_string()) {
        string value_string = it_attrs.value().get<string>();

        pio_rc = PIOc_put_att_text(ncid, varid, attrname,
                                   value_string.size(), value_string.c_str());
      } else if (it_attrs.value().is_number_float()) {
        double value_double = it_attrs.value().get<double>();
        pio_rc = PIOc_put_att_double(ncid, varid, attrname, NC_DOUBLE, 1,
                                     &value_double);
      } else if (it_attrs.value().is_number_integer()) {
        int64_t value_long = it_attrs.value().get<int64_t>();
        pio_rc = PIOc_put_att_long(ncid, varid, attrname, NC_LONG, 1,
                                   &value_long);
      } else {
        ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
                          "Attribute type not supported", rc);
      }
      //tdk:TODO: add line number to PIO error handling
      handlePIOReturnCode(pio_rc, "Did not set attribute", rc);
    }
    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

//=============================================================================

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::close()"
void IOHandle::close(int& rc) {
  rc = ESMF_FAILURE;

  try {
    if (isIn(PIOARG::NCID, this->PIOArgs)) {
      int ncid = this->PIOArgs.at(PIOARG::NCID);

      int pio_rc = PIOc_closefile(ncid);
      handlePIOReturnCode(pio_rc, "Could not close with PIO", rc);

      this->PIOArgs.erase(this->PIOArgs.find(PIOARG::NCID));
    }

    const vector<string> to_erase = {PIOARG::VARIDS, PIOARG::DIMIDS,
                                     PIOARG::FRAMES, PIOARG::MAPLENS};
    for (const auto& key : to_erase) {
      if (isIn(key, this->PIOArgs)) {
        this->PIOArgs.erase(this->PIOArgs.find(key));
      }
    }

    // Remove any automated arguments =========================================

    for (const auto& tr: this->to_remove_on_close) {
      auto f = this->PIOArgs.find(tr);
      if (f != this->PIOArgs.end()) {
        this->PIOArgs.erase(f);
      }
    }
    this->to_remove_on_close.clear();
    assert(this->to_remove_on_close.size() == 0);

    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::dodef()"
void IOHandle::dodef(int& rc) {
  // Notes:
  //  * Sets varids & dimids

  //tdk:TODO: refactor into functions
  rc = ESMF_FAILURE;

  try {
    int ncid = this->PIOArgs.at(PIOARG::NCID);

    const json& smeta = this->meta.getStorageRef();
    const json& varmeta = smeta.at(K_VARS);
    json& varids = this->PIOArgs[PIOARG::VARIDS];
    json& dimids = this->PIOArgs[PIOARG::DIMIDS];
    int ndims;
    int pio_rc;
    int dimid;
    int dimctr;
    int varid;
    string varname;
    for (json::const_iterator it_var=varmeta.cbegin(); it_var!=varmeta.cend(); it_var++) {
      const auto it_varid = varids.find(it_var.key());
      if (it_varid == varids.end()) {
        ndims = it_var.value()[K_DIMS].size();
        int dimidsp[ndims];
        if (ndims > 0) {
          const json& dims = it_var.value()[K_DIMS];

          dimctr = 0;
          for (const auto& dimname : dims) {
            const auto it_dimid = dimids.find(dimname);
            if (it_dimid == dimids.end()) {
              dimsize_t dimsize;
              //tdk:TODO: PIO only handles one unlimited dimension?
              //tdk:TODO: PIO has to have unlimited dimension as the first dimension (last in Fortran)?
              if (this->meta.isUnlimited(dimname)) {
                dimsize = NC_UNLIMITED;
              } else {
                dimsize = this->meta.getDimensionSize(dimname, rc);
                ESMF_CHECKERR_STD("", rc, "Did not get dimension size: " +
                  dimname.get<string>(), rc);
              }

              pio_rc = PIOc_def_dim(ncid, dimname.get<string>().c_str(), dimsize,
                                    &dimid);
              handlePIOReturnCode(pio_rc, "Could not define dimension", rc);

              dimids[dimname.get<string>()] = dimid;
            } else {
              dimid = it_dimid.value();
            }
            dimidsp[dimctr] = dimid;
            dimctr++;
          }

        }

        nc_type xtype = it_var.value()[K_DTYPE];
        pio_rc = PIOc_def_var(ncid, it_var.key().c_str(), xtype, ndims,
                              dimidsp, &varid);
        handlePIOReturnCode(pio_rc, "Could not define variable", rc);

        varids[it_var.key()] = varid;

        const json& attrs = it_var.value().at(K_ATTRS);
        writePIOAttributes(attrs, ncid, varid, rc);
        ESMF_CHECKERR_STD("", rc, "Did not write attributes with PIO", rc);
      }
    }

    const json& attrs_global = smeta.at(K_ATTRS);
    writePIOAttributes(attrs_global, ncid, NC_GLOBAL, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::enddef()"
void IOHandle::enddef(int& rc) {
  rc = ESMF_FAILURE;
  try {
    int ncid = this->PIOArgs.at(PIOARG::NCID);
    int pio_rc = PIOc_enddef(ncid);
    handlePIOReturnCode(pio_rc, "Could not enddef with PIO", rc);
    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "finalize()"
void IOHandle::finalize(int& rc) {
  rc = ESMF_FAILURE;

  try {
    if (isIn(PIOARG::IOSYSID, this->PIOArgs)) {
      int iosysid = this->PIOArgs.at(PIOARG::IOSYSID);

      int pio_rc;
      if (isIn(PIOARG::IOIDS, this->PIOArgs)) {
        const json &ioids = this->PIOArgs[PIOARG::IOIDS];
        for (json::const_iterator it = ioids.cbegin();
             it != ioids.cend(); it++) {
          int ioid = it.value();
          pio_rc = PIOc_freedecomp(iosysid, ioid);
          handlePIOReturnCode(pio_rc, "Did not free decomp", rc);
          tdklog("freed ioid=" + to_string(ioid));
        }
        this->PIOArgs.erase(this->PIOArgs.find(PIOARG::IOIDS));
      }

      pio_rc = PIOc_finalize(iosysid);
      handlePIOReturnCode(pio_rc, "Could not finalize PIO", rc);
      tdklog("freed iosysid=" + to_string(iosysid));

      this->PIOArgs.erase(this->PIOArgs.find(PIOARG::IOSYSID));
    }

    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::init()"
int IOHandle::init(int& rc) {
  rc = ESMF_FAILURE;

  try {
    int iosysid;
    const int io_proc_stride = 1;
    const int io_proc_start = 0;

    ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    MPI_Comm comm = vm->getMpi_c();

    int petCount = vm->getPetCount();
    const int num_iotasks = petCount;

    int pio_rc = PIOc_Init_Intracomm(comm, num_iotasks, io_proc_stride,
                                     io_proc_start, PIODEF::REARRANGER,
                                     &iosysid);
    handlePIOReturnCode(pio_rc, "Could not start PIO Intracomm", rc);

    this->PIOArgs[PIOARG::DIMIDS] = json::object();  //tdk:TODO: remove?
    this->PIOArgs[PIOARG::IOSYSID] = iosysid;
    this->PIOArgs[PIOARG::VARIDS] = json::object();  //tdk:TODO: remove?

    to_remove_on_close.reserve(this->to_remove_reserve_size); //tdk:TODO: default reserve size?

    rc = ESMF_SUCCESS;
    return iosysid;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::initPIODecomp()"
void IOHandle::initPIODecomp(const Array& arr, int& rc) {
  try {
    const int &iosysid = this->PIOArgs.at(
      PIOARG::IOSYSID).get_ref<json::number_integer_t &>();
    const string name = arr.getName();

    // Get variable metadata
    const json &varmeta = this->meta.getOrCreateVariable(name, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // Get PIO type
    const int &pio_type = varmeta.at(
      K_DTYPE).get_ref<const json::number_integer_t &>();
    // Get the dimension count from the ESMF Array
    const int ndims = arr.getRank();
    // Use the global dimension length when creating the variable decomposition
    const vector <dimsize_t> gdimlen_v = getArrayShape(arr,
                                                       ESMC_INDEX_GLOBAL,
                                                       rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    //tdk:TODO: will need to deal with unlimited dimensions and their location in the length array
    const int *gdimlen = gdimlen_v.data();
//  tdklog("gdimlen_v", gdimlen_v);

    // Sequence indices =====================================================

    // Create the PIO decomposition mapping. This maps an index location in ESMF
    // to its index location in the output netCDF file.
    //tdk:FEATURE: read in PIO decomposition from file
    vector <PIO_Offset> compmap = createPIOCompmap(arr, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    PIO_Offset maplen = compmap.size();
    int ioid;
    int pio_rc = PIOc_init_decomp(iosysid, pio_type, ndims, gdimlen, maplen,
                              compmap.data(), &ioid, PIODEF::REARRANGER,
                              nullptr, nullptr);
    handlePIOReturnCode(pio_rc, "Did not initialize PIO decomposition", rc);

    // Store the PIO decomposition identifier and array size for future use. For
    // example, when calling "write(<Array>)".
    this->PIOArgs[PIOARG::IOIDS][name] = ioid;
    this->PIOArgs[PIOARG::MAPLENS][name] = maplen;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::open()"
void IOHandle::open(int& rc) {
  rc = ESMF_FAILURE;

  try {
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

    int iotype = (int)(this->PIOArgs.value(PIOARG::IOTYPE, PIODEF::IOTYPE));
    int mode = this->PIOArgs.value(PIOARG::MODE, PIODEF::MODE_WRITE);
    tdklog("iohandle::open mode="+to_string(mode));
    tdklog("iohandle::open NC_NOWRITE="+to_string(NC_NOWRITE));
    tdklog("iohandle::open NC_WRITE="+to_string(NC_WRITE));
    auto it_ncid = this->PIOArgs.find(PIOARG::NCID);
    int ncid;

    ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    int localPet = vm->getLocalPet();
    // Check if file exists on the file system. This does not check if the
    // location is valid.
    int exists[1] = {0};
    if (localPet == 0) {
      exists[0] = doesFileExist(filename);
    }
    rc = vm->broadcast((void*)exists, 1, 0);

    if (it_ncid == this->PIOArgs.end()) {
      int pio_rc;
      bool clobber = this->PIOArgs.value(PIOARG::CLOBBER, PIODEF::CLOBBER);
      bool should_create = false;
      if (mode == PIODEF::MODE_WRITE) {
        if (exists[0] == 1) {
          if (clobber) {
            should_create = true;
          } else {
            auto msg = "File exists and 'clobber=false': " + filename;
            ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
          }
        } else {
          should_create = true;
        }
      } else {
        if (exists[0] == 0) {
          auto msg = "File does not exist in read mode. Nothing to do!: "
            + filename;
          ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
        }
      }
      if (should_create) {
        pio_rc = PIOc_createfile(iosysid, &ncid, &iotype, filename.c_str(),
                                 mode);
      } else {
        pio_rc = PIOc_openfile(iosysid, &ncid, &iotype, filename.c_str(),
                               mode);
      }
      handlePIOReturnCode(pio_rc, "Could not open filename: " + filename, rc);
      this->PIOArgs[PIOARG::NCID] = ncid;
    }

    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error &e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::read()"
void IOHandle::read(const Array& arr, int& rc) {
  try {
    this->readOrWrite(ESMC_RWMODE_READ, arr, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::readOrWrite()"
void IOHandle::readOrWrite(ESMC_RWMode rwmode, const Array& arr, int& rc) {
  // Notes:
  //  * The read functionality technically modifies the array's local array
  //    value buffer.

  try {
    rc = ESMF_FAILURE;

    DistGrid* distgrid = arr.getDistGrid();

    // Only one DE is currently supported.
    DELayout* de_layout = distgrid->getDELayout();
    if (de_layout->getLocalDeCount() > 1) {
      ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
                        "Only one DE supported for writing", rc);
    }
    //tdk:TODO: only one tile supported. add check for this

    // Get PIO IO System ======================================================

    // If the PIO IO System has not been created, do so. This is equivalent to
    // not calling "IOHandle::init" before calling write.
    bool should_finalize = false;
    if (!isIn(PIOARG::IOSYSID, this->PIOArgs)) {
      this->init(rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

      should_finalize = true;
    }
    const int &iosysid = this->PIOArgs.at(
      PIOARG::IOSYSID).get_ref<json::number_integer_t &>();

    // ========================================================================

    string name(arr.getName());
    if (!this->meta.hasVariable(name)) {
      ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD,
                        "Array name not found in variable metadata", rc);
    }

    if (!isIn(name, this->PIOArgs[PIOARG::IOIDS])) {
      this->initPIODecomp(arr, rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    }
    //tdk:TODO: attempt to use references here
    int ioid = this->PIOArgs[PIOARG::IOIDS].at(name);
    PIO_Offset maplen = this->PIOArgs[PIOARG::MAPLENS].at(name);

    // Get or create the netCDF file identifier ===============================

    bool should_close = false;
    if (!isIn(PIOARG::NCID, this->PIOArgs)) {
      // Supply a default mode for reading/writing if a default is not supplied
      if (!isIn(PIOARG::MODE, this->PIOArgs)) {
        int piomode;
        switch (rwmode) {
          case ESMC_RWMODE_WRITE: {
            piomode = PIODEF::MODE_WRITE;
            break;
          }
          case ESMC_RWMODE_READ: {
            piomode = PIODEF::MODE_READ;
            break;
          }
          default: {
            auto msg = "ESMC_RWMODE_* not supported: " + to_string(rwmode);
            ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, msg, rc);
          }
        }
        this->PIOArgs[PIOARG::MODE] = piomode;
        // Remove this argument since it was not added by the user
        this->to_remove_on_close.push_back(PIOARG::MODE);
      }
      this->open(rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

      should_close = true;
    }
    const int& ncid = this->PIOArgs.at(PIOARG::NCID).get_ref<const json::number_integer_t&>();

    // Get or create the netCDF variable identifier ===========================

    int pio_rc;
    if (!isIn(name, this->PIOArgs[PIOARG::VARIDS])) {
      if (rwmode == ESMC_RWMODE_WRITE) {
        // In write mode, do the file definition. The variable identifier is
        // supplied to PIOArgs during the definition phase.
        this->dodef(rc);
        ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
        this->enddef(rc);
        ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
      } else {
        // In read mode, retrieve the variable identifier from the netCDf file
        int lvarid;
        pio_rc = PIOc_inq_varid(ncid, name.c_str(), &lvarid);
        handlePIOReturnCode(pio_rc, "Did get variable identifier", rc);
        this->PIOArgs[PIOARG::VARIDS][name] = lvarid;
      }
    }
    const int& varid = this->PIOArgs.at(PIOARG::VARIDS).at(name).get_ref<
      const json::number_integer_t&>();

    // Adjust the frame (timeslice counter) if there is a frame counter =======

    if (isIn(name, this->PIOArgs[PIOARG::FRAMES])) {
      int frame = this->PIOArgs.at(PIOARG::FRAMES).at(name);  //tdk:TODO: use reference?
      pio_rc = PIOc_setframe(ncid, varid, frame);
      handlePIOReturnCode(pio_rc, "Did not set frame", rc);
    } else if (false) {
      //tdk:TODO: if there is an unlimited dimension, then a frame is required. check for this or raise an exception
    }

    // Read/Write data to/from netCDF =========================================

    void** larrayBaseAddrList =  arr.getLarrayBaseAddrList();
//    double* buffer = reinterpret_cast<double*>(larrayBaseAddrList[0]);
    void* buffer = larrayBaseAddrList[0];

    switch (rwmode) {
      case ESMC_RWMODE_WRITE: {
        tdklog("iohandle::readorwrite: in WRITE mode");
        void *fillvalue = nullptr;  //tdk:TODO: not handling fillvalue yet
        pio_rc = PIOc_write_darray(ncid, varid, ioid, maplen, buffer,
                                   fillvalue);
        handlePIOReturnCode(pio_rc, "Did not write darray", rc);

        //tdk:TODO: argument for should_sync?
        pio_rc = PIOc_sync(ncid);
        handlePIOReturnCode(pio_rc, "Did not sync", rc);
        break;
      }
      case ESMC_RWMODE_READ: {
        tdklog("iohandle::readorwrite: in READ mode");
        pio_rc = PIOc_read_darray(ncid, varid, ioid, maplen, buffer);
        handlePIOReturnCode(pio_rc, "Did not read darray", rc);
        break;
      }
      default:
        ESMF_CHECKERR_STD("ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, ESMCI_ERR_PASSTHRU,
          rc);
    }

    // Close/finalize PIO components ==========================================

    if (should_close) {
      this->close(rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
      tdklog("called this->close");
    }
    if (should_finalize) {
      this->finalize(rc);
      ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
      tdklog("called this->finalize");
    }

    rc = ESMF_SUCCESS;
  }
  catch (json::out_of_range& e) {
    ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);
  }
  catch (json::type_error &e) {
    ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);
  }
  catch (ESMCI::esmf_attrs_error& e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), ESMCI_ERR_PASSTHRU, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);
  }
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::setMetadata(<&&Metadata>)"
void IOHandle::setMetadata(ESMCI::Metadata &&meta) {
  json& this_meta_storage = this->meta.getStorageRefWritable();
  this_meta_storage = move(meta.getStorageRefWritable());
}

#undef ESMC_METHOD
#define ESMC_METHOD "IOHandle::write(<Array>)"
void IOHandle::write(const Array& arr, int& rc) {
  try {
    this->readOrWrite(ESMC_RWMODE_WRITE, arr, rc);
  }
  catch (...) {
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  }
}

}  // ESMCI
