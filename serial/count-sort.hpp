#include <vector>
#include <utility>

#ifndef __COUNT_SORT_hpp__
#define __COUNT_SORT_hpp__

/* sorts by .second */
std::vector<std::pair<int, int>>
counting_sort(const std::vector<std::pair<int, int>> &inp);

#endif // __COUNT_SORT_hpp__

