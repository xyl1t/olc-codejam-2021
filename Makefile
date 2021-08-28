### Makefile ###

# Usage: 
# make 		 -> compile project as executable
# make debug -> compile project in debug mode
# make web   -> compile project for web
# make clean -> clean up build

# clang++ -arch x86_64 -std=c++17 main.cpp -Iinclude -Llib -framework OpenGL -framework GLUT -framework Carbon -lpng -lSDL2 -lSDL2_mixer -lsoloud_static -ldl -o pge
# em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 -Iinclude main.cpp soloud.o -o pge.html --preload-file ./assets

### Variables ###
PROJECTNAME := pge
SRCDIR := src
BUILDDIR := build
PROJECT := $(BUILDDIR)/$(PROJECTNAME)
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
# CXX := clang++
CXXFLAGS := -arch x86_64 -std=c++17 -Wall -Iinclude
LDFLAGS := -Llib
LDLIBS := -framework OpenGL -framework GLUT -framework Carbon -lpng -lsoloud_static -lbox2d
# Include dependencies
deps := $(patsubst %.o,%.d,$(OBJS))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

### Phony targets ###
.PHONY: all multi clean debug web
.DEFAULT_GOAL := multi
multi: 
	$(MAKE) -j8 all
all: $(PROJECT)

debug: CXXFLAGS += -DDEBUG -g
debug: $(PROJECT)

web: CXX := em++
web: CXXFLAGS := -std=c++17 -O2 -Iinclude
web: LDLIBS := -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 -lbox2d --preload-file ./assets $(SRCDIR)/soloud.o
web: PROJECT := $(BUILDDIR)/$(PROJECTNAME).html
web: $(PROJECT)

### Compilation ###
# Compile project
$(PROJECT) : $(OBJS)
	@echo Bulding $(PROJECT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $(PROJECT)
	@echo $(PROJECT) built successfully

# Compile .o files
$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# clean build
clean: 
	@rm -rf build/*
