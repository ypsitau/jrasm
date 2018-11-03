        .ORG    0x1000
loop:
        LDX     [ptr_src]
        LDAA    [X]
        BEQ     done
        INX
        STX     [ptr_src]
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
