#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>

#include "utils.h"

BENCHMARK("test bytes per second", [](benchpress::context* ctx) {
    ctx->set_bytes(1234567890L);
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
})

SCENARIO( "bytes per second benchmark", "[benchmark]" ) {

    GIVEN( "default benchmark options" ) {
        benchpress::options bench_opts;

        WHEN( "run benchmark" ) {
            std::string ouput = benchpress::run_benchmarks(bench_opts);
            auto lines = split_string(ouput);

            THEN( "ouput not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !ouput.empty() );
                REQUIRE( lines.size() >= 1 );

                REQUIRE_THAT( lines[0], Matches( "test bytes per second\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
            }

        }
    }

}