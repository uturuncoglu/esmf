# $Id$

#tdk:TODO: need to remove netcdf files

ALL:      build_unit_tests
run:      run_unit_tests
run_uni:  run_unit_tests_uni

LOCDIR	  = src/Infrastructure/Attribute/tests

TESTS_BUILD   = $(ESMF_TESTDIR)/ESMC_IOHandleUTest $(ESMF_TESTDIR)/ESMC_MetadataUTest
#TESTS_BUILD   = $(ESMF_TESTDIR)/ESMC_MetadataUTest
#TESTS_BUILD   = $(ESMF_TESTDIR)/ESMC_AttributesUTest \
#                $(ESMF_TESTDIR)/ESMC_NlohmannJSONUTest \
#                $(ESMF_TESTDIR)/ESMF_AttributesUTest \
#                $(ESMF_TESTDIR)/ESMF_AttributesProfileUTest \
#                $(ESMF_TESTDIR)/ESMF_AttributeProfileUTest \
#                $(ESMF_TESTDIR)/ESMC_MetadataUTest \
#                $(ESMF_TESTDIR)/ESMC_IOHandleUTest
                #$(ESMF_TESTDIR)/ESMC_PackageFactoryUTest

#TESTS_RUN     = RUN_ESMC_IOHandleUTest
#TESTS_RUN     = RUN_ESMC_MetadataUTest
TESTS_RUN     = RUN_ESMC_AttributesUTest \
                RUN_ESMC_NlohmannJSONUTest \
                RUN_ESMF_AttributesUTest \
                RUN_ESMF_AttributesProfileUTest \
                RUN_ESMF_AttributeProfileUTest \
                RUN_ESMC_MetadataUTest \
                RUN_ESMC_IOHandleUTest
                #RUN_ESMC_PackageFactoryUTest

#TESTS_RUN_UNI = RUN_ESMC_IOHandleUTestUNI
TESTS_RUN_UNI = RUN_ESMC_AttributesUTestUNI \
                RUN_ESMC_NlohmannJSONUTestUNI \
                RUN_ESMF_AttributesUTestUNI \
                RUN_ESMF_AttributesProfileUTest \
                RUN_ESMF_AttributesProfileUTest \
                RUN_ESMC_MetadataUTest \
                RUN_ESMC_IOHandleUTest
#                #RUN_ESMC_PackageFactoryUTestUNI


include ${ESMF_DIR}/makefile

DIRS        =

CLEANDIRS   = 
CLEANFILES  = $(TESTS_BUILD)
CLOBBERDIRS =


RUN_ESMC_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ctest

RUN_ESMC_AttributesUTestUNI:
	$(MAKE) TNAME=AttributesUNI NP=1 ctest

RUN_ESMC_NlohmannJSONUTest:
	$(MAKE) TNAME=NlohmannJSON NP=4 ctest

RUN_ESMC_NlohmannJSONUTestUNI:
	$(MAKE) TNAME=NlohmannJSONUNI NP=1 ctest

RUN_ESMF_AttributesUTest:
	$(MAKE) TNAME=Attributes NP=4 ftest

RUN_ESMF_AttributesUTestUNI:
	$(MAKE) TNAME=AttributesUNI NP=1 ftest

RUN_ESMF_AttributesProfileUTest:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributesProfile NP=4 ftest

RUN_ESMF_AttributesProfileUTestUNI:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributesProfileUNI NP=1 ftest

RUN_ESMF_AttributeProfileUTest:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributeProfile NP=4 ftest

RUN_ESMF_AttributeProfileUTestUNI:
	env ESMF_RUNTIME_PROFILE=ON $(MAKE) TNAME=AttributeProfileUNI NP=1 ftest

RUN_ESMC_MetadataUTest:
	$(MAKE) TNAME=Metadata NP=4 ctest

RUN_ESMC_MetadataUTestUNI:
	$(MAKE) TNAME=MetadataUNI NP=1 ctest

RUN_ESMC_IOHandleUTest:
	$(MAKE) TNAME=IOHandle NP=4 ctest

RUN_ESMC_IOHandleUTestUNI:
	$(MAKE) TNAME=IOHandleUNI NP=1 ctest

#RUN_ESMC_PackageFactoryUTest:
#	$(MAKE) TNAME=PackageFactory NP=4 ctest

#RUN_ESMC_PackageFactoryUTestUNI:
#	$(MAKE) TNAME=PackageFactoryUNI NP=1 ctest
