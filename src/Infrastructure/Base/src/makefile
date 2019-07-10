# $Id$

ALL: build_here

SOURCEC	  = ESMCI_Base.C ESMCI_Info2.C ESMC_Info2CDef.C \
            ESMC_Info2CDefGeneric.C
SOURCEF	  =
STOREH    = ESMCI_Base.h \
            nlohmann/json/json.hpp \
            ESMCI_Info2.h

OBJSC     = $(addsuffix .o, $(basename $(SOURCEC)))
OBJSF     = $(addsuffix .o, $(basename $(SOURCEF)))
TEXFILES  = $(addsuffix _fapi.tex, $(basename $(AUTOGEN)))

LIBBASE	  = libesmf
LOCDIR	  = src/Infrastructure/Base/src

CLEANDIRS   =
CLEANFILES  = $(AUTOGEN) $(addprefix ../doc/, $(TEXFILES) )
CLOBBERDIRS =

include $(ESMF_DIR)/makefile

DIRS =
