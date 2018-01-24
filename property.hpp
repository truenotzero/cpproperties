
/*
 * property.hpp
 *
 *  Created on: Jan 23, 2018
 *      Author: truenotzero
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <functional>
#include <utility>
#include <type_traits>

namespace property {

namespace detail {
struct list_t{};
struct ctor_t {};
} /* namespace detail */

inline namespace tag {
constexpr detail::list_t list{};
constexpr detail::ctor_t ctor{};
} /* namespace tag */

template<typename T, typename GetterRet=T&, typename SetterRet=T&>
class Property
{
	/* detail */
	using Getter = std::function<GetterRet(T& field)>;
	using Setter = std::function<SetterRet(T& field, T const& new_value)>;
	using U = std::conditional_t<std::is_same_v<GetterRet, T&>, T, std::remove_const_t<T>>;
	/* detail end */
public:
	Property() = default;

	// forward constructor
	template<typename... Args>
	Property(detail::ctor_t, Args&&... args) : field_ ( std::forward<Args>(args)... ) {}

	// forward initializer list
	template<typename... Args>
	Property(detail::list_t, Args&&... args) : field_ { std::forward<Args>(args)... } { }


	// get/set defining constructors
	Property(Getter const& getter) : getter_ { getter } {}
	Property(Getter const& getter, Setter const& setter) : getter_ { getter }, setter_ { setter } {}

	T& set(T const& t)
	{
		return setter_(field_, t);
	}

	// copy semantics
	Property(T const& t)
	{
		set(t);
	}

	SetterRet operator=(T const& t)
	{
		return set(t);
	}

	// move semantics
	Property(T&& t) : field_ (std::forward<T>(t)) {}

	SetterRet operator=(T&& t)
	{
		return field_ = std::forward<T>(t);
	}

	GetterRet get()
	{
		return getter_(field_);
	}

	// commodity operators
	U* operator->()
	{
		if constexpr (std::is_lvalue_reference_v<GetterRet>)
		{
			return &get();
		}
		else
		{
			field_ = get();
			return &field_;
		}
	}

	operator GetterRet()
	{
		return get();
	}

	// no need for these
	Property(Property<T> const&) = delete;
	Property(Property<T>&&) = delete;
	Property<T>& operator=(Property<T> const&) = delete;
	Property<T>& operator=(Property<T>&&) = delete;
private:
	U field_ {};

	Getter getter_ = [] (T& field) -> T& {
		return field;
	};

	Setter setter_ = [] (T& field, T const& new_value) -> T& {
		if constexpr (!std::is_const_v<T>)
		{
			field = new_value;
		}

		return field;
	};
};

} /* namespace property */

#endif /* PROPERTY_HPP_ */

