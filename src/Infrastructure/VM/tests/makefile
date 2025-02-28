# $Id$ 

ifeq ($(ESMF_TESTFORCEOPENMP),ON)
ifeq ($(ESMF_OPENMP),OFF)
ESMF_F90COMPILEOPTS = $(ESMF_OPENMP_F90COMPILEOPTS)
ESMF_F90LINKOPTS = $(ESMF_OPENMP_F90LINKOPTS)
endif
endif


ALL:      build_unit_tests
run:      run_unit_tests
run_uni:  run_unit_tests_uni

LOCDIR	  = src/Infrastructure/VM/tests

.NOTPARALLEL:
TESTS_BUILD   = $(ESMF_TESTDIR)/ESMC_VMUTest \
		$(ESMF_TESTDIR)/ESMF_VMUTest \
		$(ESMF_TESTDIR)/ESMF_VMAccUTest \
		$(ESMF_TESTDIR)/ESMF_VMOpenMPUTest \
		$(ESMF_TESTDIR)/ESMF_VMUserMpiInitUTest \
		$(ESMF_TESTDIR)/ESMF_VMSendVMRecvUTest \
		$(ESMF_TESTDIR)/ESMF_VMSendNbVMRecvNbUTest \
		$(ESMF_TESTDIR)/ESMF_VMSendRecvUTest \
		$(ESMF_TESTDIR)/ESMF_VMSendRecvNbUTest \
		$(ESMF_TESTDIR)/ESMF_VMScatterUTest \
		$(ESMF_TESTDIR)/ESMF_VMGatherUTest \
		$(ESMF_TESTDIR)/ESMF_VMAllGatherUTest \
		$(ESMF_TESTDIR)/ESMF_VMAllGatherVUTest \
		$(ESMF_TESTDIR)/ESMF_VMBroadcastUTest \
		$(ESMF_TESTDIR)/ESMF_VMAllToAllUTest \
		$(ESMF_TESTDIR)/ESMF_VMAllToAllVUTest \
		$(ESMF_TESTDIR)/ESMF_VMBarrierUTest \
		$(ESMF_TESTDIR)/ESMF_VMEpochLargeMsgUTest \
		$(ESMF_TESTDIR)/ESMF_VMComponentUTest

TESTS_RUN     = RUN_ESMC_VMUTest \
		RUN_ESMF_VMUTest \
		RUN_ESMF_VMAccUTest \
                RUN_ESMF_VMOpenMPUTest \
                RUN_ESMF_VMUserMpiInitUTest \
                RUN_ESMF_VMSendVMRecvUTest \
                RUN_ESMF_VMSendNbVMRecvNbUTest \
                RUN_ESMF_VMSendRecvUTest \
                RUN_ESMF_VMSendRecvNbUTest \
                RUN_ESMF_VMScatterUTest \
                RUN_ESMF_VMGatherUTest \
                RUN_ESMF_VMAllGatherUTest \
                RUN_ESMF_VMAllGatherVUTest \
		RUN_ESMF_VMBroadcastUTest \
                RUN_ESMF_VMAllToAllUTest \
                RUN_ESMF_VMAllToAllVUTest \
		RUN_ESMF_VMBarrierUTest \
		RUN_ESMF_VMEpochLargeMsgUTest \
		RUN_ESMF_VMComponentUTest 

TESTS_RUN_UNI = RUN_ESMC_VMUTestUNI \
		RUN_ESMF_VMUTestUNI \
		RUN_ESMF_VMAccUTestUNI \
                RUN_ESMF_VMOpenMPUTestUNI \
                RUN_ESMF_VMUserMpiInitUTestUNI \
                RUN_ESMF_VMSendNbVMRecvNbUTestUNI \
                RUN_ESMF_VMSendRecvUTestUNI \
                RUN_ESMF_VMSendRecvNbUTestUNI \
                RUN_ESMF_VMScatterUTestUNI \
                RUN_ESMF_VMGatherUTestUNI \
                RUN_ESMF_VMAllGatherUTestUNI \
                RUN_ESMF_VMAllGatherVUTestUNI \
                RUN_ESMF_VMBroadcastUTestUNI \
                RUN_ESMF_VMAllToAllUTestUNI \
                RUN_ESMF_VMAllToAllVUTestUNI \
                RUN_ESMF_VMBarrierUTestUNI \
                RUN_ESMF_VMComponentUTestUNI


include ${ESMF_DIR}/makefile

CLEANDIRS   = 
CLEANFILES  = $(TESTS_BUILD)
CLOBBERDIRS =

#
# VM
#
RUN_ESMF_VMUTest:
	$(MAKE) TNAME=VM NP=4 ftest

RUN_ESMF_VMUTestUNI:
	$(MAKE) TNAME=VM NP=1 ftest

#
# VM Accelerator Interface test
#
RUN_ESMF_VMAccUTest:
	$(MAKE) TNAME=VMAcc NP=4 ftest

RUN_ESMF_VMAccUTestUNI:
	$(MAKE) TNAME=VMAcc NP=1 ftest

