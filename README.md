# CPP Module 09

*This project has been created as part of the 42 curriculum.*

## Description

The final C++ module: **STL containers in anger**. Each exercise must use a
*different* container, and the choice has to be justified — the module is
less about syntax than about picking the right data structure and paying
attention to complexity.

Everything is written to the **C++98** standard.

## Exercises

| Exercise | Name | Summary |
| --- | --- | --- |
| `ex00` | Bitcoin Exchange | Loads a CSV price history into a `std::map`, then for each line of an input file finds the value on that date — or the closest earlier date — and multiplies it out. Validates dates, ranges and malformed lines. |
| `ex01` | RPN | Evaluates a reverse Polish notation expression given as a single argument, using a stack and reporting an error on malformed input or division by zero. |
| `ex02` | PmergeMe | Implements the Ford–Johnson merge-insertion sort on two different containers and times both, printing the sorted sequence and the duration of each run. |

## Build & run

```bash
cd ex00 && make && ./btc input.txt
cd ex01 && make && ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
cd ex02 && make && ./PmergeMe $(shuf -i 1-100000 -n 3000 | tr '\n' ' ')
```

## Project structure

```
CPP_Module_09/
├── ex00/  BitcoinExchange.{hpp,cpp}, main.cpp, data.csv, input.txt, Makefile
├── ex01/  RPN.{hpp,cpp}, main.cpp, Makefile
└── ex02/  PmergeMe.{hpp,cpp}, main.cpp, Makefile
```

## Container choices

| Exercise | Container | Why |
| --- | --- | --- |
| `ex00` | `std::map` | Keys are dates and lookups need the *closest earlier* entry — `lower_bound` on an ordered map gives that directly. |
| `ex01` | `std::stack` | RPN evaluation is the textbook stack algorithm: push operands, pop two on an operator, push the result. |
| `ex02` | `std::vector` and `std::deque` | The subject requires two containers so the same algorithm can be timed against different memory layouts. |

## Notes

`ex00` treats `data.csv` as the reference series and `input.txt` as the
queries; a date earlier than the whole series is an error, while any other
missing date falls back to the most recent earlier price.

`PmergeMe` is worth the trouble because Ford–Johnson minimises the *number
of comparisons*, not the running time — the timing output usually shows the
plain `std::sort` baseline is faster in wall-clock terms, which is the
lesson rather than a bug.
