#ifndef __LINUX_SLAB_H__
#define __LINUX_SLAB_H__

#include <stdlib.h>

#include <asm/page.h>

#define kzalloc(x, y) calloc(1, x)
#define kmalloc(x, y) malloc(x)
#define kmalloc_array(x, y, z) kmalloc((x * y), z)
#define kfree(x) free(x)
#define kvfree(x) free(x)
#define vmalloc(x) malloc(x)
#define vfree(x) free(x)

#endif /* __LINUX_SLAB_H__ */

