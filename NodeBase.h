#ifndef NODEBASE_H
#define NODEBASE_H
	#include <string>
	#include <iterator>
	#include <vector>
	#include <map>
	#include <iostream>
	#include <memory>

	#include "DepProv.h"

	using namespace std;
//faire package basic tree
//template
	class NodeBase
	{
		public:
			enum LogicalOperator { AND, OR, NOT };
			virtual bool isLeaf()= 0;
		    virtual DepProv* getValue() = 0;
		    virtual const vector<NodeBase*> getChildren() = 0;//const_iterator
		    virtual LogicalOperator getLogOp()const = 0;
			virtual const string toString(void) const = 0;
		    virtual ~NodeBase()=default;

		protected:
			map<string, LogicalOperator> logOpStr = {
				{"AND", AND}, 
				{"OR", OR},
				{"NOT", NOT}
			};
};

#endif /* NODEBASE_H */
