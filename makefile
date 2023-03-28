SRCS       := main.c pmcontainer.c pmconv.c pmgui.c pmlist.c pmtimer.c pmwidget.c pmmisc.c pmobj.c pmvector.c
PROGRAM    := a.exe
CXXFLAGS   :=
LDFLAGS    := 
LDLIBS     := 
##########################################################
CXX        := clang
OUT_DIR    := build/
TMP_DIR    := $(OUT_DIR)tmp/
##########################################################
OBJS       := $(SRCS:%.c=$(TMP_DIR)%.o)
DEPS       := $(SRCS:%.c=$(TMP_DIR)%.d)
CXXFLAGS   += -O2 -MMD -MP
LDFLAGS    += -static-libgcc -static -mwindows
LDLIBS     += 
##########################################################
.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all
all: $(OUT_DIR) $(TMP_DIR) $(OUT_DIR)$(PROGRAM)

-include $(DEPS)

$(OUT_DIR)$(PROGRAM): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	strip -s $(OUT_DIR)$(PROGRAM)

$(TMP_DIR)%.o : %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $@

$(TMP_DIR):
	mkdir -p $@

.PHONY: clean 
clean:
	rm -f -r $(TMP_DIR) $(OUT_DIR)

.PHONY: dll
dll:
	objdump -p $(OUT_DIR)$(PROGRAM) | findstr "dll"




