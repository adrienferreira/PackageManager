#include "Leaf.h"

Leaf::Leaf(DepProv* value){
	this->value=value;
}

Leaf::~Leaf(){
	cout<<"~Leaf"<<endl;
	delete this->value;
}


bool Leaf::isLeaf(){
	return true;
}

DepProv* Leaf::getValue(){
	return this->value;
}

const vector<NodeBase*> Leaf::getChildren(){
	throw runtime_error("Not a node");
}

NodeBase::LogicalOperator Leaf::getLogOp()const{
	throw runtime_error("Not a node");
}

const string Leaf::toString(void) const{
	return this->value->toString();
}