!
! Earth System Modeling Framework
! Copyright (c) 2002-2024, University Corporation for Atmospheric Research,
! Massachusetts Institute of Technology, Geophysical Fluid Dynamics
! Laboratory, University of Michigan, National Centers for Environmental
! Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
! NASA Goddard Space Flight Center.
! Licensed under the University of Illinois-NCSA License.
!
!==============================================================================
!
      program ESMF_ConfigOverviewEx

!-------------------------------------------------------------------------
!ESMF_EXAMPLE        String used by test script to count examples.
!==============================================================================
!
! !DESCRIPTION:
!BOE
! This example/test code performs simple Config/Resource File routines. It does not
! include attaching a Config to a component. The important thing to remember there
! is that you can have one Config per component. 
!
! There are two methodologies for accessing data in a Resource File.  This example will
! demonstrate both.

! Note the API section contains a complete description of arguments in
! the methods/functions demonstrated in this example.
!EOE 
#include "ESMF.h"

      ! ESMF Framework module
      use ESMF
      use ESMF_TestMod
      implicit none

      ! Local variables
      integer             :: i, j, result
      type(ESMF_VM)       :: vm

      character(ESMF_MAXSTR) :: testname
      character(ESMF_MAXSTR) :: failMsg

!BOE
!\subsubsection{Variable declarations}

! The following are the variable declarations used as arguments in the following code 
! fragments. They represent the locals names for the variables listed in the Resource 
! File (RF).  Note they do not need to be the same.
!EOE

!BOC
      character(ESMF_MAXPATHLEN) :: fname ! config file name
      character(ESMF_MAXPATHLEN) :: fn1, fn2, fn3, input_file ! strings to be read in
      integer       :: rc            ! error return code (0 is OK)
      integer       :: i_n           ! the first constant in the RF
      real          :: param_1       ! the second constant in the RF
      real          :: radius        ! radius of the earth
      real          :: table(7,3)    ! an array to hold the table in the RF

      type(ESMF_Config)   :: cf      ! the Config itself
      type(ESMF_HConfig)  :: hconfig ! HConfig variable
!EOC

!--------------------------------------------------------
! Set up the status messages and initialize the program
!--------------------------------------------------------
      integer :: finalrc
      finalrc = ESMF_SUCCESS                      ! Establish initial success

!-------------------------------------------------------------------------
!-------------------------------------------------------------------------

      write(failMsg, *) "Example failure"
      write(testname, *) "Example ESMF_ConfigOverviewEx"

!-------------------------------------------------------------------------
!-------------------------------------------------------------------------


  
      call ESMF_Initialize(defaultlogfilename="ConfigOverviewEx.Log", &
                    logkindflag=ESMF_LOGKIND_MULTI, rc=rc)                 ! Initialize

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_Initialize' failed"
      endif
  
      call ESMF_VMGetGlobal(vm, rc=rc)               ! Establish the VM

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call to ESMF_VMGetGlobal' failed"
      endif

!-------------------------------------------------------------------------
!  Begin the Example
!------------------------------------------------------------------------

!BOE
!\subsubsection{Creation of a Config}

! While there are two methodologies for accessing the data within a Resource File, 
! there is only one way to create the initial Config and load its ASCII text into 
! memory. This is the first step in the process.

! Note that subsequent calls to {\tt ESMF\_ConfigLoadFile} will OVERWRITE the current
! Config NOT append to it. There is no means of appending to a Config. 
!
!EOE

!BOC
      cf = ESMF_ConfigCreate(rc=rc)             ! Create the empty Config
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigCreate' failed"
      endif

!BOC
      fname = "myResourceFile.rc"                ! Name the Resource File
      call ESMF_ConfigLoadFile(cf, fname, rc=rc) ! Load the Resource File 
                                                 ! into the empty Config
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigLoadFile' failed"
      endif

!----------------------------------------------------------------
! First Method of Retrieval
!----------------------------------------------------------------
!BOE
!\subsubsection{How to retrieve a label with a single value}
! The first method for retrieving information from the 
! Resource File takes advantage of the <label,value> relationship
! within the file and access the data in a dictionary-like manner. This is the
! simplest methodology, but it does imply the use of only one value per label
! in the Resource File.  
! 
! Remember,
! that the order in which a particular label/value pair is retrieved
! is not dependent upon the order which they exist within the Resource File. 
!EOE

!BOC 
    call ESMF_ConfigGetAttribute(cf, radius, label='radius_of_the_earth:', &
                                 default=1.0, rc=rc)
