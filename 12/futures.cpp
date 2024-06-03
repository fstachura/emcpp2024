#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>
#include <map>

void countCharValues(std::promise<std::size_t> result, std::ifstream f) {
    std::size_t sum;
    while(true) {
        char ch = f.get();
        sum += ch;
        if(f.eof() || f.fail() || f.bad()) {
            break;
        }
    }
    result.set_value(sum);
}

int main(int argc, char** argv) {
    if(argc <= 1) {
        std::cout << "usage: filename..." << std::endl;
        return 0;
    }

    std::vector<std::future<std::size_t>> futures;
    std::vector<std::thread> ts;
    for(int i = 1; i < argc; i++) {
        std::ifstream f;
        f.open(argv[i]);

        std::promise<std::size_t> result;
        futures.push_back(std::move(result.get_future()));
        std::thread t(countCharValues, std::move(result), std::move(f));
        ts.push_back(std::move(t));
    }

    char** fn = &argv[1];
    std::map<std::size_t, std::vector<char*>> resultsMap;
    for(auto&& future: futures) {
        future.wait();
        auto c = future.get();
        std::cout<<(*fn)<<": "<<c<<std::endl;

        resultsMap[c].push_back(*fn);

        fn++;
    }

    for(auto&& r: resultsMap) {
        if(r.second.size() > 1) {
            std::cout << r.first << ": ";
            for(auto&& fn: r.second) {
                std::cout << fn << ", ";
            }
            std::cout << std::endl;
        }
    }

    for(auto&& t: ts) {
        t.join();
    }
}
