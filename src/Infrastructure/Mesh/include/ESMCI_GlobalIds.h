#ifndef ESMCI_GlobalIds_h
#define ESMCI_GlobalIds_h

#include <vector>

namespace ESMCI {

// Retrieve a list of new global ids, given the set of current ids
void GlobalIds(const std::vector<long> &current_ids,
                          std::vector<long> &new_ids);


} // namespace

#endif
