#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <queue>
using namespace std;

bool biggerThan5(int x){ return x>5; }

template<
	template<typename, typename> typename OutContainer, 
	typename T, typename Alloc, typename Predicate, 
	template<typename, typename> typename InContainer=OutContainer>
OutContainer<T,Alloc> selectIf(InContainer<T,Alloc> c, Predicate p) {
	OutContainer<T, Alloc> result;
	for(auto&& el: c) {
		if(p(el)) {
			result.emplace(result.end(), el);
		}
	}
	return result;
}

int main(){
	auto print = [](auto v) { 
		for(auto x: v) cout << x << " ";
		cout << endl;
	};

	std::vector<int> v={1, 2, 13, 4, 5, 54};
	std::list<int> result = selectIf<std::list>(v, biggerThan5);
	print(result);  //  13 54

	auto result2 = selectIf<std::deque>(v, [](int x)->bool{ return x%2; } );
	print(result2); //  1 13 5
		
	auto result3 = selectIf<std::vector>(result2, biggerThan5);
	print(result3); //  13

	return 0;
}
