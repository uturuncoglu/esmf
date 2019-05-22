// $Id$
//
// Earth System Modeling Framework
//\ Copyright 2002-2018, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

#define ESMC_FILENAME "./src/Infrastructure/Attribute/src/ESMC_AttributesCDef.C"

// Attributes C-Fortran method implementation (body) file

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
#include "ESMCI_Attributes.h"
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

ESMCI::Attributes *baseAddressToAttributes(const long int &baseAddress) {
  void *v = (void *) baseAddress;
  ESMC_Base *base = reinterpret_cast<ESMC_Base *>(v);
  ESMCI::Attributes *attrs = base->ESMC_BaseGetAttrs();
  return attrs;
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
#define ESMC_METHOD "ESMC_BaseGetAttributes()"
ESMCI::Attributes* ESMC_BaseGetAttributes(long int &baseAddress) {
  ESMC_Base *base = reinterpret_cast<ESMC_Base*>((void*)baseAddress);
  return base->ESMC_BaseGetAttrs();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCopy()"
ESMCI::Attributes* ESMC_AttributesCopy(ESMCI::Attributes *attrs, int &rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes(attrs->getStorageRef());
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreate()"
ESMCI::Attributes* ESMC_AttributesCreate(int& rc) {
  rc = ESMF_SUCCESS;
  return new ESMCI::Attributes();
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreateByKey()"
ESMCI::Attributes* ESMC_AttributesCreateByKey(ESMCI::Attributes *srcAttrs,
  char* key, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Attributes *attrs;
  try {
    std::string local_key(key);
    json new_storage = srcAttrs->get<json>(local_key, rc);
    attrs = new ESMCI::Attributes(std::move(new_storage));
  }
  ESMF_CATCH_ISOC;
  return attrs;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesCreateByParse()"
ESMCI::Attributes* ESMC_AttributesCreateByParse(char *payload, int& rc) {
  rc = ESMF_FAILURE;
  ESMCI::Attributes *attrs;
  try {
    std::string local_payload(payload);
    attrs = new ESMCI::Attributes(payload, rc);
  }
  ESMF_CATCH_ISOC;
  return attrs;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesDestroy()"
void ESMC_AttributesDestroy(ESMCI::Attributes* attrs, int& rc) {
  delete attrs;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesErase()"
void ESMC_AttributesErase(ESMCI::Attributes* attrs, char* keyParent,
                          char* keyChild, int& rc) {
  rc = ESMF_FAILURE;
  std::string localkeyParent(keyParent);
  std::string localkeyChild(keyChild);

  // This seems strange. This is the best method to delete from the Fortran
  // interface to avoid passing "" as the parent key when you want to delete
  // from the root. Otherwise a parent and child key are always required which
  // seems redundant.
  try {
    if (localkeyChild == "") {
      attrs->erase(localkeyChild, localkeyParent, rc);
    } else {
      attrs->erase(localkeyParent, localkeyChild, rc);
    }
  }
  ESMF_CATCH_ISOC;
}

#undef ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesInquire()"
void ESMC_AttributesInquire(ESMCI::Attributes *attrs, ESMCI::Attributes *inq,
                            char *key, bool recursive, int *idx, int &rc) {
  rc = ESMF_FAILURE;
  try {
    std::string localKey(key);
    json jinq = attrs->inquire(localKey, rc, recursive, idx);
    json &inqref = inq->getStorageRefWritable();
    inqref = std::move(jinq);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsEqual()"
void ESMC_AttributesIsEqual(ESMCI::Attributes *lhs, ESMCI::Attributes *rhs,
  bool &res, int &rc) {
  rc = ESMF_FAILURE;
  try {
    res = lhs->getStorageRef() == rhs->getStorageRef();
  }
  ESMF_CATCH_ISOC;
  rc = ESMF_SUCCESS;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsPresent()"
void ESMC_AttributesIsPresent(ESMCI::Attributes *attrs, char *key, bool &res, int &rc,
        int &isptr) {
  std::string local_key(key);
  try {
    res = attrs->hasKey(local_key, rc, isptr);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesIsSet()"
void ESMC_AttributesIsSet(ESMCI::Attributes *attrs, char *key, int &isSet, int &rc) {
  std::string local_key(key);
  try {
    isSet = attrs->isSetNull(local_key, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesPrint()"
void ESMC_AttributesPrint(ESMCI::Attributes *attrs, int &indent, int &rc) {
  rc = ESMF_FAILURE;
  try {
    std::cout << attrs->dump(indent, rc) << std::endl;
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesUpdate()"
void ESMC_AttributesUpdate(ESMCI::Attributes *lhs, ESMCI::Attributes *rhs, int &rc) {
  rc = ESMF_FAILURE;
  try {
    lhs->update(*rhs, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesBaseSyncDo"
void ESMC_AttributesBaseSyncDo(std::vector<long int> &base_addresses, int &rootPet, int &rc) {
  rc = ESMF_FAILURE;
  try {
    ESMCI::VM *vm = ESMCI::VM::getCurrent(&rc);
    ESMF_CHECKERR_STD("", rc, "Did not get current VM", rc);
    int localPet = vm->getLocalPet();
    json j = json::object();
    if (localPet == rootPet) {
      // For each base address, blind-cast to an attributes object and determine
      // if that object has been updated (dirty). If it has been updated, then
      // stick its serialized string representation inside a JSON map. For
      // broadcasting.
      for (std::size_t ii = 0; ii < base_addresses.size(); ++ii) {
        ESMCI::Attributes *attrs = baseAddressToAttributes(base_addresses[ii]);
        bool is_dirty = attrs->isDirty();
        if (is_dirty) {
          try {
            j[std::to_string(ii)] = attrs->getStorageRef().dump();
          }
          ESMF_CATCH_JSON;
        }
      }
    }
    // Broadcast the update map.
    ESMCI::Attributes battrs(std::move(j));
    broadcastAttributes(&battrs, rootPet, rc);
    // Update for each string key/index in the update map.
    const json &storage = battrs.getStorageRef();
    int ikey;
    for (json::const_iterator it=storage.cbegin(); it!=storage.cend(); it++) {
      ikey = std::stoi(it.key());  // Convert the string index to an integer
      ESMCI::Attributes *attrs_to_update = baseAddressToAttributes(base_addresses[ikey]);
      // This object is created from a serialized string stored in the update
      // map.
      ESMCI::Attributes rhs(it.value(), rc);
      try {
        if (localPet != rootPet) { attrs_to_update->update(rhs, rc); }
      }
      ESMF_CATCH_JSON;
    }
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesBaseSync()"
void ESMC_AttributesBaseSync(ESMCI::Attributes *inqstate, int &rootPet, int &rc) {
  rc = ESMF_FAILURE;
  try {
    const json &j_inqstate = inqstate->getStorageRef();
    int ctr = 0;
    updateDirtyInfo(j_inqstate, &ctr, nullptr);
    std::vector<long int> base_addresses(ctr, 0);
    updateDirtyInfo(j_inqstate, nullptr, &base_addresses);
    ESMC_AttributesBaseSyncDo(base_addresses, rootPet, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesBroadcast()"
void ESMC_AttributesBroadcast(ESMCI::Attributes *attrs, int &rootPet, int &rc) {
  rc = ESMF_FAILURE;
  try {
    broadcastAttributes(attrs, rootPet, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesReadJSON()"
void ESMC_AttributesReadJSON(ESMCI::Attributes *attrs, char *filename,
                             int &rc) {
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
    json &out = attrs->getStorageRefWritable();
    out = move(j);
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesWriteJSON()"
void ESMC_AttributesWriteJSON(ESMCI::Attributes *attrs, char *filename,
  int &rc) {
  rc = ESMF_FAILURE;
  std::string filename2(filename);
  try {
    std::ofstream file;
    file.open(filename2);
    if (!file.is_open()) {
      std::string errmsg = "Error opening output file: " + filename2;
      ESMF_CHECKERR_STD("ESMC_RC_FILE_OPEN", ESMC_RC_FILE_OPEN, errmsg, rc);
    }
    file << attrs->getStorageRef();
    file.close();
    rc = ESMF_SUCCESS;
  }
  ESMF_CATCH_ISOC
}

//-----------------------------------------------------------------------------

//#undef ESMC_METHOD
//#define ESMC_METHOD "ESMC_InfoGetAttPack()"
//void ESMC_InfoGetAttPack(ESMCI::Attributes *info, ESMCI::AttPack *attpack,
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
#define ESMC_METHOD "ESMC_AttributesGetCH()"
void ESMC_AttributesGetCH(ESMCI::Attributes* attrs, char *key, char *value,
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
    as_str = attrs->get<std::string>(local_key, rc, def_str_ptr, index);
  }
  ESMF_CATCH_ISOC;
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
#define ESMC_METHOD "ESMC_AttributesSetCH()"
void ESMC_AttributesSetCH(ESMCI::Attributes *attrs, char *key, char *value,
                              bool &force, int &rc, int *index) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  std::string localValue(value);
  try {
    attrs->set<std::string>(localKey, localValue, force, rc, index);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetATTRS()"
void ESMC_AttributesSetATTRS(ESMCI::Attributes *attrs, char *key,
  ESMCI::Attributes *value, bool &force, int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set(localKey, *value, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetNULL()"
void ESMC_AttributesSetNULL(ESMCI::Attributes *attrs, char *key, bool &force,
  int &rc) {
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set(localKey, force, rc);
  }
  ESMF_CATCH_ISOC;
}

#undef  ESMC_METHOD
#define ESMC_METHOD "ESMC_AttributesSetArrayCH()"
void ESMC_AttributesSetArrayCH(ESMCI::Attributes *attrs, char *key, int &count,
  bool &force, int &rc) {
  // Notes:
  //  * Only allocates storage. Does not actually insert anything!
  rc = ESMF_FAILURE;
  std::string localKey(key);
  try {
    attrs->set<std::vector<std::string>>(localKey, nullptr, count, force, rc);
  }
  ESMF_CATCH_ISOC;
}

}  // extern "C"
