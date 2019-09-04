# $Id$ 

# default rule if no explicit target is given to make
ALL: build_here

SOURCEC	  = ESMCI_Metadata.C \
            ESMCI_IOHandle.C \
            ESMCI_TestUtilAttributes.C
SOURCEF	  =
STOREH    = ESMCI_Metadata.h \
            ESMCI_IOHandle.h \
            ESMCI_TestUtilAttributes.h

OBJSC     = $(addsuffix .o, $(basename $(SOURCEC)))
OBJSF     = $(addsuffix .o, $(basename $(SOURCEF)))
TEXFILES  = $(addsuffix _fapi.tex, $(basename $(AUTOGEN)))

LIBBASE	  = libesmf
LOCDIR	  = src/Infrastructure/Attribute/src

CLEANDIRS   = 
CLEANFILES  = $(AUTOGEN) $(addprefix ../doc/, $(TEXFILES) )
CLOBBERDIRS = 

include $(ESMF_DIR)/makefile

DIRS = 
