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
	const std::chrono::high_resolution_clock::time_point time_point = std::chrono::high_resolution_clock::now();

	m_points.push_back({ time_point, cpu_cycles });

	#ifndef BENCHMARK_MODE
	std::cout << "----------------------------------";
	#endif
}

void Benchmark::print() const {
	
	if (m_points.size() <= 1) {
		return;
	}

	for (size_t i = 1; i < m_points.size(); ++i) {

		const std::pair<std::chrono::steady_clock::time_point, uint64_t>& previous = m_points[i - 1];
		const std::pair<std::chrono::steady_clock::time_point, uint64_t>& next = m_points[i];

		#define TIME_RESOLUTION_1H std::chrono::hours(1)
		#define TIME_RESOLUTION_DESIRED std::chrono::microseconds

		const std::pair<TIME_RESOLUTION_DESIRED, uint64_t> difference({
			std::chrono::duration_cast<TIME_RESOLUTION_DESIRED>(next.first.time_since_epoch()) - std::chrono::duration_cast<TIME_RESOLUTION_DESIRED>(previous.first.time_since_epoch()),
			next.second - previous.second
			});

		const TIME_RESOLUTION_DESIRED time_resolution_in_time_resolution_desired = std::chrono::duration_cast<TIME_RESOLUTION_DESIRED>(TIME_RESOLUTION_1H);

		std::cout
			<< "\nBenchmark point #" << i
			<< "\nDuration (microseconds): " << difference.first.count()
			<< "\nCPU cycles burnt: " << difference.second
			<< "\nPower consumption (nanowatts/duration): " << (cpu_tdp * difference.first / time_resolution_in_time_resolution_desired) * 1000000000
			<< std::endl;

		//std::cout << difference.first.count() << ';' << difference.second << ';' << (cpu_tdp * difference.first / time_resolution_in_time_resolution_desired) * 1000000000 << '\n';
	}
}