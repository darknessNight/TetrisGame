/* definicje metod NonSTL::vector
*/
#include "vectorTempl.h"
#include <memory>

namespace NonSTL{
	template <class T, int startSize=32> vector<T, startSize>::vector()
	{
		currentMulti = 1;
		tab = new T[multiplier];
		memset(tab, 0, multiplier*sizeof(T));
		sizeTab = 0;
	}


	template <class T, int startSize=32> vector<T, startSize>::~vector()
	{
		delete tab;
	}


	template <class T, int startSize=32> void vector<T, startSize>::push_back(T val)
	{
		resize(1);
		tab[sizeTab] = val;
		sizeTab++;
	}


	template <class T, int startSize=32> T vector<T, startSize>::pop_back()
	{
		T ret = tab[sizeTab-1];
		resize(-1);
		sizeTab--;
		return ret;
	}


	template <class T, int startSize=32> unsigned int vector<T, startSize>::size()
	{
		return sizeTab;
	}


	template <class T, int startSize=32> void vector<T, startSize>::resize(int sign){//zmiana wielkoœci tablicy
		if (sign == 0) return;
		if (sign < 0) {
			if (sizeTab - 1 < (multiplier/2)*currentMulti)
				currentMulti/=2;
			else return;
		}
		else {
			if (sizeTab + 1 >= multiplier*currentMulti)
				currentMulti*=2;
			else return;
		}

		if (currentMulti == 0){ currentMulti = 1; return; }

		T* tmpTab = new T[multiplier*currentMulti];
		memset(tmpTab, 0, multiplier*currentMulti*sizeof(T));

		memcpy(tmpTab, tab, sizeof(T)*multiplier*currentMulti);
		delete tab;
		tab = tmpTab;
	}


	template <class T, int startSize=32> T& vector<T, startSize>::operator[](unsigned i){
		if (i < sizeTab) return tab[i];
		else throw std::exception("Overrange index");
	}


	template <class T, int startSize=32> bool vector<T, startSize>::operator==(const vector<T, startSize>& com){
		if (com.sizeTab != this->sizeTab) return false;

		for (unsigned i = 0; i < sizeTab;i++)
		if (com[i] != tab[i]) return false;

		return true;
	}


	template <class T, int startSize=32> void vector<T, startSize>::operator=(const vector<T, startSize>& com){
		this->currentMulti = com.currentMulti - 1;
		this->resize(1);
		this->sizeTab = com.sizeTab;
		memcpy(tab, com.tab, com.sizeTab*sizeof(T));
	}

	template <class T, int startSize=32> void vector<T, startSize>::remove(T el) {
		for (unsigned i = 0; i < sizeTab; i++) {
			if (tab[i] == el) {
				for (int j = i; j < sizeTab - 1; j++) {
					tab[j] = tab[j + 1];
				}
				sizeTab--;
				break;
			}
		}
	}

	template <class T, int startSize=32> void vector<T, startSize>::clear() {
		sizeTab = 0;
		currentMulti = 1;
		delete[] tab;
		tab = new T[multiplier*currentMulti + 1];
	}
}