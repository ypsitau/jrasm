# jrasm - Assembler for JR-200 

This is an assembler of Motorola 6800 MPU that features some functions useful to develop
programs for JR-200, a micro computer made by Panasonic in 1983.

## Build for Windows

Open the solution file `jrasm.sln` with Visual Studio 2017 and build it.
The executable `jrasm.exe` will be produced in the following directories according to the configuration:

- `x64\Release` .. 64-bit executable for release.
- `x64\Debug` .. 64-bit executable for debug purpose.
- `x86\Release` .. 32-bit executable for release.
- `x86\Debug` .. 32-bit executable for debug purpose.

## Build for Mac and Linux

Execute the following commands:

```
$ mkdir build
$ cd build
$ ../configure
$ make
```

