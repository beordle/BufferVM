.text
.globl host_exit
.globl host_write
.globl host_read
.globl allocate_page
.globl map_physical_page

host_exit:
    movq $0x0, %rax
    hlt

host_write:
    movq $0x1, %rax
    hlt
    ret

host_read:
    movq $0x2, %rax
    hlt
    ret

allocate_page:
    movq $0x3, %rax
    hlt
    ret

map_physical_page:
    movq $0x4, %rax
    hlt
    ret
