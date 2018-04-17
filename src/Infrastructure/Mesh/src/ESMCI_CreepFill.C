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
//==============================================================================
#include <Mesh/include/ESMCI_Search.h>
#include <Mesh/include/ESMCI_MeshTypes.h>
#include <Mesh/include/ESMCI_MeshObjTopo.h>
#include <Mesh/include/ESMCI_Mapping.h>
#include <Mesh/include/ESMCI_MeshObj.h>
#include <Mesh/include/ESMCI_Mesh.h>
#include <Mesh/include/ESMCI_MeshUtils.h>
#include <Mesh/include/ESMCI_OTree.h>
#include <Mesh/include/ESMCI_Mask.h>
#include <Mesh/include/ESMCI_ParEnv.h>
#include <Mesh/include/ESMCI_MeshRegrid.h>
#include <Mesh/include/ESMCI_MathUtil.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <ostream>

#include <map>
#include <limits>
#include <vector>
using std::vector;
using std::map;



//-----------------------------------------------------------------------------
// leave the following line as-is; it will insert the cvs ident string
// into the object file for tracking purposes.
static const char *const version = "$Id$";
//-----------------------------------------------------------------------------

namespace ESMCI {

  bool creep_debug=false;

  struct CreepNode {    
    int level; 
    MeshObj *node;
    vector<CreepNode *> donors;

    void add_donor(CreepNode *cn) {

      // If the creep node is already in the list, then leave
      for (int i=0; i<donors.size(); i++) {
        if (cn->node->get_id() == donors[i]->node->get_id()) return;
      }

      // If not there, add to list
      donors.push_back(cn);
    } 

  };

  // Prototypes for local subroutines
  static void _convert_creep_levels_to_weights(int num_creep_levels, vector <CreepNode *> *creep_levels, WMat &wts);
  static void _write_level(const char *filename, Mesh &mesh, vector<CreepNode *> &level);
  static void _get_node_nbrs_in_elem(MeshObj *node, MeshObj *elem, MeshObj **nbr_node1, MeshObj **nbr_node2);

