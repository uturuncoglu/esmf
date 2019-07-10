# $Id$

ALL:  build_unit_tests
run:  run_unit_tests

LOCDIR	  = src/Infrastructure/Base/tests

#TESTS_BUILD = $(ESMF_TESTDIR)/ESMC_Info2UTest
TESTS_BUILD   = $(ESMF_TESTDIR)/ESMF_BaseUTest \
                $(ESMF_TESTDIR)/ESMC_NlohmannJSONUTest \
                $(ESMF_TESTDIR)/ESMC_Info2UTest \
                $(ESMF_TESTDIR)/ESMF_Info2UTest \
                $(ESMF_TESTDIR)/ESMF_Info2ProfileUTest \
                $(ESMF_TESTDIR)/ESMF_Info2ArrayUTest \
                $(ESMF_TESTDIR)/ESMF_Info2SyncUTest

TESTS_RUN     = RUN_ESMF_BaseUTest \
                RUN_ESMC_NlohmannJSONUTest \
                RUN_ESMC_Info2UTest \
                RUN_ESMF_Info2UTest \
                RUN_ESMF_Info2ProfileUTest \
                RUN_ESMF_Info2ArrayUTest \
                RUN_ESMF_Info2SyncUTest

TESTS_RUN_UNI = RUN_ESMF_BaseUTestUNI \
                RUN_ESMC_NlohmannJSONUTestUNI \
                RUN_ESMC_Info2UTestUNI \
                RUN_ESMF_Info2UTestUNI \
                RUN_ESMF_Info2ProfileUTestUNI \
                RUN_ESMF_Info2ArrayUTestUNI \
                RUN_ESMF_Info2SyncUTestUNI


include ${ESMF_DIR}/makefile

DIRS        =

CLEANDIRS   =
CLEANFILES  = $(TESTS_BUILD)
CLOBBERDIRS =


#
#  Base class
#

RUN_ESMF_BaseUTest:
	$(MAKE) TNAME=Base NP=4 ftest

RUN_ESMF_BaseUTestUNI:
	$(MAKE) TNAME=Base NP=1 ftest

# Info ==================================================================

RUN_ESMC_NlohmannJSONUTest:
	$(MAKE) TNAME=NlohmannJSON NP=4 ctest

RUN_ESMC_NlohmannJSONUTestUNI:
	$(MAKE) TNAME=NlohmannJSONUNI NP=1 ctest

RUN_ESMC_Info2UTest:
	$(MAKE) TNAME=Info2 NP=4 ctest

RUN_ESMC_Info2UTestUNI:
	$(MAKE) TNAME=Info2UNI NP=1 ctest

RUN_ESMF_Info2UTest:
	$(MAKE) TNAME=Info2 NP=4 ftest

RUN_ESMF_Info2UTestUNI:
	$(MAKE) TNAME=Info2UNI NP=1 ftest

RUN_ESMF_Info2ProfileUTest:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=Info2Profile NP=4 ftest

RUN_ESMF_Info2ProfileUTestUNI:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=Info2ProfileUNI NP=1 ftest

RUN_ESMF_Info2ArrayUTest:
	$(MAKE) TNAME=Info2Array NP=4 ftest

RUN_ESMF_Info2ArrayUTestUNI:
	$(MAKE) TNAME=Info2ArrayUNI NP=1 ftest

RUN_ESMF_Info2SyncUTest:
	$(MAKE) TNAME=Info2Sync NP=4 ftest

RUN_ESMF_Info2SyncUTestUNI:
	$(MAKE) TNAME=Info2SyncUNI NP=1 ftest
