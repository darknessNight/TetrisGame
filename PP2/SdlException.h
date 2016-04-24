/* Klasa zawieraj¹ca wyj¹tek, którym rzuca SdlLibrary::Window*/
#pragma once
#include "Libraries/stringNSTL.hpp"
namespace SdlLibrary {
	class Exception
	{
	public:
		Exception(NonSTL::string message, int code=0);
		~Exception();
		const NonSTL::string Message();//pobierz tekst wyj¹tku
		const int Code();//pobierz kod wyj¹tku
	private:
		NonSTL::string message;
		int code;
	};

}