!EOC


    if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute' for radius failed"
    else
        print*, "---------------------------------------------------------------"
        print*, "Results from using the dictionary-like method of data retrieval"
        print*, "The radius of the earth was retrieved from the Resource File"
        print*, "Its value is: ", radius
        print*, "---------------------------------------------------------------"
    endif

    if(radius .ne. 6.37E6)then
      finalrc = ESMF_FAILURE
      print*, "******** Radius not retrieved correctly"
      print*, trim (input_file)
    endif


!BOE
! Note that the colon must be included in the label string when using this
! methodology.  It is also important to provide a default value in case the label
! does not exist in the file
!EOE


!BOE
! This methodology works for all types. The following is an example of retrieving a 
! string:
!EOE

!BOC 
    call ESMF_ConfigGetAttribute(cf, input_file, label='input_file_name:', &
                                 default="./default.nc", rc=rc)
!EOC


    if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute'  for input_file failed"
    else
        print*, "Results from using the dictionary-like method of data retrieval of a string"
        print*, "The input file name (a string) was retrieved from the Resource File"
        print*, "Its value is: ", trim (input_file)
        print*, "---------------------------------------------------------------"
    endif

    if(input_file .ne. "dummy_input.nc")then
       finalrc = ESMF_FAILURE
       print*, "******* input_file not retrieved correctly"
       print*, trim (input_file)
    endif

!BOE
! The same code fragment can be used to demonstrate what happens when the label is not 
! present.  Note that "file\_name" does not exist in the Resource File. The result of 
! its absence is the default value provided in the call.
!EOE

!BOC 
    call ESMF_ConfigGetAttribute(cf, input_file, label='file_name:', &
                                 default="./default.nc", rc=rc)
!EOC

    if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute'  for input_file failed"
    else
        print*, "Results when the label in the call does not exist in the Resource File"
        print*, "The default value is returned."
        print*, "Its value is: ", trim (input_file)
        print*, "---------------------------------------------------------------"
    endif

    if (input_file .ne. "./default.nc") then
      finalrc = ESMF_FAILURE
      print*, "****** Demonstration of default value (input_file) not working correctly"
      print*, trim (input_file)
    endif
!----------------------------------------------------------------
! Second Method of Retrieval
!----------------------------------------------------------------
!BOE
!\subsubsection{How to retrieve a label with multiple values}
! When there are multiple, mixed-typed values associated with a label, the 
! values can be retrieved in two steps:  1) Use ESMF\_ConfigFindLabel() 
! to find the label in the Config class; 2) use
! ESMF\_ConfigGetAttribute() without the optional 'label' argument to 
! retrieve the values one at a time, reading from left to right in
! the record. 
!
! A second reminder that the order in which a particular label/value pair is 
! retrieved is not dependent upon the order which they exist within the 
! Resource File. The label used in this method allows the user to skip to
! any point in the file. 
!EOE

!BOC
      call ESMF_ConfigFindLabel(cf, 'constants:', rc=rc) ! Step a) Find the 
                                                         ! label 
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigFindLabel' failed"
      endif 

!BOE
! Two constants, radius and i\_n, can now be retrieved without having to specify their
! label or use an array. They are also different types.
!EOE

!BOC
      call ESMF_ConfigGetAttribute(cf, param_1, rc=rc) ! Step b) read in the 
                                                       ! first constant in 
                                                       ! the sequence
      call ESMF_ConfigGetAttribute(cf, i_n, rc=rc)     ! Step c) read in the 
                                                       ! second constant in 
                                                       ! the sequence
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute' failed"
      else
        print*, "Results from retrieving multiple values:"
        print*, "The first constant was was retrieved from the Resource File"
        print*, "Its value is: ", param_1
        print*, "The second constant was was retrieved from the Resource File"
        print*, "Its value is: ", i_n
        print*, "---------------------------------------------------------------"
      endif

      if (param_1 .ne. 3.1415 .or. i_n .ne. 25) then
        finalrc = ESMF_FAILURE
        print*, "*****The constants param_1 and i_n did not retrieve correctly from the file"
      endif

!BOE
! This methodology also works with strings.
!EOE

!BOC
       call ESMF_ConfigFindLabel(cf, 'my_file_names:', &
               rc=rc)                       ! Step a) find the label
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigFindLabel' failed"
      endif

!BOC
       call ESMF_ConfigGetAttribute(cf, fn1, &
                 rc=rc)                    ! Step b) retrieve the 1st filename
       call ESMF_ConfigGetAttribute(cf, fn2, &
                 rc=rc)                    ! Step c) retrieve the 2nd filename
       call ESMF_ConfigGetAttribute(cf, fn3, &
                 rc=rc)                    ! Step d) retrieve the 3rd filename
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute' for multiple file name retrieval failed"
      else
        print*, "Results from retrieving multi-value strings:"
        print*, "The first file name was was retrieved from the Resource File"
        print*, "Its value is: ", trim (fn1)
        print*, "The second file name was was retrieved from the Resource File"
        print*, "Its value is: ", trim (fn2)
        print*, "The third file name was was retrieved from the Resource File"
        print*, "Its value is: ", trim (fn3)
        print*, "---------------------------------------------------------------"
      endif

      if (fn1.ne.'jan87.dat' .or. fn2.ne.'jan88.dat' .or. fn3.ne.'jan89.dat') then
        finalrc = ESMF_FAILURE
        print*, "*****The file names not retrieved correctly using the multi-value method"
      endif

