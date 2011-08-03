#
# Main Makefile for the whole project
#
# Frank Cieslok, 03.06.2006
#

SUBDIRS = libs/system \
          libs/command \
          libs/core \
          libs/tree \
          gui/display \
          gui/diagram \
          apps/traveling

BACKUPDIR = $(HOME)/Dropbox/Projects/GEP

all:
	@for DIR in $(SUBDIRS); \
	do \
		$(MAKE) -C $$DIR all; \
	done

clean:
	@for DIR in $(SUBDIRS); \
	do \
		$(MAKE) -C $$DIR clean; \
	done
	rm -f .depend

backup: clean
	@export BACKUPFILE=backup_`date +"%d_%m_%Y"`.tbz; \
	export COUNT=1; \
	while [ -e $(BACKUPDIR)/$$BACKUPFILE ]; \
	do \
		export BACKUPFILE=backup_`date +"%d_%m_%Y"`_$$COUNT.tbz; \
		export COUNT=$$(($$COUNT+1)); \
	done; \
	cd $(PWD); \
	tar cjf $(BACKUPDIR)/$$BACKUPFILE . ; \
	cd -
