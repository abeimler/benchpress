#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>

#include "utils.h"

BENCHMARK("[10]  test      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
})

BENCHMARK("[100]  test     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
})

BENCHMARK("[500]  test     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
})

BENCHMARK("[1000] test    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
})

BENCHMARK("[2000] test    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
})

SCENARIO( "simple benchmark with plotdata", "[benchmark]" ) {

    GIVEN( "default benchmark options" ) {
        benchpress::options bench_opts;

        WHEN( "run benchmark" ) {
            std::string output = benchpress::run_benchmarks(bench_opts);
            auto lines = split_string(output);

            THEN( "output not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !output.empty() );
                REQUIRE( lines.size() >= 5 );

                REQUIRE_THAT( lines[0], Matches( "\\[10]\\s+test\\s+(10)\\s+updates\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
                REQUIRE_THAT( lines[1], Matches( "\\[100]\\s+test\\s+(100)\\s+updates\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
                REQUIRE_THAT( lines[2], Matches( "\\[500]\\s+test\\s+(500)\\s+updates\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
                REQUIRE_THAT( lines[3], Matches( "\\[1000]\\s+test\\s+(1000)\\s+updates\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
                REQUIRE_THAT( lines[4], Matches( "\\[2000]\\s+test\\s+(2000)\\s+updates\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
            }

        }
    }

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*test.*" } )
            .plotdata(true);

        WHEN( "run benchmark" ) {
            std::string output = benchpress::run_benchmarks(bench_opts);
            auto lines = split_string(output);

            THEN( "output not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !output.empty() );
                REQUIRE( lines.size() >= 13 );

                REQUIRE_THAT( lines[6], Matches( "# plot data" ) );
                REQUIRE_THAT( lines[7], Matches( "\\s+test" ) );
                REQUIRE_THAT( lines[8], Matches( "\\s+10\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[9], Matches( "\\s+100\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[10], Matches( "\\s+500\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[11], Matches( "\\s+1000\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[12], Matches( "\\s+2000\\s+(\\d+)" ) );
            }

        }
    }

}