# README.tidy.md

THe first search is for an extern tidy library, if USE_EXTERNAL_TIDY is left ON by default.

If such an 'external' tidy library is NOT found, then the source in the submodule will be used.

To use the tidy-html5 submodule, on the first clone of this repo need to use -

`git submodule init`

To update to the latest tidy-html5 source...

`git submodule foreach git pull origin master`

This did nothing: `git submodule update --recursive`

; eof
