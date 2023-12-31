/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (c) 2011 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <bspopts.h>
#include <rtems/powerpc/powerpc.h>

#if BSP_DATA_CACHE_ENABLED \
  && PPC_CACHE_ALIGNMENT == 32 \
  && !defined(BSP_DATA_CACHE_USE_WRITE_THROUGH)

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <libcpu/powerpc-utility.h>

#define WORD_SIZE 4

#define WORD_MASK (WORD_SIZE - 1)

static bool aligned(const void *a, const void *b)
{
  return ((((uintptr_t) a) | ((uintptr_t) b)) & WORD_MASK) == 0;
}

void *memcpy(void *dst_ptr, const void *src_ptr, size_t n)
{
  uint8_t *dst = dst_ptr;
  const uint8_t *src = src_ptr;

  ppc_data_cache_block_touch(src);

  if (__builtin_expect(n >= WORD_SIZE && aligned(src, dst), 1)) {
    uint32_t *word_dst = (uint32_t *) dst - 1;
    const uint32_t *word_src = (const uint32_t *) src - 1;

    if (n >= 2 * PPC_CACHE_ALIGNMENT - WORD_SIZE) {
      while ((uintptr_t) (word_dst + 1) % PPC_CACHE_ALIGNMENT != 0) {
        uint32_t tmp;
        __asm__ volatile (
          "lwzu %[tmp], 0x4(%[src])\n"
          "stwu %[tmp], 0x4(%[dst])\n"
          : [src] "+b" (word_src),
            [dst] "+b" (word_dst),
            [tmp] "=&r" (tmp)
        );
        n -= WORD_SIZE;
      }

      while (n >= PPC_CACHE_ALIGNMENT) {
        uint32_t dst_offset = 4;
        uint32_t src_offset = 32 + 4;
        uint32_t tmp0;
        uint32_t tmp1;
        uint32_t tmp2;
        uint32_t tmp3;
        __asm__ volatile (
          "dcbz %[dst],  %[dst_offset]\n"
          "lwz  %[tmp0], 0x04(%[src])\n"
          "dcbt %[src],  %[src_offset]\n"
          "lwz  %[tmp1], 0x08(%[src])\n"
          "lwz  %[tmp2], 0x0c(%[src])\n"
          "lwz  %[tmp3], 0x10(%[src])\n"
          "stw  %[tmp0], 0x04(%[dst])\n"
          "stw  %[tmp1], 0x08(%[dst])\n"
          "stw  %[tmp2], 0x0c(%[dst])\n"
          "stw  %[tmp3], 0x10(%[dst])\n"
          "lwz  %[tmp0], 0x14(%[src])\n"
          "lwz  %[tmp1], 0x18(%[src])\n"
          "lwz  %[tmp2], 0x1c(%[src])\n"
          "lwzu %[tmp3], 0x20(%[src])\n"
          "stw  %[tmp0], 0x14(%[dst])\n"
          "stw  %[tmp1], 0x18(%[dst])\n"
          "stw  %[tmp2], 0x1c(%[dst])\n"
          "stwu %[tmp3], 0x20(%[dst])\n"
          : [src] "+b" (word_src),
            [dst] "+b" (word_dst),
            [tmp0] "=&r" (tmp0),
            [tmp1] "=&r" (tmp1),
            [tmp2] "=&r" (tmp2),
            [tmp3] "=&r" (tmp3)
          : [src_offset] "r" (src_offset),
            [dst_offset] "r" (dst_offset)
        );
        n -= PPC_CACHE_ALIGNMENT;
      }
    }

    while (n >= WORD_SIZE) {
      uint32_t tmp;
      __asm__ volatile (
        "lwzu %[tmp], 0x4(%[src])\n"
        "stwu %[tmp], 0x4(%[dst])\n"
        : [src] "+b" (word_src),
          [dst] "+b" (word_dst),
          [tmp] "=&r" (tmp)
      );
      n -= WORD_SIZE;
    }

    dst = (uint8_t *) word_dst + 4;
    src = (const uint8_t *) word_src + 4;
  }

  while (n > 0) {
    *dst = *src;
    ++src;
    ++dst;
    --n;
  }

  return dst_ptr;
}

#endif /* BSP_DATA_CACHE_ENABLED && PPC_CACHE_ALIGNMENT == 32 */
