/*
* Copyright (C) 2015 Christopher Gilbert.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef BENCHPRESS_HPP
#define BENCHPRESS_HPP

#include <algorithm>   // max, min
#include <atomic>      // atomic_intmax_t
#include <chrono>      // high_resolution_timer, duration
#include <functional>  // function
#include <iomanip>     // setw
#include <regex>       // regex, regex_match
#include <sstream>     // stringstream
#include <string>      // string
#include <thread>      // thread
#include <vector>      // vector
#include <memory>      // unique_ptr, make_unique
#include <map>         
#include <unordered_map>
#include <set>
#include <sstream>
#include <tuple>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/format-inl.h"
#include "fmt/ostream.h"
#include "fmt/chrono.h"

namespace benchpress {

/*
 * The options class encapsulates all options for running benchmarks.
 *
 * When including benchpress, a main function can be emitted which includes a command-line parser for building an
 * options object. However from time-to-time it may be necessary for the developer to have to build their own main
 * stub and construct the options object manually.
 *
 * options opts;
 * opts
 *     .bench(".*")
 *     .benchtime(1)
 *     .cpu(4);
 */
class options {
    std::vector<std::string> d_bench;
    std::chrono::seconds     d_benchtime;
    size_t      d_cpu;
    bool        d_plotdata;
    int         d_col_width;
    std::string d_csvoutput;
    std::string d_csvprefix;
    std::string d_csvsuffix;
    bool        d_csvsort;
    std::string d_csvunit;
public:
    options()
        : d_bench( { ".*" } )
        , d_benchtime(1)
        , d_cpu(std::thread::hardware_concurrency())
        , d_plotdata(false)
        , d_col_width(16)
        , d_csvsort(false)
    {}
    options& bench(const std::vector<std::string>& bench) {
        d_bench = bench;
        return *this;
    }
    options& benchtime(std::chrono::seconds benchtime) {
        d_benchtime = benchtime;
        return *this;
    }
    options& cpu(size_t cpu) {
        d_cpu = cpu;
        return *this;
    }
    options& plotdata(bool plotdata) {
        d_plotdata = plotdata;
        return *this;
    }
    options& colwidth(size_t col_width) {
        d_col_width = col_width;
        return *this;
    }
    options& csvoutput(std::string csvoutput) {
        d_csvoutput = csvoutput;
        return *this;
    }
    options& csvprefix(std::string csvprefix) {
        d_csvprefix = csvprefix;
        return *this;
    }
    options& csvsuffix(std::string csvsuffix) {
        d_csvsuffix = csvsuffix;
        return *this;
    }
    options& csvsort(bool csvsort) {
        d_csvsort = csvsort;
        return *this;
    }
    options& csvunit(std::string csvunit) {
        d_csvunit = csvunit;
        return *this;
    }

    std::vector<std::string> bench() const {
        return d_bench;
    }
    std::vector<std::string> get_bench() const { return bench(); }
    
    std::chrono::seconds benchtime() const {
        return d_benchtime;
    }
    size_t get_benchtime() const { return benchtime().count(); }

    size_t cpu() const {
        return d_cpu;
    }
    size_t get_cpu() const { return cpu(); }

    bool plotdata() const {
        return d_plotdata;
    }
    bool get_plotdata() const { return plotdata(); }

    int colwidth() const {
        return d_col_width;
    }
    int get_colwidth() const { return colwidth(); }

    std::string csvoutput() const {
        return d_csvoutput;
    }
    std::string csvprefix() const {
        return d_csvprefix;
    }
    std::string csvsuffix() const {
        return d_csvsuffix;
    }
    bool csvsort() const {
        return d_csvsort;
    }
    std::string csvunit() const {
        return d_csvunit;
    }
};

class context;

/*
 * The benchmark_info class is used to store a function name / pointer pair.
 *
 * benchmark_info bi("example", [](benchpress::context* b) {
 *     // benchmark function
 * });
 */
