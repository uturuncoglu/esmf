# $Id$ 

ALL:  build_unit_tests
run:  run_unit_tests

LOCDIR	  = src/Infrastructure/Base/tests

TESTS_BUILD   = $(ESMF_TESTDIR)/ESMF_BaseUTest \
                $(ESMF_TESTDIR)/ESMC_NlohmannJSONUTest \
                $(ESMF_TESTDIR)/ESMC_AttributesUTest \
                $(ESMF_TESTDIR)/ESMF_AttributesUTest \
                $(ESMF_TESTDIR)/ESMF_AttributesProfileUTest

TESTS_RUN     = RUN_ESMF_BaseUTest \
                RUN_ESMC_NlohmannJSONUTest \
                RUN_ESMC_AttributesUTest \
                RUN_ESMF_AttributesUTest \
                RUN_ESMF_AttributesProfileUTest

TESTS_RUN_UNI = RUN_ESMF_BaseUTestUNI \
                RUN_ESMC_NlohmannJSONUTestUNI \
                RUN_ESMC_AttributesUTestUNI \
                RUN_ESMF_AttributesUTestUNI \
                RUN_ESMF_AttributesProfileUTestUNI


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

# Attributes ==================================================================

RUN_ESMC_NlohmannJSONUTest:
	$(MAKE) TNAME=NlohmannJSON NP=4 ctest

RUN_ESMC_NlohmannJSONUTestUNI:
	$(MAKE) TNAME=NlohmannJSONUNI NP=1 ctest

RUN_ESMC_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ctest

RUN_ESMC_AttributesUTestUNI:
	$(MAKE) TNAME=AttributesUNI NP=1 ctest

RUN_ESMF_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ftest

RUN_ESMF_AttributesUTestUNI:
	$(MAKE) TNAME=AttributesUNI NP=1 ftest

RUN_ESMF_AttributesProfileUTest:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributesProfile NP=4 ftest

RUN_ESMF_AttributesProfileUTestUNI:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributesProfileUNI NP=1 ftest
