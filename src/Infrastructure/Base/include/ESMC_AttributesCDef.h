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
// ESMF Attributes C-Fortran Definition include file
//
//-----------------------------------------------------------------------------
//

#ifndef ESMC_ATTRIBUTESCDEF_H
#define ESMC_ATTRIBUTESCDEF_H

// Fortran interface functions
extern "C" {

ESMCI::Attributes* ESMC_AttributesCreate(int& rc);
void ESMC_AttributesDestroy(ESMCI::Attributes* attrs, int& rc);
void ESMC_AttributesErase(ESMCI::Attributes* attrs, char* keyParent,
                          char* keyChild, int& rc);

//-----------------------------------------------------------------------------

void ESMC_AttributesGet_C_INT(ESMCI::Attributes* attrs, char* key, int &value, int& rc, int* def);
long int ESMC_AttributesGet_C_LONG(ESMCI::Attributes* attrs, char* key, int& rc, long int* def);
float ESMC_AttributesGet_C_FLOAT(ESMCI::Attributes* attrs, char* key, int& rc, float* def);
double ESMC_AttributesGet_C_DOUBLE(ESMCI::Attributes* attrs, char* key, int& rc, double* def);

//-----------------------------------------------------------------------------

void ESMC_AttributesGetArray_C_INT(ESMCI::Attributes* attrs, char* key,
                                   int* values, int& count, int& count_only, int& rc);
void ESMC_AttributesGetArray_C_LONG(ESMCI::Attributes* attrs, char* key,
                                    long int* values, int& count, int& count_only, int& rc);
void ESMC_AttributesGetArrayC_FLOAT(ESMCI::Attributes* attrs, char* key,
                                    float* values, int& count, int& count_only, int& rc);
void ESMC_AttributesGetArray_C_DOUBLE(ESMCI::Attributes* attrs, char* key,
                                      double* values, int& count, int& count_only, int& rc);

//-----------------------------------------------------------------------------

int ESMC_AttributesIsPresent(ESMCI::Attributes* attrs, char* key, int& rc,
                             int& isptr);
void ESMC_AttributesPrint(ESMCI::Attributes* attrs, int& indent, int& rc);

//-----------------------------------------------------------------------------

void ESMC_AttributesSet_C_INT(ESMCI::Attributes* attrs, char* key, int& value,
                              int& force, int& rc);
void ESMC_AttributesSet_C_LONG(ESMCI::Attributes* attrs, char* key, long int& value,
                               int& force, int& rc);
void ESMC_AttributesSet_C_FLOAT(ESMCI::Attributes* attrs, char* key, float& value,
                                int& force, int& rc);
void ESMC_AttributesSet_C_DOUBLE(ESMCI::Attributes* attrs, char* key, double& value,
                                 int& force, int& rc);

//-----------------------------------------------------------------------------

void ESMC_AttributesSetArray_C_INT(ESMCI::Attributes* attrs, char* key,
                                   int* value, int& n, int& force, int& rc);
void ESMC_AttributesSetArray_C_LONG(ESMCI::Attributes* attrs, char* key,
                                    long int* value, int& n, int& force, int& rc);
void ESMC_AttributesSetArray_C_FLOAT(ESMCI::Attributes* attrs, char* key,
                                     float* value, int& n, int& force, int& rc);
void ESMC_AttributesSetArray_C_DOUBLE(ESMCI::Attributes* attrs, char* key,
                                      double* value, int& n, int& force, int& rc);

}  // extern "C"

#endif //ESMC_ATTRIBUTESCDEF_H
