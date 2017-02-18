#ifndef DEPENDENCY_H
#define DEPENDENCY_H
	#include "DepProv.h"
	
	class Dependency;
	#include "Providing.h"
	
	class Dependency:public DepProv
	{
		private:
			Providing*resolved;

		public:
			const string getName()const;
			Dependency(const string&name);
			bool isResolved()const;
			void setResolved(Providing*p);
			const string toString(void) const;

			~Dependency();
	};
#endif /* DEPENDENCY_H */
