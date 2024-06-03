#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
/**
 * Trie structure
 *
 * HINT: use std::map to implement it!
 *       My implementation adds less than 25 lines of code. 
 */
class Trie{
	struct wordsmap {
		std::map<std::string, wordsmap> m;
		wordsmap(): m{} {
		}
	};	
	wordsmap m = wordsmap();

	static void print(const wordsmap& wm, int ident) {
		for(auto&& k: wm.m) {
			std::cout<<std::string(2*ident, ' ')<<k.first<<std::endl;
			print(k.second, ident+1);
		}
	}

public:
	static void printSentence(const std::vector<std::string>  & sentence ){
		for(const auto & w : sentence)
			cout << w << " ";
	}

	static void printWordmap(const wordsmap& wm, const std::vector<std::string>& start) {
		for(auto&& p: wm.m) {
			if(p.second.m.empty()) {
				std::cout << "> ";
				printSentence(start);
				std::cout<<p.first<<std::endl;
			} else {
				auto tmp = start;
				tmp.push_back(p.first);
				printWordmap(p.second, tmp);
			}
		}
	}

	void add(const std::vector<std::string>  & sentence ){
		cout << "Adding : ";
		printSentence(sentence);
		cout << "\n";
		wordsmap* wm = &m;
		for(auto&& word: sentence) {
			wm = &wm->m[word];
		}
	}

	void printPossibleEndings(const std::vector<std::string>  & beginningOfSentece ){
		cout << "Endings for \"";
		printSentence(beginningOfSentece);
		cout << "\" are :\n";
		wordsmap* wm = &m;

		for(auto&& word: beginningOfSentece) {
			if(wm->m.count(word) == 0) {
				wm = nullptr;
				break;
			} else {
				wm = &wm->m[word];
			}
		}

		if(wm != nullptr) {
			for(auto&& p: wm->m) {
				if(!p.second.m.empty()) {
					std::vector<std::string> tmp{p.first};
					printWordmap(p.second, tmp);
				} else {
					std::cout<<"> "<<p.first<<std::endl;
				}
			}
			std::cout<<std::endl;
		}
	}

	void print() {
		print(m, 0);
	}

	void load(const std::string & fileName){
		ifstream file(fileName);
		if(!file){
			cerr << "Error when openning file " << fileName << endl;
			return;
		}
		string word;
		vector<string> sentence;
		while( file >> word) {
			sentence.push_back(word);
			// is it end of the sentence?
			if(word.find_last_of('.') != string::npos) {
				add(sentence);
				sentence.clear();
			}
		}
	}
};


int main(){
	Trie dictionary;
	dictionary.load("./sample.txt");
	//dictionary.load("hamletEN.txt");

	dictionary.printPossibleEndings({"Curiosity"});
	dictionary.printPossibleEndings({"Curiosity", "killed"});
	dictionary.printPossibleEndings({"The", "mouse","was","killed"});
	dictionary.printPossibleEndings({"Curiosity", "killed", "the"});

	cout << "Beginning of sentence (ended with single .): ";
	string word;
	vector<string> sentence;
	while(true){
		cin >> word;
		if(word == ".")
			break;
		sentence.push_back(word);
		dictionary.printPossibleEndings(sentence);
	}
	return 0;
}
/* Expected output:
Adding : Curiosity killed the cat.
Adding : Curiosity killed the mouse.
Adding : Curiosity saved the cat.
Adding : Curiosity killed the cat and the mouse.
Adding : The cat was killed by curiosity.
Adding : The mouse was killed by cat.
Adding : The mouse was killed by curiosity.
Endings for "Curiosity " are :
> killed the cat.
> killed the mouse.
> killed the cat and the mouse.
> saved the cat.

Endings for "Curiosity killed " are :
> killed the cat.
> killed the mouse.
> killed the cat and the mouse.

Endings for "The mouse was killed " are :
> by cat.
> by curiosity.

*/
