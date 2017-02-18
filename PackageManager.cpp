#include "PackageManager.h"


PackageManager::PackageManager(const string packDir)
{
	if (opendir(packDir.c_str()) == NULL)
	   throw runtime_error("Pack directory doesn't exists.");

	this->fromDir = packDir;
}

PackageManager::~PackageManager()
{
    cout<<"[CALL] ~PackageManager"<<endl;
    for(auto&pack:this->instPacks)
    {
        this->deleteTree(pack.getDeps());
        this->deleteTree(pack.getProvs());
    }
}

/**
 *  @brief Stores the package named @packName.
 *  @param packName the name of the backage (not its path).
 *  @return true on success. False otherwise.
*/
void PackageManager::installPackByName(const string& packName){
    cout<<"[CALL] installPackByName"<<endl;
    Package p =this->getPackByName(packName);
    this->instPacks.emplace_back(move(p));
}

/**
 *  @brief 
 *      Eases the access to getPackFromFile.
 *      Find the package named @name (if any) and returns it.
 *  @note lambda here means : Only the packages with @packFN name.
 *  @throw runtime_error Exactly one package is expected.
 *  @param name the name of the package we want.
 *  @return the package named @name
*/
Package PackageManager::getPackByName(const string& name) const
{
    cout<<"[CALL] getPackByName"<<endl;
    const string packFN = Package::getPackFileNameComplete(name);

    vector<string> packPath = this->rec_getPackageFiles(
        this->fromDir,
        [] (string&a,string&b) -> bool {return a==b;},
        packFN.c_str()
    );

    if(packPath.size() != 1)
        throw runtime_error("Impossible to get package.");

    return this->getPackFromFile(packPath.front());
}

/**
 *  @brief Parses the JSON file pointed by @packFilePath and build the package.
 *  @param packFilePath The full-path of the json file describing the package.
 *  @return A package with its two trees built (dependency and providing).
*/
Package PackageManager::getPackFromFile(const string& packFilePath) const
{
    cout<<"[CALL] getPackFromFile"<<endl;
    ifstream jsonFile; 
    rapidjson::Document d;

    jsonFile.open(packFilePath.c_str());
    stringstream jsonStream; 
    jsonStream << jsonFile.rdbuf();

    d.Parse(jsonStream.str().c_str());
    const string name(d["name"].GetString()); //move or copy

    const rapidjson::Value& a = d["depend"];
    const rapidjson::Value& b = d["provide"];

    assert(a.IsArray());
    assert(b.IsArray());

    /*Get the root of the two trees*/
    NodeBase *const deps = this->rec_loadDependencies<Dependency>(a);
    NodeBase *const provs = this->rec_loadDependencies<Providing>(b);

    /*Give it to the newly built package*/
    return Package(name, packFilePath, deps, provs);
}

/**
 *  @brief Recursively parses the dependencies in the json file and build it.
 *  @note Dependency and providing structure is the same hence the template.
 *  @throw runtime_error Exactly one logical operator needed for each subexpr.
 *  @return The root of the Dependency/providing tree.
*/
template <typename T>
NodeBase* PackageManager::rec_loadDependencies(const rapidjson::Value&deps) const
{
    cout<<"[CALL] rec_loadDependencies"<<endl;

    string logOp;
    vector<NodeBase*> v;
    NodeBase* ret = NULL;

    for (rapidjson::SizeType i = 0; i < deps.Size(); i++)
    {
        if(deps[i].IsString())
        {
            /*logical operator*/
            logOp = deps[i].GetString();
        }

        if(deps[i].IsObject())
        {
            /*property name, build leaf*/
            cout<<deps[i]["name"].GetString()<<endl;
            const string& s = string(deps[i]["name"].GetString());
            DepProv *nb =new T(s);
            NodeBase *dep= new Leaf(nb);
            v.push_back(dep);
        }

        if(deps[i].IsArray())
        {
            /*array, recursion*/
            cout<<"Is IsArray"<<endl;
            NodeBase *dep= this->rec_loadDependencies<T>(deps[i]);
            if(dep)
                v.push_back(dep);
        }
    }
    
    if(logOp.empty()){
        throw runtime_error("Boolean operator not error.");
    }

    ret = (NodeBase*)new Node(v, logOp);
    return ret;
}

/**
 *  @brief Search for the dependency @id in the tree @nb.
 *  @param nb The root of the tree in which @id might be
 *  @param id The id of the dependency we're looking for
 *  @return The Dependency with the id @id. Or NULL if it's not in @nb.
*/
DepProv* PackageManager::rec_getDeprovById(NodeBase* nb, const unsigned short id)
{
    DepProv* ret = NULL;

    if(nb->isLeaf())
    {
        ret = nb->getValue();
        if(ret->getDepProvId() != id)
            ret = NULL;
    }
    else
    {
        for(auto&child:nb->getChildren())
            if((ret = this->rec_getDeprovById(child,id)))
                break;
    }

    return ret;
}   

