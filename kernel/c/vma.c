//
// Created by ben on 29/12/16.
//

#include "../../libc/stdlib.h"
#include "../h/vma.h"
#include "../h/utils.h"
#include "../h/kernel.h"
#include "../../common/paging.h"
#include "../h/rbtree.h"
#include "../../common/syscall.h"

vma_node_t *vma_free_list;
size_t vma_max_num;
size_t vma_alloc_num;

rb_root_t vma_rb_root;
uint64_t vma_highest_addr;
vm_area_t *vma_list_start;

void *vma_heap_head;

static bool vma_list_find(vma_node_t *next, vm_area_t *addr){
    if (next == NULL) return false;
    else if (&next->vma == addr) return true;
    else return vma_list_find(next->next_freed, addr);
}

static void *vma_next_freed(){
    if (vma_free_list == NULL) return NULL;
    else{
        vma_node_t *node;

        node = vma_free_list;
        vma_free_list = node->next_freed;
        return node;
    }
}

void vma_init(size_t max_entries){
    vma_free_list = NULL;
    vma_alloc_num = 0;
    vma_rb_root.rb_node = NULL;
    vma_highest_addr = 0;
    vma_list_start = NULL;

    vma_max_num = PAGE_ALIGN(max_entries * sizeof (vma_node_t)) / sizeof (vma_node_t) - 1;
    vma_heap_head = (void *)PAGE_ALIGN_DOWN(kernel_min_address - vma_max_num);
    kernel_min_address = (uint64_t)vma_heap_head;

    map_physical_page((uint64_t)vma_heap_head, -1, PDE64_NO_EXE | PDE64_WRITEABLE, 1, false, 0);
}

vm_area_t *vma_alloc(){
    vma_node_t* node;
    uint64_t old_head, new_head;

    if ((node = vma_next_freed()) == NULL){
        if (vma_alloc_num++ == vma_max_num+1)
            return NULL;

        node = (vma_node_t *)vma_heap_head;

        old_head = (uint64_t)vma_heap_head;
        new_head = old_head + sizeof (vma_node_t);

        if (PAGE_ALIGN_DOWN(old_head) != PAGE_ALIGN_DOWN(new_head)){
            for (uint64_t p = PAGE_ALIGN_DOWN(old_head) + PAGE_SIZE; p <= PAGE_ALIGN_DOWN(new_head); p += PAGE_SIZE){
                map_physical_page(p, -1, PDE64_NO_EXE | PDE64_WRITEABLE, 1, false, 0);
            }
        }

        vma_heap_head = (void *)new_head;
    }

    return &node->vma;
}

vm_area_t *vma_zalloc(){
    vm_area_t* node;
    node = vma_alloc();

    if (node != NULL)
        memset(node, 0, sizeof(vm_area_t));

    return node;
}

void vma_free(vm_area_t *addr){
    if (vma_list_find(vma_free_list, addr)){
        printf("Double free of VMA!\n");
    } else{
        vma_node_t *free_node;

        free_node = container_of(addr, vma_node_t, vma);

        //if list not empty, slip in at front
        if (vma_free_list != NULL)
            free_node->next_freed = vma_free_list;

        vma_free_list = free_node;

    }
}

void vma_print_node(vm_area_t *vma)
{
    if (vma == NULL) return;

    printf("VMA addr: %p end: %p\n", vma->vm_start, vma->vm_end);

    vma_print_node(vma->vm_next);
}

void vma_print(){
    printf("VMAs:\n");
    printf("-------------------------------------\n");

    vma_print_node(vma_list_start);
}

//change this to return NULL on fail
vm_area_t *vma_find(uint64_t addr)
{
    rb_node_t *rb_node;
    vm_area_t *vma = NULL;

    rb_node = vma_rb_root.rb_node;

    while (rb_node) {
        vm_area_t*tmp;

        tmp = container_of(rb_node, vm_area_t, vm_rb);

        if (tmp->vm_end > addr) {
            vma = tmp;
            if (tmp->vm_start <= addr){
                break;
            }
            rb_node = rb_node->rb_left;
        } else
            rb_node = rb_node->rb_right;
    }

    return vma;
}