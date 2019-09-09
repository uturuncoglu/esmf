// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_Info2CDef.C"

// Info C-Fortran method implementation (body) file

// single blank line to make protex happy.
//BOPI

//EOPI
//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//-----------------------------------------------------------------------------

#include "ESMC.h"
#include "ESMCI_Base.h"
#include "ESMCI_Macros.h"
#include "ESMCI_Info2.h"
#include "ESMCI_LogErr.h"
#include "ESMCI_Util.h"
#include "ESMCI_VM.h"
#include "json.hpp"

#include <iostream>
#include <vector>

using json = nlohmann::json;

//-----------------------------------------------------------------------------
 // leave the following line as-is; it will insert the cvs ident string
 // into the object file for tracking purposes.
 static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

ESMCI::Info2 *baseAddressToInfo2(const long int &baseAddress) {
  void *v = (void *) baseAddress;
  ESMC_Base *base = reinterpret_cast<ESMC_Base *>(v);
  ESMCI::Info2 *info = base->ESMC_BaseGetInfo();
  return info;
}

void updateDirtyInfo(const json &inqstate, int *ctr, std::vector<long int> *base_addresses) {
  int l_ctr = 0;
  if (ctr) {
    l_ctr = *ctr;
  }
  for (json::const_iterator it=inqstate.cbegin(); it!=inqstate.cend(); it++) {
    if (it.value().at("base_is_valid")) {
        if (base_addresses) {
          long int base_address = it.value().at("base_address");
          (*base_addresses)[l_ctr] = base_address;
        }
      l_ctr++;
    }
    const json &members = it.value().at("members");
    if (not members.is_null()) {
      updateDirtyInfo(members, &l_ctr, base_addresses);
    }
  }
  if (ctr) {
    *ctr = l_ctr;
  }
}

//-----------------------------------------------------------------------------

