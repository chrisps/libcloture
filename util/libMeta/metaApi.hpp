#pragma once

namespace cloture::util
{
	class meta : __markAsCtfe(), __markApiObject()
	{
	private:
		struct metaBool	: __markAsCtfe()
		{} __unused;

		template<typename T>
		static constexpr bool internalMetaBoolValue()
		{
			static_assert(
			isMetaBool<T>,
			"metaBoolValue was provided a non-metaBool type.");
			return _Generic((*static_cast<T*>(nullptr)),
				const metaFalse:
					false,
				const metaTrue:
					true,
				volatile metaFalse:
					false,
				volatile metaTrue:
					true,
				metaFalse:
					false,
				metaTrue:
					true
				);
		}

		template<typename T>
		static constexpr int internalMetaIntValue()
		{
			static_assert(
			isMetaInt<T>,
			"isMetaInt was provided a non-metaInt type."
			);

			static_assert(
					sizeof(T::_internal_value_::value[0]) == 1,
					"Recheck the definition of metaInt. Something was changed."
			);
			return static_cast<int>(sizeof(T::_internal_value_::value));
		}

		struct _metaInt : __markAsCtfe()
		{} __unused;

	public:
		template<int metaval>
		struct metaInt : __markAsCtfe(), public _metaInt
		{
			struct _internal_value_
			{
				__unused bool value[metaval];
			}__unused;
		}__unused;

		struct metaTrue : public metaBool, __markAsCtfe()
		{
			using isTrue = void;
			static constexpr auto clotureTypeName()
			{
				return "metaBool<value = True>";
			}
		}__unused;

		struct metaFalse : public metaBool, __markAsCtfe()
		{
			using isFalse = void;
			static constexpr auto clotureTypeName()
			{
				return "metaBool<value = False>";
			}
		}__unused;

		struct metaNull : __markAsCtfe()
		{
			static constexpr auto clotureTypeName()
			{
				return "(NULL type)";
			}
		}__unused;

		template<bool value>
		struct makeMetabool
		{

		}__unused;

		template<>
		struct makeMetabool<false>
		{
			using type = metaFalse;
		}__unused;

		template<>
		struct makeMetabool<true>
		{
			using type = metaTrue;
		}__unused;

		template<typename funcType>
		struct metaFunctionBuilder
		{
			template<funcType func>
			struct metaFunction
			{
				static constexpr funcType f = func;
				metaFunction() {}
			};

			template<funcType f>
			static constexpr auto buildFunction()
			{
				return metaFunction<f>();
			}
		};

		template<typename T1, typename T2>
		struct metaDuo
		{
			using first 	= T1;
			using second 	= T2;
		};

		template<typename T1, typename T2, typename T3>
		struct metaTrio : public metaDuo<T1, T2>
		{
			//using first		= T1;
			//using second	= T2;
			using third		= T3;
		};

		template<typename T1, typename T2, typename T3, typename T4>
		struct metaQuartet : public metaTrio<T1, T2, T3>
		{
			using fourth = T4;
		};

		template<typename T>
		struct makeMetaVal
		{
			template<T val>
			struct metaValue
			{
				static constexpr T value = val;
			};
		}__unused;

		template<typename T>
		static constexpr auto getMetaFunction = T::f;

		template<typename T>
		static constexpr bool isMetaNull = false;

		template<>
		static constexpr bool isMetaNull<metaNull> = true;

		template<typename T>
		static constexpr bool isMetaBool = false;

		template<>
		static constexpr bool isMetaBool<metaTrue> = true;

		template<>
		static constexpr bool isMetaBool<metaFalse> = true;

		template<typename T>
		static constexpr bool isMetaInt = __is_base_of(_metaInt, T);

		/*
		 * remember to check with isMetaBool before using this
		 */
		template<typename T>
		static constexpr bool metaBoolValue = internalMetaBoolValue<T>();

		template<typename T>//const char* className>
		struct namedClass
		{
			static constexpr auto clotureTypename()
			{
				return T::str;//className;
			}
		};

	#define		__clClassname(name)		\
	public cloture::util::meta::namedClass<mMetaString("nameTest", 128)>

	};
}

#define		mIfMetaTrue(condition)		__if_exists(cloture::util::meta::makeMetabool<condition>::type::isTrue)
#define		mIfMetaFalse(condition)		__if_not_exists(cloture::util::meta::makeMetabool<condition>::type::isTrue)

#define		mMakeMetaFunc(f)			typeof(cloture::util::meta::metaFunctionBuilder<typeof(f)>::buildFunction<f>())

#define		mMakeMetaValue(value)		cloture::util::meta::makeMetaVal<typeof(value)>::metaValue<value>
#define		mGetMetaValue(metat)		metat::value


#define static_if(expr)     __if_exists(cloture::util::meta::makeMetabool<(expr)>::type::isTrue)
#define static_else(expr)   __if_not_exists(cloture::util::meta::makeMetabool<(expr)>::type::isTrue)