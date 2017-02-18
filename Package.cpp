#include "Package.h"

const string Package::PACK_EXT = "json";
const string Package::PACK_SUFFIX = "package";

// Should be zero anyway
unsigned short Package::curId = 0;

// quand et que fait le constructeur vide

Package::~Package(){
	cout<<"~Package"<<endl;
	cout<<(void*)this<<endl;
	this->name.clear();
	this->path.clear();
	this->deps  = NULL;
	this->provs = NULL;
}

Package::Package(const string & name, const string & path, NodeBase*d, NodeBase*p)
{
	printf("%p\n",(void*)this);

	cout<<"Package"<<endl;
	this->id = curId;
	this->name = name;
	this->path = path;
	this->deps = d;
	this->provs = p;
	curId++;
}

NodeBase* Package::getDeps()const{
	return this->deps;
}
NodeBase* Package::getProvs()const{
	return this->provs;
}

/**
 *	@brief 
 *		Checks if Dependencies/providings are satisfied.
 *		Care is given logical operator of each subexpression.
 *	@note The template allows to factorize, same code for Dep/Prov.
 *	@param isResolvingEd tells if dependency/providing is resolved/ing.
 *	@param nb the root of the tree to check.
 *	@return True if all the dependencies/providings check theirs logical exprs.
*/
template<typename T, bool (T::*isResolvIngEd )()const>
bool Package::_checkProvsDepsResolved(NodeBase*nb)
{
	NodeBase::LogicalOperator curLogOp;
	unsigned short nbElems = 0;
	unsigned short recBoolMatch = 0;	

	if(nb->isLeaf())
    {
        T*handle=dynamic_cast<T*>(nb->getValue());
        return handle && (handle->*isResolvIngEd)();
    }
    else
    {
    	curLogOp = nb->getLogOp();
    	auto& children = deps->getChildren();
		nbElems = children.size();
        for(auto&child:children){
			/*count each match*/
        	if(this->_checkProvsDepsResolved<T,isResolvIngEd>(child))
        		recBoolMatch++;
        }
	}

	if(nbElems){
		/* test against le logical operator*/
		if(curLogOp == NodeBase::OR && recBoolMatch>0)
			return true;
		if(curLogOp == NodeBase::AND && recBoolMatch==nbElems)
			return true;
	}

    return false;
}

bool Package::checkProvsDepsResolved(){
	const bool depsOk = this->_checkProvsDepsResolved<Dependency, &Dependency::isResolved>(this->deps);
	const bool provsOk = this->_checkProvsDepsResolved<Providing, &Providing::isResolving>(this->provs);
	return depsOk && provsOk;
}

//TODO static ??
const string Package::getPackFileSufix(void){
	return  "_" + Package::PACK_SUFFIX + "." + Package::PACK_EXT;
}

const string Package::getPackFileNameComplete(const string&name){
	return  name + Package::getPackFileSufix();
}

const bool Package::isNameOfPackFile(const string& fileName){
	const string& pSuf = Package::getPackFileSufix();
	if(fileName.length()<=pSuf.length())return false;
	return ! (fileName.compare(fileName.length() - pSuf.length(), pSuf.length(), pSuf));
}

const string& Package::getName(void)const{
	return this->name;
}

const string Package::toString(void) const{
  	string str;
  	str += "=====\n";
  	str += "Name : " +  this->getName() + '\n';
  	str += "Id : " + to_string(this->id) + '\n';
  	str += "Path : " + this->path + '\n';

  	str+="Dependencies\n";
  	if(! (this->deps->isLeaf())){
	  	for(auto&ds:this->deps->getChildren())
	  		str += ds->toString();
  	}
	else{
		str += this->deps->toString();	
	}
 
  	str+="Providings\n";
	if(! (this->provs->isLeaf())){
	  	for(auto&ps:this->provs->getChildren())
	  		str += ps->toString();
	}
	else{
		str += this->provs->toString();	
	}
 
  	return str;
}

/*cant be in the destructor*/
/*void Package::freeProvDepTree(void) const{
	//TODO
}*/