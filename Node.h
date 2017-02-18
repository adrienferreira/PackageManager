#ifndef NODE_H
#define NODE_H

	#include "DepProv.h"
	#include "NodeBase.h"
	#include <string>
	#include <vector>
	#include <memory>
	using namespace std;

	class Node:public NodeBase
	{
		public:
			bool isLeaf();
			DepProv* getValue();
			const vector<NodeBase*> getChildren();
			LogicalOperator getLogOp()const;
			const string toString(void) const;
			Node(vector<NodeBase*>nb, string &logop);
		private:
			//todo in package ?
			LogicalOperator logOp;
			vector<NodeBase*>nb;
	};
#endif /* NODE_H */
