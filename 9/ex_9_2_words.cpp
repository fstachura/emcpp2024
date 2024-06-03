#include <iostream>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>
using namespace std;

/**
 * Removes all non alphanumeric characters from given word and converts to lower case.
 * @param[in,out] word on return word consist only of lower case characters.
 */
void toLowerAlpha(std::string & s1) {
    s1.erase(std::remove_if(s1.begin(), s1.end(), 
        [](char c) { return !((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)); }),
        s1.end()
    );
    std::transform(s1.begin(), s1.end(), s1.begin(), [](char c){ return std::tolower(c); });
    //2. Implement using stl algorithms only
    //   Hint: use remove_if, transform, erase
}

int main(){
    int count = 0;
    std::string word;
    map<string, int> c;
    std::vector<int> v;
    while( cin >> word) {
       toLowerAlpha(word);

       if(word != ""){
           c[word]++;
           count++;
       }
    }

    auto nr = c | ranges::views::transform([](auto a){ return make_pair(a.second, a.first); });
    multimap<int, string> m2;
    m2.insert(nr.begin(), nr.end());

    cout << "Number of distinct words : " << count << endl;
    cout << "\nThe top 20 most popular words: \n";
    for(std::pair<int, std::string>&& k: m2 | ranges::views::reverse | ranges::views::take(20)) {
        std::cout<<k.second<<": "<<k.first<<std::endl;
    }
    return 0;
}
/*
 * Expected output for ./words < hamletEN.txt

Number of distinct words : 4726

The top 20 most popular words:
the : 1145
and : 967
to : 745
of : 673
i : 569
you : 550
a : 536
my : 514
hamlet : 465
in : 437
it : 417
that : 391
is : 340
not : 315
lord : 311
this : 297
his : 296
but : 271
with : 268
for : 248
your : 242

 */
