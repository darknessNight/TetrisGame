#define IMPLEM
#include "MemGuard.h"
#include <stdio.h>
#include <memory>


class MemoryGuard{
public:
	MemoryGuard();
	~MemoryGuard();
	void delDynVar(void* p);
	void addDynVar(void* p, size_t size, const char* file, unsigned line);
private:
	void ToScreen();
	void ToFile();
private:
	bool toFile = true;
	bool toScreen = true;
	char fileName[1000];
	static const unsigned int maxEls = 10000000;//maksymalna liczba zainicjowanych zmniennych

	unsigned dynVar = 0;//iloœæ aktualnie zainicjowanych i nieusuniêtych zmiennych
	unsigned long long dynVarBytes = 0;
	unsigned* dynVars;
	char** dynVarsFile;
	unsigned* dynVarsLine;
	size_t* dynVarsBytes;

	const char commChar = ';';
}guard;
//-----------------------------------------------------------------------------------------------------------------------------------



void operator delete[](void* p) {
	guard.delDynVar(p);
	free(p);
}
//-----------------------------------------------------------------------------------------------------------------------------------


void operator delete(void* p) {
	guard.delDynVar(p);
	free(p);
}
//-----------------------------------------------------------------------------------------------------------------------------------


void* operator new(size_t size){
	void* m = malloc(size);
	guard.addDynVar(m,size,0,0);
	return m;
}
//-----------------------------------------------------------------------------------------------------------------------------------


void* operator new[](size_t size){
	void* m = malloc(size);
	guard.addDynVar(m, size, 0, 0);
	return m;
}
//-----------------------------------------------------------------------------------------------------------------------------------


void* operator new(size_t size, const char* file, int line){
	void* m = malloc(size);
	guard.addDynVar(m, size, file, line);
	return m;
}
//-----------------------------------------------------------------------------------------------------------------------------------


void* operator new[](size_t size, const char* file, int line){
	void* m = malloc(size);
	guard.addDynVar(m, size, file, line);
	return m;
}
//-----------------------------------------------------------------------------------------------------------------------------------


MemoryGuard::MemoryGuard(){
	dynVars = (unsigned*) malloc(maxEls*sizeof(unsigned));
	dynVarsFile = (char**)malloc(maxEls*sizeof(char*));
	dynVarsLine = (unsigned*)malloc(maxEls*sizeof(unsigned));
	dynVarsBytes = (size_t*)malloc(maxEls*sizeof(size_t));
	if (dynVars == nullptr) throw std::exception("Cannot initialize tabs");

	memset(dynVars, 0, maxEls*sizeof(unsigned));
	strcpy(fileName, "MemoryLeaksLog.log");

	char buff[10000];
	FILE* file = fopen("MemoryGuard.conf", "r");
	if (file != nullptr){
		while (!feof(file)){
			fscanf(file, "%[^\n]\n", buff);
			if (buff[0] == commChar || buff[0] == 0) continue;

			if (strstr(buff, "ToFile=")){
				if (strstr(buff, "true")) toFile = true;
				else toFile = false;
			}

			if (strstr(buff, "ToScreen=")){
				if (strstr(buff, "true")) toScreen = true;
				else toScreen = false;
			}

			if (strstr(buff, "Filename=") && strlen(buff + strlen("Filename="))>0){
				strcpy(fileName,buff + strlen("Filename="));
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------


MemoryGuard::~MemoryGuard(){
	if (toScreen) ToScreen();
	if (toFile) ToFile();

	free(dynVars);
	free(dynVarsFile);
	free(dynVarsLine);
	free(dynVarsBytes);
}
//-----------------------------------------------------------------------------------------------------------------------------------


void MemoryGuard::ToScreen(){
	system("cls");
	printf("%s %d\n", "Pozostalo nie usunietych elementow: ", dynVar);
	printf("%s %d\n", "W sumie zaalokowano pamieci: ", dynVarBytes);
	system("pause");
	printf("%s\n", "Niezwolnione elementy: ");
	for (long long i = 0; i < maxEls; i++){
		if (dynVars[i]==0) continue;
		printf("%X File: %s Line: %d Bytes: %d\n", dynVars[i], dynVarsFile[i], dynVarsLine[i], dynVarsBytes[i]);
	}
	system("pause");
}
//-----------------------------------------------------------------------------------------------------------------------------------


void MemoryGuard::ToFile(){
	FILE* file = fopen(fileName, "w");
	if (file != nullptr){
		fprintf(file,"%s %d\n", "Pozostalo nie usunietych elementow: ", dynVar);
		fprintf(file, "%s %d\n\n", "W sumie zaalokowano pamieci: ", dynVarBytes);
		fprintf(file, "%s\n", "Niezwolnione elementy: ");
		for (long long i = 0; i < maxEls; i++){
			if (dynVars[i]==0) continue;
			fprintf(file, "%X File: %s Line: %d Bytes: %d\n", dynVars[i], dynVarsFile[i], dynVarsLine[i], dynVarsBytes[i]);
		}
		fclose(file);
	}
	else{
		printf("\n\n%s", "Nie udalo sie zapisac do pliku");
		system("pause");
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------


void MemoryGuard::delDynVar(void* p){
	dynVar--;
	for (int i = 0; i < maxEls; i++){
		if (dynVars[i] == (unsigned)p){
			dynVars[i] = 0;
			return;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------


void MemoryGuard::addDynVar(void* p, size_t size, const char* file, unsigned line){
	dynVar++;

	if (dynVar > maxEls){
		printf("%s %d %s\n", "Ilosc zainicjowanych zmiennych przekroczyla",maxEls,"program zakonczy dzialanie natychmiastowo");
		system("cls");
		exit(0);
	}

	dynVarBytes += size;
	for (int i = 0; i < maxEls; i++){
		if (dynVars[i]==0){
			dynVars[i] = (unsigned)p;
			dynVarsBytes[i] = size;
			dynVarsFile[i] = (char*)file;
			dynVarsLine[i] = line;
			break;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------