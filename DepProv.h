#ifndef DEPPROV_H
#define DEPPROV_H

	#include <string>
	using namespace std;

	class DepProv{
	private:
		static unsigned short curDepProvId;

	protected:
		string name;
		unsigned int depProvId;

	public:
		DepProv();
		unsigned int getDepProvId()const;
		virtual const string getName()const = 0;
		virtual const string toString(void) const = 0;
		virtual ~DepProv()=default;
	};
#endif /* DEPPROV_H */
