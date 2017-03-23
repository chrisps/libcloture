#pragma once

/*
 * 	template/macros for use with variadic templates
 */

namespace cloture		{
namespace util			{
namespace templates		{
namespace variadic		{



struct ___NO_VARG___
{
	static constexpr auto clotureTypeName()
	{
		return "___NO_VARG___";
	}
	static constexpr char filler = 0;

	constexpr operator void*() const
	{
		return nullptr;
	}
	constexpr operator const void*()
	{
		return nullptr;
	}
	constexpr operator const char*() const
	{
		return nullptr;
	}
	constexpr operator const char*()
	{
		return nullptr;
	}
}__unused;

template<typename T>
static constexpr size_t vaSizeof = sizeof(T);

template<>
static constexpr size_t vaSizeof<___NO_VARG___> = 0;

template<typename T = ___NO_VARG___, typename... others>
struct ___getTypeVArg1
{
	using type = T;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2 = ___NO_VARG___, typename... others>
struct ___getTypeVArg2
{
	using type = T2;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2 = ___NO_VARG___,
typename T3 = ___NO_VARG___, typename... others>
struct ___getTypeVArg3
{
	using type = T3;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2= ___NO_VARG___,
typename T3= ___NO_VARG___, typename T4  = ___NO_VARG___, typename... others>
struct ___getTypeVArg4
{
	using type = T4;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2= ___NO_VARG___, typename T3= ___NO_VARG___,
typename T4= ___NO_VARG___, typename T5  = ___NO_VARG___, typename... others>
struct ___getTypeVArg5
{
	using type = T5;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2= ___NO_VARG___, typename T3= ___NO_VARG___,
typename T4= ___NO_VARG___, typename T5= ___NO_VARG___, typename T6  = ___NO_VARG___, typename... others>
struct ___getTypeVArg6
{
	using type = T6;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1 = ___NO_VARG___, typename T2= ___NO_VARG___, typename T3= ___NO_VARG___,
typename T4= ___NO_VARG___, typename T5= ___NO_VARG___, typename T6= ___NO_VARG___, typename T7 = ___NO_VARG___, typename... others>
struct ___getTypeVArg7
{
	using type = T7;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2= ___NO_VARG___, typename T3= ___NO_VARG___, typename T4= ___NO_VARG___,
typename T5= ___NO_VARG___, typename T6= ___NO_VARG___, typename T7= ___NO_VARG___, typename T8 = ___NO_VARG___, typename... others>
struct ___getTypeVArg8
{
	using type = T8;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T1= ___NO_VARG___, typename T2= ___NO_VARG___,
typename T3= ___NO_VARG___, typename T4= ___NO_VARG___,
typename T5= ___NO_VARG___, typename T6 = ___NO_VARG___, typename T7= ___NO_VARG___,
typename T8= ___NO_VARG___, typename T9 = ___NO_VARG___, typename... others>
struct ___getTypeVArg9
{
	using type = T9;
	static constexpr size_t typeSize 		= vaSizeof<type>;
	static constexpr size_t remainingArgs 	= sizeof...(others);
}__unused;

template<typename T> static constexpr bool isValidVArg = true;
template<> static constexpr bool isValidVArg<___NO_VARG___> = false;

#define		getNthTypename(n, args)		\
typename cloture::util::templates::variadic::___getTypeVArg##n <args...>::type

#define		getNthTypenameSize(n, args)	\
		cloture::util::templates::variadic::___getTypeVArg##n <args...>::typeSize

/*
 * 	container for non-typename elements
 */
//template<typename T, T... pack>
template<auto... pack>
struct paramPack
{
	static constexpr size_t size = sizeof...(pack);
	//using type = T;
	/*static constexpr typeof( pack , ...) arr[] =
	{
		(... , pack)
	};*/
	typeof(pack , ...) arr[size];

	constexpr paramPack() : arr{pack...}{}

	constexpr auto operator[](size_t index) const
	{
		return arr[index];
	};
	/*
	constexpr auto& operator[](size_t index) const
	{
		return arr[index];
	}*/
};
		/*
template<>
struct paramPack<void>
{
	static constexpr size_t size = 0;
	using type = void;
}__unused;*/
#if 0
template<typename... list>
struct MetaList
{
	template<size_t listLength, typename... list_>
	struct ListBuilder
	{
		template<typename T> struct defaultTemplate
		{
			using data = T;
		};
		template<
		size_t index,
		typename previousNode,
		template<typename... UU> class dataHolder = defaultTemplate,
		typename T = void,
		typename NextType = meta::metaNull,
		typename... nextOnes
		>
		struct Node
		{
			using NodeType	= Node<index, previousNode, dataHolder, T, NextType, nextOnes...>;
			using previous	= previousNode;
			using next		= Node<index + 1, NodeType, dataHolder, NextType, nextOnes...>;
			using data		= T;
			static constexpr size_t Index=index;
			using getLast = typename NodeType::next::getLast;
			
			static constexpr bool hasPrevious()
			{
				return !meta::isMetaNull<previous>;
			}
			static constexpr bool hasNext()
			{
				return true;
			}

			static constexpr size_t getIndex()
			{
				return index;
			}

			template<size_t n> struct getAtIndex
			{
				template<bool isMe = n == getIndex(), bool hasNext_ = hasNext()>
				struct helper
				{
					using Result = typename next::getAtIndex<n>::helper::Result;
				};

				template<bool hasNext_>
				struct helper<true, hasNext_>
				{
					using Result = NodeType;
				};
			};

