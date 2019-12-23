interface ESMF_InfoSync
  module procedure ESMF_InfoSyncCplComp
  module procedure ESMF_InfoSyncGridComp
  module procedure ESMF_InfoSyncSciComp
  module procedure ESMF_InfoSyncField
  module procedure ESMF_InfoSyncFieldBundle
  module procedure ESMF_InfoSyncState
end interface

interface ESMF_Info2GetHandle
  module procedure ESMF_Info2GetHandleArray
  module procedure ESMF_Info2GetHandleArrayBundle
  module procedure ESMF_Info2GetHandleCplComp
  module procedure ESMF_Info2GetHandleGridComp
  module procedure ESMF_Info2GetHandleSciComp
  module procedure ESMF_Info2GetHandleDistGrid
  module procedure ESMF_Info2GetHandleField
  module procedure ESMF_Info2GetHandleFieldBundle
  module procedure ESMF_Info2GetHandleGrid
  module procedure ESMF_Info2GetHandleState
  module procedure ESMF_Info2GetHandleLocStream
end interface