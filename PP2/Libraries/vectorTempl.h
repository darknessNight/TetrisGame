/* odpowiednik stosu std::vector z biblioteki vector. Przygotowane ze wzglêdu na zakaz u¿ycia bibliotek STL
*/
#pragma once

namespace NonSTL{
	template <class T, int startSize=32> class vector
	{
	public:
		vector();
		~vector();
		void push_back(T val);
		T pop_back();
		unsigned int size();
		T& operator[](unsigned i);
		bool operator==(const vector<T, startSize>&);
		void operator=(const vector<T, startSize>&);
		void remove(T el);
		void clear();
	protected:
		void resize(int sign);
	protected:
		unsigned sizeTab=0;
		T* tab;
		unsigned int currentMulti = 1;
		const unsigned int multiplier = startSize;
	};

}