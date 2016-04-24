/* Klasa zawieraj�ca wyj�tek, kt�rym rzuca SdlLibrary::Window*/
#pragma once
#include "Libraries/stringNSTL.hpp"
namespace SdlLibrary {
	class Exception
	{
	public:
		Exception(NonSTL::string message, int code=0);
		~Exception();
		const NonSTL::string Message();//pobierz tekst wyj�tku
		const int Code();//pobierz kod wyj�tku
	private:
		NonSTL::string message;
		int code;
	};

}