class benchmark_info {
    std::string                         d_name;
    const std::function<void(context*)> d_func;

public:
    benchmark_info(std::string name, std::function<void(context*)> func)
        : d_name(name)
        , d_func(func)
    {}

    std::string                   name() const { return d_name; }
    std::function<void(context*)> func() const { return d_func; }
    std::string                   get_name() const { return d_name; }
    std::function<void(context*)> get_func() const { return d_func; }
};

/*
 * The registration class is responsible for providing a single global point of reference for registering
 * benchmark functions.
 *
 * registration::get_ptr()->register_benchmark(info);
 */
class registration {
    std::vector<benchmark_info> d_benchmarks;

public:
    static registration* ptr() {
        static std::unique_ptr<registration> d_this = std::make_unique<registration>();
        return d_this.get();
    }
    static registration* get_ptr() { return ptr(); }

    void register_benchmark(benchmark_info& info) {
        d_benchmarks.push_back(info);
    }

    std::vector<benchmark_info> benchmarks() { return d_benchmarks; }
    std::vector<benchmark_info> get_benchmarks() { return d_benchmarks; }
};

/*
 * The auto_register class is a helper used to register benchmarks.
 */
class auto_register {
public:
    auto_register(const std::string& name, std::function<void(context*)> func) {
        benchmark_info info(name, func);
        registration::ptr()->register_benchmark(info);
    }
};

#define CONCAT(x, y) x ## y
#define CONCAT2(x, y) CONCAT(x, y)
#ifdef __COUNTER__ 
#define REGISTER_NAME CONCAT2(__LINE__, __COUNTER__)
#else
#define REGISTER_NAME __LINE__
#endif

// The BENCHMARK macro is a helper for creating benchmark functions and automatically registering them with the
// registration class.
#define BENCHMARK(x, f) benchpress::auto_register CONCAT2(register_, REGISTER_NAME)((x), (f));


// This macro will prevent the compiler from removing a redundant code path which has no side-effects.
#define DISABLE_REDUNDANT_CODE_OPT() { asm(""); }

/*
 * This function can be used to keep variables on the stack that would normally be optimised away
 * by the compiler, without introducing any additional instructions or changing the behaviour of
 * the program.
 * 
 * This function uses the Extended Asm syntax of GCC. The volatile keyword indicates that the 
 * following instructions have some unknowable side-effect, and ensures that the code will neither 
 * be moved, nor optimised away.
 *
 * AssemblerTemplate: No operands.
 *
 * OutputOperands: None.
 *
 * InputOperands: The "g" is a wildcard constraint which tells the compiler that it may choose what 
 * to use for p (eg. a register OR a memory reference).
 *
 * Clobbers: The "memory" clobber tells the compiler that the assembly code performs reads or writes
 * to the memory pointed to by one of the input parameters.
 *
 * Example usage:
 *  std::vector<int> v;
 *  v.reserve(10);
 *  escape(v.data());
 */
#ifdef _MSC_VER

#pragma optimize("", off)

template <class T>
void escape(T&& datum)
{
  // see here: http://stackoverflow.com/questions/28287064/how-not-to-optimize-away-mechanics-of-a-folly-function
  datum = datum;
}

#pragma optimize("", on)

#else

inline void escape(void *p)
{
  asm volatile("" : : "g"(p) : "memory");
}

#endif

/*
 * This function can be used to disable the optimiser. It has the effect of creating a read / write
 * memory barrier for the compiler, meaning it does not assume that any values read from memory before
 * the asm remain unchanged after that asm; it reloads them as needed.
 *
 * Example usage:
 *  std::vector<int> v;
 *  v.reserve(10);
 *  escape(v.data());
 *  v.push_back(42);
 *  clobber(); // Ensure the integer pushed is read
 */
#ifdef _MSC_VER

inline void clobber()
{
  // see here: http://stackoverflow.com/questions/14449141/the-difference-between-asm-asm-volatile-and-clobbering-memory
  _ReadWriteBarrier();
}

