/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Xilinx nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file	utilities.h
 * @brief	Utility routines for libmetal.
 */

#ifndef __METAL_UTILITIES__H__
#define __METAL_UTILITIES__H__

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup utilities Simple utilities
 *  @{ */

/** Marker for unused function arguments/variables. */
#define metal_unused(x)	do { (x) = (x); } while (0)

/** Figure out number of elements in an array. */
#define metal_dim(x)	(sizeof(x) / sizeof(x[0]))

/** Minimum of two numbers (warning: multiple evaluation!).  */
#define metal_min(x, y)	((x) < (y) ? (x) : (y))

/** Maximum of two numbers (warning: multiple evaluation!).  */
#define metal_max(x, y)	((x) > (y) ? (x) : (y))

/** Sign of a number [-1, 0, or 1] (warning: multiple evaluation!).  */
#define metal_sign(x)	((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

/** Align 'size' down to a multiple of 'align' (must be a power of two). */
#define metal_align_down(size, align)			\
	((size) & ~((align) - 1))

/** Align 'size' up to a multiple of 'align' (must be a power of two). */
#define metal_align_up(size, align)			\
	metal_align_down((size) + (align) - 1, align)

/** Divide (and round down). */
#define metal_div_round_down(num, den)			\
	((num) / (den))

/** Divide (and round up). */
#define metal_div_round_up(num, den)			\
	metal_div_round_down((num) + (den) - 1, (den))

/** Align 'ptr' down to a multiple of 'align' (must be a power of two). */
#define metal_ptr_align_down(ptr, align)		\
	(void *)(metal_align_down((uintptr_t)(ptr), (uintptr_t)(align)))

/** Align 'ptr' up to a multiple of 'align' (must be a power of two). */
#define metal_ptr_align_up(ptr, align)			\
	(void *)(metal_align_up((uintptr_t)(ptr), (uintptr_t)(align)))

/** Compute offset of a field within a structure. */
#define metal_offset_of(structure, member)		\
	((uintptr_t) &(((structure *) 0)->member))

/** Compute pointer to a structure given a pointer to one of its fields. */
#define metal_container_of(ptr, structure, member)	\
	(void *)((uintptr_t)(ptr) - metal_offset_of(structure, member))

#define METAL_BITS_PER_ULONG	(8 * sizeof(unsigned long))

#define metal_bit(bit)		(1UL << (bit))

#define metal_bitmap_longs(x)	metal_div_round_up((x), METAL_BITS_PER_ULONG)

static inline void metal_bitmap_set_bit(unsigned long *bitmap, int bit)
{
	bitmap[bit / METAL_BITS_PER_ULONG] |=
		metal_bit(bit & (METAL_BITS_PER_ULONG - 1));
}

static inline int metal_bitmap_is_bit_set(unsigned long *bitmap, int bit)
{
	return bitmap[bit / METAL_BITS_PER_ULONG] &
		metal_bit(bit & (METAL_BITS_PER_ULONG - 1));
}

static inline void metal_bitmap_clear_bit(unsigned long *bitmap, int bit)
{
	bitmap[bit / METAL_BITS_PER_ULONG] &=
		~metal_bit(bit & (METAL_BITS_PER_ULONG - 1));
}

static inline int metal_bitmap_is_bit_clear(unsigned long *bitmap, int bit)
{
	return !metal_bitmap_is_bit_set(bitmap, bit);
}

static inline unsigned int
metal_bitmap_next_set_bit(unsigned long *bitmap, unsigned int start,
			  unsigned int max)
{
	unsigned int bit;
	for (bit = start;
	     bit < max && !metal_bitmap_is_bit_set(bitmap, bit);
	     bit ++)
		;
	return bit;
}

#define metal_bitmap_for_each_set_bit(bitmap, bit, max)			\
	for ((bit) = metal_bitmap_next_set_bit((bitmap), 0, (max));	\
	     (bit) < (max);						\
	     (bit) = metal_bitmap_next_set_bit((bitmap), (bit), (max)))

static inline unsigned int
metal_bitmap_next_clear_bit(unsigned long *bitmap, unsigned int start,
			    unsigned int max)
{
	unsigned int bit;
	for (bit = start;
	     bit < max && !metal_bitmap_is_bit_clear(bitmap, bit);
	     bit ++)
		;
	return bit;
}

#define metal_bitmap_for_each_clear_bit(bitmap, bit, max)		\
	for ((bit) = metal_bitmap_next_clear_bit((bitmap), 0, (max));	\
	     (bit) < (max);						\
	     (bit) = metal_bitmap_next_clear_bit((bitmap), (bit), (max)))

static inline unsigned long metal_log2(unsigned long in)
{
	unsigned long result;

	assert((in & (in - 1)) == 0);

	for (result = 0; (1UL << result) < in; result ++)
		;
	return result;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __METAL_UTILITIES__H__ */