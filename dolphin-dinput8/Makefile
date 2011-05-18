WINE_INCLUDE_PATH="/home/lucas/gsoc/wine-source/include"
CC=i586-mingw32msvc-c++ -I$(WINE_INCLUDE_PATH) #-IPluginSpecs/

OBJECTS = DIHandler.o MultiDI.o

dolphin-plugin.exe: main.cpp $(OBJECTS)
	$(CC)  -o $@ $^ -Wall -lole32 -ldxguid -ldxerr9 -ldinput8 -lodbc32 -lodbccp32

#dolphin-plugin.dll: dolphin-plugin.o $(OBJECTS)
#	$(CC)  -shared -o $@ $^ -Wl --out-implib -lole32 -ldxguid -ldxerr9 -ldinput8 -lodbc32 -lodbccp32

#dolphin-plugin.o: main.cpp
#	$(CC)  -c -o $@ main.cpp

%.o: %.cpp
	$(CC)  -c -o $@ $^

copy:
	cp dolphin-plugin.exe ~/Desktop/Dropbox/windows