#else

inline void clobber()
{
  asm volatile("" : : : "memory");
}

#endif

/*
 * The result class is responsible for producing a printable string representation of a benchmark run.
 */
class result {
    using fsec_t = std::chrono::duration<double>;
    using duration_t = std::chrono::nanoseconds;

    size_t                   d_num_iterations;
    std::chrono::nanoseconds d_duration;
    size_t                   d_num_bytes;

    std::string             d_name; 
    std::string             d_col;
    std::string             d_row;

public:
    result(size_t num_iterations, std::chrono::nanoseconds duration, size_t num_bytes)
        : d_num_iterations(num_iterations)
        , d_duration(duration)
        , d_num_bytes(num_bytes)
    {}

    result& name(std::string name) {
        d_name = name;
        return *this;
    }
    std::string name() const { return d_name; }

    result& col(std::string col) {
        d_col = col;
        return *this;
    }
    std::string col() const { return d_col; }

    result& row(std::string row) {
        d_row = row;
        return *this;
    }
    std::string row() const { return d_row; }

    size_t ns_per_op() const {
        if (d_num_iterations <= 0) {
            return 0;
        }
        return std::chrono::duration_cast<std::chrono::nanoseconds>(d_duration).count() / d_num_iterations;
    }

    size_t ms_per_op() const {
        if (d_num_iterations <= 0) {
            return 0;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(d_duration).count() / d_num_iterations;
    }

    double s_per_op() const {
        if (d_num_iterations <= 0) {
            return 0;
        }
        return std::chrono::duration_cast<fsec_t>(d_duration).count() / d_num_iterations;
    }

    std::chrono::nanoseconds time_per_op() const {
        if (d_num_iterations <= 0) {
            return std::chrono::nanoseconds::zero();
        }
        return d_duration / d_num_iterations;
    }

    double mb_per_s() const {
        if (d_num_iterations <= 0 || d_duration.count() <= 0 || d_num_bytes <= 0) {
            return 0;
        }
        return (double(d_num_bytes) * double(d_num_iterations) / double(1e6)) /
                std::chrono::duration<double>(d_duration).count();
    }

    std::string to_string() const {
        std::stringstream tmp;

        fmt::print(tmp, "{:>12}", d_num_iterations);

        //tmp << std::setw(18) << std::right << npo << std::setw(0) << " ns/op";
        fmt::print(tmp, "{:>12} ns/op", ns_per_op());

        size_t mpo = ms_per_op();
        if(mpo > 0){
            //tmp << std::setw(14) << std::right << mpo << std::setw(0) << " ms/op";
            fmt::print(tmp, "{:>14} ms/op", mpo);
        }

        double spo = s_per_op();
        if(spo > 0.01){
            /*
            tmp << std::setw(14) << std::right 
                << std::fixed << std::setprecision( 2 ) << spo
                << std::setw(0) << " s/op";
            */

            fmt::print(tmp, "{:>14.2f} s/op", spo);
        }

        double mbs = mb_per_s();
        if (mbs > 0.0) {
            //tmp << std::setw(14) << std::right << mbs << std::setw(0) << " MB/s";
            fmt::print(tmp, "{:>14} MB/s", mbs);
        }

        return tmp.str();
    }
};

/*
 * The parallel_context class is responsible for providing a thread-safe context for parallel benchmark code.
 */
class parallel_context {
    std::atomic_intmax_t d_num_iterations;
public:
    parallel_context(size_t num_iterations)
        : d_num_iterations(num_iterations)
    {}

