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






SCENARIO( "multible benchmarks with headers, plotdata, different order and missing tags; print csv", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvoutput("./")
            .csvsuffix("update");

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

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

            REQUIRE( results.size() == 12 );
            
            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 3 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);
            auto csv_results01_content_lines = split_string(csv_results[1].content);
            auto csv_results02_content_lines = split_string(csv_results[2].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 3);

                REQUIRE ( csv_results[0].filename == "./barupdate.csv" );
                REQUIRE ( csv_results[1].filename == "./fooupdate.csv" );
                REQUIRE ( csv_results[2].filename == "./testupdate.csv" );
            }


            THEN ( "csv_results bar csv content is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 4);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(".{}.;{}", "10", results[3].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{}", "100", results[4].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{}", "500", results[5].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{}", "1000", results[6].ns_per_op()) ) );
            }

            THEN ( "csv_results foo csv content is right" ) {
                REQUIRE(csv_results01_content_lines.size() == 3);

                REQUIRE_THAT( csv_results01_content_lines[0], Matches( fmt::format(".{}.;{}", "10", results[0].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[1], Matches( fmt::format(".{}.;{}", "100", results[1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[2], Matches( fmt::format(".{}.;{}", "500", results[2].ns_per_op()) ) );
            }

            THEN ( "csv_results test csv content is right" ) {
                REQUIRE(csv_results02_content_lines.size() == 5);

                REQUIRE_THAT( csv_results02_content_lines[0], Matches( fmt::format(".{}.;{}", "10", results[7].ns_per_op()) ) );
                REQUIRE_THAT( csv_results02_content_lines[1], Matches( fmt::format(".{}.;{}", "100", results[8].ns_per_op()) ) );
                REQUIRE_THAT( csv_results02_content_lines[2], Matches( fmt::format(".{}.;{}", "500", results[9].ns_per_op()) ) );
                REQUIRE_THAT( csv_results02_content_lines[3], Matches( fmt::format(".{}.;{}", "1000", results[10].ns_per_op()) ) );
                REQUIRE_THAT( csv_results02_content_lines[4], Matches( fmt::format(".{}.;{}", "2000", results[11].ns_per_op()) ) );
            }
        }
    }

}


SCENARIO( "multible benchmarks with headers, plotdata, different order and missing tags; print csv with simple path", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvoutput(".")
            .csvsuffix("update");

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

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

            REQUIRE( results.size() == 12 );
            
            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 3 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);
            auto csv_results01_content_lines = split_string(csv_results[1].content);
            auto csv_results02_content_lines = split_string(csv_results[2].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 3);

                REQUIRE ( csv_results[0].filename == "./barupdate.csv" );
                REQUIRE ( csv_results[1].filename == "./fooupdate.csv" );
                REQUIRE ( csv_results[2].filename == "./testupdate.csv" );
            }
        }
    }

}


SCENARIO( "multible benchmarks with headers, plotdata, different order and missing tags; print csv with other file reperator", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvoutput(".\\")
            .csvsuffix("update");

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

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

            REQUIRE( results.size() == 12 );
            
            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 3 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);
            auto csv_results01_content_lines = split_string(csv_results[1].content);
            auto csv_results02_content_lines = split_string(csv_results[2].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 3);

                REQUIRE ( csv_results[0].filename == "./barupdate.csv" );
                REQUIRE ( csv_results[1].filename == "./fooupdate.csv" );
                REQUIRE ( csv_results[2].filename == "./testupdate.csv" );
            }
        }
    }

}





SCENARIO( "multible benchmarks with headers, plotdata, different order and tags; print csv without csvsuffix but with csvprefix", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvprefix("update")
            .csvoutput("./");

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

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

            REQUIRE( results.size() == 12 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 1);

                REQUIRE ( csv_results[0].filename == "./update.csv" );
            }


            THEN ( "csv_results is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 6);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(";.{}.;.{}.;.{}.", "bar", "foo", "test") ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{};{};{}",   "10", results[3].ns_per_op(),  results[0].ns_per_op(), results[7].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{};{};{}",  "100", results[4].ns_per_op(),  results[1].ns_per_op(), results[8].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{};{};{}",  "500", results[5].ns_per_op(),  results[2].ns_per_op(), results[9].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[4], Matches( fmt::format(".{}.;{};{};{}", "1000", results[6].ns_per_op(),               "\"N/A\"", results[10].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[5], Matches( fmt::format(".{}.;{};{};{}", "2000",              "\"N/A\"",               "\"N/A\"", results[11].ns_per_op()) ) );
            }

        }
    }

}

SCENARIO( "multible benchmarks with headers, plotdata, different order and tags; print csv without csvsuffix and without csvprefix", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*", ".*bar.*", ".*test.*" } )
            .csvoutput("./");

        WHEN( "run benchmark" ) {
            using Catch::Matchers::Matches;

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

            REQUIRE( results.size() == 12 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 1);

                REQUIRE ( csv_results[0].filename == "./output.csv" );
            }

        }
    }

}