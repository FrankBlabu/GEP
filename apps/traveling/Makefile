#
# Makefile for a testing application
#


PROJECT = traveling
LIBRARY = $(PROJECT)

SRC  = traveling_individual.cpp \
       traveling_main_window.cpp \
       traveling_population.cpp \
       traveling.cpp

UI_SRC  = traveling_main_window.ui

MOC_SRC = GEPTravelingMainWindow.hpp

LIBS = $(GEPLIBS) $(SYSTEMLIBS)

include ../../Makefile.include

all: $(BINDIR)/$(PROJECT)

$(BINDIR)/$(PROJECT): $(UI_HEADER) $(MOC_OBJ) $(OBJ)
	$(LINK) $(LFLAGS) -o $@ $(OBJ) $(MOC_OBJ) $(LIBS)

clean: library-clean
	$(RM) -f $(BINDIR)/$(PROJECT)
	
-include .depend
