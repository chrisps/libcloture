#pragma once

/*
 *	Two types of "variables" for reflection: traits and attributes
 *	traits are booleans. they may either exist or not exist
 *
 *	attributes consist of two parts:
 *		a type alias (for checking if the attribute exists using SFINAE)
 *		the static constexpr value (declared type is the alias)
 *
 *	reflection is mostly wrapped with macros. This is so that
 *	future C++ revisions may be taken advantage of if they add reflection
 *	to the language.
 *
 * __if_exists and __if_not_exists are used on compilers that support it,
 * to cut down on excessive template instantiation.
 *
 * The system is set up so that SFINAE may be used if __if_exists
 * is not an option.
 *
 * type trait names, attributes types, and attribute names
 * are all "mangled" by the macros to reduce namespace pollution/potential
 * name conflicts.
*/


#define		__has_member_checker(memberType, identifier, checker)		\
		template<typename T> struct checker								\
		{																\
			struct fallback {	memberType identifier;	};				\
			struct derived	:	T, fallback{};							\
			template<typename C, C> struct chT;							\
			template<typename C> static char 							\
			(&f(chT<memberType fallback::*, &C::identifier>*))[1];		\
			template<typename C> static char (&f(...))[2];				\
			static constexpr bool value = sizeof(f<derived>(0)) == 2;	\
}

#define		___CHECKER_DUMMY_STATIC_FUNCTION_ATTRIBUTES __unused __cold


#define		___DO_EXPANSION(x, y)		x##y

#define		__MANGLE_IDENTIFIER_CHECKER_DUMMY_STATFUNC_NAME(x)		\
			___DO_EXPANSION(___IDENTIFIER_CHECKER_DUMMY_STATFUNC_, x)

#define		__MANGLE_IDENTIFIER_CHECKER_STRUCT_NAME(identifier)	\
			___DO_EXPANSION(___IDENTIFER_CHECKER_STRUCTURE_, identifier)

#define		__MANGLE_IDENTIFIER_CHECKER_STRUCT_TYPENAME_ARG(identifier)	\
			___DO_EXPANSION(___IDENTIFER_CHECKER_TEMPLATE_TYPENAME_, identifier)


#define		mCreateIdentifierChecker(identifier)										\
	template<																			\
	typename 																			\
	__MANGLE_IDENTIFIER_CHECKER_STRUCT_TYPENAME_ARG(identifier)							\
	> 																					\
	class __MANGLE_IDENTIFIER_CHECKER_STRUCT_NAME(identifier)							\
	{																					\
		___CHECKER_DUMMY_STATIC_FUNCTION_ATTRIBUTES 									\
		static auto 																	\
		__MANGLE_IDENTIFIER_CHECKER_DUMMY_STATFUNC_NAME(identifier)		()				\
		{																				\
			__if_exists(																\
			__MANGLE_IDENTIFIER_CHECKER_STRUCT_TYPENAME_ARG(identifier)::identifier)	\
			{																			\
				return .0f;																\
			}																			\
			__if_not_exists(															\
			__MANGLE_IDENTIFIER_CHECKER_STRUCT_TYPENAME_ARG(identifier)::identifier)	\
			{																			\
				return 0LL;																\
			}																			\
		}																				\
	public:																				\
		static constexpr bool value = sizeof(__typeof(									\
		__MANGLE_IDENTIFIER_CHECKER_DUMMY_STATFUNC_NAME(identifier) () )) 				\
		== sizeof(float);																\
	}


#define		mCallIdentifierChecker(TYPENAME, identifier)							\
	__MANGLE_IDENTIFIER_CHECKER_STRUCT_NAME(identifier) <TYPENAME>::value


#define		__MANGLE_TYPE_RESOLVER_DUMMY_STATFUNC_NAME(x)		\
			___DO_EXPANSION(___TYPE_RESOLVER_DUMMY_STATFUNC_, x)

