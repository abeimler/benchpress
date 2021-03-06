#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>

#include "utils.h"

BENCHMARK("multi-threaded test", [](benchpress::context* ctx) {
    ctx->run_parallel([](benchpress::parallel_context* pctx) {
        while (pctx->next()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
})


SCENARIO( "multi-threaded benchmark", "[benchmark]" ) {

    GIVEN( "default benchmark options" ) {
        benchpress::options bench_opts;

        WHEN( "run benchmark" ) {
            std::string output = benchpress::run_benchmarks(bench_opts);
            auto lines = split_string(output);

            THEN( "output not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !output.empty() );
                REQUIRE( lines.size() >= 1 );

                REQUIRE_THAT( lines[0], Matches( "multi-threaded test\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+[\\s\\.\\d]+s\\/op)?" ) );
                REQUIRE_THAT( lines[0], Matches( "multi-threaded test\\s+(\\d+)(\\s+[\\s\\d]+ns\\/op)(\\s+[\\s\\d]+ms\\/op)?(\\s+1.[\\d]+\\s+s\\/op)?" ) );
            }

        }
    }

}