#
# VM -- C interface
#
RUN_ESMC_VMUTest:
	$(MAKE) TNAME=VM NP=4 ctest

RUN_ESMC_VMUTestUNI:
	$(MAKE) TNAME=VM NP=1 ctest

#
# VM
#
RUN_ESMF_VMOpenMPUTest:
	$(MAKE) TNAME=VMOpenMP NP=4 ftest

RUN_ESMF_VMOpenMPUTestUNI:
	$(MAKE) TNAME=VMOpenMP NP=1 ftest

#
# VM user mpi init
#
RUN_ESMF_VMUserMpiInitUTest:
	$(MAKE) TNAME=VMUserMpiInit NP=4 ftest

RUN_ESMF_VMUserMpiInitUTestUNI:
	$(MAKE) TNAME=VMUserMpiInit NP=1 ftest

#
# VM Send VM Recv
#
RUN_ESMF_VMSendVMRecvUTest:
	$(MAKE) TNAME=VMSendVMRecv NP=4 ftest

#
# VM SendNb VM RecvNb
#
RUN_ESMF_VMSendNbVMRecvNbUTest:
	$(MAKE) TNAME=VMSendNbVMRecvNb NP=4 ftest

RUN_ESMF_VMSendNbVMRecvNbUTestUNI:
	$(MAKE) TNAME=VMSendNbVMRecvNb NP=1 ftest

#
# VM SendRecv
#
RUN_ESMF_VMSendRecvUTest:
	$(MAKE) TNAME=VMSendRecv NP=4 ftest

RUN_ESMF_VMSendRecvUTestUNI:
	$(MAKE) TNAME=VMSendRecv NP=1 ftest

#
# VM SendRecvNb
#
RUN_ESMF_VMSendRecvNbUTest:
	$(MAKE) TNAME=VMSendRecvNb NP=4 ftest

RUN_ESMF_VMSendRecvNbUTestUNI:
	$(MAKE) TNAME=VMSendRecvNb NP=1 ftest

#
# VM Scatter 
#
RUN_ESMF_VMScatterUTest:
	$(MAKE) TNAME=VMScatter NP=4 ftest

RUN_ESMF_VMScatterUTestUNI:
	$(MAKE) TNAME=VMScatter NP=1 ftest

#
# VM Gather
#
RUN_ESMF_VMGatherUTest:
	$(MAKE) TNAME=VMGather NP=4 ftest

RUN_ESMF_VMGatherUTestUNI:
	$(MAKE) TNAME=VMGather NP=1 ftest


#
# VM AllGather
#
RUN_ESMF_VMAllGatherUTest:
	$(MAKE) TNAME=VMAllGather NP=4 ftest

RUN_ESMF_VMAllGatherUTestUNI:
	$(MAKE) TNAME=VMAllGather NP=1 ftest


#
# VM AllGatherV
#
RUN_ESMF_VMAllGatherVUTest:
	$(MAKE) TNAME=VMAllGatherV NP=4 ftest

RUN_ESMF_VMAllGatherVUTestUNI:
	$(MAKE) TNAME=VMAllGatherV NP=1 ftest

#
#
# VM Broadcast
#

RUN_ESMF_VMBroadcastUTest:
	$(MAKE) TNAME=VMBroadcast NP=4 ftest

RUN_ESMF_VMBroadcastUTestUNI:
	$(MAKE) TNAME=VMBroadcast NP=1 ftest

#
# VM AllToAll
#
RUN_ESMF_VMAllToAllUTest:
	$(MAKE) TNAME=VMAllToAll NP=4 ftest

RUN_ESMF_VMAllToAllUTestUNI:
	$(MAKE) TNAME=VMAllToAll NP=1 ftest

#
# VM AllToAllV
#
RUN_ESMF_VMAllToAllVUTest:
	$(MAKE) TNAME=VMAllToAllV NP=4 ftest

RUN_ESMF_VMAllToAllVUTestUNI:
	$(MAKE) TNAME=VMAllToAllV NP=1 ftest

#
# VM Barrier
#
RUN_ESMF_VMBarrierUTest:
	$(MAKE) TNAME=VMBarrier NP=4 ftest

RUN_ESMF_VMBarrierUTestUNI:
	$(MAKE) TNAME=VMBarrier NP=1 ftest

#
# VM Epoch and Large Messages
#
RUN_ESMF_VMEpochLargeMsgUTest: ESMF_MPILAUNCHOPTIONS := $(shell echo $(ESMF_MPILAUNCHOPTIONS) | sed 's/\-\-exact//')
RUN_ESMF_VMEpochLargeMsgUTest:
	$(MAKE) TNAME=VMEpochLargeMsg NP=2 ftest
#
# VM Component
#
RUN_ESMF_VMComponentUTest:
	$(MAKE) TNAME=VMComponent NP=4 ftest

RUN_ESMF_VMComponentUTestUNI:
	$(MAKE) TNAME=VMComponent NP=1 ftest


