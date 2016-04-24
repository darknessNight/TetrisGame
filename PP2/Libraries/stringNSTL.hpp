/* definicje metod z stringNSTL.h
*/
#include "stringNSTL.h"
#pragma once

#ifndef STRINGNSTL_HPP
#define STRINGNSTL_HPP

namespace NonSTL{

	template <typename T> stringBase<T>::stringBase(){
		this->push_back('\0');
	}


	template <typename T> stringBase<T>::~stringBase(){
	}

	template <typename T> stringBase<T>::stringBase(const T val){
		this->push_back(val);
		this->push_back('\0');
	}


	template <typename T> stringBase<T>::stringBase(const T* val){
		unsigned tmp = cStrSize(val);
		for (unsigned int i = 0; i < tmp;i++)
			this->push_back(val[i]);
		this->push_back('\0');
	}


	template <typename T> stringBase<T>::stringBase(const stringBase<T>& val){
		(*this) = val;
	}


	template <typename T> stringBase<T> operator+(const T val1, const stringBase<T>& val2){
		return stringBase<T>(val1) + val2;
	}


	/*template <typename T> stringBase<T> operator+(const stringBase<T>& val1, const T val2){
		return val1 + stringBase<T>(val2);
	}*/


	template <typename T> stringBase<T> operator+(const T* val1, const stringBase<T>& val2){
		return stringBase<T>(val1) +val2;
	}


	template <typename T> stringBase<T> operator+(const stringBase<T>& val1, const T* val2){
		return val1 + stringBase<T>(val2);
	}

	template <typename T> stringBase<T> stringBase<T>::operator+(const stringBase<T>& val1) {
		stringBase<T> tmp=(*this);
		tmp->pop_back();
		for (unsigned i = 0; i < val1.size(); i++)
			tmp.push_back(val1[i]);
		return tmp;
	}


	template <typename T> void stringBase<T>::operator+=(const stringBase<T>& val1) {
		this->pop_back();
		for (unsigned i = 0; i < val1.size(); i++)
			this->push_back(val1[i]);
	}


	template <typename T> void stringBase<T>::operator+=(const T val){
		if (sizeTab == 0)
			this->push_back(val);
		else tab[sizeTab - 1] = val;
		this->push_back('\0');
	}


	template <typename T> void stringBase<T>::operator+=(const T* val){
		(*this) = (*this) + stringBase<T>(val);
	}

	template <typename T> void stringBase<T>::operator=(const T val){
		(*this) = stringBase<T>(val);
	}


	template <typename T> void stringBase<T>::operator=(const T* val){
		(*this) = stringBase<T>(val);
	}


	template <typename T> int stringBase<T>::find(const stringBase<T>& val){
		for (unsigned i = 0; i < this->sizeTab - val.size(); i++){

		}
		return -1;
	}


	template <typename T> int stringBase<T>::rfind(const stringBase<T>&){}


	template <typename T> int stringBase<T>::compare(const stringBase<T>& v){
		for (int i = 0; i < tabSize; i++) {
			if (tab[i] != v.tab[i]) {
				if (tab[i] < v.tab[i]) return -1;
				else return 1;
			}
		}

		return 0;
	}


	template <typename T> int stringBase<T>::compare(const T* v){
		return compare(stringBase<T>(v));
	}


	template <typename T> bool stringBase<T>::operator<(const stringBase<T>& v){
		if (compare(v) > 0) return true;
		else return false;
	}


	template <typename T> bool stringBase<T>::operator<(const T* v){
		return operator<(stringBase<T>(v));
	}


	template <typename T> bool stringBase<T>::operator>(const stringBase<T>& v){
		return !operator<(v);
	}


	template <typename T> bool stringBase<T>::operator>(const T* v){
		return operator>(stringBase<T>(v));
	}


	//template <typename T> bool stringBase<T>::operator==(const T*){}


	template <typename T> const T& stringBase<T>::operator[] (unsigned pos){
		return at(pos);
	}


	template <typename T> const T& stringBase<T>::at(unsigned pos){
		if (pos >= sizeTab) return 0;
		else return tab[pos];
	}


	template <typename T> unsigned stringBase<T>::size(){
		return lenght();
	}


	template <typename T> unsigned stringBase<T>::lenght(){
		return sizeTab-1;
	}


	template <typename T> stringBase<T> stringBase<T>::substr(unsigned pos, unsigned len){
		if (len == 0)//jeœli nie ma d³ugoœci domyœlna d³ugoœæ leci do koñca
			len = this->size()-pos;

		stringBase<T> buff;
		
		for (unsigned i = pos; i<size())
	}


	template <typename T> const T* stringBase<T>::c_str(){
		return (const T*)tab;
	}


	template <typename T> unsigned stringBase<T>::cStrSize(const T* t){
		for (unsigned int i = 0;;i++)
		if (t[i] == 0) return i;
	}
}

#endif