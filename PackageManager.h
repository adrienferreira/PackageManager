#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H
#include <string>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <iostream>//TODO debug
#include <memory>
#include "rapidjson/document.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "Package.h"

#include "Node.h"
#include "Leaf.h"
#include "Providing.h"
#include "Dependency.h"


using namespace std;

//TODO PackageFolderManager


class PackageManager
{
  private:
	// Static
	string fromDir;

	vector<Package> instPacks;

	////Past here, do not change attributes////
	Package getPackByName(const string&) const;
	string rec_getPackPath(const string&, const string&) const;
	Package getPackFromFile(const string&) const;
	const vector<Package> getInstPacks(void) const;
	
	template <typename T> 
	NodeBase* rec_loadDependencies(const rapidjson::Value&deps) const;
	DepProv* rec_getDeprovById(NodeBase* nb, const unsigned short id);

  public:
  	//void constructor ?
	PackageManager(const string  packDir);
	~PackageManager(void);

	void installPackByName(const string& packName);
	bool resolveDependency(unsigned short, unsigned short, unsigned short, unsigned short);

	bool checkProvDeps(void);
	bool checkProvDeps(const Package&p);

  	const string toString(void) const;

	vector<string> rec_getPackageFiles(
    	const string& curPath,
	    bool (*should_visit)(string&,string&), 
    	string shldvis_param
    )const;


	vector<string> getAllPackagesName(void) ;
	vector<string> getAllPackageFilesName(void);
	void deleteTree(NodeBase*nb);

};
#endif // PACKAGEMANAGER_H
