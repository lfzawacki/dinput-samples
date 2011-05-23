WINE_INCLUDE_PATH="~/gsoc/wine-source/include"
CC=i586-mingw32msvc-c++ -I$(WINE_INCLUDE_PATH)

joystick: joystick.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid

configure_devices: configure_devices.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid -lgdi32

dump_devices: dump_devices.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid

enum_semantics: enum_semantics.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid

enum_keyboard: enum_keyboard.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid

setactionmap: setaction.cpp
	$(CC)  -o $@.exe $^ -ldinput8 -ldxguid

copy:
	cp *.exe  ~/Desktop/Dropbox/windows

clean:
	rm *.exe

