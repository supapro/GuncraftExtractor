Guncraft Extractor [![status](https://travis-ci.org/leha-bot/GuncraftExtractor.svg?branch=master)](https://travis-ci.org/leha-bot/GuncraftExtractor/)[![Build status](https://ci.appveyor.com/api/projects/status/cu8918dwdx6u1kft?svg=true)](https://ci.appveyor.com/project/leha-bot/guncraftextractor)
==================

This tool is made for modding the texture and sound files of Guncraft, but you can use it for other games too with little to no modifications.

# Usage
Drag and drop the file(s) you want to convert onto the executable, it will detect the file types and convert them accordingly. The converted file will have the same name as the original file, but with another extension. This program overwrite files without a warning so be sure to have a backup.

# Build
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

# Note on Android NDK / some MinGW / etc with lame C++11 stdlib
	You should set option USE\_TO\_STRING\_HACK (cmake -DUSE\_TO\_STRING\_HACK ..) and clone hackish implementation of std::to\_string from https://github.com/leha-bot/to\_string (git submodule update)

# Possible convertions
- Xnb Texture2D <---> Png
- Xnb SoundEffect <---> Wav
