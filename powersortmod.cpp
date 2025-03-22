
#include <cassert>
#include "insertionsort.h"
#include "merging.h"
#include <vector>
#include <iterator>
#include <iostream>

typedef unsigned int power_t;

// Node power bitwise modificado:
power_t node_power_mod(size_t begin, size_t end, size_t beginA, size_t beginB, size_t endB){
	size_t xor_result = beginA ^ beginB;
	int first_bit = __builtin_ctz(xor_result);
	power_t k = first_bit + 1;
    std::cout << "node power mod: ";
    std::cout << beginA << " " << beginB << " " << " " << endB << " " << k << "\n";
	return k; 
}
// Main sort:
void power_sort_paper2(Iterator begin, Iterator end) {
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
        // extend to minRunLen
        size_t lenB = runB.end - runB.begin;
        std::cout << "começo da run b: " << *runB.begin << " ";
        std::cout << "fim da run b: " << *(runB.end-1) << " ";
        runA.power = node_power_mod(0, n,
                                (size_t) (runA.begin-begin),
                                (size_t) (runB.begin-begin),
                                (size_t) (runB.end-begin) );
        // Invariant: powers on stack must be increasing from bottom to top
        while (stack[top].power >= runA.power) {
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