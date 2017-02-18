#ifndef PROVIDING_H
#define PROVIDING_H
	#include "DepProv.h"

	class Providing;
	#include "Dependency.h"

	

	class Providing:public DepProv
	{
		private:
			Dependency*resolving;
		public:
			Providing(const string&);
			const string getName()const;
			void setResolving(Dependency*resv);
			bool isResolving()const;
			const string toString(void) const;
	};
#endif /* PROVIDING_H */
