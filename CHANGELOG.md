# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [v2.47] - 2023-05-08

### Changes [RPM]

* Integrated/Borrowed XOSL EXESPLIT routine for generation of XXF files
* Extended EXESPLIT to contain size of last part and number of parts in header
* Adopted HELPER to specify source translation file
* Splitted translation file to multiple translation files (to get codepage per file correct)
* Added/Reintegrated translation for PART_CMD
* Completed german translation
* Removed unused translation terms for all languages
* Increased version to reflect the major changes

### Changes [BuildEnvironment]

* Updated Powershell Building/Cleaning
* Removed unused files (C, ASM, BAT)
* Reimplemented MAKEFILE to enable the buildung of all language translations
* Removed unused / only partially available targets in MAKEFILE
* Added automatic binary compression of all build targets including packaging in zip files for release
* Updated Borland build tools (non version change) and add documentation
* Updated most filenames and translation selectors to uppercase
* Updated .gitignore to reflect all changes
* Updated documentation

## [v2.46] - 2023-05-01

### Changes [RPM]

* -None-

### Changes [BuildEnvironment]

* Removed docker container and added cleanup/build environment for windows
  hosts only (no docker, no wine -> so no *NIX support within this toolchain).
* Replaced DOSBox with DOSBox-X
* Updated ToolChain to Borland C++ 5.0 (with exact versions which have been
  used by original author(s) build-pipeline)
* Reused original EXE compression method (PKLITE) in build-pipeline to reduce
  size of generated binaries - but the compressed version will NOT be used for
  XOSL (as decompression fails when executed inside of it)

## [v2.46] - 2020-06-23

### Changes [RPM]

* -None-

### Changes [BuildEnvironment]

* The Docker container used to run the toolchain is now based on
  `opensuse/leap:15.2`, as the Arch Linux based one used so far has
  issue with running DOS executables via Wine.

## [v2.46] - 2019-11-02

### Changes [RPM]

* Replace literal magic numbers with constants.
* MBR can be saved and restored either from the GUI or from the command line.
* CMOS data can be saved and restored either from the GUI or from the command
  line.
* Added contributor information.

### Changes [BuildEnvironment]

* Wine/DosBox based build system.
* Dockerfile for building helper container.