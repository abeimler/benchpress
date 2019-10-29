#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>
#include <tuple>

#include "utils.h"

BENCHMARK("[10]  foo      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
})

BENCHMARK("[100]  foo     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
})

BENCHMARK("[500]  foo     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
})

BENCHMARK("[1000] foo    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
})

BENCHMARK("[2000] foo    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
})




BENCHMARK("[10]  test      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
})

BENCHMARK("[100]  test     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(53));
    }
})

BENCHMARK("[500]  test     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }
})

BENCHMARK("[1000] test    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(96));
    }
})

BENCHMARK("[2000] test    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(83));
    }
})



BENCHMARK("[10]  bar      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
})

BENCHMARK("[100]  bar     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
})

BENCHMARK("[500]  bar     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
})

BENCHMARK("[1000] bar    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
})

BENCHMARK("[2000] bar    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
})





SCENARIO( "multible benchmarks with headers, plotdata and different order", "[benchmark]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .plotdata(true);

        WHEN( "run benchmark" ) {
            auto results = benchpress::run_benchmarks_details(bench_opts);

            std::string output = std::get<0>(results);
            auto result = std::get<1>(results).results;

            auto lines = split_string(output);

            THEN( "output not empty" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !output.empty() );
                REQUIRE( lines.size() >= 23 );

                INFO(output)

                REQUIRE_THAT( lines[16], Matches( "# plot data" ) );
                REQUIRE_THAT( lines[17], Matches( "\\s+bar\\s+foo\\s+test" ) );

                REQUIRE_THAT( lines[18], Matches( "\\s+10\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[19], Matches( "\\s+100\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[20], Matches( "\\s+500\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[21], Matches( "\\s+1000\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" ) );
                REQUIRE_THAT( lines[22], Matches( "\\s+2000\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" ) );

                AND_THEN( "plotdata is right" ) {
                    REQUIRE_THAT( lines[18], Matches( fmt::format("\\s+10\\s+({})\\s+({})\\s+({})",   result[5].ns_per_op(), result[0].ns_per_op(), result[10].ns_per_op()) ) );
                    REQUIRE_THAT( lines[19], Matches( fmt::format("\\s+100\\s+({})\\s+({})\\s+({})",  result[6].ns_per_op(), result[1].ns_per_op(), result[11].ns_per_op()) ) );
                    REQUIRE_THAT( lines[20], Matches( fmt::format("\\s+500\\s+({})\\s+({})\\s+({})",  result[7].ns_per_op(), result[2].ns_per_op(), result[12].ns_per_op()) ) );
                    REQUIRE_THAT( lines[21], Matches( fmt::format("\\s+1000\\s+({})\\s+({})\\s+({})", result[8].ns_per_op(), result[3].ns_per_op(), result[13].ns_per_op()) ) );
                    REQUIRE_THAT( lines[22], Matches( fmt::format("\\s+2000\\s+({})\\s+({})\\s+({})", result[9].ns_per_op(), result[4].ns_per_op(), result[14].ns_per_op()) ) );
                }
            }

        }
    }

}