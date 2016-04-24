/* odpowiednik std::string. Przygotowane ze wzglêdu na zakaz u¿ycia bibliotek STL
*/
#pragma once
#include "vectorTempl.h"

namespace NonSTL{

template <typename T> class stringBase:public vector<T>
	{
	public:
		stringBase();
		stringBase(const T);
		stringBase(const T*);
		stringBase(const stringBase<T>&);

		~stringBase();

		stringBase<T> operator+(const stringBase<T>&);

		void operator+=(const T);
		void operator+=(const T*);
		void operator+=(const stringBase<T>&);

		void operator=(const T);
		void operator=(const T*);
		//void operator=(const stringBase<T>&);

		int find(const stringBase<T>&);
		int rfind(const stringBase<T>&);

		int compare(const stringBase<T>&);
		int compare(const T*);

		bool operator<(const stringBase<T>&);
		bool operator<(const T*);

		bool operator>(const stringBase<T>&);
		bool operator>(const T*);

		//bool operator==(const T*);

		const T& operator[] (unsigned pos);
		const T& at (unsigned pos);

		unsigned size();
		unsigned lenght();

		stringBase<T> substr(unsigned pos = 0, unsigned len = 0);

		const T* c_str();

	private:
		unsigned cStrSize(const T*);

	};

	template <typename T> stringBase<T> operator+(const T, const stringBase<T>&);
	//template <typename T> stringBase<T> operator+(const stringBase<T>&, const T);
	template <typename T> stringBase<T> operator+(const T*, const stringBase<T>&);
	template <typename T> stringBase<T> operator+(const stringBase<T>&, const T*);
	//template <typename T> stringBase<T> operator+(const stringBase<T>&, const stringBase<T>&);


typedef stringBase<char> string;
typedef stringBase<wchar_t> wstring;

}