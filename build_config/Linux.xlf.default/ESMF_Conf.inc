#if 0
$Id$

Earth System Modeling Framework
Copyright (c) 2002-2024, University Corporation for Atmospheric Research,
Massachusetts Institute of Technology, Geophysical Fluid Dynamics
Laboratory, University of Michigan, National Centers for Environmental
Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
NASA Goddard Space Flight Center.
Licensed under the University of Illinois-NCSA License.
#endif

#if 0
Define the proper way to flush Fortran units.  Typically this will
be one of the following:

  * Fortran-2003 FLUSH statement (preferred and default)
  * Two argument FLUSH call
  * One argument FLUSH call

  Use F2003 FLUSH statement from 9.0-onwards IBM.  If an older
  compiler is being used, enable the following macro.
#endif

#if 0
#define ESMF_IOFlushMacro(unit,status) call flush_ (unit)
#endif
