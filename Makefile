#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif
include $(DEVKITARM)/gp2x_rules

BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include build
DATA		:=	data


export ORCUS_MAJOR	:= 1
export ORCUS_MINOR	:= 2
export ORCUS_PATCH	:= 0

VERSION	:=	$(ORCUS_MAJOR).$(ORCUS_MINOR).$(ORCUS_PATCH)

ARCH	:=	

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	:=	-g -O3 -Wall -Wno-switch -Wno-multichar -mtune=arm9tdmi -fomit-frame-pointer -ffast-math $(ARCH) $(INCLUDE)
ASFLAGS	:=	-g -Wa,--warn $(ARCH)

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export TARGET	:=	$(CURDIR)/lib/liborcus.a

export VPATH	:=	$(foreach dir,$(DATA),$(CURDIR)/$(dir)) $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))

export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES	:=	$(OFILES_BIN) $(OFILES_SRC)

export HFILES	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/build

.PHONY: $(BUILD) clean docs

$(BUILD):
	@[ -d lib ] || mkdir -p lib
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

docs:
	doxygen orcus.dox
	@tar -cvjf orcus-$(VERSION)-docs.tar.bz2 docs

clean:
	@echo clean ...
	@rm -fr $(BUILD) lib *.tar.bz2

dist: $(BUILD)
	@tar --exclude=*CVS* --exclude=.svn --exclude=*~ --exclude=*build* --exclude=*.bz2 -cvjf orcus-src-$(VERSION).tar.bz2 include source Makefile LICENSE README.md
	@tar --exclude=*CVS* --exclude=.svn --exclude=*~ -cvjf orcus-$(VERSION).tar.bz2 include lib LICENSE README.md

install: dist
	mkdir -p $(DESTDIR)$(DEVKITPRO)/liborcus
	bzip2 -cd orcus-$(VERSION).tar.bz2 | tar -xvf - -C $(DESTDIR)$(DEVKITPRO)/liborcus

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
$(TARGET): $(OFILES)

$(OFILES_SRC)	: $(HFILES)

#---------------------------------------------------------------------------------
%.a: $(OFILES)
	@echo $@
	@rm -f $@
	@$(AR) rcs $@ $(OFILES)

%_bmp.h %.bmp.o: %.bmp
	@echo $(notdir $<)
	bmp2bin -q $< $@
	@$(bin2o)

-include $(DEPENDS)

endif
#---------------------------------------------------------------------------------
