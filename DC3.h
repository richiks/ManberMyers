/**
 * @headerfile DC3.h
 * @author: Richik Vivek Sen (rsen9@gatech.edu)
 * @date 10/28/2019
 * @brief Header file for DC3.cpp.
 */

#ifndef DC3_H
#define DC3_H

#include "manbermyers.h"

/* Computes a suffix array using the DC3 algorithm. As with the Manber-Myers
 * algorithm, this implementation assumes that the input text string has been
 * suffixed with a sentinel and had each of its characters remapped to its rank
 * among all characters.
 */
SuffixArray dc3(const std::vector<std::size_t>& text);

#endif // DC3_H
