#Compiler and Linker
CC          := gcc

#The Target Binary Program
TARGET      := cppcanfd.out
TARGET_LIB	:= libcppcanfd.a


#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -g
LIB         := -leci116DriverLinux-usb-1.0
INC         := 

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

SOURCES_LIB := src/CanBitrate.cpp \
		src/CanController.cpp \
		src/CanMessage.cpp \
		src/LinuxCanTranslater.cpp
HEADERS = src/CanBitrate.hh \
		src/CanController.hh \
		src/CanOperatingModes.hh \
		src/CanMessage.hh

OBJECTS_LIB	:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES_LIB:.$(SRCEXT)=.$(OBJEXT)))


#Defauilt Make
all: directories $(TARGET)

lib: directories $(TARGET_LIB)

#Remake
re: fclean all

relib: fclean lib

install: 
	cp $(TARGETDIR)/$(TARGET_LIB) /usr/lib32/
	cp $(HEADERS) /usr/local/include/

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
fclean: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

$(TARGETDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

$(TARGETDIR)/$(TARGET_LIB): $(OBJECTS_LIB)
	ar rcs -o $(TARGETDIR)/$(TARGET_LIB) $(OBJECTS_LIB)

$(TARGET): $(TARGETDIR)/$(TARGET)
#	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)
$(TARGET_LIB): $(TARGETDIR)/$(TARGET_LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
