# $Id$ 

ALL: build_system_tests

LOCDIR   = src/system_tests

include $(ESMF_DIR)/makefile

# arranged roughly in order of complexity
DIRS      = ESMF_CompCreate \
            ESMF_CompFortranAndC \
            ESMF_RecursiveComponent \
            ESMF_ArrayScatterGather \
            ESMF_Attribute \
            ESMF_ComplianceChecker \
            ESMF_ArraySparseMatMul \
            ESMF_ArrayRedist \
            ESMF_ArrayRedistMPMD \
            ESMF_ArrayRedistSharedObj \
            ESMF_ArrayRedist3D \
            ESMF_ArrayBundleSparseMatMul \
            ESMF_ArrayBundleRedist \
            ESMF_ArrayRedistOpenMP \
            ESMF_ArrayRedistOpenACC \
            ESMF_ArraySharedDeSSI \
            ESMF_ConcurrentComponent \
            ESMF_FieldConcurrentComp \
            ESMF_FieldBundleSMM \
            ESMF_FieldBundleSMMPacked \
            ESMF_FieldBundleRedistPacked \
            ESMF_FieldBundleRegridPacked \
            ESMF_FieldRedist \
            ESMF_FieldRedistBlk2Blk \
            ESMF_FieldRedistBlk2Arb \
            ESMF_FieldRedistArb2Arb \
            ESMF_FieldBundleLSRedistArb2Arb \
            ESMF_FieldBundleLSRedistArb2ArbUngrdDim \
            ESMF_FieldBundleRedistBlk2Blk \
            ESMF_FieldBundleRedistBlk2Arb \
            ESMF_FieldBundleRedistArb2Arb \
            ESMF_FieldRegrid \
            ESMF_FieldRegridDisjoint \
            ESMF_FieldRegridOverlap \
            ESMF_FieldRegridMesh \
            ESMF_FieldRegridMeshToMesh \
            ESMF_FieldRegridLS \
            ESMF_FieldRegridPatchDisjoint \
            ESMF_FieldSparseMatMul \
            ESMF_FieldSharedDeSSI \
            ESMF_FieldLSRedistArb2Arb \
            ESMF_FieldLSRedistArb2ArbUngrdDim \
            ESMF_FieldLSSMM \
            ESMF_FieldMeshSMM \
            ESMF_TransferGrid \
            ESMF_TransferMesh \
            ESMF_SequentialEnsemble \
            ESMF_ConcurrentEnsemble \
            ESMF_DirectCoupling \
            ESMF_XGridConcurrent \
            ESMF_XGridSerial \
            ESMF_Trace
            
ifeq ($(ESMF_MAPPER_BUILD),ON)            
DIRS     += ESMF_MapperTwoConcurrentComps \
            ESMF_MapperSimpleTwoConcurrentComps
endif
            
# Disabled STests
#           ESMF_FieldRegridConserv \ cover by Regrid
#           ESMF_FieldRegridMulti \   cover by Regrid
#           ESMF_FieldRegridOrder \   cover by Regrid


CLEANDIRS   =
CLEANFILES  =
CLOBBERDIRS =

# DO NOT DELETE
