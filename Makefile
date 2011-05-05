
WINE_INCLUDE_PATH="~/gsoc/wine-source/include"
CC=i586-mingw32msvc-c++ -I$(WINE_INCLUDE_PATH)

joystick:
	$(CC)  -o $@.exe $@.cpp -ldinput8 -ldxguid

configure_devices:
	$(CC)  -o $@.exe $@.cpp -ldinput8 -ldxguid -lgdi32

dump_devices:
	$(CC)  -o $@.exe $@.cpp -ldinput8 -ldxguid

enum_semantics:
	$(CC)  -o $@.exe $@.cpp -ldinput8 -ldxguid

copy:
	cp *.exe  ~/Desktop/Dropbox/windows

