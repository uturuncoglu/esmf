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
// ESMF Attributes C++ include file
//
//-----------------------------------------------------------------------------
//

#ifndef ESMCI_ATTRIBUTES_H
#define ESMCI_ATTRIBUTES_H

//-----------------------------------------------------------------------------

#include <vector>

#include "ESMCI_Util.h"
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.
using std::string;

// Standard ESMF check error macros
#define ESMF_CHECKERR_STD(name_rc, actual_rc, msg, update_rc) {\
  esmf_attrs_error local_macro_error(name_rc, actual_rc, msg);\
  if (ESMC_LogDefault.MsgFoundError(actual_rc, local_macro_error.what(), \
      ESMC_CONTEXT, &update_rc)) throw(local_macro_error);}\

#define ESMF_THROW_JSON(json_exc, name_rc, actual_rc, update_rc) {\
  ESMC_LogDefault.MsgFoundError(actual_rc, json_exc.what(), ESMC_CONTEXT,\
    &update_rc); throw(esmf_attrs_error(name_rc, actual_rc, json_exc.what()));}\

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  Attributes
//
// !DESCRIPTION:
// The code in this file implements the Attributes defined type
// and methods.
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

//------------------------------------------------------------------------------

class esmf_attrs_error : public std::exception
{
public:
  esmf_attrs_error(const string &code_name, int rc, const string &msg) {
    string the_msg;
    if (code_name != "") {
      the_msg = "Error/Return Code " + std::to_string(rc) + " (" + \
                     code_name + ") - " + msg;
    } else {
      the_msg = "Error/Return Code " + std::to_string(rc) + " - " + msg;
    }
    this->msg = the_msg;
    this->rc = rc;
  }

  int getReturnCode() {return this->rc;}

  const char* what() const noexcept {
    return this->msg.c_str();
  }
private:
  string msg;
  int rc;
};

//------------------------------------------------------------------------------

class Attributes
{
 private:
    json storage;  // JSON object store for keys/values managed by this instance

    // Prevent accidental copying
    Attributes(const Attributes&);

    static json::json_pointer formatKey(const string &key, int &rc);

 public:
    Attributes(void);
    ~Attributes(void);
    Attributes(const json &storage);
    Attributes(json &&storage);
    Attributes(const string &input, int &rc);

    string dump(int &rc) const;
    string dump(int indent, int &rc) const;

    void erase(const string &key, const string &keyChild, int &rc);

    const json& getStorageRef(void) const;

    bool hasKey(const string &key, int &rc) const;

    template <typename T, typename JT>
    T get(const string &key, int &rc) const;

    template <typename T>
    void set(const string &key, T value, bool force, int &rc);

    void update(const Attributes &attrs, int &rc);

};

//------------------------------------------------------------------------------

json createJSONPackage(const string &pkgKey, int &rc);

//------------------------------------------------------------------------------

} // namespace

// Fortran interface functions
extern "C" {
}

#endif  // ifdef barrier
