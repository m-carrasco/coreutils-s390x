
/* factor -- print prime factors of n.
   Copyright (C) 1986-2021 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Originally written by Paul Rubin <phr@ocf.berkeley.edu>.
   Adapted for GNU, fixed to factor UINT_MAX by Jim Meyering.
   Arbitrary-precision code adapted by James Youngman from Torbjörn
   Granlund's factorize.c, from GNU MP version 4.2.2.
   In 2012, the core was rewritten by Torbjörn Granlund and Niels Möller.
   Contains code from GNU MP.  */

/* Efficiently factor numbers that fit in one or two words (word = uintmax_t),
   or, with GMP, numbers of any size.

  Code organisation:

    There are several variants of many functions, for handling one word, two
    words, and GMP's mpz_t type.  If the one-word variant is called foo, the
    two-word variant will be foo2, and the one for mpz_t will be mp_foo.  In
    some cases, the plain function variants will handle both one-word and
    two-word numbers, evidenced by function arguments.

    The factoring code for two words will fall into the code for one word when
    progress allows that.

  Algorithm:

    (1) Perform trial division using a small primes table, but without hardware
        division since the primes table store inverses modulo the word base.
        (The GMP variant of this code doesn't make use of the precomputed
        inverses, but instead relies on GMP for fast divisibility testing.)
    (2) Check the nature of any non-factored part using Miller-Rabin for
        detecting composites, and Lucas for detecting primes.
    (3) Factor any remaining composite part using the Pollard-Brent rho
        algorithm or if USE_SQUFOF is defined to 1, try that first.
        Status of found factors are checked again using Miller-Rabin and Lucas.

    We prefer using Hensel norm in the divisions, not the more familiar
    Euclidian norm, since the former leads to much faster code.  In the
    Pollard-Brent rho code and the prime testing code, we use Montgomery's
    trick of multiplying all n-residues by the word base, allowing cheap Hensel
    reductions mod n.

    The GMP code uses an algorithm that can be considerably slower;
    for example, on a circa-2017 Intel Xeon Silver 4116, factoring
    2^{127}-3 takes about 50 ms with the two-word algorithm but would
    take about 750 ms with the GMP code.

  Improvements:

    * Use modular inverses also for exact division in the Lucas code, and
      elsewhere.  A problem is to locate the inverses not from an index, but
      from a prime.  We might instead compute the inverse on-the-fly.

    * Tune trial division table size (not forgetting that this is a standalone
      program where the table will be read from secondary storage for
      each invocation).

    * Implement less naive powm, using k-ary exponentiation for k = 3 or
      perhaps k = 4.

    * Try to speed trial division code for single uintmax_t numbers, i.e., the
      code using DIVBLOCK.  It currently runs at 2 cycles per prime (Intel SBR,
      IBR), 3 cycles per prime (AMD Stars) and 5 cycles per prime (AMD BD) when
      using gcc 4.6 and 4.7.  Some software pipelining should help; 1, 2, and 4
      respectively cycles ought to be possible.

    * The redcify function could be vastly improved by using (plain Euclidian)
      pre-inversion (such as GMP's invert_limb) and udiv_qrnnd_preinv (from
      GMP's gmp-impl.h).  The redcify2 function could be vastly improved using
      similar methoods.  These functions currently dominate run time when using
      the -w option.
*/


/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "factor"

#define AUTHORS \
  proper_name ("Paul Rubin"),                                           \
  proper_name_utf8 ("Torbjorn Granlund", "Torbj\303\266rn Granlund"),   \
  proper_name_utf8 ("Niels Moller", "Niels M\303\266ller")

int
main (int argc, char **argv)
{
  return 0;
}