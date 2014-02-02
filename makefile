CC=g++
CFLAGS=-c -Iengine\include -Iuser\include -std=c++11
LDFLAGS=-Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSOIL -lglew32 -lglu32 -lopengl32 -lOpenAL32 -lassimp

SOURCES=engine\src\_globals.cpp engine\src\_main.cpp engine\src\_object.cpp engine\src\_systems.cpp engine\src\components\_component.cpp engine\src\components\_component_audio.cpp engine\src\components\_component_camera.cpp engine\src\components\_component_dummys.cpp engine\src\components\_component_gui.cpp engine\src\components\_component_mesh_render.cpp engine\src\components\_component_particle_emitter.cpp engine\src\components\_component_transform.cpp engine\src\engine\_engine.cpp engine\src\engine\_instance.cpp engine\src\systems\_data.cpp engine\src\systems\_debug.cpp engine\src\systems\_input.cpp engine\src\systems\_manager.cpp engine\src\systems\_mixer.cpp engine\src\systems\_other.cpp engine\src\systems\_render.cpp engine\src\systems\_resource.cpp engine\src\systems\_shader.cpp user\src\gameObjects.cpp user\src\instances.cpp user\src\predefined_functions.cpp user\src\particles\other_particles.cpp user\src\particles\fireworks\fireworks.cpp user\src\particles\rainbow\rainbow.cpp user\src\random\random_stuff.cpp user\src\render\mesh.cpp user\src\shaders\shaders.cpp

OBJECTS=$(SOURCES:.cpp=.o)
OUT_OBJECTS := $(foreach object,$(OBJECTS),Debug\$(object))
EXECUTABLE=Debug\go-engine.exe
OBJDIR=Debug

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OUT_OBJECTS) $(LDFLAGS) $(PATHFLAGS) -o $@
    
	cp $(EXECUTABLE) go-engine.exe

link:
	$(CC) $(OUT_OBJECTS) $(LDFLAGS) $(PATHFLAGS) -o $@
    
.cpp.o:
	$(CC) $(CFLAGS) $(PATHFLAGS) $< -o $(OBJDIR)\$@
	
clean:
	del $(OUT_OBJECTS) $(EXECUTABLE)

clear:
	del $(OUT_OBJECTS)
	
doxy:
	doxygen documentation.Doxyfile

findsources:
	dir *.cpp /b/s