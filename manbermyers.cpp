/**
 * @file ManberMyers.cpp
 * @author: Richik Vivek Sen (rsen9@gatech.edu)
 * @date 10/28/2019
 * @brief Implementation of the Manber-Myers algorithm.
 */

#include "manbermyers.h"
#include <tuple>
using namespace std;

namespace {
  /* Type representing a string formed from a pair of strings, along with an
   * initial index. These represent the 2^k-suffixes and are radix sorted in
   * each pass. We need to keep the index at which this suffix occurs for later
   * use so that we can map from a particular 2^k-suffix to its initial
   * position.
   */
  struct Suffix {
    tuple<size_t, size_t> entry;
    size_t index;
  };

  /* Sorts the given list of pairs via their Index'th component using counting
   * sort. To avoid reallocating memory unnecessarily, the client is expected
   * to provide a set of buckets.
   */
  template <size_t Index>
  void countingSort(vector<Suffix>& strings,
                    vector<vector<Suffix>>& buckets) {
    /* Distribute entries into buckets. */
    for (const auto& str: strings) {
      buckets[get<Index>(str.entry)].push_back(str);
    }

    /* Gather them all back in order. */
    size_t i = 0;
    for (auto& bucket: buckets) {
      for (const auto& entry: bucket) {
        strings[i] = entry;
        i++;
      }
      bucket.clear(); // Since we'll reuse these buckets later.
    }
  }

  /* Given a list of 2^k-suffixes, sorts those suffixes into ascending order
   * using radix sort.
   */
  void radixSort(vector<Suffix>& strings) {
    /* Buckets into which the elements will be dropped. We need no more buckets
     * than suffixes, since each suffix's entries are ranks of previous
     * suffixes and the number of suffixes never changes.
     */
    vector<vector<Suffix>> buckets(strings.size());

    countingSort<1>(strings, buckets);
    countingSort<0>(strings, buckets);
  }
}

/* Constructs a suffix array for the given text using the Manber-Myers
 * algorithm.
 */
SuffixArray manberMyers(const vector<size_t>& text) {
  SuffixArray result = text;

  /* Keep building strings of longer and longer widths until the strings exceed
   * the length of the string.
   */
  for (size_t halfSize = 1; halfSize < result.size(); halfSize *= 2) {
    /* Form the pairs that will be used for the radix sorting. */
    vector<Suffix> strings;
    for (size_t i = 0; i < result.size(); i++) {
      /* Each suffix is represented by a pair of ranks corresponding to its two
       * halves. The first half corresponds to the 2^k-suffix starting at the
       * current position (whose rank is result[i]), and the second half is
       * given by the rank of the suffix 2^k steps forward. However, that might
       * be at an index past the end of the string. In that case, we pretend
       * that the suffix ends with the sentinel, which always has rank 0.
       */
      strings.push_back({ make_tuple(result[i], (i + halfSize < result.size()? result[i + halfSize] : 0)), i });
    }

    /* Radix sort everything to get back the new ordering. */
    radixSort(strings);

    /* Map the sorted list back into the suffix array. Remember to collapse together
     * adjacent items that are identical to one another.
     */
    size_t index = 0;
    for (size_t i = 0; i < strings.size(); i++) {
      result[strings[i].index] = index;

      /* If the next block doesn't perfectly match us, make sure to assign it
       * the next index.
       */
      if (i != strings.size() - 1 && strings[i].entry != strings[i+1].entry) {
          index++;
      }
    }
  }

  /* We now have a table associating each element with its sorted position, but we
   * want the inverse of this (an array associating each sorted position with its
   * element).
   */
  SuffixArray sa(result.size());
  for (size_t i = 0; i < sa.size(); i++) {
    sa[result[i]] = i;
  }

  return sa;
}
