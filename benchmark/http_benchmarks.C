#include "http_request.H"

#include <benchmark/benchmark.h>
#include <cstdlib>

auto variance = [](const std::vector<double>& values) {
    double       sum_x         = 0;
    double       sum_x_squared = 0;
    const double n             = values.size();
    for (const auto v : values) {
        sum_x += v;
        sum_x_squared += v * v;
    }
    sum_x /= n;
    sum_x_squared /= n;
    return (sum_x_squared - (sum_x * sum_x));
};

static void BM_HttpRequestDeserialize(benchmark::State& state) {
    const std::string sample_request = "GET / HTTP/1.1\n"
                                       "Host: wttr.in\n"
                                       "User-Agent: curl/7.58.0\n"
                                       "Accept: */*\r\n"
                                       "\r\n";
    for (auto _ : state) {
        auto req = shrek::http_request::deserialize(
            reinterpret_cast<const uint8_t*>(sample_request.data()),
            sample_request.size());
        benchmark::DoNotOptimize(req);
    }
}

BENCHMARK(BM_HttpRequestDeserialize)->ComputeStatistics("variance", variance);

BENCHMARK_MAIN();
