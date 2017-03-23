#pragma once

namespace cloture::util::ctfe::machine
{

	template<size_t nbytes>
	struct Memory
	{
		using memElement = common::uint8;
		memElement mempool[nbytes];

		constexpr Memory() : mempool()
		{}

		constexpr void store(const size_t index, const common::uint8 value)
		{
			mempool[index] = value;
		}
		constexpr void store(const size_t index, const common::uint16 value)
		{
			using common::uint8;
			using common::bitSizeof;
			//store low byte
			mempool[index] 		= static_cast<uint8>(value);
			//store high byte
			mempool[index + 1] 	= static_cast<uint8>(value >> bitSizeof<uint8> );
		}

		constexpr void store(const size_t index, const common::uint32 value)
		{
			using common::uint16;
			using common::bitSizeof;
			store(
				index,
				static_cast<uint16>(value)
			);

			store(
					index + sizeof(uint16),
					static_cast<uint16>(value >> bitSizeof<uint16>)
			);

		}
		constexpr void store(const size_t index, const common::real32 value)
		{
			using common::uint32;
			using common::bitSizeof;

			const uint32 raw = ctfe::math::rawBits(value);

			store(index, raw);

		}
		constexpr void store(const size_t index, const common::uint64 value)
		{
			using common::uint32;
			using common::bitSizeof;

			store(
				index,
				static_cast<uint32>(value)
			);

			store(
				index + sizeof(uint32),
				static_cast<uint32>(value >> bitSizeof<uint32>)
			);
		}
		constexpr void store(const size_t index, const common::real64 value)
		{
			using common::uint64;
			using common::bitSizeof;

			const uint64 raw = ctfe::math::rawBits(value);

			store(index, raw);

		}

#if !defined(NO_INT128_IMPL)
		constexpr void store(const size_t index, const common::uint128 value)
		{
			using common::uint64;
			using common::bitSizeof;

			store(
				index,
				static_cast<uint64>(value)
			);

			store(
				index + sizeof(uint64),
				static_cast<uint64>(value >> bitSizeof<uint64>)
			);
		}
#endif
		template<typename T>
		constexpr T load(const size_t index)
		{
			using namespace common;
			return static_cast<T>(_Generic(
			(*static_cast<T*>(nullptr)),

			uint8:
			mempool[index],

			int8:
			mempool[index],

			uint16:
			static_cast<uint16>(mempool[index]) 				|
			(static_cast<uint16>(mempool[index + 1]) << 8),

			int16:
			static_cast<uint16>(mempool[index]) 				|
			(static_cast<uint16>(mempool[index + 1]) << 8),

			uint32:
			static_cast<uint32>(mempool[index]) 				|
			(static_cast<uint32>(mempool[index + 1]) << 8) 		|
			(static_cast<uint32>(mempool[index + 2]) << 16) 	|
			(static_cast<uint32>(mempool[index + 3]) << 24),

			int32:
			static_cast<uint32>(mempool[index]) 				|
			(static_cast<uint32>(mempool[index + 1]) << 8) 		|
			(static_cast<uint32>(mempool[index + 2]) << 16) 	|
			(static_cast<uint32>(mempool[index + 3]) << 24),

			uint64:
			static_cast<uint64>(mempool[index]) 				|
			(static_cast<uint64>(mempool[index + 1]) << 8ULL) 	|
			(static_cast<uint64>(mempool[index + 2]) << 16ULL) 	|
			(static_cast<uint64>(mempool[index + 3]) << 24ULL)	|
			(static_cast<uint64>(mempool[index + 4]) << 32ULL) 	|
			(static_cast<uint64>(mempool[index + 5]) << 40ULL) 	|
			(static_cast<uint64>(mempool[index + 6]) << 48ULL) 	|
			(static_cast<uint64>(mempool[index + 7]) << 56ULL),
			int64:
			static_cast<uint64>(mempool[index]) 				|
			(static_cast<uint64>(mempool[index + 1]) << 8ULL) 	|
			(static_cast<uint64>(mempool[index + 2]) << 16ULL) 	|
			(static_cast<uint64>(mempool[index + 3]) << 24ULL)	|
			(static_cast<uint64>(mempool[index + 4]) << 32ULL) 	|
			(static_cast<uint64>(mempool[index + 5]) << 40ULL) 	|
			(static_cast<uint64>(mempool[index + 6]) << 48ULL) 	|
			(static_cast<uint64>(mempool[index + 7]) << 56ULL),
#if !defined(NO_INT128_IMPL)
			uint128:
			static_cast<uint128>(load<uint64>(index)) 			|
			(static_cast<uint128>(load<uint64>(index + sizeof(uint64))) << static_cast<uint128>(64)),
			int128:
			static_cast<uint128>(load<uint64>(index)) 			|
			(static_cast<uint128>(load<uint64>(index + sizeof(uint64))) << static_cast<uint128>(64)),
#endif
			real32:
			ctfe::math::fromRaw(load<int32>(index)),
			real64:
			ctfe::math::fromRaw(load<int64>(index)),
			default:
				-1
			));
		}


	};

	template< const Memory<65535>& memory >
	struct Machine
	{
		//Memory<65536> memory;
		static constexpr auto getMemory()
		{
			return memory;
		}
	};

}
