/** @author Sebastian Wild (wild@liverpool.ac.uk) */

#ifndef MERGESORTS_MERGING_H
#define MERGESORTS_MERGING_H

#include <algorithm>


#ifdef COUNT_MERGECOST
	const bool COUNT_MERGE_COSTS = true;
#else
	const bool COUNT_MERGE_COSTS = false;
#endif
	long long volatile totalMergeCosts = 0;
	long long volatile totalBufferCosts = 0;


	/**
	 * Merges runs A[l..m) and A[m..r) in-place into A[l..r)
	 * by copying both to buffer B and merging back into A.
	 * B must have space at least r-l.
	 */
	template<typename Iter, typename Iter2>
	void merge_runs_basic(Iter l, Iter m, Iter r, Iter2 B) {
		auto n1 = m-l, n2 = r-m;
		if (COUNT_MERGE_COSTS) totalMergeCosts += (n1+n2);
        std::copy(l,r,B);
        if (COUNT_MERGE_COSTS) totalBufferCosts += (n1+n2);
        auto c1 = B, e1 = B + n1, c2 = e1, e2 = e1 + n2;
        auto o = l;
        while (c1 < e1 && c2 < e2)
            *o++ = *c1 <= *c2 ? *c1++ : *c2++;
        while (c1 < e1) *o++ = *c1++;
        while (c2 < e2) *o++ = *c2++;
	}

#ifdef USE_OLD_RUN_DETECTION_LOOPS_WITH_IF_IN_BODY
/** returns maximal i <= end s.t. [begin,i) is weakly increasing */
	template<typename Iterator>
	Iterator weaklyIncreasingPrefix(Iterator begin, Iterator end) {
		while (begin + 1 < end)
			if (*begin <= *(begin + 1)) ++begin;
			else break;
		return begin + 1;
	}

	/** returns minimal i >= begin s.t. [i, end) is weakly increasing */
	template<typename Iterator>
	Iterator weaklyIncreasingSuffix(Iterator begin, Iterator end) {
		while (end - 1 > begin)
			if (*(end - 2) <= *(end - 1)) --end;
			else break;
		return end - 1;
	}

	template<typename Iterator>
	Iterator strictlyDecreasingPrefix(Iterator begin, Iterator end) {
		while (begin + 1 < end)
			if (*begin > *(begin + 1)) ++begin;
			else break;
		return begin + 1;
	}

	template<typename Iterator>
	Iterator strictlyDecreasingSuffix(Iterator begin, Iterator end) {
		while (end - 1 > begin)
			if (*(end - 2) > *(end - 1)) --end;
			else break;
		return end - 1;
	}
#else
	/** returns maximal i <= end s.t. [begin,i) is weakly increasing */
	template<typename Iterator>
	Iterator weaklyIncreasingPrefix(Iterator begin, Iterator end) {
		while (begin + 1 < end && *begin <= *(begin + 1)) 
		{++begin;
		std::cout << *begin << " ";}
		std::cout << "Fim da run " << "\n";
		return begin + 1;
	}

	/** returns minimal i >= begin s.t. [i, end) is weakly increasing */
	template<typename Iterator>
	Iterator weaklyIncreasingSuffix(Iterator begin, Iterator end) {
		while (end - 1 > begin && *(end - 2) <= *(end - 1)) --end;
		return end - 1;
	}

	template<typename Iterator>
	Iterator strictlyDecreasingPrefix(Iterator begin, Iterator end) {
		while (begin + 1 < end &&  *begin > *(begin + 1)) 
		{++begin;
		std::cout << *begin << " ";}
		std::cout << "Fim da run " << "\n";
		return begin + 1;
	}

	template<typename Iterator>
	Iterator strictlyDecreasingSuffix(Iterator begin, Iterator end) {
		while (end - 1 > begin && *(end - 2) > *(end - 1)) --end;
		return end - 1;
	}
#endif // USE_OLD_RUN_DETECTION_LOOPS_WITH_IF_IN_BODY

	template<typename Iterator>
	Iterator extend_and_reverse_run_right(Iterator begin, Iterator end) {
		Iterator j = begin;
		std::cout << "Run: " << *begin << " ";
		if (j == end) return j;
		if (j+1 == end) return j+1;
		if (*j > *(j+1)) {
			j = strictlyDecreasingPrefix(begin, end);
			std::reverse(begin, j);
		} else {
			j = weaklyIncreasingPrefix(begin, end);
		}
		return j;
	}

#endif //MERGESORTS_MERGING_H
