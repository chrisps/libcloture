#pragma once

/**
 * 		implementation of the quadruple precision float type
 *		just for shits'n'giggles, y'know?
 *
 *		this file is included in extended_types.hpp
 *		the class float128 belong to the namespace cloture::util::common.
 *
 *		written using libquadmath's source as a reference
*/

class real128
{
	int128 v;

public:
	static constexpr uint128 signMask 	= 0x80000000000000000000000000000000_u128;
	static constexpr uint128 absMask	= ~signMask;
	static constexpr int128 posInf		= 0x7fff0000000000000000000000000000_i128;
	static constexpr int128 negInf		= 0xffff0000000000000000000000000000_i128;
	static constexpr uint128 significandMask	=	~negInf;
	//
	static constexpr size_t fractionalBits	= 112;
	static constexpr size_t exponentBits	= 15;

	static constexpr size_t exponentBias	= 16383;
	static constexpr size_t exponentMax		= 32767;
	static constexpr size_t floatRebias		= 16256;
	constexpr real128(const float f) : v(0_i128)
	{
		using cloture::util::ctfe::math::rawBits;

		const int raw				= rawBits(f);
		uint128 fmantissa 	= raw & 0x7FFFFF;
		unsigned exponent		= static_cast<uint8>(raw >> 23) & 0x7Fu;
		exponent += floatRebias;

		fmantissa <<= 89;
		uint128 exponent128 = exponent;
		exponent128 <<= fractionalBits;

		const int128 calcval = exponent128 | fmantissa;
		v = (f < .0f) ? calcval | signMask : calcval;
	}

	#if 0
	struct s0 {
	    int32 f0;
	    uint64 f4;
	};

	int32_t g0;

	struct s1
	{
	    uint64 f0;
	    uint64 f8;
	};
	void __dtoq(uint64 doublebits, struct s0* rdi)
	{
		struct s1* rdi2;
		uint64 rdx3;
		uint64 v4;
		uint64 rax5;
		uint64 rax6;
		int64 r10_7;
		int32 ecx9;
		uint64 r10_10;

		constexpr uint64 doubleSignMask = 1 << 63;
		constexpr uint64 doubleAbsMask = ~doubleSignMask;

		//uint64 rsi0 = ;
		//rdi->f0 = g0;
		rdi2 = (struct s1*)&rdi->f4;
		uint64 rsi0 = doubleSignMask & v4;
		rdx3 = doubleAbsMask & v4;

		if (rdx3 == 0)
		{
			rdi2->f0 = 0;
			rdi2->f8 = 0;
			return;
		}

		rax5 = rdx3 >> 52;
		if (static_cast<int32>(rax5 & 0xFFFFFFFF) == 0)
		{
			rax6 = rdx3 >> 32;
			uint32 r8d0 = rdx3;
			r10_7 = -11;

			if (static_cast<int32>(rax6) == 0)
				r10_7 = 0x15;
			else
				rd8d0 = static_cast<uint32>(rax6);

			uint32 r11d0 = 1 << 31;
			if(!r8d0)
				r11d0 = 0;
			else
			{
				int i = 31;
				while(r11d0 != 0 && (r11d0 & r8d0) == 0)
				{
					--i;
					r11d0 >>= 1;
				}
				r11d0 = -i;
			}

			ecx9 = (int32)(r10_7 + 31__stdcall);
			rdx3 = rdx3 << *(unsigned char*)&ecx9;
			*(int32*)&rax5 = -ecx9 + 1;
		}
		r10_7 = static_cast<int64>(static_cast<int>(rax5 & 0xFFFFFFFF));

		r10_10 = (uint64)((r10_7 << 48) + 0x3c00000000000000);
		if (*(int32*)&rax5 == 0x7ff)
		{
			r10_10 = 0x7fff000000000000;
		}
		rdi2->f0 = rdx3 << 60;
		rdi2->f8 = 0 | r10_10 | (0xfffffffffffff & rdx3) >> 4;
		return;

	}
	#endif

};
