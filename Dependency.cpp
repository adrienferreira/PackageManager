#include "Dependency.h"

const string Dependency::getName()const{
	return this->name;
}

Dependency::Dependency(const string&name):DepProv(){
	this->name=name;
	this->resolved = NULL;
}

void Dependency::setResolved(Providing*p){
	this->resolved = p;
}

bool Dependency::isResolved(void)const{
	return this->resolved!=NULL;
}

const string Dependency::toString(void) const{
	string str;
	str+=DepProv::toString() + '\n';
	str+="---\n" ;
	if(this->resolved!=NULL)
		str+="@"+this->resolved->getName();
	else
		str+="?";

	str+="\n";
	return  str;
}

Dependency::~Dependency(){}
