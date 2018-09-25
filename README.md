# jrasm - Assembler for JR-200 

This is an assembler of Motorola 6800 MPU that features some functions useful to develop
programs for JR-200, a micro computer made by Panasonic in 1983.


## Build for Windows

Open the solution file `jrasm.sln` with Visual Studio 2017 and build it.
The executable `jrasm.exe` will be produced in the following directories
according to the selected configuration:

- `x64\Release` ... 64-bit executable for release.
- `x64\Debug` ... 64-bit executable for debug purpose.
- `x86\Release` ... 32-bit executable for release.
- `x86\Debug` ... 32-bit executable for debug purpose.


## Build for Mac and Linux

Execute the following commands:

```
$ mkdir build
$ cd build
$ ../configure
$ make
```


## Let's Try It

Consider the following source file named `helloworld.asm`:

```
        .ORG    0x2000
loop:
        LDX     [ptr_src]
        LDAA    [X]
        INX
        STX     [ptr_src]
        CMPA    0x00
        BEQ     done
        LDX     [ptr_dst]
        STAA    [X]
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

### String

A string literal consists of a series of characters surrounded by a pair of double quotations (e.g. `"Hello World"`).

A string literal can contain escape characters listed below:

- `\0`
- `\r`
- `\n`

### Number

The format of number literal is as follows:

- Decimal number ... Begins with `1` to `9` and cosists of digit characters (e.g. `123`).
- Hexadecimal number ... Begins with `0x` and consists of digit and `A` to `F` characters (e.g. `0x3a22`).
- Octal number ... Begins with `0` and consts of `0` to `7` characters (e.g. `0327`).

### Symbol

A sybmol literal consists of a series of characters and is used for following purposes:

- Label.
- Instruction's operation code.
- Directive's name.
- Register name.

A symbol is case insensitive.
This means that you can describe an instruction `LDAA` with a symbol `LDAA`, `ldaa`, `Ldaa`, `LdAA` and so on.
Also, when you define a label named `Label1`, it can be referred to as `label1`, `LABEL1`, `LaBel1`
and anything like that.

### Bit-Pattern

A string preceded by "`b`" is a bit-pattern literal that consists of a series of ascii characters
that correspond to binary data `0` and `1`.
The space, period '`.`', comma '`,`', under score '`_`' and hyphen '`-`' are recognized as `0` and
others are `1`.
The bit-pattern literal generates a sequence of byte-sized data like follows:


```
        .db     b"#.#.#.#."                                 ; 0xaa
        .db     b"#.#......##...#."                         ; 0xa0, 0x62
        .db     b"#.#......##...#.#.#.#.#.########..#....." ; 0xa0, 0x62, 0xaa, 0xff, 0x20
```

## Directive

The jrasm assembler supports following directives:


### .CSEG, .DSEG and .ISEG

The directives `.CSEG` and `.DSEG` declare the beginning of code and data segment respectively.
They don't put any restriction on what items are place in: you can write data sequence using directive `.DB`
in the code segment and can put instructions in the data segment as well.
It could be used to put some data sequence in the middle of program code like follows:

```
        .CSEG
        LDX     Hello
        .DSEG
Hello:  "Hello"
        .CSEG
        LDAA    [X]
        ; ... any jobs ...
```

The assembler program is pleced in the code segment before any `.CSEG` or `.DSEG` directive appears.
Each segment must have the current address initialized using `.ORG` directive.

Directive `.ISEG` ...

Example:
```
        .ISEG
```


### .DB and .DW

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

Specivies a filename up to 16 charactesrs that is to be stored in CJR file.
This name is displayed when you run `LOAD` or `MLOAD` command in JR BASIC environment.
If this directive is omitted, the name of the assembler source file will be stored.

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

You can specify more than one `.ORG` directive in a program.


### .PROC and .ENDP

You can localize labels by using `.PROC` and `.ENDP` directives.
Any labels that appear in these directives are hidden from outside.
It's possible to specify the same symbol for the global and the local labels,
which are dealt with as different ones.

```
label1: .EQU    0x1111
label2: .EQU    0x2222

        .PROC
label1: .EQU    0x1234   ; not visible from outside
        .DW     label1   ; 0x1234
        .DW     label2   ; 0x2222
        .ENDP

        .PROC
