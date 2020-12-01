#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <bitset>
#include <algorithm>
#include <math.h>

using namespace std;

// class of multiple functions used in minimization
class Functions{
public:
	string combineMinterms(string a, string b) {
		// return combined minterm with the differ bit replaced with a third bit '-'
		for(int i = 0; i < a.length(); i++) {
			if (a[i]!=b[i])
				a[i]='-';
		}
		return a;
	}

	bool checkEmpty(vector< vector< string> > table){
		for (int i = 0; i < table.size(); ++i){
			if (table[i].size()!=0) {
				return false;
			}
		}
		return true;
	}

	string binToVariables(string x){
		// converts binary string to alphabetic variables
		string out = "";
		for (int i = 0; i < x.size(); ++i){
			if (x[i]=='1') {
				char x = 97 + i;
				out += x;
			}
			else if (x[i]=='0') {
				char x = 97 + i;
				out += x;
				out += "'";
			}
		}
		return out;
	}
	string binToVariablesMaxterm(string x){
		// converts binary string to alphabetic variables
		string out = "";
		for (int i = 0; i < x.size(); ++i){
			if (x[i]=='0') {
				char x = 97 + i;
				out += x;
				out += "+";
			}
			else if (x[i]=='1') {
				char x = 97 + i;
				out += x;
				out += "'";
				out += "+";
			}
		}
		out.erase(out.end()-1);
		return out;
	}

	string intToBin(int size, int val){
		string bin;
		bin = bitset<100>(val).to_string();
		bin = bin.substr(100-size);
		return bin;
	}

	int getNum1s(string x) {
		// returns the number of 1s in a binary string
		int count = 0;
		for (int i = 0; i < x.size(); ++i){
			if (x[i] == '1')
				count++;
		}
		return count;
	}

	bool compare(string a, string b) {
		// checks if two minterms differ exactly one bit
		int count = 0;
		for(int i = 0; i < a.length(); i++) {
			if (a[i]!=b[i])
				count++;
		}
		if(count == 1)
			return true;
		return false;
	}

	bool primeImpContains(string imp, string minTerm){
		// check if a prime implicant involves a minterm
		for (int i = 0; i < imp.size(); ++i){
			if (imp[i]!='-'){
				if(imp[i]!=minTerm[i]){
					return false;
				}
			}
		}
		return true;
	}

	int getNumVar(set<int> comb, vector<string> primeImp){
		// returns the number of variables in petrick method combination
		int count = 0;
		set<int> :: iterator itr;
		for (itr = comb.begin(); itr != comb.end(); ++itr){
			int imp = *itr;
			for (int i = 0; i < primeImp[imp].size(); ++i){
				if (primeImp[imp][i]!='-')
					count ++;
			}
		}
		return count;
	}
};

class QuineMcCluskey{
	string mode[2]={"minterm", "maxterm"};
	Functions func;
 	int nBits; // number of bits
 	int nMin; // number of minterms
	vector<int> minInt; // minterms in decimal
 	vector<string> minBin; // minterms in binary
	int nMax; // number of maxterms
	vector<int> maxInt; // maxterms in decimal
 	vector<string> maxBin; // maxterms in binary
	int nDc; // number of don't-cares
	vector<int> dcInt; // don't-cares in decimal
	vector<string> dcBin; // don't-cares in binary
	vector<string> bin; // all binary values (minterms and don't-cares)
 	vector<vector<string>> table;
 	vector<string> primeImp;
 	vector<set<int>> results;

public:
	void initialize(int numMode) {
		cout << "Input number of variables:" << endl;
		cin >> nBits;

		cout << "Input number of " << mode[numMode-1] << ":" << endl;
		cin >> nMin;

		cout << "Input "<< mode[numMode-1] << " (separated by spaces):" << endl;
		for (int i = 0; i < nMin; ++i){
			int x;
			cin >> x;
			minInt.push_back(x);
			minBin.push_back(func.intToBin(nBits, x));
			bin.push_back(func.intToBin(nBits, x));
		}

		cout << "Input number of don't-cares:" << endl;
		cin >> nDc;

		cout << "Input don't-cares (separated by spaces):" << endl;
		for (int i = 0; i < nDc; ++i){
			int x;
			cin >> x;
			dcInt.push_back(x);
			dcBin.push_back(func.intToBin(nBits, x));
			bin.push_back(func.intToBin(nBits, x));
		}

		cout << "\nDisplay values of " << mode[numMode-1] << " in binary form:" << endl;
		for (int i = 0; i < nMin; ++i){
			cout << minInt[i] << ") " << minBin[i] << endl;
		}

		cout << "\nDon't-cares in binary form:"<<endl;
		for (int i=0; i<nDc; ++i)
			cout<< dcInt[i] << ") " << dcBin[i] <<endl;

		table = vector<vector<string>>(nBits+1);
	}