/**
 *  @brief Resolves the dep @pDepId thanks to @provId.
 *  @param pDepId The package that has an unresolved dependency
 *  @param depId The id of the dependency in @pDepId
 *  @param pProvId The package giving the providing
 *  @param provId The id of the providing in @pProvId
 *  @todo maybe it would be better in Package
 *  @return True if the resoving worked.
*/
bool PackageManager::resolveDependency(
    unsigned short pDepId,
    unsigned short depId,
    unsigned short pProvId,
    unsigned short provId)
{
    /* Get packages */
    const Package pDep = instPacks.at(pDepId);
    const Package pProv = instPacks.at(pProvId);

    /* Find providing and dependency */
    Providing*p=(Providing*)this->rec_getDeprovById(pProv.getProvs(), provId);
    Dependency*d=(Dependency*)this->rec_getDeprovById(pDep.getDeps(), depId);

    if(!p || !d)
        throw runtime_error("Cannot load dependency.");

    /* Same name : compatible */
    if(p->getName() != d->getName())
        throw runtime_error("Incompatible dependencies.");        

    /*Link them together*/
    d->setResolved(p);
    p->setResolving(d);

    return true;
}
/**
 * @brief Iterates over the packages to check if they have unresolved depends.
 * @return True if all the packages have all their dependencies resolved.
*/
bool PackageManager::checkProvDeps(){
    for(auto&pack:this->instPacks)
        if(!(pack.checkProvsDepsResolved()))
            return false;
    return true;
}

const string PackageManager::toString(void) const
{
    string str;
	for(auto&pack:this->instPacks)
		str+=pack.toString();
  return str;
}

const vector<Package> PackageManager::getInstPacks(void) const{
	return this->instPacks;
}

/**
 * @brief All the packages avaiables.
 * @note lambda here means : visit all the package files.
 * @return The names of all the packages in @fromDir.
*/
vector<string> PackageManager::getAllPackagesName(void) 
{
    vector<string> packsName;

    /*Get all the paths to the json files*/
    vector<string> allPacksFN = this->rec_getPackageFiles(
        this->fromDir,
        [] (string&,string&) -> bool {return true;},
        string()
    );

    for(auto&pack:allPacksFN){
        /*Get their names and push it to @packsName*/
        packsName.push_back(this->getPackFromFile(pack).getName());
    }

    return packsName;
}

/**
 * @brief All the packages file names.
 * @note lambda here means : visit all the package files.
 * @return The file names of all the packages in @fromDir.
*/
vector<string> PackageManager::getAllPackageFilesName(void)
{
    return this->rec_getPackageFiles(
        this->fromDir,
        [] (string&,string&) -> bool {return true;},
        string()
    );
}

/**
 *  @brief A kind of visitor pattern. Recursivly check all the files with @shouldVisit
 *  @note Keep doing c_str to keep C code as short as possible, doc says it's constant time.
 *  @param curPath The package in which to search for package files.
 *  @param shouldVisit Takes @shldvisParam and the cur file, tells us if we should keep it.
 *  @param shldvisParam Caller param, blindly passed to @shouldVisit.
 *  @return All the packages file-names that @shouldVisit told us to keep
*/
vector<string> PackageManager::rec_getPackageFiles(const string& curPath, bool (*shouldVisit)(string&,string&), string shldvisParam)const
{    
    DIR *dir;
    struct dirent *entry;
    vector<string> depthFiles;
    
    /*Does the folder exists*/
    if (!(dir = opendir(curPath.c_str())) || !(entry = readdir(dir)))
        return depthFiles;

    do
    {
        if (entry->d_type == DT_DIR)
        {
            string nextDir(entry->d_name);
            string nextPath = curPath + "/" + entry->d_name;

            if (nextDir == "." || nextDir == "..")
                continue;

            /*It's a folder, recursion*/
            vector<string> depthRes = this->rec_getPackageFiles(
                nextPath,
                shouldVisit,
                shldvisParam
            );

            if(depthRes.size())
            {
                /*Add vectors*/
                depthFiles.insert(
                    depthFiles.end(), 
                    depthRes.begin(), 
                    depthRes.end()
                );

                return depthFiles;
            }
        }

        if (entry->d_type == DT_REG)
        {   
            /*It's a file*/
            string curFile(entry->d_name);
            if (Package::isNameOfPackFile(curFile))
            {
                /*Should we add it to @depthFiles*/
                if(shouldVisit(shldvisParam, curFile))
                    depthFiles.push_back(curPath + '/' + curFile);
            }
        }
    }
    while ((entry = readdir(dir)));

    closedir(dir);
    return depthFiles;
}

void PackageManager::deleteTree(NodeBase*nb)
{
    if(!nb->isLeaf())
        for(auto&child:nb->getChildren())
            this->deleteTree(child);

    delete nb;
}