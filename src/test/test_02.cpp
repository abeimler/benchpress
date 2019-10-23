#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>

#include "utils.h"

unsigned int fib(unsigned int n) {
    return n < 2 ? n : fib(n-1) + fib(n-2);
}

BENCHMARK("fib 0", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto f = fib(0);
        benchpress::escape(&f);
    }
})

BENCHMARK("fib 5", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto f = fib(5);
        benchpress::escape(&f);
    }
})

BENCHMARK("fib 10", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto f = fib(10);
        benchpress::escape(&f);
    }
})

BENCHMARK("fib 15", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto f = fib(15);
        benchpress::escape(&f);
    }
})

BENCHMARK("fib 20", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto f = fib(20);
        benchpress::escape(&f);
    }
})



SCENARIO( "multible benchmark", "[benchmark]" ) {

    GIVEN( "default benchmark options" ) {
        benchpress::options bench_opts;

        WHEN( "run benchmark" ) {
            std::string ouput = benchpress::run_benchmarks(bench_opts);
            auto lines = split_string(ouput);

            THEN( "ouput not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !ouput.empty() );
                REQUIRE( lines.size() >= 5 );

                REQUIRE_THAT( lines[0], Matches( "fib 0\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)" ) );
                REQUIRE_THAT( lines[1], Matches( "fib 5\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)" ) );
                REQUIRE_THAT( lines[2], Matches( "fib 10\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)" ) );
                REQUIRE_THAT( lines[3], Matches( "fib 15\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)" ) );
                REQUIRE_THAT( lines[4], Matches( "fib 20\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)" ) );
            }

        }
    }

}