#undef ESMC_METHOD
#define ESMC_METHOD "ESMC_BaseGetInfo2()"
ESMCI::Info2* ESMC_BaseGetInfo2(long int &baseAddress) {
  ESMC_Base *base = reinterpret_cast<ESMC_Base*>((void*)baseAddress);
  return base->ESMC_BaseGetInfo();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Copy()"
ESMCI::Info2* ESMC_Info2Copy(ESMCI::Info2 *info, int &rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Info2(info->getStorageRef());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2CopyForAttribute()"
void ESMC_Info2CopyForAttribute(const ESMCI::Info2* src, ESMCI::Info2* dst, int& rc) {
  rc = ESMF_FAILURE;
  try {
    dst->getStorageRefWritable() = src->getStorageRef();
  }
  ESMF_CATCH_INFO
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Create()"
ESMCI::Info2* ESMC_Info2Create(int& rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Info2();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2CreateByKey()"
ESMCI::Info2* ESMC_Info2CreateByKey(ESMCI::Info2 *srcInfo,
  char* key, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Info2 *info;
  try {
    std::string local_key(key);
    json new_storage = srcInfo->get<json>(local_key, rc);
    info = new ESMCI::Info2(std::move(new_storage));
  }
  ESMF_CATCH_ISOC
  return info;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2CreateByParse()"
ESMCI::Info2* ESMC_Info2CreateByParse(char *payload, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Info2 *info;
  try {
    std::string local_payload(payload);
    info = new ESMCI::Info2(payload, rc);
  }
  ESMF_CATCH_ISOC
  return info;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Destroy()"
void ESMC_Info2Destroy(ESMCI::Info2* info, int& rc) {
  delete info;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Dump()"
void ESMC_Info2Dump(ESMCI::Info2 *info, char *output, int &rc) {
  // Test:
  // Notes:
  rc = ESMF_FAILURE;
  try {
    const std::string c_output = info->dump(rc);
    for (std::size_t ii = 0; ii < c_output.size(); ++ii) {
      output[ii] = c_output[ii];
    }
  }
  ESMF_CATCH_ISOC
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2DumpLength()"
void ESMC_Info2DumpLength(ESMCI::Info2 *info, int &dump_length, int &rc) {
  // Test:
  // Notes:
  rc = ESMF_FAILURE;
  try {
    dump_length = info->dump(rc).size();
  }
  ESMF_CATCH_ISOC
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Erase()"
void ESMC_Info2Erase(ESMCI::Info2* info, char* keyParent,
                          char* keyChild, bool &recursive, int& rc) {
  rc = ESMF_FAILURE;
  std::string localkeyParent(keyParent);
  std::string localkeyChild(keyChild);

  // This seems strange. This is the best method to delete from the Fortran
  // interface to avoid passing "" as the parent key when you want to delete
  // from the root. Otherwise a parent and child key are always required which
  // seems redundant.
  try {
    if (localkeyChild == "") {
      info->erase(localkeyChild, localkeyParent, rc, recursive);
    } else {
      info->erase(localkeyParent, localkeyChild, rc, recursive);
    }
  }
  ESMF_CATCH_ISOC
}

#undef ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Inquire()"
void ESMC_Info2Inquire(ESMCI::Info2 *info, ESMCI::Info2 *inq,
                            char *key, int &fortran_recursive, int *idx, int &rc) {
  rc = ESMF_FAILURE;
  bool recursive = false;
  if (fortran_recursive == 1) {recursive = true;}
  try {
    std::string localKey(key);
    json jinq = info->inquire(localKey, rc, recursive, idx);
    json &inqref = inq->getStorageRefWritable();
    inqref = std::move(jinq);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2IsEqual()"
void ESMC_Info2IsEqual(ESMCI::Info2 *lhs, ESMCI::Info2 *rhs,
  bool &res, int &rc) {
  rc = ESMF_FAILURE;
  try {
    res = lhs->getStorageRef() == rhs->getStorageRef();
  }
  ESMF_CATCH_ISOC
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2IsPresent()"
void ESMC_Info2IsPresent(ESMCI::Info2 *info, char *key, int &fortran_bool_res,
        int &rc, int &fortran_bool_recursive, int &fortran_bool_isptr) {
  std::string local_key(key);
  bool recursive = (fortran_bool_recursive == 1) ? true:false;
  bool isptr = (fortran_bool_isptr == 1) ? true:false;
  try {
    bool res = info->hasKey(local_key, rc, isptr, recursive);
    fortran_bool_res = (res == true) ? 1:0;
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2IsSet()"
void ESMC_Info2IsSet(ESMCI::Info2 *info, char *key, int &isSet, int &rc) {
  std::string local_key(key);
  try {
    isSet = info->isSetNull(local_key, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Print()"
void ESMC_Info2Print(ESMCI::Info2 *info, int &indent, int &rc) {
  rc = ESMF_FAILURE;
  try {
    std::cout << info->dump(indent, rc) << std::endl;
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Update()"
void ESMC_Info2Update(ESMCI::Info2 *lhs, ESMCI::Info2 *rhs, int &rc) {
  rc = ESMF_FAILURE;
  try {
    lhs->update(*rhs, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2BaseSyncDo"
void ESMC_Info2BaseSyncDo(const std::vector<long int> &base_addresses, const int &rootPet, const long int &vmAddress, int &rc) {
  rc = ESMF_FAILURE;
  try {
    void *v = (void *)vmAddress;
    ESMCI::VM *vm = reinterpret_cast<ESMCI::VM *>(v);
    int localPet = vm->getLocalPet();
    json j = json::object();
    if (localPet == rootPet) {
      // For each base address, blind-cast to an Info object and determine
      // if that object has been updated (dirty). If it has been updated, then
      // stick its serialized string representation inside a JSON map. For
      // broadcasting.
      for (std::size_t ii = 0; ii < base_addresses.size(); ++ii) {
        ESMCI::Info2 *info = baseAddressToInfo2(base_addresses[ii]);
        bool is_dirty = info->isDirty();
        if (is_dirty) {
          try {
            j[std::to_string(ii)] = info->getStorageRef().dump();
            // This data will be broadcast and we can consider the object
            // clean.
            info->setDirty(false);
          }
          ESMF_CATCH_JSON
        }
      }
    }
    // Broadcast the update map.
    ESMCI::Info2 binfo(std::move(j));
    broadcastInfo(&binfo, rootPet, *vm, rc);
    // Update for each string key/index in the update map.
    const json &storage = binfo.getStorageRef();
    int ikey;
    for (json::const_iterator it=storage.cbegin(); it!=storage.cend(); it++) {
      ikey = std::stoi(it.key());  // Convert the string index to an integer
      ESMCI::Info2 *info_to_update = baseAddressToInfo2(base_addresses[ikey]);
      // This object is created from a serialized string stored in the update
      // map.
      ESMCI::Info2 rhs(it.value(), rc);
      try {
        if (localPet != rootPet) {
          info_to_update->update(rhs, rc);
          // Since this is part of the sync operation, we do not consider this
          // data dirty after the update.
          info_to_update->setDirty(false);
        }
      }
      ESMF_CATCH_JSON
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2BaseSync()"
void ESMC_Info2BaseSync(ESMCI::Info2 *inqstate, int &rootPet, long int &vmAddress, int &rc) {
  rc = ESMF_FAILURE;
  try {
    const json &j_inqstate = inqstate->getStorageRef();
    int ctr = 0;
    updateDirtyInfo(j_inqstate, &ctr, nullptr);
    std::vector<long int> base_addresses(ctr, 0);
    updateDirtyInfo(j_inqstate, nullptr, &base_addresses);
    ESMC_Info2BaseSyncDo(base_addresses, rootPet, vmAddress, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2Broadcast()"
void ESMC_Info2Broadcast(ESMCI::Info2 *info, int &rootPet, int &rc) {
  //tdk:todo: consider adding vm as an optional argument
  rc = ESMF_FAILURE;
  ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
  ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);

  try {
    broadcastInfo(info, rootPet, *vm, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2ReadJSON()"
void ESMC_Info2ReadJSON(ESMCI::Info2 *info, char *filename, int &rc) {
  rc = ESMF_FAILURE;
  std::string filename2(filename);
  try {
    std::ifstream i(filename2, std::ifstream::in);
    if (!i.good()){
      std::string errmsg = "File location not working: " + filename2;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, errmsg, rc);
    }
    json j;
    try {
      i >> j;
    } catch (json::parse_error& e) {
      ESMF_THROW_JSON(e, "ESMC_RC_FILE_READ", ESMC_RC_FILE_READ, rc);
    }
    i.close();
    json &out = info->getStorageRefWritable();
    out = move(j);
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2WriteJSON()"
void ESMC_Info2WriteJSON(ESMCI::Info2 *info, char *filename, int &rc) {
  rc = ESMF_FAILURE;
  std::string filename2(filename);
  try {
    std::ofstream file;
    file.open(filename2);
    if (!file.is_open()) {
      std::string errmsg = "Error opening output file: " + filename2;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_OPEN", ESMC_RC_FILE_OPEN, errmsg, rc);
    }
    file << info->getStorageRef();
    file.close();
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC
}

//-----------------------------------------------------------------------------

//#undef ESMC_METHOD
//#define ESMC_METHOD "ESMC_InfoGetAttPack()"
//void ESMC_InfoGetAttPack(ESMCI::Info2 *info, ESMCI::AttPack *attpack,
//  char *convention, char *purpose, int &rc) {
//  rc = ESMF_FAILURE;
//  try {
//    std::string conv(convention);
//    std::string purp(purpose);
//    attpack = new ESMCI::AttPack(*info, conv, purp, rc);
////    info->addAttPack(reinterpret_cast<void**>(&attpack));
//  }
//  ESMF_CATCH_ISOC
//}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2GetCH()"
void ESMC_Info2GetCH(ESMCI::Info2* info, char *key, char *value,
  int &vlen, int &rc, char *def, int *index) {
  // String pointer used to define the default value if present
  std::string *def_str_ptr;
  // String object that holds the default if present
  std::string def_str;
  if (def) {
    // Set the default pointer to the string object created from the char
    // array from Fortran
    def_str = std::string(def);
    def_str_ptr = &def_str;
  } else {
    def_str_ptr = nullptr;
  }
  std::string as_str;
  std::string local_key(key);
  try {
    as_str = info->get<std::string>(local_key, rc, def_str_ptr, index);
  }
  ESMF_CATCH_ISOC
  // Transfer the string characters into the Fortran character array using
  // spaces to fill the Fortran array if we are past the max string length.
  for (int ii = 0; ii < vlen; ++ii) {
    if (ii < (int)as_str.size()) {
      value[ii] = as_str[ii];
    } else {
      value[ii] = ' ';
    }
  }
}

//-----------------------------------------------------------------------------

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetCH()"
void ESMC_Info2SetCH(ESMCI::Info2 *info, char *key, char *value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  std::string localValue(value);
  try {
    info->set<std::string>(localKey, localValue, force, rc, index);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetINFO()"
void ESMC_Info2SetINFO(ESMCI::Info2 *info, char *key,
  ESMCI::Info2 *value, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set(localKey, *value, force, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetNULL()"
void ESMC_Info2SetNULL(ESMCI::Info2 *info, char *key, bool &force,
  int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set(localKey, force, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_Info2SetArrayCH()"
void ESMC_Info2SetArrayCH(ESMCI::Info2 *info, char *key, int &count,
  bool &force, int &rc) {
  // Notes:
  //  * Only allocates storage. Does not actually insert anything!
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    info->set<std::vector<std::string>>(localKey, nullptr, count, force, rc);
  }
  ESMF_CATCH_ISOC
}

}  // extern "C"
