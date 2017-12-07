# HTML Tidy 2 Plugin for Notepad++ 

This is a quick and dirty plugin to add easy HTML tidying to Notepad++, given the few issues reported with the existing Tidy plugin.

This uses the excellent tidy-html5 from [https://github.com/htacg/tidy-html5][1] Make sure you update the `tidy-html5` module, to get the latest source, **OR** build using a separate clone, build and install of HTML Tidy.

The code desperately needs a tidy up, this is a few hours work just to solve the current problems.

[1]:https://github.com/htacg/tidy-html5

## Building

The build is now done using [CMake](http://www.cmake.org/download/) to generate native build files. It supports most version of MSVC. Use `cmake --help` to show the **generators** available on your platform.

 1. cd build
 2. cmake ..
 3. cmake --build . --config Release

It seems in general cmake, used simply as above, will normally default to build in 32-bits. So **extra** steps are required for a **64-bit** build...

 1. Establish MSVC x64 command prompt
 2. cd build.x64
 3. cmake .. -G "Visual Studio 14 Win64" [OPTIONS]
 4. cmake --build . --config Release

Usually there is a shortcut for the MSVC x64 command prompt, but if not it can be established by locating the `vcvarsall.bat` in the MSVC install directories, and running it with the appropriate architecture, like `AMD64`. Normally this is the value in the `PROCESSOR_ARCHITECTURE` in the environment... run `set`...

And it is important to tell cmake which **generator** to use. The above is for MSVC 14, 2015.

The additional `OPTIONS` can be used to allow cmake, using the `FindTidy.cmake` in `CMakeModules\` to find your install of tidy. 

This can be `-DCMAKE_INSTALL_PREFIX=d:\path\to\tidy\install`, and/or `-DCMAKE_PREFIX_PATH=d:\path\to\tidy\install`, and/or `-DTIDY_ROOT=d:\path\to\tidy\install`... The `TIDY_ROOT` can also be set in the environment...
 
This should succeed in building a **64-bit** `Tidy2.dll`, which includes **64-bit** `tidys.lib`, for use with **64-bit** NPP...

## Installation

Just copy the Tidy2.dll to your Notepad++\plugins directory. It is a good idea the backup any current Tidy2.dll in case your build is rejected by Notepad++ ;=() And remember you may have to do this copying with `administration` priviledges, depending on where Notepad++ is installed.

Ideally add quickref.html which should be in the `Notepad++\plugins\doc\Tidy2\` folder, so you will have the correct documentation, and the "Show Config Help" option will show the documentation that applies to your version (rather than simply the latest on the web).

Further, the config folder contains 3 sample tidy config files. These should be checked, and copied to `C:\Users\<user>\AppData\Roaming\Notepad++\plugins\config\.`. They can be edited in NotePad++ to contain what ever tidy options you want.

; eof 20170519

