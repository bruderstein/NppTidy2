# HTML Tidy 2 Plugin for Notepad++ 

This is a quick and dirty plugin to add easy HTML tidying to Notepad++, given the few issues reported with the existing Tidy plugin.

This uses the excellent tidy-html5 from [https://github.com/htacg/tidy-html5][1] Make sure you update the tidy-html4 module, to get the latest source.

The code desperately needs a tidy up, this is a few hours work just to solve the current problems.

[1]:https://github.com/htacg/tidy-html5

## Building

The build is now done using [CMake][2] to generate native build files. It supports most version of MSVC.

 1. cd build
 2. cmake ..
 3. cmake --build . --config Release

[2]:http://www.cmake.org/download/
 
## Installation

Just copy the Tidy2.dll to your Notepad++\plugins directory. It is a good idea the backup any current Tidy2.dll in case your build is rejected by Notepad++ ;=() And remember you may have to do this copying with `administartion` priviledges, depending on where Notepad++ is installed.

Ideally add quickref.html which should be in the tidy-html5\documentation folder to the doc folder to Notepad++\plugins directory too, so you will have the correct documentation, and the "Show Config Help" option will show the documentation that applies to your version (rather than simply the latest on the web).

Further, the config folder contains 3 sample tidy config files. These should be checked, and copied to `C:\Users\<user>\AppData\Roaming\Notepad++\plugins\config\.`. They can be edited in NotePad++ to contain what ever you want.

; eof
