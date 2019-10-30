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






SCENARIO( "multible benchmarks with headers, plotdata, different order and missing tags; print csv", "[benchmark]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvoutput("./")
            .csvsuffix("update");

        WHEN( "run benchmark" ) {
            auto res = benchpress::run_benchmarks_details(bench_opts);
            std::string output = std::get<0>(res);
            auto results = std::get<1>(res).results;
            auto headers = std::get<1>(res).headers;
            auto fields = std::get<1>(res).fields;
            auto results_map = std::get<1>(res).results_map;

            std::vector<std::string> headers_vector (headers.size());
            std::copy(std::begin(headers), std::end(headers), std::begin(headers_vector));

            std::vector<std::string> fields_vector (fields.size());
            std::copy(std::begin(fields), std::end(fields), std::begin(fields_vector));

            THEN ( "headers keys are set" ) {
                using Catch::Matchers::Matches;

                REQUIRE( !headers.empty() );
                REQUIRE( headers.size() == 3 );

                REQUIRE_THAT( headers_vector[0], Matches( "\\s*bar\\s*" ) );
                REQUIRE_THAT( headers_vector[1], Matches( "\\s*foo\\s*" ) );
                REQUIRE_THAT( headers_vector[2], Matches( "\\s*test\\s*" ) );

                AND_THEN ( "fields keys are set" ) {

                    REQUIRE( !fields.empty() );
                    REQUIRE( fields.size() == 5 );

                    REQUIRE_THAT( fields_vector[0], Matches( "\\s*10\\s*" ) );
                    REQUIRE_THAT( fields_vector[1], Matches( "\\s*100\\s*" ) );
                    REQUIRE_THAT( fields_vector[2], Matches( "\\s*500\\s*" ) );
                    REQUIRE_THAT( fields_vector[3], Matches( "\\s*1000\\s*" ) );
                    REQUIRE_THAT( fields_vector[4], Matches( "\\s*2000\\s*" ) );

                    AND_THEN ( "results_map keys are set" ) {

                        REQUIRE( !results_map.empty() );
                        REQUIRE( results_map.size() == 5 );

                        REQUIRE( results_map.find(fields_vector[0]) != std::end(results_map) );
                        REQUIRE( results_map.find(fields_vector[1]) != std::end(results_map) );
                        REQUIRE( results_map.find(fields_vector[2]) != std::end(results_map) );
                        REQUIRE( results_map.find(fields_vector[3]) != std::end(results_map) );
                        REQUIRE( results_map.find(fields_vector[4]) != std::end(results_map) );

                        REQUIRE( results_map.find(fields_vector[0])->second.find(headers_vector[0]) != std::end(results_map.find(fields_vector[0])->second) );
                        REQUIRE( results_map.find(fields_vector[1])->second.find(headers_vector[0]) != std::end(results_map.find(fields_vector[0])->second) );
                        REQUIRE( results_map.find(fields_vector[2])->second.find(headers_vector[0]) != std::end(results_map.find(fields_vector[0])->second) );
                        REQUIRE( results_map.find(fields_vector[3])->second.find(headers_vector[0]) != std::end(results_map.find(fields_vector[0])->second) );
                        REQUIRE( results_map.find(fields_vector[4])->second.find(headers_vector[0]) != std::end(results_map.find(fields_vector[0])->second) );
                    
                
                        THEN ( "results_map bar results are set" ) {
                            auto result00 = results_map.find(fields_vector[0])->second.find(headers_vector[0])->second;
                            auto result10 = results_map.find(fields_vector[1])->second.find(headers_vector[0])->second;
                            auto result20 = results_map.find(fields_vector[2])->second.find(headers_vector[0])->second;
                            auto result30 = results_map.find(fields_vector[3])->second.find(headers_vector[0])->second;

                            REQUIRE_THAT( result00, Matches( fmt::format("\\s*{}\\s*", results[3].ns_per_op()) ) );
                            REQUIRE_THAT( result10, Matches( fmt::format("\\s*{}\\s*", results[4].ns_per_op()) ) );
                            REQUIRE_THAT( result20, Matches( fmt::format("\\s*{}\\s*", results[5].ns_per_op()) ) );
                            REQUIRE_THAT( result30, Matches( fmt::format("\\s*{}\\s*", results[6].ns_per_op()) ) );
                        }

                        THEN ( "results_map foo results are set" ) {
                            auto result01 = results_map.find(fields_vector[0])->second.find(headers_vector[1])->second;
                            auto result11 = results_map.find(fields_vector[1])->second.find(headers_vector[1])->second;
                            auto result21 = results_map.find(fields_vector[2])->second.find(headers_vector[1])->second;

                            REQUIRE_THAT( result01, Matches( fmt::format("\\s*{}\\s*", results[0].ns_per_op()) ) );
                            REQUIRE_THAT( result11, Matches( fmt::format("\\s*{}\\s*", results[1].ns_per_op()) ) );
                            REQUIRE_THAT( result21, Matches( fmt::format("\\s*{}\\s*", results[2].ns_per_op()) ) );
                        }

                        THEN ( "results_map test results are set" ) {
                            auto result02 = results_map.find(fields_vector[0])->second.find(headers_vector[2])->second;
                            auto result12 = results_map.find(fields_vector[1])->second.find(headers_vector[2])->second;
                            auto result21 = results_map.find(fields_vector[2])->second.find(headers_vector[2])->second;
                            auto result31 = results_map.find(fields_vector[3])->second.find(headers_vector[2])->second;
                            auto result41 = results_map.find(fields_vector[4])->second.find(headers_vector[2])->second;

                            REQUIRE_THAT( result02, Matches( fmt::format("\\s*{}\\s*", results[7].ns_per_op()) ) );
                            REQUIRE_THAT( result12, Matches( fmt::format("\\s*{}\\s*", results[8].ns_per_op()) ) );
                            REQUIRE_THAT( result21, Matches( fmt::format("\\s*{}\\s*", results[9].ns_per_op()) ) );
                            REQUIRE_THAT( result31, Matches( fmt::format("\\s*{}\\s*", results[10].ns_per_op()) ) );
                            REQUIRE_THAT( result41, Matches( fmt::format("\\s*{}\\s*", results[11].ns_per_op()) ) );
                        }
                    }
                }
            }

        }
    }

}