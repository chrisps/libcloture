#pragma once

#define		gas(...)		#__VA_ARGS__ "\n\t"

#define		intelAsmBegin()		".intel_syntax noprefix\n\t"
#define		attAsmBegin()		".att_syntax prefix\n\t"

/*
Intel 386—i386.h

    q
        `a', b, c, or d register for the i386. For x86-64 it is equivalent to `r' class. (for 8-bit instructions that do not use upper halves)
    Q
        `a', b, c, or d register. (for 8-bit instructions, that do use upper halves)
    R
        Legacy register—equivalent to r class in i386 mode. (for non-8-bit registers used together with 8-bit upper halves in a single instruction)
    A
        Specifies the `a' or `d' registers. This is primarily useful for 64-bit integer values (when in 32-bit mode) intended to be returned with the `d' register holding the most significant bits and the `a' register holding the least significant bits.
    f
        Floating point register
    t
        First (top of stack) floating point register
    u
        Second floating point register
    a
        `a' register
    b
        `b' register
    c
        `c' register
    C
        Specifies constant that can be easily constructed in SSE register without loading it from memory.
    d
        `d' register
    D
        `di' register
    S
        `si' register
    x
        `xmm' SSE register
    y
        MMX register
    I
        Constant in range 0 to 31 (for 32-bit shifts)
    J
        Constant in range 0 to 63 (for 64-bit shifts)
    K
        `0xff'
    L
        `0xffff'
    M
        0, 1, 2, or 3 (shifts for lea instruction)
    N
        Constant in range 0 to 255 (for out instruction)
    Z
        Constant in range 0 to 0xffffffff or symbolic reference known to fit specified range. (for using immediates in zero extending 32-bit to 64-bit x86-64 instructions)
    e
        Constant in range −2147483648 to 2147483647 or symbolic reference known to fit specified range. (for using immediates in 64-bit x86-64 instructions)
    G
        Standard 80387 floating point constant 
*/
