// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_InfoCDef.C"

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
#include "ESMCI_Info.h"
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

ESMCI::Info *baseAddressToInfo(const long int &baseAddress) {
  void *v = (void *) baseAddress;
  ESMC_Base *base = reinterpret_cast<ESMC_Base *>(v);
  ESMCI::Info *info = base->ESMC_BaseGetInfo();
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
#define ESMC_METHOD "ESMC_BaseGetInfo()"
ESMCI::Info* ESMC_BaseGetInfo(long int &baseAddress) {
  ESMC_Base *base = reinterpret_cast<ESMC_Base*>((void*)baseAddress);
  return base->ESMC_BaseGetInfo();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoCopy()"
ESMCI::Info* ESMC_InfoCopy(ESMCI::Info *info, int &rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Info(info->getStorageRef());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoCopyForAttribute()"
void ESMC_InfoCopyForAttribute(const ESMCI::Info* src, ESMCI::Info* dst, int& rc) {
  rc = ESMF_FAILURE;
  try {
    dst->getStorageRefWritable() = src->getStorageRef();
  }
  ESMF_CATCH_INFO
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoCreate()"
ESMCI::Info* ESMC_InfoCreate(int& rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Info();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoCreateByKey()"
ESMCI::Info* ESMC_InfoCreateByKey(ESMCI::Info *srcInfo,
  char* key, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Info *info;
  try {
    std::string local_key(key);
    json new_storage = srcInfo->get<json>(local_key, rc);
    info = new ESMCI::Info(std::move(new_storage));
  }
  ESMF_CATCH_ISOC
  return info;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoCreateByParse()"
ESMCI::Info* ESMC_InfoCreateByParse(char *payload, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Info *info;
  try {
    std::string local_payload(payload);
    info = new ESMCI::Info(payload, rc);
  }
  ESMF_CATCH_ISOC
  return info;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoDestroy()"
void ESMC_InfoDestroy(ESMCI::Info* info, int& rc) {
  delete info;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoDump()"
void ESMC_InfoDump(ESMCI::Info *info, char *output, int &rc) {
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
#define ESMC_METHOD "ESMC_InfoDumpLength()"
void ESMC_InfoDumpLength(ESMCI::Info *info, int &dump_length, int &rc) {
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
#define ESMC_METHOD "ESMC_InfoErase()"
void ESMC_InfoErase(ESMCI::Info* info, char* keyParent,
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
#define ESMC_METHOD "ESMC_InfoInquire()"
void ESMC_InfoInquire(ESMCI::Info *info, ESMCI::Info *inq, char *key,
                       int &fortran_recursive, int *idx, int &fortran_attr_compliance,
                       int &rc) {
  rc = ESMF_FAILURE;
  bool recursive = (fortran_recursive == 1) ? true:false;
  bool attr_compliance = (fortran_attr_compliance == 1) ? true:false;
  try {
    std::string localKey(key);
    json jinq = info->inquire(localKey, rc, recursive, idx, attr_compliance);
    json &inqref = inq->getStorageRefWritable();
    inqref = std::move(jinq);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoIsEqual()"
void ESMC_InfoIsEqual(ESMCI::Info *lhs, ESMCI::Info *rhs,
  bool &res, int &rc) {
  rc = ESMF_FAILURE;
  try {
    res = lhs->getStorageRef() == rhs->getStorageRef();
  }
  ESMF_CATCH_ISOC
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoIsPresent()"
void ESMC_InfoIsPresent(ESMCI::Info *info, char *key, int &fortran_bool_res,
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
#define ESMC_METHOD "ESMC_InfoIsSet()"
void ESMC_InfoIsSet(ESMCI::Info *info, char *key, int &isSet, int &rc) {
  std::string local_key(key);
  try {
    isSet = info->isSetNull(local_key, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoPrint()"
void ESMC_InfoPrint(ESMCI::Info *info, int &indent, int &rc) {
  rc = ESMF_FAILURE;
  try {
    std::cout << info->dump(indent, rc) << std::endl;
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoUpdate()"
void ESMC_InfoUpdate(ESMCI::Info *lhs, ESMCI::Info *rhs, int &rc) {
  rc = ESMF_FAILURE;
  try {
    lhs->update(*rhs, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoBaseSyncDo"
void ESMC_InfoBaseSyncDo(const std::vector<long int> &base_addresses, const int &rootPet, const long int &vmAddress, int &rc) {
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
        ESMCI::Info *info = baseAddressToInfo(base_addresses[ii]);
        bool is_dirty = info->isDirty();
        if (is_dirty) {
          try {
            j[std::to_string(ii)] = info->getStorageRef().dump();
            // This data will be broadcast and we can consider the object
            // clean.
//            info->setDirty(false); //tdk:todo: do we really want to remove dirtiness support?
          }
          ESMF_CATCH_JSON
        }
      }
    }
    // Broadcast the update map.
    ESMCI::Info binfo(std::move(j));
    broadcastInfo(&binfo, rootPet, *vm, rc);
    // Update for each string key/index in the update map.
    const json &storage = binfo.getStorageRef();
    int ikey;
    for (json::const_iterator it=storage.cbegin(); it!=storage.cend(); it++) {
      ikey = std::stoi(it.key());  // Convert the string index to an integer
      ESMCI::Info *info_to_update = baseAddressToInfo(base_addresses[ikey]);
      // This object is created from a serialized string stored in the update
      // map.
      ESMCI::Info rhs(it.value(), rc);
      try {
        if (localPet != rootPet) {
          info_to_update->getStorageRefWritable() = rhs.getStorageRef();
          // Note (bekozi): Commented in favor or replacing to avoid tracking
          // down removals in the root PET.
//          info_to_update->update(rhs, rc);
          // Since this is part of the sync operation, we do not consider this
          // data dirty after the update.
//          info_to_update->setDirty(false); //tdk:todo: do we really want to remove dirtiness support?
        }
      }
      ESMF_CATCH_JSON
    }
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoBaseSync()"
void ESMC_InfoBaseSync(ESMCI::Info *inqstate, int &rootPet, long int &vmAddress, int &rc) {
  rc = ESMF_FAILURE;
  try {
    const json &j_inqstate = inqstate->getStorageRef();
    int ctr = 0;
    updateDirtyInfo(j_inqstate, &ctr, nullptr);
    std::vector<long int> base_addresses(ctr, 0);
    updateDirtyInfo(j_inqstate, nullptr, &base_addresses);
    ESMC_InfoBaseSyncDo(base_addresses, rootPet, vmAddress, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoBroadcast()"
void ESMC_InfoBroadcast(ESMCI::Info *info, int &rootPet, int &rc) {
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
#define ESMC_METHOD "ESMC_InfoReadJSON()"
void ESMC_InfoReadJSON(ESMCI::Info *info, char *filename, int &rc) {
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
#define ESMC_METHOD "ESMC_InfoWriteJSON()"
void ESMC_InfoWriteJSON(ESMCI::Info *info, char *filename, int &rc) {
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
//void ESMC_InfoGetAttPack(ESMCI::Info *info, ESMCI::AttPack *attpack,
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
#define ESMC_METHOD "ESMC_InfoGetCH()"
void ESMC_InfoGetCH(ESMCI::Info* info, char *key, char *value,
  int &vlen, int &rc, char *def, int *index, int &fortran_bool_recursive) {
  // String pointer used to define the default value if present
  std::string *def_str_ptr;
  // String object that holds the default if present
  std::string def_str;
  // Convert from Fortran integer to bool
  bool recursive = (fortran_bool_recursive == 1) ? true:false;
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
    as_str = info->get<std::string>(local_key, rc, def_str_ptr, index, recursive);
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
#define ESMC_METHOD "ESMC_InfoSetCH()"
void ESMC_InfoSetCH(ESMCI::Info *info, char *key, char *value,
                              bool &force, int &rc, int *index, char *pkey) {
  rc = ESMF_FAILURE;
  std::string local_key(key);
  std::string local_value(value);
  std::string local_pkey(pkey);
  std::string *local_pkeyp = nullptr;
  if (local_pkey.size() != 0) {local_pkeyp = &local_pkey;}
  try {
    info->set<std::string>(local_key, local_value, force, rc, index, local_pkeyp);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoSetArrayCH()"
void ESMC_InfoSetArrayCH(ESMCI::Info *info, char *key, int &count,
                          bool &force, int &rc, char *pkey) {
  // Notes:
  //  * Only allocates storage. Does not actually insert anything!
  rc = ESMF_FAILURE;
  std::string local_key(key);
  std::string local_pkey(pkey);
  std::string *local_pkeyp = nullptr;
  if (local_pkey.size() != 0) {local_pkeyp = &local_pkey;}
  try {
    info->set<std::vector<std::string>>(local_key, nullptr, count, force, rc, local_pkeyp);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoSetINFO()"
void ESMC_InfoSetINFO(ESMCI::Info *info, char *key,
  ESMCI::Info *value, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string local_key(key);
  try {
    info->set(local_key, *value, force, rc);
  }
  ESMF_CATCH_ISOC
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_InfoSetNULL()"
void ESMC_InfoSetNULL(ESMCI::Info *info, char *key, bool &force,
  int &rc) {
  rc = ESMF_FAILURE;
  std::string local_key(key);
  try {
    info->set(local_key, force, rc);
  }
  ESMF_CATCH_ISOC
}
}  // extern "C"
