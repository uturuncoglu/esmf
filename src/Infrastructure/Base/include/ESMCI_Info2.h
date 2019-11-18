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
// ESMF Info C++ include file
//
//-----------------------------------------------------------------------------
//

#ifndef ESMCI_INFO_H
#define ESMCI_INFO_H

//-----------------------------------------------------------------------------

#include <vector>
#include <fstream>

#include "ESMCI_Util.h"
#include "json.hpp"

using json = nlohmann::json;  // Convenience rename for JSON namespace.

//tdk:LAST: remove all tdk stuff (search for tdk)

// Standard ESMF check error macros
#define ESMF_CHECKERR_STD(name_rc, actual_rc, msg, update_rc) {if (actual_rc != ESMF_SUCCESS) {ESMCI::esmf_info_error local_macro_error(name_rc, actual_rc, msg); if (ESMC_LogDefault.MsgFoundError(actual_rc, local_macro_error.what(), ESMC_CONTEXT, &update_rc)) throw(local_macro_error);}}

#define ESMF_THROW_JSON(json_exc, name_rc, actual_rc, update_rc) {ESMC_LogDefault.MsgFoundError(actual_rc, json_exc.what(), ESMC_CONTEXT, &update_rc); throw(ESMCI::esmf_info_error(name_rc, actual_rc, json_exc.what()));}

#define ESMF_HANDLE_PASSTHRU(exc_esmf) {ESMC_LogDefault.MsgFoundError(exc_esmf.getReturnCode(), exc_esmf.what(), ESMC_CONTEXT, nullptr); throw(exc_esmf);}

#define ESMF_CATCH_PASSTHRU catch (ESMCI::esmf_info_error &exc_esmf) {ESMF_HANDLE_PASSTHRU(exc_esmf)}

#define ESMF_CATCH_ISOC catch (ESMCI::esmf_info_error &exc_esmf) {ESMC_LogDefault.MsgFoundError(exc_esmf.getReturnCode(), exc_esmf.what(), ESMC_CONTEXT, nullptr); rc = exc_esmf.getReturnCode();} catch(...) {std::string msg;if (rc == ESMF_SUCCESS) {msg = "Unhandled throw and return code is ESMF_SUCCESS(?). Changing return code to ESMF_FAILURE";rc = ESMF_FAILURE;} else {msg = "Unhandled throw";}ESMC_LogDefault.MsgFoundError(rc, msg, ESMC_CONTEXT, nullptr);}
#define ESMF_CATCH_ISOCP catch (ESMCI::esmf_info_error &exc_esmf){ESMC_LogDefault.MsgFoundError(exc_esmf.getReturnCode(), exc_esmf.what(), ESMC_CONTEXT, rc);}catch(...) {std::string msg;if (localrc == ESMF_SUCCESS) {msg = "Unhandled throw and return code is ESMF_SUCCESS(?). Changing return code to ESMF_FAILURE";localrc = ESMF_FAILURE;} else {msg = "Unhandled throw";}ESMC_LogDefault.MsgFoundError(localrc, msg, ESMC_CONTEXT, rc);}

#define ESMF_CATCH_INFO catch (json::out_of_range &exc_json) {ESMF_THROW_JSON(exc_json, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);} catch (json::type_error &exc_json) {ESMF_THROW_JSON(exc_json, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);} catch (ESMCI::esmf_info_error &exc_esmf) {ESMF_HANDLE_PASSTHRU(exc_esmf);} catch (...) {ESMF_CHECKERR_STD("", ESMF_FAILURE, "Unhandled throw", rc);}

#define ESMF_CATCH_JSON catch (json::out_of_range &e) {ESMF_THROW_JSON(e, "ESMC_RC_NOT_FOUND", ESMC_RC_NOT_FOUND, rc);} catch (json::type_error &e) {ESMF_THROW_JSON(e, "ESMC_RC_ARG_BAD", ESMC_RC_ARG_BAD, rc);}

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  Info
//
// !DESCRIPTION:
// The code in this file implements the Info defined type
// and methods.
//
//-----------------------------------------------------------------------------
//
// !USES:

namespace ESMCI {

class VM;

typedef const std::string key_t;
typedef std::vector<json> const *vecjson_t;
typedef json::array_t const *arrjson_t;
typedef std::unordered_map<std::string, std::size_t> count_map_t;

enum ESMC_ISOCType {C_INT, C_LONG, C_FLOAT, C_DOUBLE, C_CHAR};

//-----------------------------------------------------------------------------

class esmf_info_error : public std::exception
{
public:
  esmf_info_error(key_t &code_name, int rc, key_t &msg);

  key_t getCodeName() {return this->code_name;}

  int getReturnCode() {return this->rc;}

  const char* what() const noexcept {return this->msg.c_str();}

private:
  std::string msg;
  int rc;
  key_t code_name;
};

//-----------------------------------------------------------------------------

void alignOffset(int &offset);
std::size_t get_attpack_count(const json &j);
json::iterator find_by_index(json &j, std::size_t index, bool recursive, bool attr_compliance, std::size_t *index_current = nullptr, bool *found = nullptr);
void update_json_pointer(const json &j, json const **jdp, const json::json_pointer &key, bool recursive);
count_map_t create_json_attribute_count_map(void);
void update_json_attribute_count_map(count_map_t &counts, const json &j, bool first);
bool isIn(key_t& target, const std::vector<std::string>& container);
bool isIn(const std::vector<std::string>& target, const std::vector<std::string>& container);
bool isIn(key_t& target, const json& j);
ESMC_TypeKind_Flag json_type_to_esmf_typekind(const json &j) noexcept;

//-----------------------------------------------------------------------------

class Info2 {

private:
  bool dirty = false;

protected:
  json storage;  // JSON object store for keys/values managed by this instance //tdk:todo: make this private