    bool next() {
        return (d_num_iterations.fetch_sub(1) > 0);
    }
};

/*
 * The context class is responsible for providing an interface for capturing benchmark metrics to benchmark functions.
 */
class context {
    bool                                           d_timer_on;
    std::chrono::high_resolution_clock::time_point d_start;
    std::chrono::nanoseconds                       d_duration;
    std::chrono::seconds                           d_benchtime;
    size_t                                         d_num_iterations;
    size_t                                         d_num_threads;
    size_t                                         d_num_bytes;
    benchmark_info                                 d_benchmark;

public:
    context(const benchmark_info& info, const options& opts)
        : d_timer_on(false)
        , d_start()
        , d_duration()
        , d_benchtime(opts.benchtime())
        , d_num_iterations(1)
        , d_num_threads(opts.cpu())
        , d_num_bytes(0)
        , d_benchmark(info)
    {}

    size_t num_iterations() const { return d_num_iterations; }
    size_t get_num_iterations() const { return d_num_iterations; }

    context& num_threads(size_t n) { d_num_threads = n; return *this; }
    void set_num_threads(size_t n) { num_threads(n); }
    size_t num_threads() const { return d_num_threads; }

    void start_timer() {
        if (!d_timer_on) {
            d_start = std::chrono::high_resolution_clock::now();
            d_timer_on = true;
        }
    }
    void stop_timer() {
        if (d_timer_on) {
            d_duration += std::chrono::high_resolution_clock::now() - d_start;
            d_timer_on = false;
        }
    }
    void reset_timer() {
        if (d_timer_on) {
            d_start = std::chrono::high_resolution_clock::now();
        }
        d_duration = std::chrono::nanoseconds::zero();
    }

    context& bytes(int64_t bytes) { d_num_bytes = bytes; return *this; }
    void set_bytes(int64_t bytes) { d_num_bytes = bytes; }

    size_t ns_per_op() {
        if (d_num_iterations <= 0) {
            return 0;
        }
        return d_duration.count() / d_num_iterations;
    }

    void run_n(size_t n) {
        d_num_iterations = n;
        reset_timer();
        start_timer();
        d_benchmark.func()(this);
        stop_timer();
    }

    void run_parallel(std::function<void(parallel_context*)> f) {
        parallel_context pc(d_num_iterations);
        std::vector<std::thread> threads;
        for (size_t i = 0; i < d_num_threads; ++i) {
            threads.push_back(std::thread([&pc,&f]() -> void {
                f(&pc);
            }));
        }
        for(auto& thread : threads){
            thread.join();
        }
    }

    result run() {
        size_t n = 1;
        run_n(n);
        while (d_duration < d_benchtime && n < 1e9) {
            size_t last = n;
            if (ns_per_op() == 0) {
                n = static_cast<size_t>(1e9);
            } else {
                n = d_duration.count() / ns_per_op();
            }
            n = std::max(std::min(n+n/2, 100*last), last+1);
            n = round_up(n);
            run_n(n);
        }
        return result(n, d_duration, d_num_bytes);
    }

private:
    template<typename T>
    T round_down_10(T n) {
        int tens = 0;
        while (n > 10) {
            n /= 10;
            tens++;
        }
        int result = 1;
        for (int i = 0; i < tens; ++i) {
            result *= 10;
        }
        return result;
    }

    template<typename T>
    T round_up(T n) {
        T base = round_down_10(n);
        if (n < (2 * base)) {
            return 2 * base;
        }
        if (n < (5 * base)) {
            return 5 * base;
        }
        return 10 * base;
    }
};

}; // namespace benchpress

#if defined(BENCHPRESS_CONFIG_MAIN) || defined(BENCHPRESS_CONFIG_RUN_BENCHMARKS)

#include <iostream>    // cout
#include <fstream>

namespace benchpress {

struct benpress_results {

    struct results_map_value {
        std::string value_str;
        std::chrono::nanoseconds value;
    };

