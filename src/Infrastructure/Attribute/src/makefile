# $Id$ 

# default rule if no explicit target is given to make
ALL: build_here

SOURCEC	  = ESMCI_Attribute.C ESMCI_AttributeUpdate.C ESMCI_AttributeWrite.C \
            ESMCI_AttributeWriteCIM.C ESMCI_AttributeWriteCIMgrids.C \
            ESMCI_AttributeWriteTab.C ESMCI_AttributeWriteXML.C \
            ESMCI_AttributeWriteJSON.C \
            ESMCI_Attributes.C \
            ESMCI_Metadata.C \
            ESMCI_IOHandle.C \
            ESMCI_TestUtilAttributes.C
SOURCEF	  =
STOREH    = ESMCI_Attribute.h \
            nlohmann/json/json.hpp \
            ESMCI_Attributes.h \
            ESMCI_Metadata.h \
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
