// $Id$
//
// Earth System Modeling Framework
// Copyright 2002-2016, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//
//=============================================================================

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ESMC.h"
#include "ESMC_Test.h"
#include "ESMCI_IOHandle.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Metadata.h"
#include "ESMCI_TestUtilAttributes.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"

using namespace ESMCI;
using namespace ESMCI::MKEY;  // Contains key vnames for metadata dictionary
using namespace std;

//=============================================================================
//BOP
// !PROGRAM: ESMC_IOHandleUTest
//
// !DESCRIPTION:
//
//EOP
//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "testOpenClose()"
void testOpenClose(int& rc, char failMsg[]) {
  //tdk:RENAME: just to "test"
  //tdk:TODO: add full circle test once reading is implemented
  rc = ESMF_FAILURE;
  bool failed = true;

  // Test failure without URI =================================================

  IOHandle ioh;
  try {
    ioh.open(rc);
    return finalizeFailure(rc, failMsg, "Expected to fail without URI");
  }
  catch (esmf_attrs_error& e) {};

  // Test the basic IO system =================================================

  IOHandle ioh2;
  json jmeta = createTestJSONMetadata(rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  Metadata meta(move(jmeta));
  ioh2.setMetadata(move(meta));

  string filename = "test_pio_open.nc";
  ioh2.PIOArgs[PIOARG::FILENAME] = filename;
  ioh2.PIOArgs[PIOARG::MODE] = NC_WRITE;

  ioh2.open(rc);
  ESMF_CHECKERR_STD("", rc, "Did not open", rc);

  ioh2.dodef(rc);
  ESMF_CHECKERR_STD("", rc, "Did not dodef", rc);

  ioh2.enddef(rc);
  ESMF_CHECKERR_STD("", rc, "Did not enddef", rc);

  ioh2.close(rc);
  ESMF_CHECKERR_STD("", rc, "Did not close", rc);

  ioh2.finalize(rc);
  ESMF_CHECKERR_STD("", rc, "Did not finalize", rc);

//  cout<<ioh2.PIOArgs.dump(2)<<endl;
  if (ioh2.PIOArgs.size() != 2) {
    return finalizeFailure(rc, failMsg, "Remaining arg count problem");
  }

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  int localPet = vm->getLocalPet();
  if (localPet == 0 && remove(filename.c_str()) != 0) {
    return finalizeFailure(rc, failMsg, "Test file not removed");
  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testWrite1DArray()"
void testWrite1DArray(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  json dgparms;
  dgparms[ESMFARG::DISTDIMS] = {"dim_lon"};
  DistGrid* distgrid = meta.createDistGrid(dgparms, rc);
  ESMF_CHECKERR_STD("", rc, "DistGrid creation failed", rc);

  json jsonParms;
  jsonParms[ESMFARG::DISTDIMS] = {"dim_lon"};
  jsonParms[ESMFARG::VARIABLENAME] = "the_xc";

  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, "Array creation failed", rc);

  ESMC_TypeKind_Flag tk = arr->getTypekind();
  LocalArray** larrayList = arr->getLocalarrayList();
  int rank = arr->getRank();
  int index[rank] = {1};
  ESMC_R8 data = std::numeric_limits<ESMC_R8>::min();

  rc = larrayList[0]->getData(index, &data);
  ESMF_CHECKERR_STD("", rc, "Failure when getting data from local array", rc);

  if (data == std::numeric_limits<ESMC_R8>::min()) {
    return finalizeFailure(rc, failMsg, "Did not get value from local array");
  }

  vector<dimsize_t> arrshp = getArrayShape(*arr, ESMC_INDEX_DELOCAL, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  void** larrayBaseAddrList =  arr->getLarrayBaseAddrList();
  double* buffer = reinterpret_cast<double*>(larrayBaseAddrList[0]);
  for (auto ii=0; ii<arrshp[0]; ii++) {
    buffer[ii] = (1000 * (localPet + 1)) + ii + 1.5;
  }

  const string filename = "test_pio_write_1d_array.nc";
  IOHandle ioh;
  ioh.PIOArgs[PIOARG::FILENAME] = filename;
  const vector<string> dimnames = {"the_longitude"};
  ioh.meta.update(*arr, &dimnames, rc);
  ESMF_CHECKERR_STD("", rc, "Metadata not updated", rc);

  json& smeta = ioh.meta.getStorageRefWritable();
  smeta.at(K_VARS).at("the_xc").at(K_ATTRS)[K_AXIS] = "X";

  //tdk:TODO: this test should only call "write" and all these methods should be intelligently called

  ioh.open(rc);
  ESMF_CHECKERR_STD("", rc, "Did not open file", rc);

  ioh.dodef(rc);
  ESMF_CHECKERR_STD("", rc, "Did not define", rc);

  ioh.enddef(rc);
  ESMF_CHECKERR_STD("", rc, "Did not enddef", rc);

  ioh.write(*arr, rc);
  ESMF_CHECKERR_STD("", rc, "Did not write array", rc);

  ioh.close(rc);
  ESMF_CHECKERR_STD("", rc, "Did not close", rc);

  ioh.finalize(rc);
  ESMF_CHECKERR_STD("", rc, "Did not finalize", rc);

  //tdk:TEST: structure of PIOArgs

  rc = ESMCI::Array::destroy(&arr);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  //tdk:UNCOMM
//  if (localPet == 0 && remove(filename.c_str()) != 0) {
//    return finalizeFailure(rc, failMsg, "Test file not removed");
//  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testReadWrite1DArrayIsolated()"
void testReadWrite1DArrayIsolated(int& rc, char failMsg[]) {
  try {
    rc = ESMF_FAILURE;

    ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    int localPet = vm->getLocalPet();
    int petCount = vm->getPetCount();

    json root = createTestJSONMetadata(rc);
    Metadata meta(move(root));

    // Create array and distgrid ==============================================

    json dgparms;
    dgparms[ESMFARG::DISTDIMS] = {"dim_lon"};
    DistGrid* distgrid = meta.createDistGrid(dgparms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    json jsonParms;
    jsonParms[ESMFARG::DISTDIMS] = {"dim_lon"};
    jsonParms[ESMFARG::VARIABLENAME] = "the_xc";

    // Array to write
    ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // Array to read (fill)
    ESMCI::Array* arr2fill = meta.createArray(*distgrid, jsonParms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // Fill data values =======================================================

    vector<dimsize_t> arrshp = getArrayShape(*arr, ESMC_INDEX_DELOCAL, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    void** larrayBaseAddrList =  arr->getLarrayBaseAddrList();
    double* buffer = reinterpret_cast<double*>(larrayBaseAddrList[0]);
    for (auto ii=0; ii<arrshp[0]; ii++) {
      buffer[ii] = (1000 * (localPet + 1)) + ii + 1.5;
    }

    // Write netCDF file ======================================================

    const string filename = "test_pio_read_write_isolated_1d_array.nc";
    IOHandle ioh;
    ioh.PIOArgs[PIOARG::FILENAME] = filename;
    const vector<string> dimnames = {"the_longitude"};
    ioh.meta.update(*arr, &dimnames, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    json& smeta = ioh.meta.getStorageRefWritable();
    smeta.at(K_VARS).at("the_xc").at(K_ATTRS)[K_AXIS] = "X";

    ioh.write(*arr, rc);
    ESMF_CHECKERR_STD("", rc, "Did not write array", rc);

//    std::cout << ioh.PIOArgs.dump(2) << std::endl;  //tdk:p

    if (ioh.PIOArgs.size() != 1) {
      return finalizeFailure(rc, failMsg, "PIO args wrong size");
    }

    // Fill the array that is identical to the write array ====================

//    std::cout << ioh.PIOArgs.dump() << std::endl;  //tdk:p
    ioh.read(*arr2fill, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    double* arr2fill_buffer = reinterpret_cast<double*>(arr2fill->getLarrayBaseAddrList()[0]);
    for (auto ii=0; ii<arrshp[0]; ii++) {
      if (buffer[ii] != arr2fill_buffer[ii]) {
        return finalizeFailure(rc, failMsg, "Buffers not equal");
      }
    }

    if (ioh.PIOArgs.size() != 1) {
      return finalizeFailure(rc, failMsg, "PIO args wrong size");
    }

    // Clean-up ===============================================================

    rc = ESMCI::Array::destroy(&arr);
    rc = ESMCI::DistGrid::destroy(&distgrid);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    //tdk:UNCOMM
//  if (localPet == 0 && remove(filename.c_str()) != 0) {
//    return finalizeFailure(rc, failMsg, "Test file not removed");
//  }

    rc = ESMF_SUCCESS;
    return;
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
#define ESMC_METHOD "testReadWrite1DArrayZeroLength()"
void testReadWrite1DArrayZeroLength(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  //===========================================================================
  // Create array with fewer elements than can be distributed on 4 procs. This
  // leads to the last PET having no elements.
  //===========================================================================

  Metadata meta;

  json& smalls_dims = meta.getOrCreateDimension("smalldim", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  const int size = 3;
  smalls_dims[K_SIZE] = size;

  json& smalls = meta.getOrCreateVariable("smalls", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  smalls[K_DIMS].push_back("smalldim");
  smalls[K_DTYPE] = NC_INT;

  json jsonParms;
  jsonParms[ESMFARG::DISTDIMS] = {"smalldim"};
  ESMCI::DistGrid* distgrid = meta.createDistGrid(jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  jsonParms[ESMFARG::VARIABLENAME] = "smalls";
  ESMCI::Array* arr = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  ESMCI::Array* arr2fill = meta.createArray(*distgrid, jsonParms, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  // Confirm the created arrays have size 0 on the last PET
  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  int localPet = vm->getLocalPet();
  const vector<ESMCI::Array *> arrs = {arr, arr2fill};
  for (const auto &a: arrs) {
    const vector <dimsize_t> arrshp = getArrayShape(*a, ESMC_INDEX_DELOCAL,
                                                    rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    if ((localPet == 3) && (arrshp[0] != 0)) {
      return finalizeFailure(rc, failMsg, "Last PET should be empty");
    }
  }

  // Fill data values =========================================================

  void **larrayBaseAddrList = arr->getLarrayBaseAddrList();
  ESMC_I4 *buffer = reinterpret_cast<ESMC_I4 *>(larrayBaseAddrList[0]);
  if (localPet < size) {
    buffer[0] = localPet;
  }

  ESMC_I4 *buffer2fill = reinterpret_cast<ESMC_I4 *>(arr2fill->getLarrayBaseAddrList()[0]);
  if (localPet < size) {
    buffer[0] = -999;
  }

//  std::cout << meta.dump(2, rc) << std::endl;  //tdk:p
//  distgrid->print(); //tdk:p
//  arr->print(); //tdk:p

  // Read/write data to netCDF ================================================

  IOHandle ioh;
  ioh.setMetadata(move(meta));
  const string filename = "test_pio_empty_pet.nc";
  ioh.PIOArgs[PIOARG::FILENAME] = filename;

  ioh.write(*arr, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if ((buffer[0] == buffer2fill[0]) && (localPet < size)) {
    return finalizeFailure(rc, failMsg, "Buffers should not be equal");
  }

  ioh.read(*arr2fill, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  if ((buffer[0] != buffer2fill[0]) && (localPet < size)) {
    return finalizeFailure(rc, failMsg, "Buffers should be equal after read");
  }

  // Clean-Up =================================================================

  rc = ESMCI::Array::destroy(&arr);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testWrite3DArray()"
void testWrite3DArray(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  //---------------------------------------------------------------------------

  // Dimension names for ESMF Array
  const vector<string> dimnames = {"dim_realization", "dim_other", "dim_seven"};
  // Variable name in metadata used to create ESMF Array
  const string varname = "simple_3D";

  // All the distributed dimension combinations to test
  vector<vector<string>> poss_distdims;
  poss_distdims.push_back(vector<string>({"dim_realization"}));
  poss_distdims.push_back(vector<string>({"dim_other"}));
  poss_distdims.push_back(vector<string>({"dim_seven"}));
  poss_distdims.push_back(vector<string>({"dim_other", "dim_seven"}));
  poss_distdims.push_back(vector<string>({"dim_realization", "dim_other", "dim_seven"}));
  poss_distdims.push_back(vector<string>({"dim_realization", "dim_other"}));
  poss_distdims.push_back(vector<string>({"dim_realization", "dim_seven"}));

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  for (const auto& distdims : poss_distdims) {

    // Create DistGrid ========================================================

    json dgparms;
    dgparms[ESMFARG::DISTDIMS] = distdims;
    DistGrid *distgrid = meta.createDistGrid(dgparms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // JSON Parameters for Array creation =====================================

    json jsonParms;
    jsonParms[ESMFARG::DISTDIMS] = distdims;
    jsonParms[ESMFARG::VARIABLENAME] = varname;

    // Create Array to fill from file =========================================

    ESMCI::Array *arr2fill = meta.createArray(*distgrid, jsonParms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // Create Array to write ==================================================

    ESMCI::Array *arr = meta.createArray(*distgrid, jsonParms, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
//    arr->print(); //tdk:p

    vector <dimsize_t> arrshp = getArrayShape(*arr, ESMC_INDEX_DELOCAL, rc);
//  std::reverse(arrshp.begin(), arrshp.end());  // Reverse to Fortran order
    tdklog("testWrite3DArray arrshp", arrshp);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

    // Fill the buffer to write with some data
    void **larrayBaseAddrList = arr->getLarrayBaseAddrList();
    double *buffer = reinterpret_cast<double *>(larrayBaseAddrList[0]);
    dimsize_t arrsize = sizeFromShape(arrshp);
    for (auto ii = 0; ii < arrsize; ii++) {
      buffer[ii] = 1000 * (localPet + 1) + ii + 0.5;
    }

    // Confirm data values are not equal before read...just in case
    double *buffer2fill = reinterpret_cast<double *>(arr2fill->getLarrayBaseAddrList()[0]);
    for (auto ii = 0; ii < arrsize; ii++) {
      buffer2fill[ii] = std::numeric_limits<double>::max();
    }
    for (auto ii = 0; ii < arrsize; ++ii) {
      if (buffer[ii] == buffer2fill[ii]) {
        return finalizeFailure(rc, failMsg, "Buffers should not be equal");
      }
    }

    // Create IOHandle ========================================================

    IOHandle ioh;
    const string filename = "test_pio_write_3D_array.nc";
    ioh.PIOArgs[PIOARG::FILENAME] = filename;
    ioh.PIOArgs[PIOARG::CLOBBER] = true;

    // Add the Array-to-write to the output metadata
    ioh.meta.update(*arr, &dimnames, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    // Add a custom attribute to the metadata for the outgoing array
    json &smeta = ioh.meta.getStorageRefWritable();
    smeta.at(K_VARS).at(varname).at(K_ATTRS)["context"] = "testWrite3DArray";

    // Do a full write operation ==============================================

    ioh.open(rc);
    ESMF_CHECKERR_STD("", rc, "Did not open file", rc);

    ioh.dodef(rc);
    ESMF_CHECKERR_STD("", rc, "Did not define", rc);

    ioh.enddef(rc);
    ESMF_CHECKERR_STD("", rc, "Did not enddef", rc);

    ioh.write(*arr, rc);
    ESMF_CHECKERR_STD("", rc, "Did not write array", rc);

//    std::cout << ioh.PIOArgs.dump(2) << std::endl;  //tdk:p

    ioh.close(rc);
    ESMF_CHECKERR_STD("", rc, "Did not close", rc);

    ioh.finalize(rc);
    ESMF_CHECKERR_STD("", rc, "Did not finalize", rc);

    // Confirm the user-provided arguments are all that is left after finalizing
    if (ioh.PIOArgs.size() != 2) {
      return finalizeFailure(rc, failMsg, "Wrong PIOArgs size after write");
    }

    // Read netCDF data back in ===============================================

    ioh.read(*arr2fill, rc);
    ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
    for (auto ii = 0; ii < arrsize; ++ii) {
      if (buffer[ii] != buffer2fill[ii]) {
        return finalizeFailure(rc, failMsg, "Buffers should be equal");
      }
    }

    if (ioh.PIOArgs.size() != 2) {
      return finalizeFailure(rc, failMsg, "Wrong PIOArgs size after read");
    }

    // Clean-Up ===============================================================

    rc = ESMCI::Array::destroy(&arr);
    rc = ESMCI::Array::destroy(&arr2fill);
    rc = ESMCI::DistGrid::destroy(&distgrid);
    ESMF_CHECKERR_STD("", rc, "Problem when destroying objects", rc);

  }


  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testWriteUnlimDimArray()"
void testWriteUnlimDimArray(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;

  const string filename = "test_pio_unlim_dim_array.nc";
  const string varname = "foo";
  const vector<string> distdims = {"dim_lat", "dim_lon"};

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  // --------------------------------------------------------------------------

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));

  // Adjust the time to zero as it is unlimited for this test. It could have a
  // size if we were reading the data from netCDF.
  meta.getStorageRefWritable().at(K_DIMS).at("dim_time").at(K_SIZE) = 0;

  // Create DistGrid ==========================================================

  json dgparms;
  dgparms[ESMFARG::DISTDIMS] = distdims;
  DistGrid *distgrid = meta.createDistGrid(dgparms, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  // Create Array =============================================================

  json arrParms;
  arrParms[ESMFARG::DISTDIMS] = distdims;
  arrParms[ESMFARG::VARIABLENAME] = varname;
  //tdk:TODO: add option to always create for unlimited
  ESMCI::Array *arr = meta.createArray(*distgrid, arrParms, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
//  arr->print(); //tdk:p

  // Fill Array with some data ================================================

  vector <dimsize_t> arrshp = getArrayShape(*arr, ESMC_INDEX_DELOCAL, rc);
//  std::reverse(arrshp.begin(), arrshp.end());  // Reverse to Fortran order
  tdklog("testWriteUnlimDimArray arrshp", arrshp);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  void **larrayBaseAddrList = arr->getLarrayBaseAddrList();
  double *buffer = reinterpret_cast<double *>(larrayBaseAddrList[0]);
  for (auto ii = 0; ii < sizeFromShape(arrshp); ii++) {
    buffer[ii] = 1000 * (localPet + 1) + ii + 0.5;
  }

  // Create IOHandle ==========================================================

  IOHandle ioh;
  ioh.PIOArgs[PIOARG::FILENAME] = filename;
  vector<string> dimnames = meta.getStorageRef().at(K_VARS).at(varname).at(K_DIMS);
  dimnames.erase(dimnames.begin());
  assert(!isIn("dim_time", dimnames));
  // Add Array-to-write to the metadata
  ioh.meta.update(*arr, &dimnames, rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  // Add an arbitrary attribute to the output array
  json &smeta = ioh.meta.getStorageRefWritable();
  smeta.at(K_VARS).at(varname).at(K_ATTRS)["context"] = "testWriteUnlimitedDim";
  //tdk:TODO: need to configure unlimited dimensions during update call
  smeta.at(K_DIMS)["dim_time"] = createJSONPackage("ESMF:Metadata:Dimension", rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
  smeta.at(K_DIMS).at("dim_time").at(K_SIZE) = 0;
  smeta.at(K_DIMS).at("dim_time").at(K_UNLIM) = true;
  smeta.at(K_DIMS).at("dim_time").at(K_NAME) = "dim_time";
  vector<string> dims = smeta.at(K_VARS).at(varname).at(K_DIMS);
  dims.insert(dims.begin(), "dim_time");
  smeta.at(K_VARS).at(varname).at(K_DIMS) = dims;
  assert(smeta.at(K_VARS).at(varname).at(K_DIMS)[0] == "dim_time");

  // Run the full IOHandle write system =======================================

  ioh.open(rc);
  ESMF_CHECKERR_STD("", rc, "Did not open file", rc);

  try {
    ioh.dodef(rc);
  }
  catch (esmf_attrs_error& e) {
    ESMF_CHECKERR_STD("", e.getReturnCode(), "Did not define", rc);
    throw;
  }

  //tdk:TEST: zero-length on a PET (to many procs for values)

  ioh.enddef(rc);
  ESMF_CHECKERR_STD("", rc, "Did not enddef", rc);

  // Run the unlimited time write loop ========================================

  for (auto ii=0; ii<20; ii++) {
    ioh.PIOArgs[PIOARG::FRAMES][varname] = ii;
    ioh.write(*arr, rc);
    ESMF_CHECKERR_STD("", rc, "Did not write array", rc);
  }

  ioh.close(rc);
  ESMF_CHECKERR_STD("", rc, "Did not close", rc);

  ioh.finalize(rc);
  ESMF_CHECKERR_STD("", rc, "Did not finalize", rc);

  //tdk:TEST: structure of PIOArgs

  // Clean-Up =================================================================

  rc = ESMCI::Array::destroy(&arr);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying objects", rc);

  //tdk:UNCOMMENT
//  if (localPet == 0 && remove(filename.c_str()) != 0) {
//    return finalizeFailure(rc, failMsg, "Test file not removed");
//  }

  rc = ESMF_SUCCESS;
  return;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "testWriteArrayBundle()"
void testWriteArrayBundle(int& rc, char failMsg[]) {
  //tdk:TEST: finish test
  rc = ESMF_FAILURE;

  const string filename = "test_pio_array_bundle.nc";
  const vector<string> distdims = {"dim_lat", "dim_lon"};

  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  int localPet = vm->getLocalPet();
  int petCount = vm->getPetCount();

  // --------------------------------------------------------------------------

  json root = createTestJSONMetadata(rc);
  Metadata meta(move(root));
  IOHandle ioh;
  ioh.setMetadata(move(meta));
  ioh.PIOArgs[PIOARG::FILENAME] = filename;

  ioh.open(rc);
  ioh.dodef(rc);
  ioh.enddef(rc);
  ioh.close(rc);
  ioh.finalize(rc);

//  json dgparms;
//  dgparms[ESMFARG::DISTDIMS] = distdims;
//  DistGrid *distgrid = meta.createDistGrid(dgparms, rc);
//  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
//
//  json arrParms;
//  arrParms[ESMFARG::DISTDIMS] = distdims;
//
//  ESMCI::Array *arr = meta.createArray(*distgrid, arrParms, rc);
//  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);
////  arr->print(); //tdk:p
//
//  tdk:TODO: add destroys
//  rc = ESMCI::Array::destroy(&arr);
//  rc = ESMCI::DistGrid::destroy(&distgrid);
//  ESMF_CHECKERR_STD("", rc, "Problem when destroying objects", rc);

  //tdk:UNCOMMENT
//  if (localPet == 0 && remove(filename.c_str()) != 0) {
//    return finalizeFailure(rc, failMsg, "Test file not removed");
//  }

  rc = ESMF_FAILURE;
  return;
}

//-----------------------------------------------------------------------------

int main(void) {

  char name[80];
  char failMsg[80];
  int result = 0;
  int rc = ESMF_FAILURE;

  strcpy(failMsg, "Did not return ESMF_SUCCESS");  // Default fail message

  //---------------------------------------------------------------------------
  ESMC_TestStart(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //tdk:UNCOMMENT
  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test opening and closing a netCDF file");
  testOpenClose(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test writing a 1D array");
  testWrite1DArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test reading & writing a 1D array - isolated");
  testReadWrite1DArrayIsolated(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test writing a 3D array");
  testWrite3DArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test writing an array w/ an unlimited dimension");
  testWriteUnlimDimArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

//  //---------------------------------------------------------------------------
//  //NEX_disabled_UTest
//  strcpy(name, "Test writing an array bundle");
//  testWriteArrayBundle(rc, failMsg);
//  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
//  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test writing an array with some empty processes");
  testReadWrite1DArrayZeroLength(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