    std::vector<result> results;
    std::set<std::string> headers;
    std::set<std::string> fields;
    std::map<std::string, std::map<std::string, results_map_value>> results_map;
};

/*
 * The run_benchmarks function will run the registered benchmarks.
 */
std::tuple<std::string, benpress_results> run_benchmarks_details(const options& opts) {
    using namespace std::string_literals;

    std::stringstream ret;
    benpress_results res;

    auto std_replace = [](std::string& str,
                const std::string& oldStr,
                const std::string& newStr) {
        std::string::size_type pos = 0u;
        while((pos = str.find(oldStr, pos)) != std::string::npos){
            str.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
    };

    for(std::string bench : opts.bench()){
        std::regex match_r(bench);

        auto benchmarks = registration::ptr()->benchmarks();

        std::string prefix = (opts.plotdata())? "## " : "";

        size_t rowcounter = 1;
        for (auto& info : benchmarks) {
            std::string name = info.name();
            if (std::regex_match(name, match_r)) {
                context c (info, opts);
                auto r = c.run();

                //std::cout << prefix << std::setw(64) << std::left << name << rstr << '\n';
                fmt::print(ret, "{}{:<{}}{}\n", prefix, name, 4*opts.colwidth(), r.to_string());

                r.name(name);

                std::string col = bench;
                std_replace(col, ".*", "");
                std_replace(col, "\\s+", "");
                std_replace(col, " ", "");
                r.col(col);

                std::string tag;
                std::regex tag_regex (".*\\[\\s*(\\d+)\\s*\\].*");
                std::smatch tag_match;
                if (std::regex_match(name, tag_match, tag_regex)) {
                    if(tag_match.size() > 1){
                        tag = tag_match[1].str();
                    }
                }
                std::string row = (!tag.empty())? tag : std::to_string(rowcounter);
                r.row(row);

                res.results.push_back(r);
                rowcounter++;
            }
        }
    }

    if(opts.plotdata() || !opts.csvoutput().empty()) {
        if (opts.plotdata()) {
            fmt::print(ret, "{}", "\n");
            fmt::print(ret, "{}", "# plot data\n");
        }

        for(auto& result : res.results) {
            std::string name = result.name();

            std::string col = result.col();
            std::string row = result.row();

            std::chrono::nanoseconds value = result.time_per_op();
            std::string result_str = std::to_string(value.count());

            result_str = (result_str.empty())? "?"s : result_str;
            result_str = (col.empty() && row.empty())? "?"s : result_str;


            std::stringstream sort_col_ss;
            //sort_col_ss << std::setw(COL_WIDTH) << std::setfill(' ') << std::right << col;
            fmt::print(sort_col_ss, "{:>{}}", col, opts.colwidth());
            std::string sort_col = sort_col_ss.str();
            
            std::stringstream sort_row_ss;
            //sort_row_ss << std::setw(COL_WIDTH) << std::setfill(' ') << std::right << row;
            fmt::print(sort_row_ss, "{:>{}}", row, opts.colwidth());
            std::string sort_row = sort_row_ss.str();

            std::stringstream sort_result_ss;
            //sort_result_ss << std::setw(COL_WIDTH) << std::setfill(' ') << std::right << result_str;
            fmt::print(sort_result_ss, "{:>{}}", result_str, opts.colwidth());
            std::string sort_result = sort_result_ss.str();


            res.headers.insert(sort_col);
            res.fields.insert(sort_row);
            res.results_map[sort_row][sort_col] = { sort_result, result.time_per_op() };
        }

        //std::cout << "#### " << "[Debug] results_map result" << '\n';
        //for(const auto& r1 : results_map) {
        //    for(const auto& r2 : r1.second) {
        //        std::cout << "### " << r1.first << " " << r2.first << " " << r2.second << '\n'; 
        //    }
        //}

        if(opts.plotdata() && !res.results_map.empty()) {
            //ret << "# " << std::setw(COL_WIDTH) << std::right << std::setfill(' ') << ' ';
            fmt::print(ret, "{:>{}} ", " ", opts.colwidth());
            for(const auto& header : res.headers) {
                //ret << header;
                fmt::print(ret, "{}", header);
            }
            //ret << '\n';
            fmt::print(ret, "{}", "\n");

            for(const auto& row_result : res.results_map) {
                const auto& row_left_header = row_result.first;
                const auto& row = row_result.second;

                if(!row_left_header.empty()) {
                    //ret << "  " << row_left_header;
                    fmt::print(ret, " {}", row_left_header);

                    for(const auto& header : res.headers) {
                        const auto find_row_value = row.find(header);
                        if(find_row_value != std::end(row)) {
                            //ret << find_row_value->second;
                            fmt::print(ret, "{:>{}}", find_row_value->second.value_str, opts.colwidth());
                        } else {
                            //ret << std::setw(COL_WIDTH) << std::right << std::setfill(' ') << "?";
                            fmt::print(ret, "{:>{}}", "?", opts.colwidth());
                        }
                    }

                    //ret << '\n';
                    fmt::print(ret, "{}", "\n");
                }
            }
        }
        //ret << '\n';
        fmt::print(ret, "{}", "\n");
    }

    return { ret.str(), res };
}

std::string run_benchmarks(const options& opts) {
    auto results = run_benchmarks_details(opts);
    return std::get<0>(results);
}

std::string ltrim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string rtrim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}


struct benchpress_csv_result {
    std::string filename;
    std::map<std::string, std::string> results;
    std::map<std::string, std::map<std::string, std::string>> results_map;
    std::string content;
};

std::vector<benchpress_csv_result> make_csv(const options& opts, const benpress_results& bench_results) {
    std::map<std::string, benchpress_csv_result> rettmp;

    const char DELIMITER = ';';
    const std::string QUOTECHAR = "\"";
    const std::string NA = QUOTECHAR + "N/A" + QUOTECHAR;
    const std::string DEFAULT_CSVFILENAME = "output";

    for (const auto& res : bench_results.results_map) {
        const auto& row_field = res.first;
        auto rows = res.second;

        for (const auto& col : rows) {
            const auto& col_header = col.first;
            const auto& values = col.second;

            std::string header = trim(col_header);
            std::string rowname = row_field;

            std::string outdir = opts.csvoutput();
            if (outdir.empty()) {
                outdir = "./";
            }
            outdir = std::regex_replace(outdir, std::regex("\\\\"), "/"); 
            if (outdir.length() > 0 && outdir[outdir.length()-1] != '/') {
                outdir += "/";
            }

            std::string header_filename = std::regex_replace(header, std::regex(opts.csvsuffix()), ""); 
            std::string filename = fmt::format("{}{}{}", outdir, opts.csvprefix() + header_filename + opts.csvsuffix(), ".csv");
            std::string filename_map = fmt::format("{}{}{}", outdir, (!opts.csvprefix().empty())? opts.csvprefix() : DEFAULT_CSVFILENAME, ".csv");

            std::string value_str = fmt::format("{}", values.value_str);
            if (opts.csvunit() == "seconds") {
                value_str = fmt::format("{:.3f}s", std::chrono::duration_cast<std::chrono::duration<double>>(values.value).count() );
            } else if (opts.csvunit() == "milliseconds") {
                value_str = fmt::format("{}ms", std::chrono::duration_cast<std::chrono::milliseconds>(values.value).count() );
            } else if (opts.csvunit() == "microseconds") {
                value_str = fmt::format("{}us", std::chrono::duration_cast<std::chrono::microseconds>(values.value).count() );
            } else if (opts.csvunit() == "nanoseconds") {
                value_str = fmt::format("{}ns", std::chrono::duration_cast<std::chrono::nanoseconds>(values.value).count() );
            } else if (opts.csvunit() == "minutes") {
                value_str = fmt::format("{}min", std::chrono::duration_cast<std::chrono::minutes>(values.value).count() );
            } else if (opts.csvunit() == "hours") {
                value_str = fmt::format("{}h", std::chrono::duration_cast<std::chrono::hours>(values.value).count() );
            }


            if (!opts.csvsuffix().empty()) {
                rettmp[filename].filename = filename;
                rettmp[filename].results[rowname] = value_str;
            } else {
                rettmp[filename_map].filename = filename_map;
                rettmp[filename_map].results_map[rowname][header] = value_str;
            }
        }

    }

    std::vector<benchpress_csv_result> ret;
    for (auto& r : rettmp) {
        const auto& filename = r.first;
        auto& content = r.second.content;

        if (!opts.csvsuffix().empty()) {
            std::vector<std::pair<std::string, std::string>> results;
            std::transform(
                std::begin(r.second.results),
                std::end(r.second.results),
                std::back_inserter(results),
                [](const std::pair<std::string, std::string>& p) {
                    return p;
                }
            );

            if (opts.csvsort()) {
                std::sort(std::begin(results), std::end(results), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b){
                    return a.second.compare(b.second) < 0;
                });
            }

            for (auto& row : results) {
                const auto& rowname = row.first;
                const auto& value = row.second;

                content = fmt::format("{}{}{}{}\n", content, QUOTECHAR + trim(rowname) + QUOTECHAR, DELIMITER, trim(value));
            }
            if (content.length() > 0 && content[content.length()-1] == '\n') {
                content.pop_back();
            }
        } else {
            std::set<std::string> colnames;
            for (auto& row : r.second.results_map) {
                const auto& rowname = row.first;
                const auto& cols = row.second;

                for (auto& col : cols) {
                    const auto& colname = col.first;
                    colnames.insert(colname);
                }
            }

            content = fmt::format("{}{}", content, DELIMITER);
            for (const auto& colname : colnames) {
                content = fmt::format("{}{}{}", content, QUOTECHAR + trim(colname) + QUOTECHAR, DELIMITER);
            }
            if (content.length() > 0 && content[content.length()-1] == DELIMITER) {
                content.pop_back();
            }
            content = fmt::format("{}\n", content);

            for (auto& row : r.second.results_map) {
                const auto& rowname = row.first;
                const auto& cols = row.second;

                content = fmt::format("{}{}{}", content, QUOTECHAR + trim(rowname) + QUOTECHAR, DELIMITER);
                for (const auto& colname : colnames) {
                    auto col = std::find_if(std::begin(cols), std::end(cols), [colname](const std::pair<std::string, std::string>& c) {
                        return c.first == colname;
                    });
                    if (col != std::end(cols)) {
                        const auto& value = col->second;
                        content = fmt::format("{}{}{}", content, trim(value), DELIMITER);
                    } else {
                        content = fmt::format("{}{}{}", content, NA, DELIMITER);
                    }
                }
                if (content.length() > 0 && content[content.length()-1] == DELIMITER) {
                    content.pop_back();
                }

                content = fmt::format("{}\n", content);
            }
            if (content.length() > 0 && content[content.length()-1] == '\n') {
                content.pop_back();
            }
        }

        ret.push_back(r.second);
    }