  virtual void init(void) {this->storage = json::object();}

public:
  Info2(void) {this->init();}
//  Info(void) = default;  // Default constructor
  virtual ~Info2(void) = default;  // Default destructor
  Info2(Info2&&) = delete; // Move constructor
  Info2(const Info2&) = delete; // Copy constructor
  Info2&operator=(const Info2&) = delete; // Copy assignment
  Info2&operator=(Info2&&) = delete; // Move assignment

  Info2(const json& storage); // Copy constructor from JSON
  Info2(json&& storage); // Move constructor from JSON
  Info2(key_t& input, int& rc); // Parse JSON string constructor

  std::string dump(int& rc) const;
  std::string dump(int indent, int& rc) const;

  void erase(key_t& key, key_t& keyChild, int& rc, bool recursive = false);

  static json::json_pointer formatKey(key_t &key, int &rc);

  //---------------------------------------------------------------------------
  template <typename T>
  T get(key_t &key, int &rc, const T *def = nullptr, const int *index = nullptr, bool recursive = false, std::string *ikey = nullptr) const;

  void get(ESMCI::Info2 &info, key_t &key, int &rc) const;
  //---------------------------------------------------------------------------

  std::size_t getCountPack(void) const {return get_attpack_count(this->getStorageRef());}

  template <typename T>
  std::vector<T> getvec(key_t &key, int &rc, bool recursive = false) const;

  //tdk:remove this interface
  void get_isoc(ESMCI::ESMC_ISOCType ictype, void *ret, char* key, int& rc,
    void* def = nullptr) const;

  virtual const json& getStorageRef(void) const { return this->storage; }
  virtual json& getStorageRefWritable(void) { return this->storage; }

  json const * getPointer(key_t &key, int &rc, bool recursive = false) const;

  bool hasKey(key_t &key, int &rc, bool isptr = false, bool recursive = false) const;
  bool hasKey(const json::json_pointer &jp, int& rc, bool recursive = false) const;

  json inquire(key_t& key, int& rc, bool recursive = false, const int *idx = nullptr,
    bool attr_compliance = false) const;

  bool isDirty() const {return this->dirty;}
  void setDirty(bool flag) {this->dirty = flag;}

  bool isSetNull(key_t &key, int &rc) const;

  void parse(key_t &input, int &rc);

  void deserialize(char *buffer, int *offset, int& rc);

  void serialize(char *buffer, int *length, int *offset,
    ESMC_InquireFlag inquireflag, int &rc);

  void set(key_t &key, json &&j, bool force, int &rc, const int *index = nullptr,
    const key_t * const pkey = nullptr);
  void set(key_t &key, const ESMCI::Info2 &info, bool force, int &rc,
    const key_t * const pkey = nullptr);
  void set(key_t &key, bool force, int &rc, const int *index = nullptr,
    const key_t * const pkey = nullptr);  // set null
  template <typename T>
  void set(key_t &key, T value, bool force, int &rc, const int *index = nullptr,
    const key_t * const pkey = nullptr);
  template <typename T>
  void set(key_t &key, T *values, int count, bool force, int &rc,
    const key_t * const pkey = nullptr);

  void update(const Info2 &info, int &rc);

  int ESMC_Print(bool tofile, const char *filename, bool append) const;
};

//-----------------------------------------------------------------------------

void broadcastInfo(ESMCI::Info2* info, int rootPet, const ESMCI::VM &vm, int &rc);

//-----------------------------------------------------------------------------

class InfoView {
private:
  json *storage = nullptr;
public:
  InfoView(void) = delete; // Default constructor
  ~InfoView(void) = default;  // Default destructor
  InfoView(InfoView&&) = delete; // Move constructor
  InfoView(const InfoView&) = delete; // Copy constructor
  InfoView&operator=(const InfoView&) = delete; // Copy assignment
  InfoView&operator=(InfoView&&) = delete; // Move assignment

  InfoView(json &j) {this->storage = &j;}

  void update_storage_ptr(json &j) {this->storage = &j;}
  void update_storage_ptr(const json::json_pointer *key, const int *idx, bool recursive);

  template<typename T>
  T copy(void);

  template<typename T>
  void copy(T target, const int &count);

  void update_ptr(bool **ptr);
  void update_ptr(long int **ptr);
  void update_ptr(double **ptr);
  void update_ptr(std::string **ptr);
  void update_ptr(json::array_t **ptr);

  json &get_ivref(void) {return *this->storage;}
};

//-----------------------------------------------------------------------------

class PackageFactory
// tdk: FEATURE: add rule of five
{
private:
  json cache = json::object();
  json uris = json::object();

public:
  PackageFactory(void) = default;  // Default constructor
  ~PackageFactory(void) = default; // Default destructor

  json getOrCreateJSON(key_t& key, int& rc, key_t& uri = "");
};

} // namespace

#endif  // ifdef barrier
