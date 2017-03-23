#pragma once

namespace cloture::util::bitwise
{
	using namespace cloture::util::common;

	/*
		I've tested this one pretty extensively and it seems to work fine
		this compiles down to three instructions (four for the byte ones, because of the required zero extension)
		
		xor result, result
		bt val, bit
		adc result, result
		
		that's actually slower than the normal x86 test, but the resulting code
		is smaller because of the elimination of the immediate operand
		
		the result of bitwise::test is guaranteed to be either 1 or 0
		this can be helpful in if statements with multiple &&s involving bit ops
		as you can eliminate branches by doing a bitwise and instead of a logical and
		
		the assumes may seem redundant, but i've confirmed that the compiler takes advantage of them
		
	*/
	
	/*
		if we're testing bit 0, we can just test against the zeroed register
		instead of using an immediate
	*/
	template<typename T, typename T2>
	__forceinline __pure __hot
	static unsigned int test(const register T val, __unused const T2 bit) noexcept
	__enableIf(bit == 0 && !generic::isPointer<T>(), "Enabled if testing bit zero.")
	{
		static_cast<void>(bit);
		backreference T oldval = val;
		static_assert(sizeof(val) == 4);
		const unsigned int isSet = 
		({
			unsigned int result = 0u;

			asm const(
				"btl %[result], %[input]\n\t"
				"adcl %[result], %[result]\n\t"
			: [result] "+r" (result)
			: [input] "r" (val)
			: "cc"
			);
			result;
		});
		__assume(isSet == (static_cast<unsigned int>(val) & 1u));	
		__assume(isSet == 0u || isSet == 1u);
		__assume(
		static_cast<unsigned int>(static_cast<size_t>(isSet) & static_cast<size_t>(0xFFFFFFFFu)) == isSet
		);
		__assume(static_cast<signed int>(isSet) >= 0);
		return isSet;
	}
	
	template<typename T, typename T2>
	__forceinline __pure __hot
	static unsigned int test(const T val, const T2 bit) noexcept
	{
		backreference T2 oldBit = bit;
		
		unsigned int isSet;
		
		mIfMetaTrue(!generic::isPointer<T>())
		{
			using dataType = T;
			backreference dataType oldVal = val;
			mIfMetaTrue(sizeof(dataType) == sizeof(common::int32))
			{
				
				isSet = 
				({
					unsigned int result = 0u;

					asm const(
						"btl %[_bit], %[input]\n\t"
						"adcl %[result], %[result]\n\t"
					: [result] "+r" (result)
					: [input] "r" (val), [_bit] "ri" (bit)
					: "cc"
					);
					result;
				});
				//these assumes let the compiler know exactly what the relationship between the input values
				//and output values are. 
				__assume(isSet == ((static_cast<unsigned int>(val) >> bit) & 1u));	
			}
			mIfMetaTrue(sizeof(dataType) == sizeof(common::uint16))
			{
				isSet = 
				({
					unsigned int result = 0u;
					asm const(
						"btw %[_bit], %[input]\n\t"
						"adcl %[result], %[result]\n\t"
					: [result] "+r" (result)
					: [input] "r" (val), [_bit] "ri" (bit)
					: "cc"
					);
					result;
				});			
				__assume(isSet == ((static_cast<unsigned short>(val) >> bit) & 1u));	
			}
			#if defined(__x86_64__)
				mIfMetaTrue(sizeof(dataType) == sizeof(common::uint64))
				{
					isSet = 
					({
						unsigned int result = 0u;
						asm const(
							"btq %[_bit], %[input]\n\t"
							"adcl %[result], %[result]\n\t"
						: [result] "+r" (result)
						: [input] "r" (val), [_bit] "ri" (bit)
						: "cc"
						);
						result;
					});			
					__assume(isSet == ((static_cast<unsigned long>(val) >> bit) & 1u));	
				}
			#endif
			mIfMetaTrue(sizeof(dataType) == sizeof(common::uint8))
			{
					isSet = 
					({
						unsigned int result = 0u;
						unsigned int extended = val;
						asm const(
							"btl %[_bit], %[input]\n\t"
							"adcl %[result], %[result]\n\t"
						: [result] "+r" (result)
						: [input] "r" (extended), [_bit] "ri" (bit)
						: "cc"
						);
						result;
					});						
					__assume(isSet == ((static_cast<unsigned char>(*val) >> bit) & 1u));	
			}
			__assume(val == oldVal);
		}
		mIfMetaTrue(generic::isPointer<T>())
		{
			using dataType = typeof(*val);
			//static_assert(	generic::isInteger<dataType>()	);
			
			backreference dataType oldVal = *val;
			
			mIfMetaTrue(sizeof(dataType) == sizeof(common::int32))
			{
				isSet = 
				({
					unsigned int result = 0u;
					asm const(
						"btl %[_bit], %[input]\n\t"
						"adcl %[result], %[result]\n\t"
					: [result] "+r" (result)
					: [input] "m" (val), [_bit] "ri" (bit)
					: "cc"
					);
					result;
				});
				__assume(isSet == ((static_cast<unsigned int>(*val) >> bit) & 1u));	
			}
			mIfMetaTrue(sizeof(dataType) == sizeof(common::uint16))
			{
				isSet = 
				({
					unsigned int result = 0u;
					asm const(
						"btw %[_bit], %[input]\n\t"
						"adcl %[result], %[result]\n\t"
					: [result] "+r" (result)
					: [input] "m" (val), [_bit] "ri" (bit)
					: "cc"
					);
					result;
				});			
				__assume(isSet == ((static_cast<unsigned short>(*val) >> bit) & 1u));	
			}
			#if defined(__x86_64__)
				mIfMetaTrue(sizeof(dataType) == sizeof(common::uint64))
				{
					isSet = 
					({
						unsigned int result = 0u;
						asm const(
							"btq %[_bit], %[input]\n\t"
							"adcl %[result], %[result]\n\t"
						: [result] "+r" (result)
						: [input] "m" (val), [_bit] "ri" (bit)
						: "cc"
						);
						result;
					});			
					__assume(isSet == ((static_cast<unsigned long>(*val) >> bit) & 1u));	
				}
			#endif
			mIfMetaTrue(sizeof(dataType) == sizeof(common::uint8))
			{
					isSet = 
					({
						unsigned int result = 0u;
						unsigned int extended = *val;
						asm const(
							"btl %[_bit], %[input]\n\t"
							"adcl %[result], %[result]\n\t"
						: [result] "+r" (result)
						: [input] "r" (extended), [_bit] "ri" (bit)
						: "cc"
						);
						result;
					});				
				__assume(isSet == ((static_cast<unsigned char>(*val) >> bit) & 1u));			
			}
			__assume(*val == oldVal);	
			
		}

		__assume(isSet == 0u || isSet == 1u);		
		__assume(
		static_cast<unsigned int>(static_cast<size_t>(isSet) & static_cast<size_t>(0xFFFFFFFFu)) == isSet
		);
		__assume(static_cast<signed int>(isSet) >= 0);
		
		__assume(bit == oldBit);

		return isSet;
	}

}//namespace cloture::util::bitwise
