#pragma once

#include <vector>
#include <chrono>
#include <utility>

class Benchmark {

	private:
		std::vector< std::pair<std::chrono::high_resolution_clock::time_point, uint64_t> > m_points;

	public:
		Benchmark();
		~Benchmark();

		void save();
		void print() const;
};