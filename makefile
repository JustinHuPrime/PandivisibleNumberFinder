# command options
C++ := g++
RM := rm -rf
MV := mv
MKDIR := mkdir -p


# file options
SRCDIRPREFIX := src
OBJDIRPREFIX := bin
DEPDIRPREFIX := dependencies
MAINSUFFIX := main
TESTSUFFIX := test

# main file options
SRCDIR := $(SRCDIRPREFIX)/$(MAINSUFFIX)
SRCS := $(shell find -O3 $(SRCDIR)/ -type f -name '*.cc')

OBJDIR := $(OBJDIRPREFIX)/$(MAINSUFFIX)
OBJS := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))

DEPDIR := $(DEPDIRPREFIX)/$(MAINSUFFIX)
DEPS := $(patsubst $(SRCDIR)/%.cc,$(DEPDIR)/%.dep,$(SRCS))

# test file options
TSRCDIR := $(SRCDIRPREFIX)/$(TESTSUFFIX)
TSRCS := $(shell find -O3 $(TSRCDIR)/ -type f -name '*.cc')

TOBJDIR := $(OBJDIRPREFIX)/$(TESTSUFFIX)
TOBJS := $(patsubst $(TSRCDIR)/%.cc,$(TOBJDIR)/%.o,$(TSRCS))

TDEPDIR := $(DEPDIRPREFIX)/$(TESTSUFFIX)
TDEPS := $(patsubst $(TSRCDIR)/%.cc,$(TDEPDIR)/%.dep,$(TSRCS))


# compiler warnings
WARNINGS := -pedantic -pedantic-errors -Wall -Wextra\
-Wmissing-include-dirs -Wswitch-default -Wuninitialized -Wstrict-overflow=5\
-Wsuggest-override -Wfloat-equal -Wundef -Wunused-macros -Wcast-qual\
-Wcast-align=strict -Wconversion -Wzero-as-null-pointer-constant -Wformat=2\
-Wuseless-cast -Wextra-semi -Wsign-conversion -Wlogical-op\
-Wmissing-declarations -Wredundant-decls -Winvalid-pch -Wctor-dtor-privacy\
-Wnoexcept -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast\
-Woverloaded-virtual -Wsign-promo

DEBUGOPTIONS := -Og -ggdb
RELEASEOPTIONS := -O3 -Wunused -Wdisabled-optimization

# compiler options
OPTIONS := -std=c++17 -D_POSIX_C_SOURCE=201911L -pthread -I$(SRCDIR) $(WARNINGS)
TOPTIONS := -I$(TSRCDIR) -Ilibs/Catch2/single_include/catch2
LIBS :=


# final executable name
EXENAME := finder
TEXENAME := finder-test


.PHONY: debug release clean
.SECONDEXPANSION:
.SUFFIXES:


debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME) $(TEXENAME)
	@echo "Running tests"
	@./$(TEXENAME)
	@echo "Done building debug!"

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME) $(TEXENAME)
	@echo "Running tests"
	@./$(TEXENAME)
	@echo "Done building release!"


clean:
	@echo "Removing all generated files and folders."
	@$(RM) $(OBJDIRPREFIX) $(DEPDIRPREFIX) $(EXENAME) $(TEXENAME)


$(EXENAME): $(OBJS)
	@echo "Linking $@"
	@$(C++) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,$(SRCDIR)/%.cc,$$@) $$(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@"
	@$(C++) $(OPTIONS) -c $< -o $@

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,$(SRCDIR)/%.cc,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(C++) $(OPTIONS) -MM -MT $(patsubst $(DEPDIR)/%.dep,$(OBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


$(TEXENAME): $(TOBJS) $(OBJS)
	@echo "Linking $@"
	@$(C++) -o $(TEXENAME) $(OPTIONS) $(TOPTIONS) $(filter-out %main.o,$(OBJS)) $(TOBJS) $(LIBS)

$(TOBJS): $$(patsubst $(TOBJDIR)/%.o,$(TSRCDIR)/%.cc,$$@) $$(patsubst $(TOBJDIR)/%.o,$(TDEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@"
	@$(C++) $(OPTIONS) $(TOPTIONS) -c $< -o $@

$(TDEPS): $$(patsubst $(TDEPDIR)/%.dep,$(TSRCDIR)/%.cc,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(C++) $(OPTIONS) $(TOPTIONS) -MM -MT $(patsubst $(TDEPDIR)/%.dep,$(TOBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


%/:
	@$(MKDIR) $@


-include $(DEPS) $(TDEPS)