!----------------------------------------------------------------
! Retrieval of a Table
!----------------------------------------------------------------
!BOE
!\subsubsection{How to retrieve a table}

! To access tabular data, the user must use the multi-value method. 
!EOE

!BOC
      call ESMF_ConfigFindLabel(cf, 'my_table_name::', &
               rc=rc)        ! Step a) Set the label location to the 
                             ! beginning of the table
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigFindLabel' failed"
      endif

!BOE
! Subsequently, {\tt call ESMF\_ConfigNextLine()} is used to move the location 
! to the next row of the table. The example table in the Resource File contains
! 7 rows and 3 columns (7,3).
!EOE

!BOC
      do i = 1, 7
        call ESMF_ConfigNextLine(cf, rc=rc) ! Step b) Increment the rows
        do j = 1, 3                         ! Step c) Fill in the table 
          call ESMF_ConfigGetAttribute(cf, table(i,j), rc=rc)
        enddo
      enddo
!EOC


      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigGetAttribute' for table retrieval failed"
      else
        print*, "Results from retrieving a table"
        print*, "The (1,3) table value should be 263.0"
        print*, "Its retrieved value is: ", table(1,3)
        print*, "The (6,1) table value should be 400"
        print*, "Its retrieved value is: ", table(6,1)
        print*, "---------------------------------------------------------------"
      endif

      if(table(1,3) .ne. 263.0 .or. table(6,1) .ne. 400)then
        finalrc = ESMF_FAILURE
        print*, "***** Tables values are incorrect"
      endif

!----------------------------------------------------------------
! Destruction of a Config
!----------------------------------------------------------------
!BOE
!\subsubsection{Destruction of a Config}

! The work with the Config object {\tt cf} is finalized by callling
! {\tt ESMF\_ConfigDestroy()}.
!EOE

!BOC
      call ESMF_ConfigDestroy(cf, rc=rc) ! Destroy the Config object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_ConfigDestroy' failed"
      end if

!----------------------------------------------------------------
! YAML File
!----------------------------------------------------------------
!BOE
!\subsubsection{Loading a YAML file}

! The Config class supports loading of YAML files. As before, an empty Config
! object is created with {\tt ESMF\_ConfigCreate()} and then populated via the
! {\tt ESMF\_ConfigLoadFile()} method.
!EOE

!BOC
      cf = ESMF_ConfigCreate(rc=rc)          ! Create the empty Config object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigCreate' failed"
      endif
!BOE
! Files ending in {\tt .yaml}, {\tt .yml}, or any combination of upper and lower
! case letters that can be mapped to these two options, are interpreted as YAML
! files. All other names are interpreted as classic Config {\em RFs} as
! documented earlier.
!EOE

!BOC
      call ESMF_ConfigLoadFile(cf, "myResourceFile.yaml", & ! Load the YAML File
        rc=rc)                                    ! into the empty Config object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigLoadFile' failed"
      endif
!BOE
! Here the {\tt myResourceFile.yaml} contains a YAML version of the previously
! used {\tt myResourceFile.rc} file contents:
!
! \begin{verbatim}
!# YAML representation of the myResourceFile.rc RF
!
!# mapping to sequences
!
!my_file_names:      [jan87.dat, jan88.dat, jan89.dat]  # all strings
!constants:          [3.1415, 25]                       # float and integer
!my_favorite_colors: [green, blue, 022]
!
!# mapping to scalars
!
!radius_of_the_earth:   6.37E6
!parameter_1:           89
!parameter_2:           78.2
!input_file_name:       dummy_input.nc
!
!# represent table as mapping to sequence of sequences
!
!my_table_name:
!- [1000,    3000,    263.0]
!- [ 925,    3000,    263.0]
!- [ 850,    3000,    263.0]
!- [ 700,    3000,    269.0]
!- [ 500,    3000,    287.0]
!- [ 400,    3000,    295.8]
!- [ 300,    3000,    295.8]
! \end{verbatim}
!
! Notice that YAML support is limited to a small subset of the full YAML
! language specification, allowing access through the classic Config API.
! Specifically, the top level in the YAML file is expected to be a mapping
! (dictionary) of scalar keys to any of the following three value options:
!  \begin{itemize}
!  \item Scalars
!  \item List of scalars
!  \item List of lists of scalars
!  \end{itemize}
! All other YAML constructs are silently ignored when loaded through this
! interface. Constructs successfully ingested become available in the
! {\tt cf} object, and can be accessed via the regular {\tt ESMF\_Config}
! methods as outlined in the previous sections.
!
! As an example, access the {\tt my\_table\_name} element:
!EOE
!BOC
      call ESMF_ConfigFindLabel(cf, 'my_table_name::', &
               rc=rc)        ! Step a) Set the label location to the 
                             ! beginning of the table
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigFindLabel' failed"
      endif

