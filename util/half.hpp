#pragma once
/*
 * 	implementation of the IEEE half-precision floating point format
 *
 *	this type is not useful for actual arithmetic. it is for storage only.
 *
 *	however, some arithmetic operations can be performed without casting to float
 *	using bit hacks.
 *
 *	combined with hardware supported vector types, this could be faster than
 *	using floats provided that the half-precision value is stored after calculations are done
 *	and is only casted at a later point
 *
 *	on ARM the NEON instruction set provides instructions for converting to/from half precision values
 *
 *	it might be best to #ifdef this type and define float16 as float32 on x86/x86-64.
 *
 *	in this case, do not use this in structures that get written to files
 */

class real16
{
	int16 h;
public:
	using maskType = uint16;

	static constexpr maskType signMask 		= 0x7FFF;
	static constexpr maskType exponentMask 	= 0x7C00;
	static constexpr maskType mantissaMask 	= 0x3FF;

	static constexpr unsigned signShift		= 15;
	static constexpr unsigned exponentShift	= 10;

	constexpr real16(const real32 f) : h(0)
	{
		using cloture::util::ctfe::math::rawBits;

		const int raw 			= rawBits(f);
		uint16 bits 			= (raw >> 16) & 0x8000;
		uint16 mantissa 		= (raw >> 12) & 0x07FF;
		const uint32 exponent 	= (raw >> 23) & 0xFF;

		//exponent underflows
		if(exponent < 103)
		{
			h = bits;	//signed zero
		}
		//NaN or Inf
		else if(exponent > 142)
		{
			bits |= 0x7C00;
			bits |= exponent == 255 && (raw & 0x007fffff);
			h = bits;
		}
		//denormal
		else if(exponent < 113)
		{
			mantissa |= 0x0800;
			bits |= (mantissa >> (114 - exponent)) + ((mantissa >> (113 - exponent)) & 1);
			h = bits;
		}
		else
		{
			bits |= ((exponent - 112) << exponentShift) | (mantissa >> 1);
			bits += mantissa & 1;
			h = bits;
		}
	}

	constexpr unsigned getRawExponent() const
	{
		const uint16 exponentOf = (h & exponentMask) >> exponentShift;
		return exponentOf;
	}
	constexpr unsigned getRawMantissa() const
	{
		const uint16 mantissaOf = h & mantissaMask;
		return mantissaOf;
	}
	constexpr unsigned getSign() const
	{
		const uint16 signOf = (h & signMask) >> signShift;
		return signOf;
	}



	/*
	 * conversion to float
	 */
	constexpr operator real32() const
	{
		using cloture::util::ctfe::math::fromRaw;
		using cloture::util::ctfe::math::rawBits;

		constexpr uint32 magic = 126u << 23u;
		constexpr real32 magicF = fromRaw<real32>(magic);

		unsigned int rawbits = 0;
		if(getRawExponent() == 0)
		{
			rawbits = magic + getRawMantissa();
			rawbits = rawBits<real32>(fromRaw<real32>(rawbits) - magicF);
		}
		else
		{
			rawbits |= getRawMantissa() << 13;
			if(getRawExponent() == 0x1F)
				rawbits |= 255u << 23u;
			else
				rawbits |= (127 - 15 + getRawExponent()) << 23u;
		}
		rawbits |= getSign() << 31;
		return fromRaw<real32>(rawbits);
	}
};

