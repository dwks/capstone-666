.section .text
.global code
.global end
code:
    add     $0x1,%ax
    add     $0x1,%eax
    add     $0x1,%rax
    add     $0x1,%al
    add     $0xbeef,%ax
    addq    $0x1eadbeef,%rax
    addw    $0x1,(%rdx,%rax,2)
end:
    nop
