// $Id$
//
// Earth System Modeling Framework
// Copyright (c) 2002-2024, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//
//==============================================================================
#include <Mesh/include/Legacy/ESMCI_MEImprint.h>
#include <Mesh/include/Legacy/ESMCI_MeshObjConn.h>
#include <Mesh/include/Legacy/ESMCI_MeshUtils.h>
#include <Mesh/include/Legacy/ESMCI_MasterElement.h>

#include <map>
#include <cstdio>

//-----------------------------------------------------------------------------
// leave the following line as-is; it will insert the cvs ident string
// into the object file for tracking purposes.
static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

void MEImprintValSets(const std::string &imname, MeshObj &obj, const MasterElementBase &me,
               std::vector<UInt> &nvalSet, std::vector<UInt> &valSetObj) {
  MeshDB &mesh = GetMeshObjMesh(obj);


  for (UInt i = 0; i < me.num_functions(); i++) {
    const int *dd = me.GetDofDescription(i);
    UInt nval = me.GetDofValSet(i);
    std::vector<UInt>::iterator nsi = std::lower_bound(nvalSet.begin(), nvalSet.end(), nval);
    if (nsi == nvalSet.end() || *nsi != nval) {
      nsi = nvalSet.insert(nsi, nval);
    }
    if (nval >= valSetObj.size()) valSetObj.resize(nval+1, 0);
    valSetObj[nval] |= dof2mtype(dd[0]);

 }

}

void MEImprint(const std::string &imname, MeshObj &obj, const MasterElementBase &me) {
  MeshDB &mesh = GetMeshObjMesh(obj);


  for (UInt i = 0; i < me.num_functions(); i++) {
    const int *dd = me.GetDofDescription(i);
    UInt nval = me.GetDofValSet(i);
    char buf[1024];
    std::sprintf(buf, "%s_%d", imname.c_str(), nval);
//std::cout << "def ctxt:" << buf << std::endl;
    UInt ctxt_id = mesh.GetContext(buf);

    // Get the object.  If element, go right to it, else if side, go through parent
    MeshObjRelationList::iterator ri;

    // if doftype is same as objtype, just present self
    MeshObj *dobj = NULL;
    if (obj.get_type() == dof2mtype(dd[0])) {
      // should be ordinal 0, just check
      if (dd[1] != 0) Throw() << "object type same as imprint type, expected ordinal 0";
      dobj = &obj;
    } else if (obj.get_type() == MeshObj::ELEMENT) {

      ri =
         MeshObjConn::find_relation(obj, dof2mtype(dd[0]), dd[1]);

      dobj=ri->obj;

    if (ri == obj.Relations.end())
      Throw() << "MEImprint, could not find obj (" << dd[0] << ", " << dd[1] << ") for:" << obj;

    } else if (obj.get_type() == (UInt) mesh.side_type()) {
      ri = MeshObjConn::find_relation(obj, MeshObj::ELEMENT);

      if (ri == obj.Relations.end())
        Throw() << "MEImprint, could not find side parent";
 
      MeshObj &elem = *ri->obj; UInt side_ord = ri->ordinal;
      const MeshObjTopo *etopo = GetMeshObjTopo(elem);
      ri = MeshObjConn::find_relation(elem, dof2mtype(dd[0]), etopo->get_side_nodes(side_ord)[dd[1]]);
      if (ri == elem.Relations.end())
        Throw() << "MEImprint, could not find side node from parent";

      dobj=ri->obj;
    
    } else
      Throw() << "Imprint not supported for type:" << MeshObjTypeString(obj.get_type());
    // Add ctxt_id to object.
    MeshObj &iobj = *dobj;
    const Attr &oattr = GetAttr(iobj);
    const Context &ctxt = GetMeshObjContext(iobj);
    Context newctxt(ctxt);
    newctxt.set(ctxt_id);
    if (newctxt != ctxt) {
      Attr attr(oattr, newctxt);
      mesh.update_obj(&iobj, attr);
    }
  } // for i

} 

} // namespace