  /* XMRKX */
  // Creep unmasked points creep_level levels into masked points yielding wts. 
  // If set_dst_status is true, then also update dst_status.
  void CreepFill(Mesh &mesh, vector<int> &valid_gids, int num_creep_levels, WMat &wts, bool set_dst_status, WMat &dst_status) {
    
    printf("CreepFill creep_levels=%d num valid gids=%d\n",num_creep_levels,valid_gids.size());

    // Creep map 
    map<int,CreepNode> creep_map;

    // Levels
    vector<CreepNode *> *creep_levels;

    // Allocate a vector for each level
    creep_levels = new vector<CreepNode *>[num_creep_levels];

    //// Add initial set of valid points
    //// TODO: to save memory eventually just add the ones that are one away. 
    for (int i=0; i<valid_gids.size(); i++) {

      // get one gid
      int gid=valid_gids[i];

      // Get node
      Mesh::MeshObjIDMap::iterator mi =  mesh.map_find(MeshObj::NODE, gid);
      if (mi == mesh.map_end(MeshObj::NODE)) {
        Throw() << "No node with this gid found in the mesh.";
      }

      // Get node
      MeshObj *node=&*mi;

      // Make creep node
      CreepNode tmp_cn;
      tmp_cn.level=0;
      tmp_cn.node=node;

      // Add to map
      std::pair< map<int,CreepNode>::iterator,bool> ret;
      ret=creep_map.insert(std::pair<int,CreepNode>(gid,tmp_cn));
 
      // Add to level 0
      creep_levels[0].push_back(&(ret.first->second));
    }

    // Debug output
    _write_level("creep_0level",mesh, creep_levels[0]);

    /// Loop connecting one level to nodes in the last one
    for (int l=1; l<num_creep_levels; l++) {

      // Loop through last level
      for (int i=0; i<creep_levels[l-1].size(); i++) {

        // Get creep node
        CreepNode *creep_node=creep_levels[l-1][i];

        // Get node from last level
        MeshObj *node_ll=creep_node->node;

        // Loop through all the nodes connected to node
        // TODO: See if you can just loop through nodes around a node??
        MeshObjRelationList::const_iterator el = MeshObjConn::find_relation(*node_ll, MeshObj::ELEMENT);
        while (el != node_ll->Relations.end() && el->obj->get_type() == MeshObj::ELEMENT){
          MeshObj *elem=el->obj;

          // Get the nbrs of the node in the element
          MeshObj *nbr_node1, *nbr_node2;
          _get_node_nbrs_in_elem(node_ll, elem, &nbr_node1, &nbr_node2);

          // Process neighor 1

          // Get node gid
          int nbr_node1_gid=nbr_node1->get_id();
            
          // See if this node is in the map, then add or add too
          map<int,CreepNode>::iterator mi1 = creep_map.find(nbr_node1_gid);
          if (mi1 == creep_map.end()) {
            // Not in the map, so add a new one

            // Make new creep node
            CreepNode tmp_cn;
            tmp_cn.level=l;
            tmp_cn.node=nbr_node1;
            tmp_cn.add_donor(creep_node);

            // Add to map
            std::pair< map<int,CreepNode>::iterator,bool> ret;
            ret=creep_map.insert(std::pair<int,CreepNode>(nbr_node1_gid,tmp_cn));
 
            // Add to next level
            creep_levels[l].push_back(&(ret.first->second));

          } else {
            // In the map, so if 1 level away add to donors

            // Get pointer to creep node
            CreepNode *found_cn=&(mi1->second);

            // If found node at this level, then add to donors
            if (found_cn->level==l) {
              found_cn->add_donor(creep_node);
            }
          }


          // Process neighor 2

          // Get node gid
          int nbr_node2_gid=nbr_node2->get_id();
            
          // See if this node is in the map, then add or add too
          map<int,CreepNode>::iterator mi2 = creep_map.find(nbr_node2_gid);
          if (mi2 == creep_map.end()) {
            // Not in the map, so add a new one

            // Make new creep node
            CreepNode tmp_cn;
            tmp_cn.level=l;
            tmp_cn.node=nbr_node2;
            tmp_cn.add_donor(creep_node);

            // Add to map
            std::pair< map<int,CreepNode>::iterator,bool> ret;
            ret=creep_map.insert(std::pair<int,CreepNode>(nbr_node2_gid,tmp_cn));
 
            // Add to next level
            creep_levels[l].push_back(&(ret.first->second));

          } else {
            // In the map, so if 1 level away add to donors

            // Get pointer to creep node
            CreepNode *found_cn=&(mi2->second);

            // If found node at this level, then add to donors
            if (found_cn->level==l) {
              found_cn->add_donor(creep_node);
            }
          }

#if 0          
          MeshObjRelationList::const_iterator nl = MeshObjConn::find_relation(*elem, MeshObj::NODE);
          while (nl != elem->Relations.end() && nl->obj->get_type() == MeshObj::NODE){
            MeshObj *node = nl->obj;

            // Get new node gid
            int node_gid=node->get_id();
            
            // If this is the same node, then skip
            if (node_gid == node_ll->get_id()) {
              ++nl;
              continue;
            }

            // See if this node is in the map, then add or add too
            map<int,CreepNode>::iterator mi = creep_map.find(node_gid);
            if (mi == creep_map.end()) {
              // Not in the map, so add a new one

              // Make new creep node
              CreepNode tmp_cn;
              tmp_cn.level=l;
              tmp_cn.node=node;
              tmp_cn.add_donor(creep_node);

              // Add to map
              std::pair< map<int,CreepNode>::iterator,bool> ret;
              ret=creep_map.insert(std::pair<int,CreepNode>(node_gid,tmp_cn));
 
              // Add to next level
              creep_levels[l].push_back(&(ret.first->second));

            } else {
              // In the map, so if 1 level away add to donors

              // Get pointer to creep node
              CreepNode *found_cn=&(mi->second);

              // If found node at this level, then add to donors
              if (found_cn->level==l) {
                found_cn->add_donor(creep_node);
              }
            }
            
            // Next node in element
            ++nl;
          }
#endif
          
          // next element around node
          ++el;
        }
      }

      // Debug output level
      char new_filename[1000];
      sprintf(new_filename,"creep_%dlevel",l);
      _write_level(new_filename,mesh, creep_levels[l]);
    }

    // Construct weights from creep information
    _convert_creep_levels_to_weights(num_creep_levels, creep_levels, wts);

  }


// Construct weights from creep information
// TODO: do I need Mesh to be passed in??? 
static void _convert_creep_levels_to_weights(int num_creep_levels, vector <CreepNode *> *creep_levels, WMat &wts) {

  // Loop through creep levels
  for (int l=1; l<num_creep_levels; l++) {

    // Loop through last level
    for (int i=0; i<creep_levels[l].size(); i++) {

      // Get creep node
      CreepNode *cnode=creep_levels[l][i];      

      // DEBUG Look at one creep node and make sure that it's ok
      if (cnode->node->get_id() == 6967) {
        printf("creep node id=%d donor ids= ",cnode->node->get_id());
        for (int j=0; j<cnode->donors.size(); j++) {
          printf(" %d ",cnode->donors[j]->node->get_id());
        }
        printf("\n");


        //// SCRAP PARTS ////
#if 0
        // Loop through donors calculating weights
        for (int j=0; j<cnode->donors.size(); j++) {
          int gid=cnode->donors[j]->node->get_id();
        }        
      

        /////// PUT IN WEIGHTS /////
        // Set col info
        IWeights::Entry col(sr.src_gid, 0, 1.0, 0);

        // Set row info (i.e. the destination id associated with the above weight)
        IWeights::Entry row(sr.dst_gid, 0, 0.0, 0);


        // Put weights into weight matrix
        // Need merge version in nearest src to dest case where there may be more than 1 src,dst pair with the same dst.
        iw.InsertRowMergeSingle(row, col);
#endif


      }


    }
  }

}

