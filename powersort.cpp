#include <cassert>
#include "insertionsort.h"
#include "merging.h"
#include <vector>
#include <iterator>
#include <iostream>



// Definitions:
typedef unsigned int power_t;
typedef int* Iterator;
struct run {
	Iterator begin; Iterator end;
	bool operator==(const run &rhs) const {
		return begin == rhs.begin && end == rhs.end;
	}
	bool operator!=(const run &rhs) const { return !(rhs == *this); }
};
run NULL_RUN {};

struct run_n_power {
	Iterator begin; Iterator end;
    power_t power = 0;
	bool operator==(const run_n_power &rhs) const {
		return begin == rhs.begin && end == rhs.end;
	}
	bool operator!=(const run_n_power &rhs) const { return !(rhs == *this); }
};

struct run_begin_n_power {
	Iterator begin;
    power_t power = 0;
	bool operator==(const run_begin_n_power &rhs) const {
		return begin == rhs.begin;
	}
	bool operator!=(const run_begin_n_power &rhs) const { return !(rhs == *this); }
};
run_begin_n_power NULL_RUN_N_POWER {};

unsigned floor_log2(unsigned int n) {
        if (n <= 0) return 0;
        return 31 - __builtin_clz( n );
    }

unsigned floor_log2(unsigned long n) {
    if (n <= 0) return 0;
    return 63 - __builtin_clzl( n );
}

int minRunLen = 5;
// Node power:
power_t node_power(size_t begin, size_t end,
                    size_t beginA, size_t beginB, size_t endB) {
	size_t n = end - begin;
	assert(n <= (1L << 31));
	unsigned long l2 = beginA + beginB - 2*begin; // 2*l
	unsigned long r2 = beginB + endB - 2*begin;   // 2*r
	auto a = static_cast<unsigned int>((l2 << 30) / n);
	auto b = static_cast<unsigned int>((r2 << 30) / n);
    power_t power = __builtin_clz(a ^ b);
    std::cout << "node power: ";
    std::cout << beginA << " " << beginB << " " << " " << endB << " " << power << "\n";
	return power;
}
//buffer:
std::vector<int> _buffer(30);

// Main sort:
void power_sort_paper(Iterator begin, Iterator end) {
    const size_t n = end - begin;
    const unsigned maxStackHeight = floor_log2(n) + 1;
    run_begin_n_power stack[maxStackHeight];
    unsigned top = 0; // topmost occupied entry in stack; keep on NULL_RUN_N_POWER in stack[0]
    run_n_power runA = {begin, extend_and_reverse_run_right(begin, end), 0};
    //extend to minRunLen
    size_t lenA = runA.end - runA.begin;
    std::cout << "começo da run A: " << *runA.begin << " ";
    std::cout << "fim da run A: " << *(runA.end-1) << " ";
    while (runA.end < end) {
        run runB = {runA.end, extend_and_reverse_run_right(runA.end, end)};
        std::cout << "começo da run b: " << *runB.begin << " ";
        std::cout << "fim da run b: " << *(runB.end-1) << " ";
        // extend to minRunLen
        size_t lenB = runB.end - runB.begin;
        runA.power = node_power(0, n,
                                (size_t) (runA.begin-begin),
                                (size_t) (runB.begin-begin),
                                (size_t) (runB.end-begin) );
        // Invariant: powers on stack must be increasing from bottom to top
        while (stack[top].power > runA.power) {
            auto top_run = stack[top--]; // pop
            merge_runs_basic(top_run.begin, runA.begin, runA.end, _buffer.begin());
            runA.begin = top_run.begin;
        }
        // store updated runA to be merged with runB at power k
        stack[++top] = {runA.begin, runA.power}; // push
        runA = {runB.begin, runB.end, 0};
    }
    assert(runA.end == end);
    while (top > 0) {
        auto top_run = stack[top--]; // pop
        merge_runs_basic(top_run.begin, runA.begin, end, _buffer.begin());
        runA.begin = top_run.begin;
    }
}