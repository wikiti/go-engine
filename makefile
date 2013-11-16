CC=g++
CFLAGS=-c -Iengine\include -Iuser\include
LDFLAGS=-Wall -lboost_serialization-mgw48-1_54 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lglu32 -lSOIL -lglew32 -lopengl32 -lassimp
SOURCES=engine\src\_globals.cpp engine\src\_object.cpp engine\src\_systems.cpp engine\src\components\_component.cpp engine\src\components\_component_camera.cpp engine\src\components\_component_dummys.cpp engine\src\components\_component_transform.cpp  engine\src\components\_component_mesh_render.cpp engine\src\engine\_engine.cpp engine\src\engine\_instance.cpp engine\src\systems\_data.cpp engine\src\systems\_debug.cpp engine\src\systems\_manager.cpp engine\src\systems\_other.cpp engine\src\systems\_render.cpp engine\src\systems\_resource.cpp engine\src\_main.cpp user\src\gameObjects.cpp user\src\instances.cpp

OBJECTS=$(SOURCES:.cpp=.o)
OUT_OBJECTS := $(foreach object,$(OBJECTS),bin\src\$(object))
EXECUTABLE=bin\GoEngine++
OBJDIR=bin\src

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OUT_OBJECTS) $(LDFLAGS) $(PATHFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(PATHFLAGS) $< -o $(OBJDIR)\$@
	
clean:
	del $(OUT_OBJECTS) $(EXECUTABLE)

clear:
	del $(OUT_OBJECTS)
	
doxy:
	doxygen documentation.Doxyfile