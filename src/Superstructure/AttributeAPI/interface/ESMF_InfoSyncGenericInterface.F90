interface ESMF_InfoSync
  module procedure ESMF_InfoSyncCplComp
  module procedure ESMF_InfoSyncGridComp
  module procedure ESMF_InfoSyncSciComp
  module procedure ESMF_InfoSyncField
  module procedure ESMF_InfoSyncFieldBundle
  module procedure ESMF_InfoSyncState
end interface

interface ESMF_InfoGetHandle
  module procedure ESMF_InfoGetHandleArray
  module procedure ESMF_InfoGetHandleArrayBundle
  module procedure ESMF_InfoGetHandleCplComp
  module procedure ESMF_InfoGetHandleGridComp
  module procedure ESMF_InfoGetHandleSciComp
  module procedure ESMF_InfoGetHandleDistGrid
  module procedure ESMF_InfoGetHandleField
  module procedure ESMF_InfoGetHandleFieldBundle
  module procedure ESMF_InfoGetHandleGrid
  module procedure ESMF_InfoGetHandleState
  module procedure ESMF_InfoGetHandleLocStream
end interface