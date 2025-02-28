! $Id$
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
#define FILENAME "src/addon/NUOPC/src/NUOPC_FieldDictionaryApi.F90"
!==============================================================================

module NUOPC_FieldDictionaryApi

  use ESMF
  use NUOPC_FieldDictionaryDef
  use NUOPC_FreeFormatDef

  implicit none

  private

  ! public types
  public NUOPC_FieldDictionaryEntryS, NUOPC_FieldDictionaryEntry

  ! public module variables
  logical, save :: NUOPC_FieldDictionaryIsSetup = .false.
  public NUOPC_FieldDictionaryIsSetup
  logical, save :: NUOPC_FieldDictionaryAutoAdd = .false.  
  public NUOPC_FieldDictionaryAutoAdd
  type(ESMF_Container), save  :: NUOPC_FieldDictionary
  public NUOPC_FieldDictionary
  
  ! public module interface for the NUOPC API
  public NUOPC_FieldDictionaryAddEntry
  public NUOPC_FieldDictionaryEgest
  public NUOPC_FieldDictionaryGetEntry
  public NUOPC_FieldDictionaryHasEntry
  public NUOPC_FieldDictionaryIngest
  public NUOPC_FieldDictionaryMatchSyno
  public NUOPC_FieldDictionarySetSyno
  public NUOPC_FieldDictionarySetup
  public NUOPC_FieldDictionarySetAutoAdd

!==============================================================================
!
! INTERFACE BLOCKS
!
!==============================================================================

  interface NUOPC_FieldDictionarySetup
    module procedure NUOPC_FieldDictionarySetupDefault
    module procedure NUOPC_FieldDictionarySetupFile
  end interface

  !-----------------------------------------------------------------------------
  contains
  !-----------------------------------------------------------------------------
  
  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionaryAddEntry - Add an entry to the NUOPC Field dictionary
! !INTERFACE:
  subroutine NUOPC_FieldDictionaryAddEntry(standardName, canonicalUnits, rc)
! !ARGUMENTS:
    character(*),                 intent(in)            :: standardName
    character(*),                 intent(in)            :: canonicalUnits
    integer,                      intent(out), optional :: rc
