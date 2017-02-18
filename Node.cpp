#include "Node.h"

Node::Node(vector<NodeBase*>nb, string& logop){
	this->nb= nb;
	this->logOp = this->logOpStr[logop];
}

bool Node::isLeaf(){
	return false;
}
DepProv* Node::getValue(){
	throw runtime_error("It's not a leaf.");
}

const vector<NodeBase*> Node::getChildren(){
	return this->nb;
}

NodeBase::LogicalOperator Node::getLogOp()const{
	return this->logOp;
}

const string Node::toString(void) const{
	string str;
	for(auto&n:this->nb)
		str+=n->toString();

	return str;
}