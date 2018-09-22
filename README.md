# jrasm - Assembler for JR-200 

This is an assembler of Motorola 6800 MPU that features some functions useful to develop
programs for JR-200, a micro computer made by Panasonic in 1983.

## Build for Windows

Open the solution file `jrasm.sln` with Visual Studio 2017 and build it.
The executable `jrasm.exe` will be produced in the following directories
according to the selected configuration:

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

## Command Line

The execution format of `jrasm` is:

```
jrasm [options] src
```

Available options are:

Long Format        |Short format|Function
-------------------+------------+-------------------------------------------------------------------
`--output=file`    |`-o file`   |Specifies the filename to output.
`--print-disasm-l` |`-d`        |Prints a disassembler dump of the product in lower case.
`--print-disasm-l` |`-d`        |Prints a disassembler dump of the product in upper case.
`--print-hexdump-l`|`-x`        |Prints a hexadecimal dump of the product in lower case.
`--print-hexdump-u`|`-X`        |Prints a hexadecimal dump of the product in upper case.
`--print-list-l`   |`-l`        |Prints a list of labels in lower case.
`--print-list-u`   |`-L`        |Prints a list of labels in upper case.
`--print-memory-l` |`-m`        |Prints a memory image in lower case.
`--print-memory-u` |`-M`        |Prints a memory image in upper case.
`--verbose`        |`-v`        |Reports various things.
`--help`           |`-h`        |Prints help message.

When you launch the program with a source file, it will generate the product of CJR format that
can be loaded to VJR-200, a JR-200 emulator.


## Directive

The jrasm assembler supports following directives:

### `.cseg`

### `.db`, `.dw`
