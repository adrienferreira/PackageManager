#ifndef LEAF_H
#define LEAF_H
	#include "DepProv.h"
	#include "NodeBase.h"
	#include <string>
	using namespace std;

	class Leaf:public NodeBase
	{
		private:
			 DepProv* value;

		public:
			bool  isLeaf();
		    DepProv* getValue();
		    const vector<NodeBase*> getChildren();
		    LogicalOperator getLogOp()const;
			Leaf( DepProv* value);
			~Leaf();
			const string toString(void) const;

	};
#endif /* LEAF_H */
