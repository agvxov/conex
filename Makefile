.PHONY: test clean
.SUFFIXES:

# --- Paths / files
SOURCE.d := source/
OBJECT.d := object/
LIB.d	 := library/

SOURCE := main.cpp tcl_tk.c
OBJECT := ${SOURCE}
OBJECT := $(subst .cpp,.o,${OBJECT})
OBJECT := $(subst .c,.o,${OBJECT})

vpath %.c ${SOURCE.d}
vpath %.cpp ${SOURCE.d}

OUT := conex

# --- Tools/Flags
ifeq (${DEBUG}, 1)
  LFLAGS   += --debug --trace
  YFLAGS   += --debug

  CPPFLAGS += -DDEBUG

  CFLAGS.D += -Wall -Wextra -Wpedantic
  CFLAGS.D += -O0 -ggdb -fno-inline
  CFLAGS.D += -fsanitize=address,undefined
  CFLAGS   += ${CFLAGS.D} -std=c2x
  CXXFLAGS += ${CFLAGS.D}
else
  CFLAGS += -O3 -flto=auto -fno-stack-protector
endif

CPPFLAGS += -I${SOURCE.d} -I${OBJECT.d} -I${LIB.d}
LDLIBS := -ldictate -lqx -lpcre2-8 $$(pkg-config --cflags --libs tcl tk)

# --- Rule Section ---
all: ${OUT}

${OUT}: ${OBJECT}
	${LINK.cpp} -o $@ $(addprefix ${OBJECT.d}/,${OBJECT}) ${LDLIBS}

%.o: %.c
	${COMPILE.c} -o ${OBJECT.d}/$@ $<

%.o: %.cpp
	${COMPILE.cpp} -o ${OBJECT.d}/$@ $<

test:
	cmdtest --fast

clean:
	-${RM} $(or ${OBJECT.d},#)/*
	-${RM} ${OUT}