  // Not really a math routine, but useful as a starting point for math routines
 static  void _get_node_nbrs_in_elem(MeshObj *node, MeshObj *elem, MeshObj **nbr_node1, MeshObj **nbr_node2) {

   // Get number of nodes in element
   const ESMCI::MeshObjTopo *topo = ESMCI::GetMeshObjTopo(*elem);

   // Find the node around elem, that matches node
   int node_ind=-1;
   for (int s = 0; s < topo->num_nodes; s++) {
     MeshObj *elem_node = elem->Relations[s].obj;

     // If this is the node, then get neighbors and leave
     if (elem_node->get_id() == node->get_id()) {
       node_ind=s;
       break;
     }
   }

   // If no matching node was found complain
   if (node_ind == -1) {
     Throw() << "node not found in element where it was expected.";
   }

   // Get neighbor before
   if (node_ind==0) {
     *nbr_node1 = elem->Relations[topo->num_nodes-1].obj;
   } else {
     *nbr_node1 = elem->Relations[node_ind-1].obj;
   }

   // Get neighbor after
   if (node_ind==topo->num_nodes-1) {
     *nbr_node2 = elem->Relations[0].obj;
   } else {
     *nbr_node2 = elem->Relations[node_ind+1].obj;
   }
 }

 static void _write_level(const char *filename, 
                           Mesh &mesh, vector<CreepNode *> &level) {

    // Coord Pointer
    MEField<> *coord_ptr = mesh.GetCoordField();

    ESMCI::PointList pl(level.size(), mesh.spatial_dim());
    for (int i=0; i<level.size(); i++) {
      
      // Get node
      MeshObj *node=level[i]->node;

      // Get pointer to coords of this node
      double *coords=coord_ptr->data(*node);

      // Add point
      pl.add(node->get_id(), coords);
    }

    // Write out
    pl.WriteVTK(filename);
  }

  } // namespace
