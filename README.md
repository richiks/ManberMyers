# ManberMyers
 * Implementation of the Manber-Myers algorithm.
 *
 * The Manber-Myers algorithm uses a combination of dynamic programming and
 * radix sort. The algorithm works by first sorting the length-1 suffixes
 * starting at each position in the string, then the length-2 suffixes, then
 * the length-4 suffixes, the length-8 suffixes, etc. until eventually sorting
 * the full suffixes starting at each position. (It's assumed that, if a suffix
 * falls off the end of the string, we just pretend it terminates earlier.)
 *
 * The key insight is that if the sorted order of the 2^k-suffixes (the
 * suffixes of lengths 2^k starting at each character) are in sorted order,
 * then the relative order of the (2^{k+1})-suffixes is much easier to compute
 * than if we had no information at all. As an example, let's look at the text
 * string DIKDIK$ and assume that we know the relative order of the 2-suffixes.
 * This means we know the relative ordering of DI, IK, KD, DI, IK, K$, and $.
 * Now, suppose we want to determine the relative ordering of the 4-suffixes,
 * which are DIKD, IKDI, KDIK, DIK$, IK$, K$, and $. Notice that, for example,
 * the 4-substring DIKD can be decomposed into two 2-suffixes, DI and KD. Since
 * we know the relative ordering of the 2-suffixes, if we're clever about how
 * we store those 2-suffixes, we know quite a lot about the structure of the
 * 4-suffix DIKD.
 *
 * To make the algorithm more concrete, assume that on entry to the stage of
 * the algorithm where we compute the relative order of the 4-suffixes that we
 * have an array saying, for each position i, what the rank of the 2-suffix is
 * starting at that position. The 2-suffixes here are DI, IK, DK, DI, IK, K$,
 * and $, and their relative ranks are shown here:
 *
 *   String:         D   I   K   D   I   K   $
 *   2-Suffix:       DI  IK  KD  DI  IK  K$  $
 *   Rank:           1   2   4   1   2   3   0
 *
 * Now, imagine, for example, that we want to compare the 4-suffixes DIKD and
 * DIK$. Rather than explicitly comparing their characters, we'll recognize
 * that DIKD and DIK$ can be split into (DI, KD) and (DI, K$). The relative
 * order of these suffixes can be determined by first comparing their first
 * components, and, since they're equal, then comparing the second components
 * as a tiebreaker. Going a step further, rather than comparing the components
 * of the 4-suffixes directly, we'll just compare their ranks, which we can
 * look up quickly. That is, instead of forming (DI, KD) and (DI, K$), we'd
 * form the strings (1, 4) and (1, 3) (replacing each 2-substring with its
 * rank), and then compare those pairs lexicographically.
 *
 * Because we can treat all 4-suffixes as pairs of ranks, we can sort all of
 * those suffixes using a least-significant-digit radix sort. Since there
 * can be at most Theta(m) different ranks and each pair has two digits, the
 * radix sorting step takes time Theta(m).
 *
 * At this point, we'll have all the 4-suffixes in sorted order. To make it
 * easier to then sort the 8-suffixes, we'll then compute the ranks of all the
 * 4-suffixes (since they're in sorted order, this just boils down to comparing
 * each against the next one) and will keep this information around for the
 * next pass.
 *
 * More generally, the algorithm works by running Theta(log m) rounds. At the
 * start of each round, we have the rank information for all the 2^k-suffixes,
 * and at the end of each round we have the rank information for all the
 * 2^{k+1}-suffixes, which we learn by radix sorting pairs corresponding to
 * those 2^{k+1}-suffixes. (We only need Theta(log m) rounds because, at that
 * point, the 2^k-suffixes will be the m-suffixes and we'll have everything in
 * sorted order. Each pass takes Theta(m) time for a total of O(m log m) total
 * time.
 
# DC3 algorithm
 * Implementation of the DC3 algorithm.
 *
 * DC3 is a recursive suffix array construction algorithm that works on general
 * integer alphabets. At a high level, the algorithm works as follows. If the
 * input string is sufficiently short, it computes the suffix array directly
 * (in our case, using Manber-Myers). Otherwise, the algorithm partitions the
 * suffixes into two groups: the T0 suffixes, whose positions are congruent to
 * zero modulo three, and the T12 suffixes, whose positions are congruent to
 * one or to two modulo three. It then obtains the sorted orders of those
 * suffixes, then merges them together using a merge step to form the overall
 * suffix array for the input string.
 *
 * The creative step in the algorithm is in determining the sorted order of the
 * T12 suffixes. Let's imagine, for example, that our input string is
 * MONSOONNOMNOMS$. The T12 suffixes here would then be
 *
 *     ONSOONNOMNOMS$
 *      NSOONNOMNOMS$
 *        OONNOMNOMS$
 *         ONNOMNOMS$
 *           NOMNOMS$
 *            OMNOMS$
 *              NOMS$
 *               OMS$
 *                 S$
 *                  $
 *
 * The algorithm first begins by forming two new strings. The string T1 is
 * formed by dropping the first character from the input string T and padding
 * the string with sentinels until its length is a multiple of three. The
 * string T2 is formed by dropping the first two characters and padding with
 * sentinels. Here, those strings are
 *
 *   T1: ONSOONNOMNOMS$$
 *   T2: NSOONNOMNOMS$$$
 *
 * These two strings are then concatenated:
 *
 *   ONSOONNOMNOMS$$NSOONNOMNOMS$$$
 *
 * There's a useful observation here that explains why this would be a useful
 * thing to do. Notice that the string can be cleanly split in half, and that
 * within the first half, every third character corresponds to a suffix at a
 * position that's congruent to one modulo three (since we dropped the first
 * character of the overall string T) and in the second half, every third
 * character corresponds to a suffix at a position that's congruent to two
 * modulo three (since we dropped the first two characters from T). This is
 * shown here, with stars indicating the suffixes in question.
 *
 *   *  *  *  *  *  *  *  *  *  *
 *   ONSOONNOMNOMS$$NSOONNOMNOMS$$$
 *
 * Now, imagine comparing any two starred suffixes within this string. The
 * claim is that the relative order of any two starred suffixes of this much
 * larger string matches the relative order of those two corresponding suffixes
 * from the original string T. The key insight is that the only way that we'd
 * get the relative ordering wrong is if the comparison "crossed over" from the
 * first half of the string into the second. But the only way this can happen
 * is if we have a suffix that crosses the sentinels in the middle of the
 * string without losing on tiebreaks. Since the sentinels only appear at the
 * very end of the string and the middle of the string, the only way this can
 * happen is if we have two suffixes that encounter sentinels at the same time.
 * But that can't happen, because the suffixes in the first half and second
 * half of the string start at positions that have different remainders modulo
 * three, meaning that one of them must arrive before the other.
 *
 * The net effect of this is that if we can get a suffix array for this new
 * string - or at least, just the starred positions - we know the sorted order
 * of the T12 suffixes. The problem is that we just made our string twice as
 * long as it used to be, which generally is something to be avoided.
 *
 * The creative insight in the DC3 algorithm is that we can reduce the size of
 * this string without losing its sorted order. After doubling the size of the
 * string, we break it apart into blocks of size three, like this:
 *
 *   ONS OON NOM NOM S$$ NSO ONN OMN OMS $$$
 *
 * Now, imagine we label each block with a number corresponding to its rank
 * among the blocks. That's shown here:
 *
 *   ONS OON NOM NOM S$$ NSO ONN OMN OMS $$$
 *    6   7   1   1   8   2   5   3   4   0
 *
 * The ranks of the blocks can be found in time O(m) by performing a radix sort
 * on the blocks, each of which is three characters long.
 *
 * The next insight here is that the sorted order of any suffix starting at a
 * position that's a multiple of three is given by the corresponding order of
 * the suffix of the string formed from the ranks of each blocks. To see why
 * this is, imagine comparing any two suffixes. Either (1) their first blocks
 * differ, in which case the sorted order depends purely on the ranks of those
 * blocks, or (2) those blocks are equal, in which case the sorted order then
 * depends on the next block (and so on). In other words, a lexicographical
 * comparison among characters corresponds to a lexicographical comparison 
 * among the blocks. And fortunately, the number of blocks is at most 2m / 3,
 * since we doubled the length of our input string (2m) and then grouped it
 * into blocks of size 3 (2m / 3).
 *
 * We can therefore get the sorted order of the T12 suffixes by making a
 * recursive call to the DC3 algorithm on the reduced string.
 *
 * Once we have the sorted order of the T12 suffixes - suffixes at positions
 * that are congruent to 1 or 2 mod 3 - we can use that to deduce the order of
 * the T0 suffixes, the suffixes whose positions are multiples of three. The
 * key insight here is that any suffix at a position that's congruent to zero
 * mod three can be thought of a single character - the character at that
 * position - followed by a T12 suffix - the suffix starting at the position
 * immediately after this. Since we know the ranks of all the T12 suffixes from
 * our last step, we can use a radix sort pass to sort the T0 suffixes, where
 * each suffix is thought of as a pair of a character and a T12 ranking.
 *
 * At this point we now have the T0 suffixes in sorted order and the T12
 * suffixes in sorted order. All that's left to do now is to merge those lists
 * together to get the overall suffix array!
 *
 * The good news here is that, by adapting the observation we used to radix
 * sort the T0 suffixes, we can compare any T0 suffix against any T12 suffix.
 * To see this, note that
 *
 * a T0 suffix consists of a pair of the form (char, T1 suffix) and
 * a T1 suffix consists of a pair of the form (char, T2 suffix), and since
 * we know the relative orders of the T1 and T2 suffixes, we can compare
 * these suffixes in constant time.
 *
 * a T0 suffix consists of a triple of the form (char, char, T2 suffix) and
 * a T2 suffix consists of a triple of the form (char, char, T1 suffix),
 * and we know the relative ordering of the T1 and T2 suffixes.
 *
 * This means that the final merge step can merge the lists with a constant
 * amount of work per comparison.
 *
 * Overall, the total work done by any recursive call is O(m), plus the cost of
 * a recursive call on an input of size 2m / 3. That gives the recurrence
 *
 *    T(m) = T(2m / 3) + O(m)
 *
 * which, via the Master Theorem, solves to O(m). It's a linear-time algorithm!
 *
 * This recursion needs to stop somewhere, so this implementation uses a
 * "stopper" algorithm to handle any sufficiently small input. We specifically
 * use the O(m log m)-time Manber-Myers algorithm, though any suffix array
 * construction algorithm would work here.