			template< template<typename... arg> class instantiateFor, typename... userData>
			static constexpr void instantiateForeach(userData... Args)
			{
				//instantiateFor<>(Args...);
				using instantiation = instantiateFor<userData...>;
				//instantiation(Args...);
				instantiation current = instantiation();
				current(Args...);
				next::instantiateForeach<instantiateFor, userData...>(Args...);
			}
			template< template<typename next, typename TT, typename... TS> class inheritor, typename... ts> 
			struct Inherit
			{
				#if 0
				template<typename next, typename T2>
				//struct pregen : public inheritor<data>, public T2
				struct pregen : public inheritor< typename NodeType::next::Inherit<pregen, ts...>::gen, T>, public T2
				{
					static constexpr bool isLast = false;
				};
	
				template<size_t ix = NodeType::Index>
				struct dogen
				{
					using gen = typename NodeType::next::Inherit<pregen, ts...>::gen;
				};
				using gen = typename NodeType::next::Inherit<pregen, ts...>::gen;
				#endif
				
				template<size_t INDEX = NodeType::Index> struct generate
				{
					template<typename NEXT_, typename T_T, typename... uhhhh >
					struct pregen : public NodeType::previous::Inherit<inheritor, ts...>::generate::pregen<pregen
					{
						
					};
				};
			};
		}__unused;

		/*
		 * last Node
		 */
		template<
		size_t index,
		typename previousNode,
		template<typename... UU> class dataHolder,
		typename T>
		struct Node<index, previousNode, dataHolder, T, meta::metaNull>
		{
			using NodeType = Node<index, previousNode, dataHolder, T, meta::metaNull>;
			using previous = previousNode;
			using next = meta::metaNull;
			using data = T;
			using getLast = NodeType;
			static constexpr bool hasPrevious()
			{
				return !meta::isMetaNull<previous>;
			}
			static constexpr size_t getIndex()
			{
				return index;
			}
			//last node, done iterating
			template< template<typename... arg> class instantiateFor, typename... userData>
			static constexpr void instantiateForeach(userData... Args)
			{
				//instantiateFor<>(Args...);
				using instantiation = instantiateFor<userData...>;
				//instantiation(Args...);
				instantiation current = instantiation();
				current(Args...);
			}

			static constexpr bool hasNext()
			{
				return false;
			}
			
	
			template<template<typename next, typename T, typename... TS> class inheritor, typename... ts> 
			struct Inherit
			{
				/*struct gen : public inheritor<data>
				{
				};*/
				//template<typename next>//>
				//struct pregen : public inheritor<data>, public T2
				
				using thisType = NodeType::Inherit<inheritor, ts...>;
				
				
				struct gen : public inheritor< ts...,//meta::metaNull, 
				T>//, public T2
				{
					static constexpr bool isLast = true;
				};
				//using gen = pregen<meta::metaNull, //next::Inherit<pregen>::gen;
				//using gen =
				
				
				
				
				template<size_t INDEX = NodeType::Index> struct generate
				{
					template<typename NEXT_, typename T_T, typename... uhhhh>
				struct pregen : public inheritor<NEXT_, NodeType::data::data, ts...>
				{
					
				};
					//using preg = 
				};
				using generated = typename NodeType::previous::Inherit<inheritor, ts...>::generate::pregen;
			};

		}__unused;

		using built = Node< 0, meta::metaNull, defaultTemplate, list_... >;
	}__unused;

	template<>
	struct ListBuilder<0>
	{
		using built = meta::metaNull;
	}__unused;
	using Builder = ListBuilder<sizeof...(list), list...>;
	using List = typename Builder::built;


	static constexpr size_t getLength()
	{
		return sizeof...(list);
	}

	static constexpr bool canIterate()
	{
		return !meta::isMetaNull<List>;
	}

	template<
	template<typename... userData> class Iterator,
	typename... uD
	>
	constexpr void cexprCallForeach(uD... userDat)
	{
		static_assert(
		canIterate(),
		"No List to iterate over!"
		);
		List::instantiateForeach<Iterator>(userDat...);
	}

	template< template< typename... args > class Iterator>
	struct synthesizeList
	{
		using Result = typename Builder::Node< 0, meta::metaNull, Iterator, list...>;
	};

	template<size_t index>
	struct getNth
	{
		static_assert(!meta::isMetaNull<List>);

	};
	
	template< template<typename next, typename T, typename... TS> class inheritor, typename master> 
	struct inheritList
	{
		using lastNode = typename List::getLast;
		
		//using inherited = typename ;
		using inherited = typename List::Inherit<inheritor, master>::gen;//<master>;
		
		//static_assert(!inherited::isLast);
	};


}__unused;


using mListTest = MetaList<int, const int*, char>;

template<typename... Ts>
struct iteratorTest
{
	template<typename T, typename... Tothers>
	struct getOne
	{
		using type = T;
	};
	using type = typename getOne<Ts...>::type;
};
using synthesized = typename mListTest::synthesizeList<iteratorTest>::Result;
#endif
constexpr paramPack</*size_t, */1, 2, 3, 4, 5> ___PARAM_PACK_TEST___;
constexpr auto ___PARMVAL___ = ___PARAM_PACK_TEST___[1];

static_assert(___PARMVAL___ == 2);

}//namespace variadic
}//namespace templates
}//namespace util
}//namespace cloture