!BOE
! When done, the resources held by the Config object are released by calling
! the {\tt ESMF\_ConfigDestroy()} method.
!EOE

!BOC
      call ESMF_ConfigDestroy(cf, rc=rc) ! Destroy the Config object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_ConfigDestroy' failed"
      end if

!----------------------------------------------------------------
! YAML HConfig
!----------------------------------------------------------------
!BOE
!\subsubsection{Creating from HConfig object}

! The Config class supports creating a Config object from a HConfig object.
! Here the HConfig object is created from the same YAML file as used before.
!EOE

!BOC
      ! Create HConfig object
      hconfig = ESMF_HConfigCreate(filename="myResourceFile.yaml", rc=rc)
!EOC
      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_HConfigCreate' failed"
      endif
!BOE
! The {\tt myResourceFile.yaml} contains the following YAML contents:
!
! \begin{verbatim}
!# YAML representation of the myResourceFile.rc RF
!
!# mapping to sequences
!
!my_file_names:      [jan87.dat, jan88.dat, jan89.dat]  # all strings
!constants:          [3.1415, 25]                       # float and integer
!my_favorite_colors: [green, blue, 022]
!
!# mapping to scalars
!
!radius_of_the_earth:   6.37E6
!parameter_1:           89
!parameter_2:           78.2
!input_file_name:       dummy_input.nc
!
!# represent table as mapping to sequence of sequences
!
!my_table_name:
!- [1000,    3000,    263.0]
!- [ 925,    3000,    263.0]
!- [ 850,    3000,    263.0]
!- [ 700,    3000,    269.0]
!- [ 500,    3000,    287.0]
!- [ 400,    3000,    295.8]
!- [ 300,    3000,    295.8]
! \end{verbatim}
!
! A Config object can be created from the HConfig object simply by passing
! {\tt hconfig} into {\tt ESMF\_CreateConfig()} as argument.
!EOE

!BOC
      ! Create Config object from HConfig
      cf = ESMF_ConfigCreate(hconfig=hconfig, rc=rc)
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_HConfigCreate' failed"
      end if

!BOE
! Notice that {\tt cf} uses the specified {\tt hconfig} object via reference.
! It remains the callers responsibility to destroy the {\tt hconfig} object
! when finished. Care must be taken to {\em not} destroy until access via
! {\tt cf} is complete.
!
! Here, as an example, access the {\tt my\_table\_name} element:
!EOE
!BOC
      call ESMF_ConfigFindLabel(cf, 'my_table_name::', &
               rc=rc)        ! Step a) Set the label location to the 
                             ! beginning of the table
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****' call ESMF_ConfigFindLabel' failed"
      endif
!EOE

! When done, the resources held by the Config object are released by calling
! the {\tt ESMF\_ConfigDestroy()} method.
!EOE

!BOC
      call ESMF_ConfigDestroy(cf, rc=rc) ! Destroy the Config object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_ConfigDestroy' failed"
      end if

!BOE
! As discussed above, the {\tt hconfig} object requires its own destroy call
! for a complete release.
!EOE

!BOC
      call ESMF_HConfigDestroy(hconfig, rc=rc) ! Destroy the HConfig object
!EOC

      if (rc .ne. ESMF_SUCCESS) then
        finalrc = ESMF_FAILURE
        print*, "*****'call ESMF_HConfigDestroy' failed"
      end if


!----------------------------------------------------------------
!----------------------------------------------------------------

      print*, "Final Word"
      print*,"---------------------------------------------------"
      if (finalrc.eq.ESMF_SUCCESS) then
        print *, "PASS: ESMF_ConfigOverviewEx.F90"
      else
        print *, "FAIL: ESMF_ConfigOverviewEx.F90"
      end if
 
      ! IMPORTANT: ESMF_STest() prints the PASS string and the # of processors in the log
      ! file that the scripts grep for.
      call ESMF_STest((finalrc.eq.ESMF_SUCCESS), testname, failMsg, result, ESMF_SRCLINE)


      call ESMF_Finalize(rc=rc)

      end program ESMF_ConfigOverviewEx
