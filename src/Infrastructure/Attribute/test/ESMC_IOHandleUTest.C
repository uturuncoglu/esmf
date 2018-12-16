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
  ESMF_CHECKERR_STD("", rc, "Did not create test JSON metadata", rc);

  json& storage = ioh2.meta.getStorageRefWritable();
  storage = move(jmeta);

  string filename = "test_pio_open.nc";
  ioh2.PIOArgs[PIOARG::FILENAME] = filename;
  ioh2.PIOArgs[PIOARG::MODE] = NC_NOWRITE;

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

  if (ioh2.PIOArgs.size() != 1) {
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
#define ESMC_METHOD "testWriteArray()"
void testWriteArray(int& rc, char failMsg[]) {
  rc = ESMF_FAILURE;
//  bool failed = true;

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

//  dimsize_t size = meta.getDimensionSize("dim_lon", rc);
  dimsize_t size = 90; //tdk:TODO: should use local size
  ESMF_CHECKERR_STD("", rc, ESMCI_ERR_PASSTHRU, rc);

  void** larrayBaseAddrList =  arr->getLarrayBaseAddrList();
  double* buffer = reinterpret_cast<double*>(larrayBaseAddrList[0]);
  for (auto ii=0; ii<size; ii++) {
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

  cout<<ioh.meta.dump(2,rc)<<endl;  //tdk:p

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

  cout<<ioh.PIOArgs.dump(2)<<endl;  //tdk:p

  ioh.finalize(rc);
  ESMF_CHECKERR_STD("", rc, "Did not finalize", rc);

  //tdk:TODO: remove netcdf test file

  rc = ESMCI::Array::destroy(&arr);
  rc = ESMCI::DistGrid::destroy(&distgrid);
  ESMF_CHECKERR_STD("", rc, "Problem when destroying objects", rc);

  rc = ESMF_SUCCESS;
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

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test opening and closing a netCDF file");
  testOpenClose(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  //NEX_UTest
  strcpy(name, "Test writing a simple array");
  testWriteArray(rc, failMsg);
  ESMC_Test((rc==ESMF_SUCCESS), name, failMsg, &result, __FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  ESMC_TestEnd(__FILE__, __LINE__, 0);
  //---------------------------------------------------------------------------

  return 0;
};
