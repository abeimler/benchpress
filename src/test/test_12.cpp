#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>
#include <tuple>

#include "utils.h"


BENCHMARK("foo create 10M entities", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
})

BENCHMARK("foo delete 10M entities", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
})

BENCHMARK("foo iterate over 10M entities 1", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
})

BENCHMARK("foo iterate over 10M entities 2", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
})



BENCHMARK("bar create 10M entities", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
})

BENCHMARK("bar delete 10M entities", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }
})

BENCHMARK("bar iterate over 10M entities 1", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
})

BENCHMARK("bar iterate over 10M entities 2", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }
})



SCENARIO( "multible benchmarks with headers, plotdata, different order and no tags; print csv", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*10M\\s+entities.*", ".*bar.*10M\\s+entities.*" } )
            .csvoutput("./")
            .csvsuffix("10Mentities");

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

            REQUIRE( results.size() == 8 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 2 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);
            auto csv_results01_content_lines = split_string(csv_results[1].content);

            //CAPTURE( csv_results[0].content );
            //CAPTURE( csv_results[1].content );

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 2);

                REQUIRE ( csv_results[0].filename == "./bar10Mentities.csv" );
                REQUIRE ( csv_results[1].filename == "./foo10Mentities.csv" );
            }


            THEN ( "csv_results bar csv content is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 4);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(".{}.;{}", "1", results[4].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{}", "2", results[5].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{}", "3", results[6].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{}", "4", results[7].ns_per_op()) ) );
            }

            THEN ( "csv_results foo csv content is right" ) {
                REQUIRE(csv_results01_content_lines.size() == 4);

                REQUIRE_THAT( csv_results01_content_lines[0], Matches( fmt::format(".{}.;{}", "1", results[0].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[1], Matches( fmt::format(".{}.;{}", "2", results[1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[2], Matches( fmt::format(".{}.;{}", "3", results[2].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[3], Matches( fmt::format(".{}.;{}", "4", results[3].ns_per_op()) ) );
            }

        }
    }

}


SCENARIO( "multible benchmarks with headers, plotdata, different order and no tags; print csv with value sort", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*10M\\s+entities.*", ".*bar.*10M\\s+entities.*" } )
            .csvoutput("./")
            .csvsuffix("10Mentities")
            .csvsort(true);

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

            REQUIRE( results.size() == 8 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 2 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);
            auto csv_results01_content_lines = split_string(csv_results[1].content);

            //CAPTURE(output);
            //CAPTURE(csv_results[0].content);
            //CAPTURE(csv_results[1].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 2);

                REQUIRE ( csv_results[0].filename == "./bar10Mentities.csv" );
                REQUIRE ( csv_results[1].filename == "./foo10Mentities.csv" );
            }


            THEN ( "csv_results bar csv content is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 4);

                AND_THEN ( "bar results are sorted" ) {
                    REQUIRE( results[3+1].ns_per_op() >= results[3+2].ns_per_op() );
                    REQUIRE( results[3+2].ns_per_op() >= results[3+4].ns_per_op() );
                    REQUIRE( results[3+4].ns_per_op() >= results[3+3].ns_per_op() );
                }

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(".{}.;{}", "3", results[3+3].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{}", "4", results[3+4].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{}", "2", results[3+2].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{}", "1", results[3+1].ns_per_op()) ) );
            }

            THEN ( "csv_results foo csv content is right" ) {
                REQUIRE(csv_results01_content_lines.size() == 4);

                AND_THEN ( "foo results are sorted" ) {
                    REQUIRE( results[1-1].ns_per_op() >= results[3-1].ns_per_op() );
                    REQUIRE( results[3-1].ns_per_op() >= results[2-1].ns_per_op() );
                    REQUIRE( results[2-1].ns_per_op() >= results[4-1].ns_per_op() );
                }

                REQUIRE_THAT( csv_results01_content_lines[0], Matches( fmt::format(".{}.;{}", "4", results[4-1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[1], Matches( fmt::format(".{}.;{}", "2", results[2-1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[2], Matches( fmt::format(".{}.;{}", "3", results[3-1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results01_content_lines[3], Matches( fmt::format(".{}.;{}", "1", results[1-1].ns_per_op()) ) );
            }

        }
    }

}



SCENARIO( "multible benchmarks with headers, plotdata, different order and no tags; print csv without csvsuffix", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*10M\\s+entities.*", ".*bar.*10M\\s+entities.*" } )
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

            REQUIRE( results.size() == 8 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 1);

                REQUIRE ( csv_results[0].filename == "./output.csv" );
            }


            THEN ( "csv_results is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 5);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(";.{}.;.{}.", "bar10Mentities", "foo10Mentities") ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{};{}", "1", results[4].ns_per_op(), results[0].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{};{}", "2", results[5].ns_per_op(), results[1].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{};{}", "3", results[6].ns_per_op(), results[2].ns_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[4], Matches( fmt::format(".{}.;{};{}", "4", results[7].ns_per_op(), results[3].ns_per_op()) ) );
            }

        }
    }

}



SCENARIO( "multible benchmarks with headers, plotdata, different order and no tags; print csv without csvsuffix and with csvunit milliseconds", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*10M\\s+entities.*", ".*bar.*10M\\s+entities.*" } )
            .csvoutput("./")
            .csvunit("milliseconds");

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

            REQUIRE( results.size() == 8 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 1);

                REQUIRE ( csv_results[0].filename == "./output.csv" );
            }


            THEN ( "csv_results is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 5);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(";.{}.;.{}.", "bar10Mentities", "foo10Mentities") ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{}ms;{}ms", "1", results[4].ms_per_op(), results[0].ms_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{}ms;{}ms", "2", results[5].ms_per_op(), results[1].ms_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{}ms;{}ms", "3", results[6].ms_per_op(), results[2].ms_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[4], Matches( fmt::format(".{}.;{}ms;{}ms", "4", results[7].ms_per_op(), results[3].ms_per_op()) ) );
            }

        }
    }

}


SCENARIO( "multible benchmarks with headers, plotdata, different order and no tags; print csv without csvsuffix and with csvunit seconds", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*foo.*10M\\s+entities.*", ".*bar.*10M\\s+entities.*" } )
            .csvoutput("./")
            .csvunit("seconds");

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

            REQUIRE( results.size() == 8 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            THEN ( "csv_results filenames are set" ) {
                REQUIRE(csv_results.size() == 1);

                REQUIRE ( csv_results[0].filename == "./output.csv" );
            }


            THEN ( "csv_results is right" ) {
                REQUIRE(csv_results00_content_lines.size() == 5);

                REQUIRE_THAT( csv_results00_content_lines[0], Matches( fmt::format(";.{}.;.{}.", "bar10Mentities", "foo10Mentities") ) );
                REQUIRE_THAT( csv_results00_content_lines[1], Matches( fmt::format(".{}.;{:.3f}s;{:.3f}s", "1", results[4].s_per_op(), results[0].s_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[2], Matches( fmt::format(".{}.;{:.3f}s;{:.3f}s", "2", results[5].s_per_op(), results[1].s_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[3], Matches( fmt::format(".{}.;{:.3f}s;{:.3f}s", "3", results[6].s_per_op(), results[2].s_per_op()) ) );
                REQUIRE_THAT( csv_results00_content_lines[4], Matches( fmt::format(".{}.;{:.3f}s;{:.3f}s", "4", results[7].s_per_op(), results[3].s_per_op()) ) );
            }

        }
    }

}