#define		__MANGLE_TYPE_RESOLVER_STRUCT_NAME(identifier)	\
			___DO_EXPANSION(___TYPE_RESOLVER_STRUCTURE_, identifier)

#define		__MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(identifier)	\
			___DO_EXPANSION(___TYPE_RESOLVER_TEMPLATE_TYPENAME_, identifier)


/*
 * if the typename subTypeName exists in the class, type is declare to be
 * the type of T::subTypeName
 * else type is T
 */
#define		mCreateTypeResolver(subTypeName)											\
	template<																			\
	typename 																			\
	__MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(subTypeName)								\
	> 																					\
	class __MANGLE_TYPE_RESOLVER_STRUCT_NAME(subTypeName)								\
	{																					\
		___CHECKER_DUMMY_STATIC_FUNCTION_ATTRIBUTES 									\
		static auto 																	\
		__MANGLE_TYPE_RESOLVER_DUMMY_STATFUNC_NAME(subTypeName)		()					\
		{																				\
			__if_exists(																\
			__MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(subTypeName)::subTypeName)		\
			{																			\
				return (																\
				typename __MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(subTypeName)::subTypeName*	\
				) nullptr;																\
			}																			\
			__if_not_exists(															\
			__MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(subTypeName)::subTypeName)		\
			{																			\
				return (__MANGLE_TYPE_RESOLVER_STRUCT_TYPENAME_ARG(subTypeName)*)		\
				 nullptr;																\
			}																			\
		}																				\
	public:																				\
		static constexpr auto type__ = 													\
		(__typeof(__MANGLE_TYPE_RESOLVER_DUMMY_STATFUNC_NAME(subTypeName) ()))nullptr;	\
		using type = __typeof(*type__);													\
	}

#define		mGetTypeResolverTypename(TYPENAME, subTypeName)	\
		typename __MANGLE_TYPE_RESOLVER_STRUCT_NAME(subTypeName) <TYPENAME>::type

/*
	if the value is true, then the typename is defined as an alias
	of bool
*/

#define		___CLOTURE_TYPE_TRAIT_true(name)					\
			using name = bool
/*
 	 if the value is false, then the typename is left undefined
*/
#define		___CLOTURE_TYPE_TRAIT_false(name)

#define		___EXPAND_CLOTURE_TYPE_TRAIT___(___indir, name)		\
			___indir(name)

#define		___MANGLE_TRAIT_NAME(name)				__CLOTURE_TRAIT_##name

#define		___MANGLE_ATTRIBUTE_TYPE_NAME(name)		__CLOTURE_ATTRIBUTE_##name
#define		___MANGLE_ATTRIBUTE_VALUE_NAME(name)	__CLOTURE_ATTRIBUTE_VALUE_##name
/*
 	 the actual boolean value is not used right now
 	 instead the text is concatenated onto ___CLOTURE_TYPE_TRAIT_
 	 so that either ___CLOTURE_TYPE_TRAIT_false or
 	 ___CLOTURE_TYPE_TRAIT_true gets called

	if you supply a value besides true or false... yer fuckin' stupid boi
*/
#define		__set_type_trait(name, value)						\
			___EXPAND_CLOTURE_TYPE_TRAIT___(					\
			___CLOTURE_TYPE_TRAIT_##value,						\
			___MANGLE_TRAIT_NAME(name))
/*
 * 	declare the type's alias and then declare the attribute
 * 	as a compile-time member of the structure/class
 */
#define		__declare_type_attribute(type, name, value)			\
			using ___MANGLE_ATTRIBUTE_TYPE_NAME(name) = type;	\
			static constexpr ___MANGLE_ATTRIBUTE_TYPE_NAME(name)\
			___MANGLE_ATTRIBUTE_VALUE_NAME(name) = (value)

/*
 * 	hasMempool should be set to true if the structure
 * 	contains a pointer to a mempool that may be allocated from
 */
