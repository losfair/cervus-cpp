#include "ffi.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <thread>

const char *s = "Print";

void print(const char *content, const char *prefix) {
    printf("%s: %s\n", prefix, content);
}
typedef void (*PrintFn)(const char *buf);

void build_and_call() {
    std::vector<std::string> param_types;
    param_types.push_back("pointer");

    Cervus::FFICaller caller(
        (void *) print,
        (void *) s,
        "void",
        param_types
    );

    PrintFn target = (PrintFn) caller.get_callable();
    target("Hello world!");
}

int main() {
    std::vector<std::thread> threads;

    for(int i = 0; i < 10; i++) {
        threads.push_back(std::thread([]() {
            for(int j = 0; j < 1000; j++) {
                build_and_call();
            }
        }));
    }

    for(auto& t : threads) {
        t.join();
    }

    return 0;
}