label1: .EQU    0x5678   ; not visible from outside
        .DW     label1   ; 0x5678
        .DW     label2   ; 0x2222
        .ENDP

        .DW     label    ; 0x1111
        .DW     label2   ; 0x2222
```

Even in the localized region, labels declared with double-colon `::` will be defined as global ones.

```
         .PROC
label1:  .EQU    0x1234   ; not visible from outside
label2:: .EQU    0x5678   ; visible from outside
         .DW     label1   ; 0x1234
         .DW     label2   ; 0x5678
         .ENDP

         .DW     label2   ; 0x5678
```


## Instructions

Here is a list of the assembler's syntax for M6800 instructions.

|Syntax              |Another Syntax      |Operation                                                 |
|--------------------|--------------------|----------------------------------------------------------|
|`ABA`               |                    |`A<-A+B`                                                  |
|`ADCA data8`        |`ADC A,data8`       |`A<-A+data8+C`                                            |
|`ADCA {addr8}`      |`ADC A,{addr8}`     |`A<-A+{addr8}+C`                                          |
|`ADCA [X+data8]`    |`ADC A,[X+data8]`   |`A<-A+[X+data8]+C`                                        |
|`ADCA [addr16]`     |`ADC A,[addr16]`    |`A<-A+[addr16]+C`                                         |
|`ADCB data8`        |`ADC B,data8`       |`B<-B+data8+C`                                            |
|`ADCB {addr8}`      |`ADC B,{addr8}`     |`B<-B+{addr8}+C`                                          |
|`ADCB [X+data8]`    |`ADC B,[X+data8]`   |`B<-B+[X+data8]+C`                                        |
|`ADCB [addr16]`     |`ADC B,[addr16]`    |`B<-B+[addr16]+C`                                         |
|`ADDA data8`        |`ADD A,data8`       |`A<-A+data8`                                              |
|`ADDA {addr8}`      |`ADD A,{addr8}`     |`A<-A+{addr8}`                                            |
|`ADDA [X+data8]`    |`ADD A,[X+data8]`   |`A<-A+[X+data8]`                                          |
|`ADDA [addr16]`     |`ADD A,[addr16]`    |`A<-A+[addr16]`                                           |
|`ADDB data8`        |`ADD B,data8`       |`B<-B+data8`                                              |
|`ADDB {addr8}`      |`ADD B,{addr8}`     |`B<-B+{addr8}`                                            |
|`ADDB [X+data8]`    |`ADD B,[X+data8]`   |`B<-B+[X+data8]`                                          |
|`ADDB [addr16]`     |`ADD B,[addr16]`    |`B<-B+[addr16]`                                           |
|`ANDA data8`        |`AND A,data8`       |`A<-A&data8`                                              |
|`ANDA {addr8}`      |`AND A,{addr8}`     |`A<-A&{addr8}`                                            |
|`ANDA [X+data8]`    |`AND A,[X+data8]`   |`A<-A&[X+data8]`                                          |
|`ANDA [addr16]`     |`AND A,[addr16]`    |`A<-A&[addr16]`                                           |
|`ANDB data8`        |`AND B,data8`       |`B<-B&data8`                                              |
|`ANDB {addr8}`      |`AND B,{addr8}`     |`B<-B&{addr8}`                                            |
|`ANDB [X+data8]`    |`AND B,[X+data8]`   |`B<-B&[X+data8]`                                          |
|`ANDB [addr16]`     |`AND B,[addr16]`    |`B<-B&[addr16]`                                           |
|`ASLA`              |`ASL A`             |Arithmetic shift left on `A`, bit 0 is set to `0`         |
|`ASLB`              |`ASL B`             |Arighmatic shift Left on `B`, bit 0 is set to `0`         |
|`ASL [X+data8]`     |`ASL [X+data8]`     |Arithmetic shift left on `[X+data8]`, bit 0 is set to `0` |
|`ASL [addr16]`      |`ASL [addr16]`      |Arithmetic shift left on `[addr16]`, bit 0 is set to `0`  |
|`ASRA`              |`ASR A`             |Arithmetic shift right on `A`, bit 0 is set to `0`        |
|`ASRB`              |`ASR B`             |Arighmatic shift right on `B`, bit 0 is set to `0`        |
|`ASR [X+data8]`     |`ASR [X+data8]`     |Arithmetic shift right on `[X+data8]`, bit 0 is set to `0`|
|`ASR [addr16]`      |`ASR [addr16]`      |Arithmetic shift right on `[addr16]`, bit 0 is set to `0` |
|`BCC disp`          |                    |if `C=0` then `PC<-PC+disp+2`                             |
|`BCS disp`          |                    |if `C=1` then `PC<-PC+disp+2`                             |
|`BEQ disp`          |                    |if `Z=1` then `PC<-PC+disp+2`                             |
|`BGE disp`          |                    |if `N^V=0` then `PC<-PC+disp+2`                           |
|`BGT disp`          |                    |if `(N^V)\|Z=0` then `PC<-PC+disp+2`                      |
|`BHI disp`          |                    |if `C\|Z=0` then `PC<-PC+disp+2`                          |
|`BITA data8`        |`BIT A,data8`       |`A&data8`                                                 |
|`BITA {addr8}`      |`BIT A,{addr8}`     |`A&{addr8}`                                               |
|`BITA [X+data8]`    |`BIT A,[X+data8]`   |`A&[X+data8]`                                             |
|`BITA [addr16]`     |`BIT A,[addr16]`    |`A&[addr16]`                                              |
|`BITB data8`        |`BIT B,data8`       |`B&data8`                                                 |
|`BITB {addr8}`      |`BIT B,{addr8}`     |`B&{addr8}`                                               |
|`BITB [X+data8]`    |`BIT B,[X+data8]`   |`B&[X+data8]`                                             |
|`BITB [addr16]`     |`BIT B,[addr16]`    |`B&[addr16]`                                              |
|`BLE disp`          |                    |if `(N^V)\|Z=1` then `PC<-PC+disp+2`                      |
|`BLS disp`          |                    |if `C\|Z=1` then `PC<-PC+disp+2`                          |
|`BLT disp`          |                    |if `N^V=1` then `PC<-PC+disp+2`                           |
|`BMI disp`          |                    |if `N=1` then `PC<-PC+disp+2`                             |
|`BNE disp`          |                    |if `Z=0` then `PC<-PC+disp+2`                             |
|`BPL disp`          |                    |if `N=0` then `PC<-PC+disp+2`                             |
|`BRA disp`          |                    |`PC<-PC+disp+2`                                           |
|`BVC disp`          |                    |if `V=0` then `PC<-PC+disp+2`                             |
|`BVS disp`          |                    |if `V=1` then `PC<-PC+disp+2`                             |
|`CBA`               |                    |A-B                                                       |
|`CLC`               |                    |C<-0                                                      |
|`CLI`               |                    |I<-0                                                      |
|`CLRA`              |`CLR A`             |`A<-0`                                                    |
|`CLRB`              |`CLR B`             |`B<-0`                                                    |
|`CLR [X+data8]`     |`CLR [X+data8]`     |`[X+data8]<-0`                                            |
|`CLR [addr16]`      |`CLR [addr16]`      |`[addr16]<-0`                                             |
|`CLV`               |                    |`V<-0`                                                    |
|`CMPA data8`        |`CMP A,data8`       |`A-data8`                                                 |
|`CMPA {addr8}`      |`CMP A,{addr8}`     |`A-{addr8}`                                               |
|`CMPA [X+data8]`    |`CMP A,[X+data8]`   |`A-[X+data8]`                                             |
|`CMPA [addr16]`     |`CMP A,[addr16]`    |`A-[addr16]`                                              |
|`CMPB data8`        |`CMP B,data8`       |`B-data8`                                                 |
|`CMPB {addr8}`      |`CMP B,{addr8}`     |`B-{addr8}`                                               |
|`CMPB [X+data8]`    |`CMP B,[X+data8]`   |`B-[X+data8]`                                             |
|`CMPB [addr16]`     |`CMP B,[addr16]`    |`B-[addr16]`                                              |
|`COMA`              |`COM A`             |`A<-0xff-A`                                               |
|`COMB`              |`COM B`             |`B<-0xff-B`                                               |
|`COM [X+data8]`     |`COM [X+data8]`     |`[X+data8]<-0xff-[X+data8]`                               |
|`COM [addr16]`      |`COM [addr16]`      |`[addr16]<-0xff-[addr16]`                                 |
|`CPX {addr8}`       |                    |`X(hi)-{addr8}, X(lo)-{addr8+1}`                          |
|`CPX [X+data8]`     |                    |`X(hi)-[X+data8], X(lo)-[X+data8+1]`                      |
|`CPX data16`        |                    |`X(hi)-data16(hi), X(lo)-data16(lo)`                      |
|`CPX [addr16]`      |                    |`X(hi)-[addr16], X(lo)-[addr16+1]`                        |
|`DAA`               |                    |Decimal adjust on `A`                                     |
|`DECA`              |`DEC A`             |`A<-A-1`                                                  |
|`DECB`              |`DEC B`             |`B<-B-1`                                                  |
|`DEC [X+data8]`     |`DEC [X+data8]`     |`[X+data8]<-[X+data8]-1`                                  |
|`DEC [addr16]`      |`DEC [addr16]`      |`[addr16]<-[addr16]-1`                                    |
|`DES`               |                    |`SP<-SP-1`                                                |
|`DEX`               |                    |`X<-X-1`                                                  |
|`EORA data8`        |`EOR A,data8`       |`A<-A^data8`                                              |
|`EORA {addr8}`      |`EOR A,{addr8}`     |`A<-A^{addr8}`                                            |
|`EORA [X+data8]`    |`EOR A,[X+data8]`   |`A<-A^[X+data8]`                                          |
|`EORA [addr16]`     |`EOR A,[addr16]`    |`A<-A^[addr16]`                                           |
|`EORB data8`        |`EOR B,data8`       |`B<-B^data8`                                              |
|`EORB {addr8}`      |`EOR B,{addr8}`     |`B<-B^{addr8}`                                            |
|`EORB [X+data8]`    |`EOR B,[X+data8]`   |`B<-B^[X+data8]`                                          |
|`EORB [addr16]`     |`EOR B,[addr16]`    |`B<-B^[addr16]`                                           |
|`INCA`              |`INC A`             |`A<-A+1`                                                  |
|`INCB`              |`INC B`             |`B<-B+1`                                                  |
|`INC [X+data8]`     |`INC [X+data8]`     |`[X+data8]<-[X+data8]+1`                                  |
|`INC [addr16]`      |`INC [addr16]`      |`[addr16]<-[addr16]+1`                                    |
|`INS`               |                    |`SP<-SP+1`                                                |
|`INX`               |                    |`X<-X+1`                                                  |
|`JMP X+data8`       |                    |`PC<-X+data8`                                             |
|`JMP addr16`        |                    |`PC<-addr16`                                              |
|`JSR X+data8`       |                    |`[SP]<-PC(lo), [SP+1]<-PC(hi), SP<-SP-2, PC<-X+data8`     |
|`JSR addr16`        |                    |`[SP]<-PC(lo), [SP+1]<-PC(hi), SP<-SP-2, PC<-addr16`      |
|`LDAA data8`        |`LDA A,data8`       |`A<-data8`                                                |
|`LDAA {addr8}`      |`LDA A,{addr8}`     |`A<-{addr8}`                                              |
|`LDAA [X+data8]`    |`LDA A,[X+data8]`   |`A<-[X+data8]`                                            |
|`LDAA [addr16]`     |`LDA A,[addr16]`    |`A<-[addr16]`                                             |
|`LDAB data8`        |`LDA B,data8`       |`B<-data8`                                                |
|`LDAB {addr8}`      |`LDA B,{addr8}`     |`B<-{addr8}`                                              |
|`LDAB [X+data8]`    |`LDA B,[X+data8]`   |`B<-[X+data8]`                                            |
|`LDAB [addr16]`     |`LDA B,[addr16]`    |`B<-[addr16]`                                             |
|`LDS {addr8}`       |                    |`SP(hi)<-{addr8}, SP(lo)<-{addr8+1}`                      |
|`LDS [X+data8]`     |                    |`SP(hi)<-[X+data8], SP(lo)<-[X+data8+1]`                  |
|`LDS data16`        |                    |`SP(hi)<-data16(hi), SP(lo)<-data16(lo)`                  |
|`LDS [addr16]`      |                    |`SP(hi)<-[addr16], SP(lo)<-[addr16+1]`                    |
|`LDX {addr8}`       |                    |`X(hi)<-{addr8}, X(lo)<-{addr8+1}`                        |
|`LDX [X+data8]`     |                    |`X(hi)<-[X+data8], X(lo)<-[X+data8+1]`                    |
|`LDX data16`        |                    |`X(hi)<-data16(hi), X(lo)<-data16(lo)`                    |
|`LDX [addr16]`      |                    |`X(hi)<-[addr16], X(lo)<-[addr16+1]`                      |
|`LSRA`              |`LSR A`             |Logical shift right on `A`, bit 7 is set to `0`           |
|`LSRB`              |`LSR B`             |Logical shift right on `B`, bit 7 is set to `0`           |
|`LSR [X+data8]`     |`LSR [X+data8]`     |Logical shift right on `[X+data8]`, bit 7 is set to `0`   |
|`LSR [addr16]`      |`LSR [addr16]`      |Logical shift right on `[addr16]`, bit 7 is set to `0`    |
|`NEGA`              |`NEG A`             |`A<-0-A`                                                  |
|`NEGB`              |`NEG B`             |`B<-0-B`                                                  |
|`NEG [X+data8]`     |`NEG [X+data8]`     |`[X+data8]<-0-[X+data8]`                                  |
|`NEG [addr16]`      |`NEG [addr16]`      |`[addr16]<-0-[addr16]`                                    |
|`NOP`               |                    |No operation                                              |
|`ORAA data8`        |`ORA A,data8`       |`A<-A\|data8`                                             |
|`ORAA {addr8}`      |`ORA A,{addr8}`     |`A<-A\|{addr8}`                                           |
|`ORAA [X+data8]`    |`ORA A,[X+data8]`   |`A<-A\|[X+data8]`                                         |
|`ORAA [addr16]`     |`ORA A,[addr16]`    |`A<-A\|[addr16]`                                          |
|`ORAB data8`        |`ORA B,data8`       |`B<-B\|data8`                                             |
|`ORAB {addr8}`      |`ORA B,{addr8}`     |`B<-B\|{addr8}`                                           |
|`ORAB [X+data8]`    |`ORA B,[X+data8]`   |`B<-B\|[X+data8]`                                         |
|`ORAB [addr16]`     |`ORA B,[addr16]`    |`B<-B\|[addr16]`                                          |
|`PSHA`              |`PSH A`             |`[SP]<-A, SP<-SP-1`                                       |
|`PSHB`              |`PSH B`             |`[SP]<-B, SP<-SP-1`                                       |
|`PULA`              |`PUL A`             |`SP<-SP+1, A<-[SP]`                                       |
|`PULB`              |`PUL B`             |`SP<-SP+1, B<-[SP]`                                       |
|`ROLA`              |`ROL A`             |Rotate left through carry flag on `A`                     |
|`ROLB`              |`ROL B`             |Rotate left through carry flag on `B`                     |
|`ROL [X+data8]`     |`ROL [X+data8]`     |Rotate left through carry flag on `[X+data8]`             |
|`ROL [addr16]`      |`ROL [addr16]`      |Rotate left through carry flag on `[addr16]`              |
|`RORA`              |`ROR A`             |Rotate right through carry flag on `A`                    |
|`RORB`              |`ROR B`             |Rotate right through carry flag on `B`                    |
|`ROR [X+data8]`     |`ROR [X+data8]`     |Rotate right through carry flag on `[X+data8]`            |
|`ROR [addr16]`      |`ROR [addr16]`      |Rotate right through carry flag on `[addr16]`             |
|`RTI`               |                    |Return from interrupt.                                    |
|`RTS`               |                    |`PC(hi)<-[SP+1], PC(lo)<-[SP+2], SP<-SP+2`                |
|`SBA`               |                    |`A<-A-B`                                                  |
|`SBCA data8`        |`SBC A,data8`       |`A<-A-data8-C`                                            |
|`SBCA {addr8}`      |`SBC A,{addr8}`     |`A<-A-{addr8}-C`                                          |
|`SBCA [X+data8]`    |`SBC A,[X+data8]`   |`A<-A-[X+data8]-C`                                        |
|`SBCA [addr16]`     |`SBC A,[addr16]`    |`A<-A-[addr16]-C`                                         |
|`SBCB data8`        |`SBC B,data8`       |`B<-B-data8-C`                                            |
|`SBCB {addr8}`      |`SBC B,{addr8}`     |`B<-B-{addr8}-C`                                          |
|`SBCB [X+data8]`    |`SBC B,[X+data8]`   |`B<-B-[X+data8]-C`                                        |
|`SBCB [addr16]`     |`SBC B,[addr16]`    |`B<-B-[addr16]-C`                                         |
|`SEC`               |                    |`C<-1`                                                    |
|`SEI`               |                    |`I<-1`                                                    |
|`SEV`               |                    |`V<-1`                                                    |
|`STAA {addr8}`      |`STA A,{addr8}`     |`{addr8}<-A`                                              |
|`STAA [X+data8]`    |`STA A,[X+data8]`   |`[X+data8]<-A`                                            |
|`STAA [addr16]`     |`STA A,[addr16]`    |`[addr16]<-A`                                             |
|`STAB {addr8}`      |`STA B,{addr8}`     |`{addr8}<-B`                                              |
|`STAB [X+data8]`    |`STA B,[X+data8]`   |`[X+data8]<-B`                                            |
|`STAB [addr16]`     |`STA B,[addr16]`    |`[addr16]<-B`                                             |
|`STS {addr8}`       |                    |`{addr8}<-SP(hi), {add8+1}<-SP(lo)`                       |
|`STS [X+data8]`     |                    |`[X+data8]<-SP(hi), [X+data8+1]<-SP(lo)`                  |
|`STS [addr16]`      |                    |`[addr16]<-SP(hi), [addr16+1]<-SP(lo)`                    |
|`STX {addr8}`       |                    |`{addr8}<-X(hi), {addr8+1}<-X(lo)`                        |
|`STX [X+data8]`     |                    |`[X+data8]<-X(hi), [X+data8+1]<-X(lo)`                    |
|`STX [addr16]`      |                    |`[addr16]<-X(hi), [addr16+1]<-X(lo)`                      |
|`SUBA data8`        |`SUB A,data8`       |`A<-A-data8`                                              |
|`SUBA {addr8}`      |`SUB A,{addr8}`     |`A<-A-{addr8}`                                            |
|`SUBA [X+data8]`    |`SUB A,[X+data8]`   |`A<-A-[X+data8]`                                          |
|`SUBA [addr16]`     |`SUB A,[addr16]`    |`A<-A-[addr16]`                                           |
|`SUBB data8`        |`SUB B,data8`       |`B<-B-data8`                                              |
|`SUBB {addr8}`      |`SUB B,{addr8}`     |`B<-B-{addr8}`                                            |
|`SUBB [X+data8]`    |`SUB B,[X+data8]`   |`B<-B-[X+data8]`                                          |
|`SUBB [addr16]`     |`SUB B,[addr16]`    |`B<-B-[addr16]`                                           |
|`SWI`               |                    |Invoke software interrupt                                 |
|`TAB`               |                    |`B<-AP`                                                   |
|`TAP`               |                    |`SR<-A`                                                   |
|`TBA`               |                    |`A<-B`                                                    |
|`TPA`               |                    |`A<-SR`                                                   |
|`TSTA`              |`TST A`             |`A-0`                                                     |
|`TSTB`              |`TST B`             |`B-0`                                                     |
|`TST [X+data8]`     |`TST [X+data8]`     |`[X+data8]-0`                                             |
|`TST [addr16]`      |`TST [addr16]`      |`[addr16]-0`                                              |
|`TSX`               |                    |`X<-SP+1`                                                 |
|`TXS`               |                    |`SP<-X-1`                                                 |
|`WAI`               |                    |Wait for interrupt                                        |

Operands:

- `A` ... accumulator A
- `B` ... accumulator B
- `X` ... index register
- `SP` ... stack pointer
- `PC` ... program counter
- `SR` ... status register
- `data8` ... immediate 8 bit value
- `data16` ... immediate 16 bit value
- `{addr8}` ... direct addressing to the internal RAM
- `[X+data8]` ... index addresing
- `[addr16]` ... external addressing to the external RAM or ROM

Meaning of operations:

- `+` ... addition
- `-` ... subtraction
- `&` ... bitwise AND
- `|` ... bitwise OR
- `^` ... bitwise XOR

The status register `SR` holds following flags:

- `C` ... carry flag
- `V` ... overflow flag
- `Z` ... zero flag
- `N` ... negative flag
- `I` ... interrupt mask flag
