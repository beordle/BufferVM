#include "../h/kernel.h"

    .section .text
    .global tss_load
tss_load:
    movw $(TSS_S | 3), %ax
    ltr %ax
    ret