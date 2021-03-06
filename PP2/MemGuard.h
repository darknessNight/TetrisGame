#pragma once

//przeciążone operatory
void* operator new(size_t cbSize);
void* operator new[](size_t cbSize);
void* operator new(size_t, const char*, int);
void* operator new[](size_t, const char*, int);
void operator delete(void* p);
void operator delete[](void* p);

//podmiana zwyklego new na rozszerzoną wersję
#ifndef IMPLEM
#define new new(__FILE__ , __LINE__)
#endif