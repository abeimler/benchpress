#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define BENCHPRESS_CONFIG_RUN_BENCHMARKS
#include <benchpress/benchpress.hpp>

#include <chrono>
#include <iostream>
#include <tuple>

#include "utils.h"


BENCHMARK("[10]  ggggggg      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
})

BENCHMARK("[100]  ggggggg     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
})

BENCHMARK("[500]  ggggggg     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
})

BENCHMARK("[1000]  ggggggg     1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
})
BENCHMARK("[2000]  ggggggg     2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
})




BENCHMARK("[10]  eeeeee1      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
})

BENCHMARK("[100]  eeeeee1     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(53));
    }
})

BENCHMARK("[500]  eeeeee1     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }
})

BENCHMARK("[1000] eeeeee1    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(96));
    }
})

BENCHMARK("[2000] eeeeee1    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(83));
    }
})



BENCHMARK("[10]  eeff      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(35));
    }
})

BENCHMARK("[100]  eeff     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(58));
    }
})

BENCHMARK("[500]  eeff     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
})

BENCHMARK("[1000] eeff    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
})

BENCHMARK("[2000] eeff    2000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(96));
    }
})


BENCHMARK("[10]  eeeeee2      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
})

BENCHMARK("[100]  eeeeee2     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
})

BENCHMARK("[500]  eeeeee2     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
})

BENCHMARK("[1000] eeeeee2    1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
})



BENCHMARK("[10]  aaaa      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
})

BENCHMARK("[100]  aaaa     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(53));
    }
})

BENCHMARK("[500]  aaaa     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
})


BENCHMARK("[10]  aaaaaa      10 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
})

BENCHMARK("[100]  aaaaaa     100 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(53));
    }
})

BENCHMARK("[500]  aaaaaa     500 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
})

BENCHMARK("[1000]  aaaaaa     1000 updates", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
})



SCENARIO( "multible benchmarks with headers, plotdata, different order and tags; print csv and plotdata, headers are in the same order", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*aaaa.*update.*", ".*aaaaaa.*update.*", ".*eeff.*update.*", ".*eeeeee1.*update.*", ".*eeeeee2.*update.*", ".*ggggggg.*update.*" } )
            .plotdata(true)
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

            REQUIRE( results.size() == 30 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            auto csv_results_00_lines = split_string(csv_results[0].content);
            auto output_lines = split_string(output);

            THEN ( "headers are in the same order" ) {
                REQUIRE(csv_results_00_lines.size() >= 1);
                REQUIRE(output_lines.size() >= 33);

                REQUIRE_THAT( csv_results_00_lines[0], Matches( fmt::format(";.{}.;.{}.;.{}.;.{}.;.{}.;.{}.", "aaaaupdate", "eeffupdate", "aaaaaaupdate", "eeeeee1update", "eeeeee2update", "gggggggupdate") ) );
                REQUIRE_THAT( output_lines[32], Matches( fmt::format("\\s+{}\\s+{}\\s+{}\\s+{}\\s+{}\\s+{}", "aaaaupdate", "eeffupdate", "aaaaaaupdate", "eeeeee1update", "eeeeee2update", "gggggggupdate") ) );
            }

        }
    }

}


SCENARIO( "multible benchmarks with headers, plotdata, different order and tags; print csv and plotdata, headers are in the same order with csvsort", "[benchmark][csv]" ) {

    GIVEN( "default benchmark options with headers and plotdata" ) {
        benchpress::options bench_opts;
        bench_opts
            .bench( { ".*aaaa.*update.*", ".*aaaaaa.*update.*", ".*eeff.*update.*", ".*eeeeee1.*update.*", ".*eeeeee2.*update.*", ".*ggggggg.*update.*" } )
            .plotdata(true)
            .csvsort(true)
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

            REQUIRE( results.size() == 30 );

            auto csv_results = benchpress::make_csv(bench_opts, std::get<1>(res));

            REQUIRE( csv_results.size() == 1 );

            auto csv_results00_content_lines = split_string(csv_results[0].content);

            CAPTURE(output);
            CAPTURE(csv_results[0].content);

            auto csv_results_00_lines = split_string(csv_results[0].content);
            auto output_lines = split_string(output);

            THEN ( "headers are in the same order" ) {
                REQUIRE(csv_results_00_lines.size() >= 1);
                REQUIRE(output_lines.size() >= 33);

                REQUIRE_THAT( csv_results_00_lines[0], Matches( fmt::format(";.{}.;.{}.;.{}.;.{}.;.{}.;.{}.", "aaaaupdate", "eeffupdate", "aaaaaaupdate", "eeeeee1update", "eeeeee2update", "gggggggupdate") ) );
                REQUIRE_THAT( output_lines[32], Matches( fmt::format("\\s+{}\\s+{}\\s+{}\\s+{}\\s+{}\\s+{}", "aaaaupdate", "eeffupdate", "aaaaaaupdate", "eeeeee1update", "eeeeee2update", "gggggggupdate") ) );
            }

        }
    }

}