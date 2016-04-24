#pragma once

//przeci¹¿one operatory
void* operator new(size_t cbSize);
void* operator new[](size_t cbSize);
void* operator new(size_t, const char*, int);
void* operator new[](size_t, const char*, int);
void operator delete(void* p);
void operator delete[](void* p);

//podmiana zwyklego new na rozszerzon¹ wersjê
#ifndef IMPLEM
#define new new(__FILE__ , __LINE__)
#endif