! !DESCRIPTION:
!   Add an entry to the NUOPC Field dictionary. If necessary the dictionary is
!   first set up.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

    call NUOPC_FieldDictionaryAddEntryI(NUOPC_FieldDictionary, &
      standardName = standardName, canonicalUnits = canonicalUnits, &
      rc = localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionaryEgest - Egest NUOPC Field dictionary into FreeFormat
! !INTERFACE:
  subroutine NUOPC_FieldDictionaryEgest(freeFormat, iofmt, rc)
! !ARGUMENTS:
    type(NUOPC_FreeFormat), intent(out)           :: freeFormat
    type(ESMF_IOFmt_Flag),  intent(in),  optional :: iofmt
    integer,                intent(out), optional :: rc
! !DESCRIPTION:
!   Egest the contents of the NUOPC Field dictionary into a FreeFormat object.
!   If I/O format option {\tt iofmt} is provided and equal to {\tt ESMF\_IOFMT\_YAML},
!   the FreeFormat object will contain the NUOPC Field dictionary expressed in YAML
!   format. Other values for {\tt iofmt} are ignored and this method behaves as if
!   the optional {\tt iofmt} argument were missing. In such a case, {\tt freeFormat}
!   will contain NUOPC Field dictionary entries in the traditional format.
!   It is the caller's responsibility to destroy the created {\tt freeFormat}
!   object.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out
    
    call NUOPC_FieldDictionaryEgestI(NUOPC_FieldDictionary, freeFormat, &
      iofmt=iofmt, rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end subroutine
  !-----------------------------------------------------------------------------
  
  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionaryGetEntry - Get information about a NUOPC Field dictionary entry
! !INTERFACE:
  subroutine NUOPC_FieldDictionaryGetEntry(standardName, canonicalUnits, rc)
! !ARGUMENTS:
    character(*),                 intent(in)            :: standardName
    character(*),                 intent(out), optional :: canonicalUnits
    integer,                      intent(out), optional :: rc
! !DESCRIPTION:
!   Return the canonical units that the NUOPC Field dictionary associates with
!   the {\tt standardName}.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

    call NUOPC_FieldDictionaryGetEntryI(NUOPC_FieldDictionary, &
      standardName = standardName, canonicalUnits = canonicalUnits, &
      rc = localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionaryHasEntry - Check whether the NUOPC Field dictionary has a specific entry
! !INTERFACE:
  function NUOPC_FieldDictionaryHasEntry(standardName, rc)
! !RETURN VALUE:
    logical :: NUOPC_FieldDictionaryHasEntry
! !ARGUMENTS:
    character(*),                 intent(in)            :: standardName
    integer,                      intent(out), optional :: rc
! !DESCRIPTION:
!   Return {\tt .true.} if the NUOPC Field dictionary has an entry with the
!   specified {\tt standardName}, {\tt .false.} otherwise.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    NUOPC_FieldDictionaryHasEntry = .false.

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

    NUOPC_FieldDictionaryHasEntry = &
      NUOPC_FieldDictionaryHasEntryI(NUOPC_FieldDictionary, &
      standardName = standardName, rc = localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end function
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOPI
! !IROUTINE: NUOPC_FieldDictionaryIngest - Ingest FreeFormat content into NUOPC Field dictionary
! !INTERFACE:
  subroutine NUOPC_FieldDictionaryIngest(freeFormat, rc)
! !ARGUMENTS:
    type(NUOPC_FreeFormat), intent(in)            :: freeFormat
    integer,                intent(out), optional :: rc
! !DESCRIPTION:
!   Ingest the content of a FreeFormat object into an existing NUOPC Field dictionary.
!EOPI
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    if (NUOPC_FieldDictionaryIsSetup) then
      ! load in FreeFormat content
      call NUOPC_FieldDictionaryIngestI(NUOPC_FieldDictionary, freeFormat, &
        rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, &
        file=FILENAME, &
        rcToReturn=rc)) &
        return  ! bail out
    end if

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionaryMatchSyno - Check whether the NUOPC Field dictionary considers the standard names synonyms
! !INTERFACE:
  function NUOPC_FieldDictionaryMatchSyno(standardName1, standardName2, rc)
! !RETURN VALUE:
    logical :: NUOPC_FieldDictionaryMatchSyno
! !ARGUMENTS:
    character(*),                 intent(in)            :: standardName1
    character(*),                 intent(in)            :: standardName2
    integer,                      intent(out), optional :: rc
! !DESCRIPTION:
!   Return {\tt .true.} if the NUOPC Field dictionary considers
!   {\tt standardName1} and {\tt standardName2} synonyms, {\tt .false.} 
!   otherwise. Also, if {\tt standardName1} and/or {\tt standardName2} do not 
!   correspond to an existing dictionary entry, {.false.} will be returned.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    NUOPC_FieldDictionaryMatchSyno = .false.

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

    NUOPC_FieldDictionaryMatchSyno = &
      NUOPC_FieldDictionaryMatchSynoI(NUOPC_FieldDictionary, &
      standardName1 = standardName1, standardName2 = standardName2, rc = localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end function
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOPI
! !IROUTINE: NUOPC_FieldDictionaryRemove - Take down the NUOPC Field dictionary
! !INTERFACE:
  subroutine NUOPC_FieldDictionaryRemove(rc)
! !ARGUMENTS:
    integer,                intent(out), optional :: rc
! !DESCRIPTION:
!   Erase the content of the NUOPC Field dictionary and free up the memory
!   associated with it. Users will need to call NUOPC_FieldDictionarySetup()
!   to re-create the NUOPC Field dictionary.
!EOPI
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    if (NUOPC_FieldDictionaryIsSetup) then

      ! clear NUOPC Field dictionary content (move to garbage)
      call NUOPC_FieldDictionaryDestroyI(NUOPC_FieldDictionary, rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, &
        file=FILENAME, &
        rcToReturn=rc)) &
        return  ! bail out

      NUOPC_FieldDictionaryIsSetup = .false.

    end if

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionarySetSyno - Set synonyms in the NUOPC Field dictionary
! !INTERFACE:
  subroutine NUOPC_FieldDictionarySetSyno(standardNames, rc)
! !ARGUMENTS:
    character(*),                 intent(in)            :: standardNames(:)
    integer,                      intent(out), optional :: rc
! !DESCRIPTION:
!   Set all of the elements of the {\tt standardNames} argument to be considered
!   synonyms by the field dictionary. Every element in {\tt standardNames} must
!   correspond to the standard name of already existing entries in the field 
!   dictionary, or else an error will be returned.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    call NUOPC_FieldDictionarySetup(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

    call NUOPC_FieldDictionarySetSynoI(NUOPC_FieldDictionary, &
      standardNames = standardNames, rc = localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, &
      file=FILENAME, &
      rcToReturn=rc)) &
      return  ! bail out

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionarySetup - Setup the default NUOPC Field dictionary
! !INTERFACE:
  ! Private name; call using NUOPC_FieldDictionarySetup()
  subroutine NUOPC_FieldDictionarySetupDefault(rc)
! !ARGUMENTS:
    integer,      intent(out), optional   :: rc
! !DESCRIPTION:
!   Setup the default NUOPC Field dictionary.
!EOP
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    if (.not.NUOPC_FieldDictionaryIsSetup) then
    
      call NUOPC_FieldDictionaryCreateI(NUOPC_FieldDictionary, rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

      call NUOPC_FieldDictionaryDefinition(NUOPC_FieldDictionary, rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out
      
      NUOPC_FieldDictionaryIsSetup = .true.
      
    endif

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOPI
! !IROUTINE: NUOPC_FieldDictionarySetup - Setup an empty NUOPC Field dictionary
! !INTERFACE:
  subroutine NUOPC_FieldDictionarySetupEmpty(rc)
! !ARGUMENTS:
    integer,      intent(out), optional   :: rc
! !DESCRIPTION:
!   Setup an empty NUOPC Field dictionary.
!
!   Note: {\tt NUOPC\_FieldDictionaryIsSetup} is set to .true. even if the NUOPC
!   Field dictionary is not fully setup (empty). This behavior needs to be
!   revisited.
!EOPI
  !-----------------------------------------------------------------------------
    integer :: localrc

    if (present(rc)) rc = ESMF_SUCCESS

    if (.not.NUOPC_FieldDictionaryIsSetup) then

      call NUOPC_FieldDictionaryCreateI(NUOPC_FieldDictionary, rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

      NUOPC_FieldDictionaryIsSetup = .true.

    endif

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOP
! !IROUTINE: NUOPC_FieldDictionarySetup - Setup the NUOPC Field dictionary from YAML file
! !INTERFACE:
  ! Private name; call using NUOPC_FieldDictionarySetup()
  subroutine NUOPC_FieldDictionarySetupFile(fileName, rc)
! !ARGUMENTS:
    character(len=*),      intent(in)              :: fileName
    integer,               intent(out), optional   :: rc
! !DESCRIPTION:
!   Setup the NUOPC Field dictionary by reading its content from YAML file.
!   If the NUOPC Field dictionary already exists, remove it and create a new one.
!   This feature requires ESMF built with YAML support. Please see the
!   ESMF User's Guide for details.
!EOP
  !-----------------------------------------------------------------------------
    integer                :: localrc
    type(NUOPC_FreeFormat) :: freeFormat

    if (present(rc)) rc = ESMF_SUCCESS

    ! create a NUOPC FreeFormat FD representation by reading from YAML file
    freeFormat = NUOPC_FreeFormatCreateFDYAML(fileName, rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

    if (NUOPC_FieldDictionaryIsSetup) then
      ! delete existing NUOPC Field dictionary
      call NUOPC_FieldDictionaryRemove(rc=localrc)
      if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
        line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out
    end if

    ! create a new empty NUOPC Field dictionary
    call NUOPC_FieldDictionarySetupEmpty(rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

    ! load FreeFormat content into empty NUOPC Field dictionary
    call NUOPC_FieldDictionaryIngest(freeFormat, rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

    ! free up memory
    call NUOPC_FreeFormatDestroy(freeFormat, rc=localrc)
    if (ESMF_LogFoundError(rcToCheck=localrc, msg=ESMF_LOGERR_PASSTHRU, &
      line=__LINE__, file=FILENAME, rcToReturn=rc)) return  ! bail out

    NUOPC_FieldDictionaryIsSetup = .true.

  end subroutine
  !-----------------------------------------------------------------------------

  !-----------------------------------------------------------------------------
!BOPI
! !IROUTINE: NUOPC_FieldDictionarySetAutoAdd - Turn on/off AutoAdd
! !INTERFACE:
  subroutine NUOPC_FieldDictionarySetAutoAdd(setting, rc)
! !ARGUMENTS:
    logical,      intent(in)              :: setting
    integer,      intent(out), optional   :: rc
! !DESCRIPTION:
!   Turn on/off AutoAdd in the NUOPC Field dictionary.
!EOPI
  !-----------------------------------------------------------------------------
    if (present(rc)) rc = ESMF_SUCCESS

    NUOPC_FieldDictionaryAutoAdd = setting

  end subroutine
  !-----------------------------------------------------------------------------

end module
