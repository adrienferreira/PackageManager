#include "NodeBase.h"
#include "Node.h"
#include "Leaf.h"
#include "Dependency.h"
#include "Providing.h"
#include "DepProv.h"
#include "PackageManager.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	PackageManager pm("/home/adrien/Documents/repos/rendu_emmanuel/");

	try{
		pm.installPackByName("app_loopback");
		pm.installPackByName("uart_xilinx");	
	}
    catch (runtime_error &e) {
		cout<<"Impossible to install those packages."<<endl;
        cout << "Error : " << e.what () << endl;
        return 1;
    }

	try{
		pm.resolveDependency(0,0,1,7);
		pm.resolveDependency(0,1,1,8);
	}
    catch (runtime_error &e) {
    	cout << "Cannot resolve dependencies." << endl;
        cout << "Error : " << e.what () << endl;
        return 1;
    }

	cout<<"AFT"<<endl;
	cout<<pm.toString()<<endl;
	cout<<"TST"<<endl;
	cout<<pm.checkProvDeps()<<endl;

	return 0;
}
