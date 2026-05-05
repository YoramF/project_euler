/*
 * euler14.c
 *
 *  Created on: Jan 21, 2019
 *      Author: yoram
 * 
 The following iterative sequence is defined for the set of positive integers:
  n -> n/2 (n is even)
  n -> 3*n+1 (n is odd)

  Using the rule above and starting with, we generate the following sequence:
	13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1

 it can be seen that this sequence (starting at 13 and finishing at 1) contains 10
 terms. Although it has not been proved yet (Collatz Problem), it is thought that all starting numbers finish at 1.

 Which starting number, under one million, produces the longest chain?

 NOTE: Once the chain starts the terms are allowed to go above one million.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	int n, m, i, li = 0, ln;
	long long tn;				// ensure highst integer value possible
	int opt, p = 0, nl = 0;
	long long *starts;			// array of known chain length for given start value
	unsigned int m_starts, l_starts, c_i; // to avoid negative values when convert from long long

	// get commad line input
	if (argc > 1)
	{
		while ((opt = getopt(argc, argv, "m:pn")) > 0)
			switch (opt)
			{
			case 'm':
				m = atoi(optarg);
				m_starts = 4*m;
				break;
			case 'p':
				p = 1;
				break;
			case 'n':
				nl = 1;
				break;
			default:
				printf("wrong input\n");
				return 1;
			}
	}
	else
	{
		printf("usage: euler14 MaxNum <p> <n> (p = print, n = no Loop)\n");
		return 1;
	}

	// allocate starts array
	if ((starts = calloc(m_starts, sizeof(long long))) == NULL) {
		fprintf(stderr, "failed to allocstr memory\n");
		return 2;
	}

	if (nl)
		n = m;
	else
		n = 2;

	for (; n <= m; n++)
	{
		i = 0;
		tn = n;
		l_starts = 0;

		if (p)
			printf("starting number: %lld\n", tn);

		while (tn > 1)
		{
			if (tn <= m_starts && starts[tn] != 0) {
				i += starts[tn];
				tn = 1;
				break;
			}
			else {
				i++;

				if (p)
					printf ("[%d], %lld\n", i, tn);

				if (tn % 2 == 0)
					tn /= 2;
				else
					tn = 3*tn + 1;

				if (l_starts < tn && tn <= m_starts) {
					l_starts = tn;
					c_i = i;
				}

			}
		}

		if (starts[n] == 0)
			starts[n] = i;
		if (starts[l_starts] == 0)
			starts[l_starts] = i - c_i;

		i++; // add count for value "1"
		if (p)
			printf ("[%d], %lld\n", i, tn);

		if (i > li)
		{
			li = i;
			ln = n;
			printf("Length: %d, starting number: %d\n", li, ln);
		}

	}

	printf("Largest list: %d, starting number: %d\n", li, ln);

	return 0;
}


