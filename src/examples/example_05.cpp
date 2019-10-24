#define BENCHPRESS_CONFIG_MAIN
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>
#include <chrono>
#include <thread>

BENCHMARK("example", [](benchpress::context* ctx) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ctx->reset_timer();
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
})