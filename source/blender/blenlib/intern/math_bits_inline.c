/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * */

/** \file blender/blenlib/intern/math_bits_inline.c
 *  \ingroup bli
 */

#ifndef __MATH_BITS_INLINE_C__
#define __MATH_BITS_INLINE_C__

#ifdef _MSC_VER
#  include <sse2neon.h>
#endif

#include "BLI_math_bits.h"

MINLINE int bitscan_forward_i(int a)
{
	BLI_assert(a != 0);
#ifdef _MSC_VER
	unsigned long ctz;
	_BitScanForward(&ctz, a);
	return ctz;
#else
	return __builtin_ctz((unsigned int)a);
#endif
}

MINLINE unsigned int bitscan_forward_uint(unsigned int a)
{
	return (unsigned int)bitscan_forward_i((int)a);
}

MINLINE int bitscan_forward_clear_i(int *a)
{
	int i = bitscan_forward_i(*a);
	*a &= (*a) - 1;
	return i;
}

MINLINE unsigned int bitscan_forward_clear_uint(unsigned int *a)
{
	return (unsigned int)bitscan_forward_clear_i((int *)a);
}

MINLINE int bitscan_reverse_i(int a)
{
	BLI_assert(a != 0);
#ifdef _MSC_VER
	unsigned long clz;
	_BitScanReverse(&clz, a);
	return clz;
#else
	return __builtin_clz((unsigned int)a);
#endif
}

MINLINE unsigned int bitscan_reverse_uint(unsigned int a)
{
	return (unsigned int)bitscan_reverse_i((int)a);
}

MINLINE int bitscan_reverse_clear_i(int *a)
{
	int i = bitscan_reverse_i(*a);
	/* TODO(sergey): This could probably be optimized. */
	*a &= ~(1 << (sizeof(int) * 8 - i - 1));
	return i;
}

MINLINE unsigned int bitscan_reverse_clear_uint(unsigned int *a)
{
	return (unsigned int)bitscan_reverse_clear_i((int *)a);
}

MINLINE unsigned int highest_order_bit_uint(unsigned int n)
{
	if (n == 0) {
		return 0;
	}
	return 1 << (sizeof(unsigned int) * 8 - bitscan_reverse_uint(n));
}

MINLINE unsigned short highest_order_bit_s(unsigned short n)
{
	n |= (n >>  1);
	n |= (n >>  2);
	n |= (n >>  4);
	n |= (n >>  8);
	return (unsigned short)(n - (n >> 1));
}

#ifndef __GNUC__
MINLINE int count_bits_i(unsigned int i)
{
	/* variable-precision SWAR algorithm. */
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
#endif

MINLINE int float_as_int(float f)
{
	union { int i; float f; } u;
	u.f = f;
	return u.i;
}

MINLINE unsigned int float_as_uint(float f)
{
	union { unsigned int i; float f; } u;
	u.f = f;
	return u.i;
}

MINLINE float int_as_float(int i)
{
	union { int i; float f; } u;
	u.i = i;
	return u.f;
}

MINLINE float uint_as_float(unsigned int i)
{
	union { unsigned int i; float f; } u;
	u.i = i;
	return u.f;
}

MINLINE float xor_fl(float x, int y)
{
	return int_as_float(float_as_int(x) ^ y);
}

#endif /* __MATH_BITS_INLINE_C__ */
