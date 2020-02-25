#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>

#include "utils.h"

BENCHMARK("test", [](benchpress::context* ctx) {
    std::this_thread::sleep_for(std::chrono::seconds(3));

    ctx->reset_timer();
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        ctx->stop_timer();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ctx->start_timer();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ctx->stop_timer();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
})

SCENARIO( "simple benchmark with start/stop timer", "[benchmark]" ) {

    GIVEN( "default benchmark options" ) {
        benchpress::options bench_opts;

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

            auto res = benchpress::run_benchmarks_details(bench_opts);
            std::string output = std::get<0>(res);
            auto results = std::get<1>(res).results;

            THEN( "result time is right" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !output.empty() );
                REQUIRE( !results.empty() );

                REQUIRE( results[0].s_per_op() >= Approx(1.0) );
                REQUIRE( results[0].s_per_op() < 2.0 );
            }

        }
    }

}