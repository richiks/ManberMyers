/**
 * @headerfile ManberMyers.cpp
 * @author: Richik Vivek Sen (rsen9@gatech.edu)
 * @date 10/28/2019
 * @brief Header file for manbermyers.cpp.
 */


#ifndef MANBERMYERS_H
#define MANBERMYERS_H

#include <vector>
#include <cstddef> // For size_t

/**
 * Type: SuffixArray
 *
 * A type representing a suffix array.
 */
using SuffixArray = std::vector<std::size_t>;

/* Constructs the suffix array of the given string in time O(m log m) using
 * the Manber-Myers algorithm described in the original paper on suffix trees.
 *
 * This implementation adopts the custom that the input string is an array of
 * integers representing the relative ranks of the characters in the original
 * string. It's assumed that the original string was suffixed by a special
 * sentinel character $ that appears nowhere else in the string. For example,
 * if the true string to build a suffix array for is ABRACADABRA, we'd begin by
 * suffixing a $ terminator to get ABRACADABRA$, then replace each character
 * with its rank ($ = 0, A = 1, B = 2, C = 3, D = 4, R = 5) to get 125131412510
 * as our input.
 */
SuffixArray manberMyers(const std::vector<std::size_t>& text);

#endif // MANBERMYERS_H
