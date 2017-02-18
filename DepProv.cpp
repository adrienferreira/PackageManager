#include "DepProv.h"

/**
 *	@brief 
 * 		used to identify each providing (post-increment).
*/
unsigned short DepProv::curDepProvId = 0;

DepProv::DepProv(){
	this->depProvId = curDepProvId++;
}

/**
 *	@brief 
 * 		Returns the id.
 *		Each providing has a unique ID. 
 */
unsigned int DepProv::getDepProvId()const{
	return this->depProvId;
}

const string DepProv::toString(void) const{
	return to_string(this->depProvId) + " : " +  this->name + '\n';
}