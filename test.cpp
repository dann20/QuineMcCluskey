#include <stdio.h>
#include <iostream>
#include <string>
#include <set>

using namespace std;

int main(){
	set<int> x;
	x.insert(1);
	x.insert(1);
	set<int> :: iterator itr;
	for (itr = x.begin(); itr != x.end(); itr++){
		cout << *itr << endl;
	}
}