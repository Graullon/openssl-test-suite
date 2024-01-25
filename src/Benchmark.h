#pragma once

#include <vector>
#include <chrono>
#include <utility>

class Benchmark {

	private:
		std::vector< std::pair<std::chrono::milliseconds, uint64_t> > m_points;

	public:
		Benchmark();

		void save();
		void print() const;
};