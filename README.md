Guncraft Extractor ![status](https://travis-ci.org/leha-bot/GuncraftExtractor.svg?branch=master)
==================

This tool is made for modding the texture and sound files of Guncraft, but you can use it for other games too with little to no modifications.

# Usage
Drag and drop the file(s) you want to convert onto the executable, it will detect the file types and convert them accordingly. The converted file will have the same name as the original file, but with another extension. This program overwrite files without a warning so be sure to have a backup.

# Build
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

# Possible convertions
- Xnb Texture2D <---> Png
- Xnb SoundEffect <---> Wav
