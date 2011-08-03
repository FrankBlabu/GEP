#
# Makefile for a testing application
#

include ../../Makefile.include

PROJECT = gep

SRC  = gep.cpp
OBJ  = ${SRC:%.cpp=%.o}

CFLAGS = $(INCLUDEDIRS) $(DEBUGFLAGS) $(OPTIMIZEFLAGS) $(COMMONFLAGS)
LFLAGS = $(LIBDIRS)

LIBS = $(GEPLIBS) $(SYSTEMLIBS)

$(PROJECT): $(OBJ)
	$(LINK) $(LFLAGS) -o $@ $(OBJ) $(LIBS)

distclean clean: clean-depend
	$(RM) -f $(PROJECT) $(BINDIR)/$(PROJECT) $(PROJECT).tbz *.o *~

depend: make-depend

install: $(PROJECT)
	$(CP) $(PROJECT) $(BINDIR)

backup:
	$(TAR) cjf $(PROJECT).tbz *.cpp Makefile

FORCE:

-include .depend
