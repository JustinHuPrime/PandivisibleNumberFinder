# Pandivisible Number Finder

This is a program to find, using a multi-threaded iterative search derived from a generative tail-recursive search, solutions to the pandigital polydivisible number problem in an arbirary base.

## The Problem

This problem is from *Things to Make and Do in the Fourth Dimension* by Matt Parker, Chapter 1.

Given a natural number *b*, arrange the integers from 1 (included) to *b* (excluded) such that, for any *m* less than *b*, the first *m* numbers in that sequence, taken as the digits in a base *b* number, are divisible by *m*.

## Search Algorithm

For a fixed base *b*, we build up the number, digit by digit. Each building step may be considered its own task, and as such, benefits from a thread pool being used. For each step, we assume that the number without its last digit is divisible by the number of digits minus one. We check - is the number (as is) divisible by the number of digits? If not, we discard the number. Otherwise, we select either an odd or an even digit, as appropriate (See *An exploration of pandivisible numbers* by Travis Dillon, Parabola, Volume 53, Issue 1) and which has not been used, and return the task to the queue.
