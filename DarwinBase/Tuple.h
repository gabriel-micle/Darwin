
#ifndef _TUPLE_H_
#define _TUPLE_H_

template <class T>
class Tuple {

public:

	T		x, y, z;

			Tuple ();
			Tuple (T x, T y, T z);

	bool	operator <  (const Tuple &) const;
	bool	operator == (const Tuple &) const;
};

template <class T>
Tuple<T>::Tuple () : x(0), y(0), z(0) {}

template <class T>
Tuple<T>::Tuple (T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

template <class T>
bool Tuple<T>::operator < (const Tuple & t) const {
	if (x == t.x) {
		if (y == t.y) {
			return z < t.z;
		} else {
			return y < t.y;
		}
	} else {
		return x < t.x;
	}
}

template <class T>
bool Tuple<T>::operator == (const Tuple & t) const {
	return x == t.x && y == t.y && z == t.z;
}

#endif
