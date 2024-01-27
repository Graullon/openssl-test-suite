#include "Benchmark.h"

#include <iostream>
#include <intrin.h>

namespace {
	static const double cpu_frequency_hz = 2800000000.0;
	static const double cpu_tdp = 45.0;
}

Benchmark::Benchmark() {
	#ifndef BENCHMARK_MODE
	std::cout << "---------------------------------------------------------------------" << std::endl;
	#endif
}

Benchmark::~Benchmark() {
	#ifndef BENCHMARK_MODE
	std::cout << "---------------------------------------------------------------------" << std::endl << std::endl;
	#endif
}

void Benchmark::save() {

	const uint64_t cpu_cycles = __rdtsc();
	const std::chrono::nanoseconds ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

	m_points.push_back({ms, cpu_cycles });

	#ifndef BENCHMARK_MODE
	std::cout << "----------------------------------";
	#endif
}

void Benchmark::print() const {
	
	if (m_points.size() <= 1) {
		return;
	}

	for (size_t i = 1; i < m_points.size(); ++i) {

		const std::pair<std::chrono::nanoseconds, uint64_t>& previous = m_points[i - 1];
		const std::pair<std::chrono::nanoseconds, uint64_t>& next = m_points[i];

		const std::pair<std::chrono::nanoseconds, uint64_t> difference({ next.first - previous.first, next.second - previous.second });

		const double max_cpu_cycles_per_time_diff = cpu_frequency_hz * difference.first.count() / 1000000000.0;
		const double max_tdp_per_time_diff = cpu_tdp * difference.first.count() / 1000000000.0;

		std::cout
			<< "\nBenchmark point #" << i
			<< "\nCPU cycles burnt: " << difference.second
			<< "\nPower consumption (watts per hour): " << difference.second * max_tdp_per_time_diff / max_cpu_cycles_per_time_diff 
			<< std::endl;
	}
}