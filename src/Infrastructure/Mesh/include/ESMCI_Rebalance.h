#ifndef ESMCI_Rebalance_h
#define ESMCI_Rebalance_h

#include <Mesh/include/ESMCI_MEField.h>

namespace ESMCI {

class Mesh;
class CommReg;

// Re load balance the mesh.
// We only provide rebalance at the granularity of the Genesis mesh.
// Returns false if no rebalancing is needed.
//
// Mesh must be in a consistent state before this is called
// If bfield is present, will use the weights in this field.
// Field should be a single entry element field.
bool Rebalance(Mesh &mesh, MEField<> *bfield=0);
bool Rebalance(Mesh &mesh, CommReg &comm);
} // namespace

#endif
