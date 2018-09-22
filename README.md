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

## First Step

Consider the following source file named `helloworld.asm`:

```
        .ORG    0x2000
loop:
        LDX     [ptr_src]
        LDAA    [x]
        INX
        STX     [ptr_src]
        CMPA    0x00
        BEQ     done
        LDX     [ptr_dst]
        STAA    [x]
        INX
        STX     [ptr_dst]
        BRA     loop
done:
        RTS
ptr_src:
        .DW     hello_world
ptr_dst:
        .DW     0xc100 + 9 + 20 * 0x20
hello_world:
        .DB     "Hello, world!", 0
```

To assemble it, just launch `jrasm` with the source file name.

```
$ jrasm helloworld.asm
```

This creates a CJR file named `helloworld.cjr`. You can load the CJR file to JR-200 emulator or
convert it into a WAV file that is fed to a real machine through a cassette recorder interface.

After loading it, you can call the program using JR-200 BASIC like follows:

```
U=USR($2000)
```

You see "Hello, world!" on the screen? Congratulations!

Specifying `-D` options prints the produced binary data with corresponding assembler codes
to the standard output.

```
$ jrasm -D helloworld.asm
```

The result is:

```
loop:
    2000 FE 20 19  LDX  [ptr_src]
    2003 A6 00     LDAA [X]
    2005 08        INX  
    2006 FF 20 19  STX  [ptr_src]
    2009 81 00     CMPA 0x00
    200B 27 0B     BEQ  done
    200D FE 20 1B  LDX  [ptr_dst]
    2010 A7 00     STAA [X]
    2012 08        INX  
    2013 FF 20 1B  STX  [ptr_dst]
    2016 20 E8     BRA  loop
done:
    2018 39        RTS  
ptr_src:
    2019 20 1D     .DW hello_world
ptr_dst:
    201B C3 89     .DW 0xC100+0x09+0x14*0x20
hello_world:
    201D 48 65     .DB "Hello, world!",0x00
    201F 6C 6C     
    2021 6F 2C     
    2023 20 77     
    2025 6F 72     
    2027 6C 64     
    2029 21 00  
```


## Command Line

The execution format of `jrasm` is:

```
jrasm [options] src
```

Available options are:

|Long Format        |Short format|Function                                                           |
|-------------------|------------|-------------------------------------------------------------------|
|`--output=file`    |`-o file`   |Specifies the filename to output.                                  |
|`--print-disasm-l` |`-d`        |Prints a disassembler dump of the product in lower case.           |
|`--print-disasm-l` |`-d`        |Prints a disassembler dump of the product in upper case.           |
|`--print-hexdump-l`|`-x`        |Prints a hexadecimal dump of the product in lower case.            |
|`--print-hexdump-u`|`-X`        |Prints a hexadecimal dump of the product in upper case.            |
|`--print-list-l`   |`-l`        |Prints a list of labels in lower case.                             |
|`--print-list-u`   |`-L`        |Prints a list of labels in upper case.                             |
|`--print-memory-l` |`-m`        |Prints a memory image in lower case.                               |
|`--print-memory-u` |`-M`        |Prints a memory image in upper case.                               |
|`--verbose`        |`-v`        |Reports various things.                                            |
|`--help`           |`-h`        |Prints help message.                                               |

When you launch the program with a source file, it will generate a product in CJR format
that can be loaded to VJR-200, a JR-200 emulator.


## Literal

A string literal consists of a series of characters surrounded by a pair of double quotations: eg. `"Hello World"`

The format of number literal is as follows:

- Decimal number ... Begins with `1` to `9` and cosists of digit characters: eg) `123`
- Hexadecimal number ... Begins with `0x` and consists of digit and `A` to `F` characters: eg) `0x3a22`
- Octal number ... Begins with `0` and consts of `0` to `7` characters: eg) `0327`

A sybmol literal consists of a series of characters and is used for following purposes:

- Label.
- Instruction's operation code.
- Directive's name.
- Register name.


## Directive

The jrasm assembler supports following directives:

### .CSEG, .DSEG

Example:
```
        .CSEG
        .DSEG
```

### .DB, .DW

These directive are used to store binary data. The directive `.DB` contains 8-bit data while `.DW` does 16-bit.

Directive `.DB` accepts string literal as well.

Example:
```
        .DB     0x00, 0x01, "ABC"
        .DW     0x1234
```

When a string literal appears in the directive, a null terminate character is NOT appended.
So, for examle, `"ABC"` in the direcive is equivalent to a sequence of `0x41`, `0x42`, `0x43`

### .FILENAME.JR

Example:
```
        .FILENAME.JR "Hello"
```


### .ORG

It specifies the current address where program or data is to be stored.
You must specify at least one `.ORG` directive before any source code that generates binary data appears.

Example:

```
        .ORG    0x2000
```

## Instructions

|Syntax              |Another Syntax      |Operation                                                          |
|--------------------|--------------------|-------------------------------------------------------------------|
|`ABA`               |                    |`[A] <- [A] + [B]`                                                 |
|`ADCA data8`        |`ADC A,data8`       |`[A] <- [A] + data8 + C`                                           |
|`ADCA {addr8}`      |`ADC A,{addr8}`     |`[A] <- [A] + {addr8} + C`                                         |
|`ADCA [X+data8]`    |`ADC A,[X+data8]`   |`[A} <- [A] + [data8+[X]] + C`                                     |
