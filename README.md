# Kraken technical assessment: matching engine

## How to

### Build

This project was written on a Linux (NixOS) machine, using the `CMake` build
system.

To build the project you should run the following commands:

```sh
mkdir build
cmake -B build
cmake --build build
```

To run unit and integration tests you can use:

```sh
cmake --build build --target test
```

### Run

The `kraken` binary should be built in `build/src/kraken`. You can see example
inputs and their matching outputs in the `data/` directory at the root of the
repository.

`kraken` reads its input from standard input, and displays its results on
the standard output. For example

```sh
kraken < ./data/inputs/balanced-book-1.in.csv
```

## Architecture

### Libraries

The project is divided into small libraries in separate directories, each for
a specific purpose:

* `book`: defines the vocabulary types to quantify orders.
* `csv`: reading and writing CSV files, in a very naive way.
* `engine`: the matching engine proper, and a listener interface used to create
  the expected output.
* `parse`: parsing the raw CSV data into a list of orders.
* `utils`: utility types, specifically a `StrongType` wrapper to ensure a `User`
  is not mistaken for a `Quantity`.

### A KISS architecture

In each step of the project, the code was kept to its simplest, trying to solve
the problem at hand in the simplest way possible, while keeping to the single
responsibility principle. This is why for example:

* The input is parsed at once, and processed in a single step, by different
  components.
* Almost no efforts were made to avoid superfluous copies, or such
  optimizations.
* The engine and the expected output are separated from each other through a
  listener interface, instead of entangling both in the same class.

### A test harness

To allow for refactoring without fear, each library has a test-suite to ensure
it is in working order.

This allowed me to simplify cancelling orders from having to potentially look
at *all currently active orders* to just a few orders on a given price level.

### Reasonably extensible

Given the focus on "events" (the engine processes each order separately, calling
the listener at opportune times), it should be fairly simple to extend the core
of this code to allow for online processing (i.e: the engine reads its input and
displays its output as it comes along), etc...

### What I would improve

#### Matching trades

The logic used when trade matching is enabled is pretty limited: it wasn't clear
to me what to do when either of the orders have left-over quantities to be
fulfilled. More explicit instructions on this point would lead to the removal of
the final `FIXME`s in the code.

Related to that point, the matching functionality should be tested further,
rather than just using the two provided examples.

#### Cancelling orders

I do not like the way I have done the `cancel_reverse_info_` mapping: to
simplify I use a `CancelOrder` value as a key instead of creating an
`Engine`-specific type.

#### Repetition

I do not like the repetition that happens due to `asks_` and `bids_` being
"mirror" of each other in the way they should be handled. This is mitigated
somewhat by making use of helper lambda functions when the code is identical.

#### Top of the book handling

I feel like the `CallbackOnTopOfBookChange` is kind of a hack, even though it
was very effective to solve the problem I had of:

* I want to memorize the current top of the book
* At the very end of my current operation, calculate the top of the book again
* Then call the listener back to notify of a change if it happened.

I think there might be a smarter way to go about this. To (mis-)quote Blaise
Pascal:

> If I had more time, I would have made it shorter.

### Complexity analysis

This will focus on the matching engine code, let's discard the complexity of
the input pre-processing from this discussion.

Given the use of `std::{multi_,}map` types, the space analysis is pretty simple:
linear with respect to the number of active orders. Empty books are not removed
and would therefore also consume a small amount of space: I am not accounting
for this in this analysis.

Let's focus on the time complexity.

#### Flush orders

The simplest to process, we just empty all the book information, in a time
complexity linear in the number of active orders in the book.

#### Cancel order

The first version of this code would have a worst-case cost linear in the
number of active orders in the book, simply iterating through each one in turn.

Thanks to a reverse-mapping, the cancel cost is now the following:

* Lookup in `cancel_reverse_info_`: logarithmic with respect to the number of
  active orders across all instruments.
* Lookup in `bids_`/`asks_` for the book on a given `Symbol`: logarithmic with
  respect to the number of symbols.
* Finding the bounds on the price range: logarithmic with respect to the number
  of orders in the given book.
* Iterating through that range: linear with respect to the number of orders
  at the given price range.

#### Trade order

* Lookup on `bids_` and `asks_` for the given symbol: logarithmic to the number
  of symbols.
* Look for a cross of the book, ensure the book is not empty (constant time),
  and look at the first value in the book: logarithmic to the number of orders
  in the book
* Inserting the order in the book: logarithmic to the number of orders in the
  book.
* Inserting into `cancel_reverse_info_` (for faster cancelling): logarithmic
  to the number of orders across all instruments.

#### Top-of-book handling

For both trade orders and cancel orders, the `CallbackOnTopOfBookChange` does
the following:

* Lookup on `bids_` and `asks_` for the given symbol: logarithmic to the number
  of symbols.
* Check the size of the book (constant time) and look at the first order's
  price: logarithmic to the number of orders in the book.
* Find the price range: logarithmic yet again.
* Iterating on the range: linear to the number of orders at the given price.
