#pragma once

namespace reflect {
template <class T1, class T2>
inline T1 ptr_cast(T2* _ptr) {
	return static_cast<T1>(static_cast<void*>(_ptr));
}

template <class T1, class T2>
inline T1 ptr_cast(const T2* _ptr) {
	return static_cast<T1>(static_cast<const void*>(_ptr));
}

} //namespace reflect
