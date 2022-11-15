#
# Makefile generated by:
# codebench 0.55
#
# Project: DEFMOUSEPOINTERS
#
# Created on: 07-10-2018 11:51:45
#
#

###################################################################
##
##////  Objects
##
###################################################################

DefMousePointers_OBJ := \
	 DMP.o DMP_cli.o


###################################################################
##
##////  Variables and Environment
##
###################################################################

CC := gcc:bin/gcc

INCPATH := -I.

CFLAGS := $(INCPATH) -gstabs -Wall -D__AMIGADATE__=\"`c:date LFORMAT %d.%m.%Y`\"


###################################################################
##
##////  General rules
##
###################################################################

.PHONY: all all-before all-after clean clean-custom realclean

all: all-before DefMousePointers all-after

all-before: DMP_strings.h
#	You can add rules here to execute before the project is built

all-after:
#	You can add rules here to execute after the project is built

clean: clean-custom
	@echo "Cleaning compiler objects..."
	@rm -f  $(DefMousePointers_OBJ)

realclean:
	@echo "Cleaning compiler objects and targets..."
	@rm -f  $(DefMousePointers_OBJ) DefMousePointers DefMousePointers.debug


###################################################################
##
##////  Targets
##
###################################################################

DefMousePointers: $(DefMousePointers_OBJ)
	@echo "Linking DefMousePointers.debug"
#	@gcc:bin/gcc -o DefMousePointers.debug $(DefMousePointers_OBJ) -N
	@gcc:bin/gcc -o DefMousePointers.debug $(DefMousePointers_OBJ)
	@echo "Creating debug target: DefMousePointers"
#	@cp -f -p DefMousePointers.debug DefMousePointers
	@copy DefMousePointers.debug DefMousePointers FORCE CLONE


###################################################################
##
##////  Standard rules
##
###################################################################

# A default rule to make all the objects listed below
# because we are hiding compiler commands from the output

.c.o:
	@echo "Compiling $<"
	@$(CC) -c $< -o $*.o $(CFLAGS)

DMP_aeon.o: DMP_aeon.c DMP_global.h DMP_rev.h \
	 DMP_strings.h DMP.h

DMP_cli.o: DMP_cli.c DMP_global.h DMP_rev.h \
	


###################################################################
##
##////  Custom rules
##
###################################################################

DMP_strings.h: DefMousePointers.cd 
	APPDIR:Catcomp DefMousePointers.cd CFILE DMP_strings.h
	$(MAKE) clean


###################################################################
