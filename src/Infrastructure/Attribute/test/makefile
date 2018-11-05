# $Id$ 

ALL:      build_unit_tests
run:      run_unit_tests
run_uni:  run_unit_tests_uni

LOCDIR	  = src/Infrastructure/Attribute/tests

TESTS_BUILD   = $(ESMF_TESTDIR)/ESMC_AttributesUTest \
                $(ESMF_TESTDIR)/ESMC_NlohmannJSONUTest \
                $(ESMF_TESTDIR)/ESMF_AttributesUTest \
                $(ESMF_TESTDIR)/ESMC_PackageFactoryUTest

TESTS_RUN     = RUN_ESMC_AttributesUTest \
                RUN_ESMC_NlohmannJSONUTest \
                RUN_ESMF_AttributesUTest \
                RUN_ESMC_PackageFactoryUTest

TESTS_RUN_UNI = RUN_ESMC_AttributesUTestUNI \
                RUN_ESMC_NlohmannJSONUTestUNI \
                RUN_ESMF_AttributesUTestUNI \
                RUN_ESMC_PackageFactoryUTestUNI


include ${ESMF_DIR}/makefile

DIRS        =

CLEANDIRS   = 
CLEANFILES  = $(TESTS_BUILD)
CLOBBERDIRS =


RUN_ESMC_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ctest

RUN_ESMC_AttributesUTestUNI:
	$(MAKE) TNAME=Attributes NP=1 ctest

RUN_ESMC_NlohmannJSONUTest:
	$(MAKE) TNAME=NlohmannJSON NP=4 ctest

RUN_ESMC_NlohmannJSONUTestUNI:
	$(MAKE) TNAME=NlohmannJSON NP=1 ctest

RUN_ESMF_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ftest

RUN_ESMF_AttributesUTestUNI:
	$(MAKE) TNAME=ESMF_Attributes NP=1 ftest

RUN_ESMC_PackageFactoryUTest:
	$(MAKE) TNAME=PackageFactory NP=4 ctest

RUN_ESMC_PackageFactoryUTestUNI:
	$(MAKE) TNAME=PackageFactory NP=1 ctest
