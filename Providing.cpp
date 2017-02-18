#include "Providing.h"

			
const string Providing::getName()const{
	return this->name;
}

Providing::Providing(const string&name):DepProv()
{
	this->name = name;
}

/**
	@brief
		This providing is used to resolve @resv.
*/
void Providing::setResolving(Dependency*resv){
	this->resolving=resv;
}

/**
	@brief Return true if this providing is used to resolve a dependency.
*/
bool Providing::isResolving(void)const{
	return this->resolving!=NULL;
}

/**
	@brief Return a string that summarising this providing.
*/
const string Providing::toString(void) const
{
	string str;
	str += DepProv::toString() + ' ';
	str += this->isResolving();
	str += "Providing.cpp\n";
	return  str;
}