#define		__traitHasMempool(value)			\
			__set_type_trait(hasMempool, value)

/*
 * isStateObject should be set to true if the structure
 * is supposed to contain the "global state" of any particular
 * piece of the engine
 *
 * for instance, the RenderState object is a state object
 */
#define		__traitIsStateObject(value)			\
			__set_type_trait(isStateObject, value)

/*
 * isPlane should be set to true for plane objects
 */
#define		__traitIsPlane(value)				\
			__set_type_trait(isPlane, value)


#define		__traitIsVector(value)		__set_type_trait(isVector, value)

#define		__traitIsMatrix(value)		__set_type_trait(isMatrix, value)

#define		__traitHasColor(value)		__set_type_trait(hasColor, value)

#define		__traitHasVertex(value)		__set_type_trait(hasVertex, value)

/*
 * 	set to true for low-level engine structures that are not
 * 	related directly to the game itself
 *
 * 	examples: renderer structures, sound structures, network structures...
 * 	you geddit
 */
#define		__traitIsInternal(value)	__set_type_trait(isInternal, value)

/*
 * 	set to true if the structure is loaded from a file
 */
#define		__traitIsFromFS(value)		__set_type_trait(isFromFS, value)



/*
 * 	set to true if the structure contains a pointer to the name of the file
 * 	it was loaded from
 */
#define		__traitHasFilename(value)	__set_type_trait(hasFilename, value)

#define		___DECLARE_TRAIT_CHECKER___(name, trait)	\
	template<typename T> static constexpr bool name ()	\
	{													\
		__if_exists(T::___MANGLE_TRAIT_NAME(trait))		\
		{												\
			return true;								\
		}												\
		__if_not_exists(T::___MANGLE_TRAIT_NAME(trait))	\
		{												\
			return false;								\
		}												\
	}

#define		__DECLARE_TRAIT_CHECKER_DEFAULT___(name)	\
	___DECLARE_TRAIT_CHECKER___(name, name)



namespace cloture::util::reflect
{
	template<typename T>
	class isIndexableChecker : __markAsCtfe()
	{
		template<typename T, bool isAClass = false>
		struct indexerSwitch
		{
			//if not a class, then just check whether its a pointer or array
			static constexpr bool value = generic::isPointer<T>() || generic::isArray<T>();
		};

		template<typename T>
		struct indexerSwitch<T, true>
		{
			//if it is a class, then we need to check for operator[]


		};


	};

	enum class AccessLevel : common::int8
	{
		Public,
		Private,
		Protected
	};

	template<
	AccessLevel accessLevel,
	typename 	fieldType,
	typename 	fieldNameAsMetaString,
	typename 	ownerClass,
	typename 	previousField = void>
	struct Field
	{
	private:
		template<typename T = void>
		struct offsetCalculator
		{
			static constexpr size_t tryAndCalculate()
			{
				constexpr size_t lastOffset 	= T::offset;
				constexpr size_t lastMemberSize = sizeof(T::type);
				constexpr size_t currAlign		= alignof(T);
				return lastOffset + lastMemberSize; //incomplete
			}
		}__unused;

		//first member in the class
		template<>
		struct offsetCalculator<void>
		{
			/*
			 * if the class is polymorphic, then the virtual function table is the first member
			 * of the class. if isPolymorphic returns true, the size will be the size of the
			 * vtbl pointer. otherwise it's zero
			 */
			static constexpr size_t vtblAdjust =
			(static_cast<size_t>(generic::isPolymorphic<ownerClass>()) & 1)
			* sizeof(void*);

			static constexpr size_t offset = 0 + vtblAdjust;

		}__unused;
	public:
		static constexpr auto name 				= fieldNameAsMetaString::str;
		static constexpr AccessLevel access 	= accessLevel;
		using type 	= fieldType;
		using owner = ownerClass;

	};

}//namespace cloture::util::reflect