	void setPI() {
		set<string> primeImpTemp;
		createTable();

		// Combine consecutive terms in the table until it's empty
		while (!func.checkEmpty(table)){
			table = combinePairs(table, primeImpTemp);
		}

		set<string > :: iterator itr;  // convert set to vector
		 for (itr = primeImpTemp.begin(); itr != primeImpTemp.end(); ++itr){
		 	string x = *itr;
        	primeImp.push_back(x);
    	}

    	cout << "\nThe Prime Implicants are:" << endl;
    	for (int i = 0; i < primeImp.size(); ++i){
    		cout  << i << ") "<< func.binToVariables(primeImp[i]) << endl;
    	}

	}

	void minimize() {
		// prepare primeImp chart
		bool primeImpChart[primeImp.size()][nMin] = {{false}};

		for (int i = 0; i < primeImp.size(); ++i){
			for (int j = 0; j < nMin; ++j){
				primeImpChart[i][j] = func.primeImpContains(primeImp[i], minBin[j]);
			}
		}

		// Display prime implicants chart
		cout << "\nPrime implicants chart with rows and columns are prime implicants and minterms, respectively:" << endl;
		for (int i = 0; i < primeImp.size(); ++i){
			for (int j = 0; j < nMin; ++j){
		 		if (primeImpChart[i][j] == true){
		 			cout << "1\t";
		 		}
		 		else {
		 			cout << "0\t";
		 		}
		 	}
		 	cout << endl;
		}

		// Petrick logic
		// convert PI chart to a instance of vector<set<int>> named petrick
		vector<set<int>> petrick;
		for (int j = 0; j < nMin; ++j){ // column iteration
			set<int> x;
			for (int i = 0; i < primeImp.size(); ++i){ // row iteration
				if (primeImpChart[i][j] == true) {
					x.insert(i);
				}
			}
			petrick.push_back(x);
		}

		// get all possible combinations
		set<set<int>> possibleComb;
		set<int> prod;
		getPossibleComb(petrick, 0, prod, possibleComb); // recursively multiply set elements
		int min = 9999;
		set<set<int>> :: iterator itr1;
		for (itr1 = possibleComb.begin(); itr1 != possibleComb.end(); ++itr1){
			set<int> comb = *itr1;
			if (comb.size() < min){
				min = comb.size();
			}
		}

		//Combinations with minimum number of terms
		vector< set<int> > minComb;
		set< set<int> > :: iterator itr;
		for (itr = possibleComb.begin(); itr != possibleComb.end(); ++itr){
			set<int> comb = *itr;
			if (comb.size() == min) {
				minComb.push_back(comb);
			}
		}

		//Combinations with minimum number of variables
		min = 9999;
		for (int i = 0; i < minComb.size(); ++i){
			if(func.getNumVar(minComb[i], primeImp) < min){
				min = func.getNumVar(minComb[i], primeImp);
			}
		}

		for (int i = 0; i < minComb.size(); ++i){
			if(func.getNumVar(minComb[i], primeImp) == min){
				results.push_back(minComb[i]);
			}
		}
	}

