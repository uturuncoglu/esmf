# $Id$

ALL:  build_unit_tests
run:  run_unit_tests
run_uni:  run_unit_tests_uni

.PRECIOUS: %.F90

LOCDIR = src/Superstructure/InfoAPI/tests

.NOTPARALLEL:
#TESTS_BUILD = $(ESMF_TESTDIR)/ESMF_AttPackFieldUTest
TESTS_BUILD = \
          $(ESMF_TESTDIR)/ESMF_InfoGetInterfaceArrayUTest \
          $(ESMF_TESTDIR)/ESMF_InfoSyncUTest

TESTS_RUN = \
          RUN_ESMF_InfoGetInterfaceArrayUTest \
          RUN_ESMF_InfoSyncUTest

TESTS_RUN_UNI = \
          RUN_ESMF_InfoGetInterfaceArrayUTestUNI \
          RUN_ESMF_InfoSyncUTestUNI

include ${ESMF_DIR}/makefile

DIRS        =

CLEANDIRS   =
CLEANFILES  = $(TESTS_BUILD) $(AUTOGEN)
CLOBBERDIRS =


# ==============================================================================

# ---  Info object-level get interfaces


RUN_ESMF_InfoGetInterfaceArrayUTest:
	$(MAKE) TNAME=InfoGetInterfaceArray NP=4 ftest

RUN_ESMF_InfoGetInterfaceArrayUTestUNI:
	$(MAKE) TNAME=InfoGetInterfaceArray NP=1 ftest
	
# --- Info Synchronization Testing

RUN_ESMF_InfoSyncUTest:
	$(MAKE) TNAME=InfoSync NP=4 ftest

RUN_ESMF_InfoSyncUTestUNI:
	$(MAKE) TNAME=InfoSync NP=1 ftest

