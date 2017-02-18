#ifndef PACKAGE_H
#define PACKAGE_H
#include <string>
#include <vector>
#include "NodeBase.h"
#include "Dependency.h"
#include "Providing.h"

using namespace std;

class Package
{
  private:
	// Auto-generated
	static unsigned short curId;
	unsigned short id;
	NodeBase *deps; /*Root of the dependencies tree*/
	NodeBase *provs;/*Root of the providing tree*/
	
	string name;
	string path;

  public:
	static const string PACK_EXT;
	static const string PACK_SUFFIX;
	Package(const string &, const string &, NodeBase*, NodeBase*);
	virtual ~Package(); 

	Package (const Package& other) = default;
	Package (Package&& other)= default;
	Package& operator=(Package&& other)= default;

	bool checkDepsResolved(void);
	bool checkProvsResolved(void);
	template<typename T, bool (T::*isResolvIngEd )()const>
	bool _checkProvsDepsResolved(NodeBase*nb);
	bool checkProvsDepsResolved(void);

	static const bool isNameOfPackFile(const string& fileName);
	static const string getPackFileSufix(void);
	static const string getPackFileNameComplete(const string&name);

	NodeBase* getDeps()const;
	NodeBase* getProvs()const;

	const string& getName()const;

  	const string toString(void) const;

};
#endif // PACKAGE_H