	void displayResults(int mode) {
		// prints output (minterm)
		cout <<"\n-----------------------------------------------------------" << endl;
		cout << "\nResults (all possible functions):\n" << endl;
		for (int i = 0; i < results.size(); ++i){
			set<int> function = results[i];
			set<int> :: iterator itr;
			cout << "Function " << i+1 << ":"<< endl;
			if (mode == 1)
			{
				for (itr = function.begin(); itr != function.end(); ++itr){
					int x = *itr;
					cout << func.binToVariables(primeImp[x]) << " + ";
				}
				cout << "\b\b  \n" << endl;
			}
			else if (mode == 2){
				for (itr = function.begin(); itr != function.end(); ++itr){
					int x = *itr;
					cout << "(" << func.binToVariablesMaxterm(primeImp[x]) << ")";
				}
				cout << "\n" << endl;
			}
		}
	}

	void getPossibleComb(vector<set<int>> &patLogic, int level, set<int> prod, set<set<int>> &posComb) {
		// a recursive function to multiple elements of set patLogic and store it in set posComb
		if (level == patLogic.size()){
			set<int> x = prod;
			posComb.insert(x);
			return;
		}
		else{
			set<int> :: iterator itr;
			for (itr = patLogic[level].begin(); itr != patLogic[level].end(); ++itr){

				int x = *itr;
	        	bool inserted = prod.insert(x).second;
	        	getPossibleComb(patLogic, level+1, prod, posComb);
	        	if (inserted){
	        		prod.erase(x);
	        	}
	    	}
		}
	}

	vector<vector<string>> combinePairs(vector<vector<string>> table, set<string>& primeImpTemp) {
		bool checked[table.size()][nMin] = {false};
		vector<vector<string>> newTable(table.size()-1);
		for (int i = 0; i < table.size() -1; ++i){
			for (int j = 0; j < table[i].size(); ++j){
				for (int k = 0; k < table[i+1].size(); k++){
					if (func.compare(table[i][j], table[i+1][k])){
						newTable[i].push_back(func.combineMinterms(table[i][j], table[i+1][k]));
						checked[i][j] = true;
						checked[i+1][k] = true;
					}
				}
			}
		}
		for (int i = 0; i < table.size(); ++i){
			for (int j = 0; j < table[i].size(); ++j){
				if (!checked[i][j]) {
					primeImpTemp.insert(table[i][j]);
				}
			}
		}
		return newTable;
	}

	void createTable() {
		for (int i = 0; i < ( nMin+nDc ); ++i){
			int num1s = func.getNum1s(bin[i]);
			table[num1s].push_back(bin[i]);
		}
		cout << "\nMinterm Table:" << endl;
		for (int i = 0; i < nBits+1; ++i){
			cout << i << ")  ";
			for (int j = 0; j < table[i].size(); ++j){
				cout << table[i][j] << ", ";
			}
			cout << endl;
		}
	}

};

void end()
{
	char c;
	cout << "Continue? Y/N? ";
	while (1)
	{
		fflush(stdin);
		c = getchar();
		if (c == 'N' || c == 'n')
		{
			cout << "\nBye.\n";
			exit(0);
		}
		else if (c == 'Y' || c == 'y')
		{
			cout << "\nContinue...\n";
			break;
		}
	}
}

int main()  {
	while (1){
		cout << "1. Minterm\n2. Maxterm\nEnter: ";
		int mode;
		cin >> mode;
		QuineMcCluskey algo;
		if (mode == 2)
			cout << "Maxterm mode!\n";
		else
			cout << "Minterm mode!\n";
		algo.initialize(mode);
		algo.setPI();
		algo.minimize();
		algo.displayResults(mode);
		end();
	}
}