    return ret;
}

}; // namespace benchpress
#endif


/*
 * If BENCHPRESS_CONFIG_MAIN is defined when the file is included then a main function will be emitted which provides a
 * command-line parser and then executes run_benchmarks.
 */
#ifdef BENCHPRESS_CONFIG_MAIN
#include "cxxopts.hpp"
int main(int argc, char** argv) {
    const int DEFAULT_COL_WIDTH = 16;
    const std::chrono::seconds DEFAULT_BENCHTIME = std::chrono::seconds(1);

    std::chrono::high_resolution_clock::time_point bp_start = std::chrono::high_resolution_clock::now();
    benchpress::options bench_opts;
    try {
        cxxopts::Options cmd_opts(argv[0], " - command line options");
        cmd_opts.add_options()
            ("bench", "run benchmarks matching the regular expression", cxxopts::value<std::vector<std::string>>()
                ->default_value( { ".*" } ))
            ("benchtime", "run enough iterations of each benchmark to take t seconds", cxxopts::value<size_t>()
                ->default_value(std::to_string(DEFAULT_BENCHTIME.count())))
            ("cpu", "specify the number of threads to use for parallel benchmarks", cxxopts::value<size_t>()
                ->default_value(std::to_string(std::thread::hardware_concurrency())))
            ("list", "list all available benchmarks")
            ("help", "print help")
        ;
        cmd_opts.add_options()
            ("plotdata", "print plot data for gnuplot (use [tags] to tag the xlabel)")
            ("colwidth", "print plot data colume width", cxxopts::value<int>()
                ->default_value(std::to_string(DEFAULT_COL_WIDTH)))
        ;
        cmd_opts.add_options()
            ("csvoutput", "CSV output directory", cxxopts::value<std::string>()
                ->default_value(""))
            ("csvsuffix", "suffix for CSV file", cxxopts::value<std::string>()
                ->default_value(""))
            ("csvprefix", "prefix for CSV file", cxxopts::value<std::string>()
                ->default_value(""))
            ("csvsort", "sort CSV files by value")
            ("csvunit", "(time) unit of the CSV file values", cxxopts::value<std::string>()
                ->default_value(""))
        ;
        cmd_opts.parse(argc, argv);

        if (cmd_opts.count("help")) {
            //std::cout << cmd_opts.help({""}) << '\n';
            fmt::print("{}\n", cmd_opts.help({""}));
            exit(0);
        }

        if (cmd_opts.count("bench")) {
            bench_opts.bench(cmd_opts["bench"].as<std::vector<std::string>>());
        }
        if (cmd_opts.count("benchtime")) {
            bench_opts.benchtime(std::chrono::seconds(cmd_opts["benchtime"].as<size_t>()));
        }
        if (cmd_opts.count("cpu")) {
            bench_opts.cpu(cmd_opts["cpu"].as<size_t>());
        }

        bench_opts.plotdata(cmd_opts.count("plotdata"));
        if (cmd_opts.count("colwidth")) {
            bench_opts.colwidth(cmd_opts["colwidth"].as<int>());
        }

        if (cmd_opts.count("csvoutput")) {
            bench_opts.csvoutput(cmd_opts["csvoutput"].as<std::string>());
            bench_opts.csvprefix(cmd_opts["csvprefix"].as<std::string>());
            bench_opts.csvsuffix(cmd_opts["csvsuffix"].as<std::string>());
            bench_opts.csvunit(cmd_opts["csvunit"].as<std::string>());
            bench_opts.csvsort(cmd_opts.count("csvsort"));
        }

        if (cmd_opts.count("list")) {
            auto benchmarks = benchpress::registration::ptr()->benchmarks();
            for (const auto& info : benchmarks) {
                //std::cout << info.get_name() << '\n';
                fmt::print("{}\n", info.name());
            }
            exit(EXIT_SUCCESS);
        }
    } catch (const cxxopts::OptionException& e) {
        std::string prefix = (bench_opts.plotdata())? "## " : "";

        //std::cout << prefix << "error parsing options: " << e.what() << '\n';
        fmt::print("{}error parsing options: {}\n", prefix, e.what());
        exit(1);
    }
    std::string prefix = (bench_opts.plotdata())? "## " : "";

    auto res = benchpress::run_benchmarks_details(bench_opts);
    std::string result = std::get<0>(res);
    fmt::print("{}\n\n", result);

    if (!bench_opts.csvoutput().empty()) {
        auto results = std::get<1>(res);
        auto results_csv = make_csv(bench_opts, results);

        for (const auto& result : results_csv) {
            std::ofstream out (result.filename, std::ofstream::out);
            out << result.content;
            out.close();
        }
    }

    auto duration = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - bp_start);
    //std::cout << prefix << argv[0] << " " << duration << "s" << '\n';
    fmt::print("{}{} {:.2f}s\n", prefix, argv[0], duration.count());
    return 0;
}
#endif

#endif // BENCHPRESS_HPP
