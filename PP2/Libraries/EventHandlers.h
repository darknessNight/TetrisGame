/*Klasa zawieraj�ca struktury i klasy pozwajaj�ce na implementacje zdarze� w programie (jako element MVVC)
*/
#pragma once
#include "vectorTempl.hpp"
#include "EventHandlersCodes.h"

namespace Events{
	struct EventArgs {
		bool Cancel = false;//je�li warto�� true zatrzymuje dalsze wykonywanie
		void* sender = nullptr;
		int time = 0;
	};
	//================================================================================================================================================
	class Object{//object pozwalaj�cy na wykorzystanie MethodHandler (klasa dziedzicz�ca moze bez problemu skorzysta� z MethodHandler)
	public:
		typedef void(Object::* memberFunction)(EventArgs*);
	};
	//================================================================================================================================================
	struct MethodHandler {//uchwyt na metody klasy Object
		Object::memberFunction method;
		Object* object;

		MethodHandler() = default;
		MethodHandler(Object* obj, Object::memberFunction func) :object(obj), method(func) {};
		void operator()(EventArgs *e) {
			if(object!=nullptr)
				(object->*method)(e);
		}

		bool operator==(MethodHandler mh) {
			if (mh.object == this->object && mh.method == this->method) return true;
			else return false;
		}
	};
	//================================================================================================================================================
	struct KeyboardEventArgs :public EventArgs{
		enum SysKeys {
			None = 0,
			LShift = 0x0001,
			RShift = 0x0002,
			LControl = 0x0040,
			RControl = 0x0080,
			LAlt = 0x0100,
			RAlt = 0x0200,
			AltGr = 0x4000,
			LWinKey = 0x0400,
			RWinKey = 0x0800,
			NumPad = 0x1000,
			CapsLock = 0x2000,
			Alt = LAlt|RAlt,
			Control = LControl|RControl,
			Shift = LShift|RShift,
		};
		typedef VirtualKeyCodes VKeyCode;

		int keyCode;//Kod fizycznego klawisza(mo�e si� r�ni� w zale�no�ci od komputera)
		int vKeyCode;//kod virtualny(niezale�ny od komputera)
		int sysKeys = 0;//=enum SysKeys. Info o dodatkowych klawiszach
	};
	//================================================================================================================================================
	struct MouseEventArgs :public EventArgs{
		enum Buttons{
			None,
			LeftButton=1,
			RightButton,
			WheelButton,
			X1Button,
			X2Button
		};
		int x;//wsp�rz�dna x kursora
		int y;//wsp�rz�dna y kursora
		Buttons button = Buttons::None;
		short hWheel=0;//obr�t k�kiem myszki w poziomie
		short vWheel = 0;//obr�t k�kiem myszki w pionie
		int clicks = 0;//ilo�� klikni��
		int lastX = 0;//poprzednie po�o�enie kursora
		int lastY = 0;//poprzednie po�o�enie kursora
	};
	//================================================================================================================================================
	template <typename T, typename P> class EventHandler{//lista funckji do wywo�ania typu stos (LIFO)
	public:
		void operator+=(T f) { if (search(f) == -1)this->functions.push_back(f); }
		void operator-=(T f){ this->functions.remove(f); }
		unsigned int size(){ return this->functions.size(); }
		void clear() { this->functions.clear(); }

		bool Invoke(P e) {
			for (int i = this->functions.size()-1; i >=0 ; i--) {
				((T)(functions[i]))(&e);
				if (e.Cancel) return true;
			}
			return false;
		}
	private:
		NonSTL::vector<T> functions;

		long long int search(T f){
			for (unsigned int i = 0; i < functions.size(); i++){
				if (functions[i]== f) return i;
			}return -1;
		}
	};
}