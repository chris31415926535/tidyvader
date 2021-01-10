
<!-- README.md is generated from README.Rmd. Please edit that file -->

# tidyvader

<!-- badges: start -->

<!-- badges: end -->

A fast, clear, and tidy implementation of the rule-based sentiment
analysis algorithm VADER (Valence Aware Dictionary and Sentiment
Reasoner).

  - **It’s fast** because the code has been re-written from scratch and
    uses C++ for the core algorithm: in benchmarking it’s over 500 times
    faster than the competing R package.
  - **It’s clear** because it tries to make the rule-based algorithm and
    the dictionaries simple to read so that users can inspect and judge
    them.
  - **It’s tidy** because it tries to follow tidy design principles and
    works well with the %\>% pipe.

## Under Development

*Please note* that this package (and this documentation) is under active
development. At present it’s pretty well tested and functional, but this
is not yet on CRAN and things may change. Expect more/better
documentation as soon as time allows.

## Installation

You can install the development version from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("chris31415926535/tidyvader")
```

## What is VADER?

## Example

This example shows how to send sentences in a dataframe through
`vader()`. It also shows how punctuation, capitalization, and emojis all
work together to affect a sentence’s compound score.

``` r
library(tidyvader)
library(tibble)
library(magrittr)

# set up a tibble with some sentences
texts <- tibble(sentences = c("I feel happy today.",
                              "I feel happy today!",
                              "I feel HAPPY today!",
                              "I feel HAPPY today! :)",
                              "But somtimes I feel sad :("))

# pipe the data to tidyvader::vader() and specify the column with text 
texts %>%
  tidyvader::vader(sentences) %>%
  knitr::kable()
```

| sentences                  | compound |    pos |    neu |    neg |
| :------------------------- | -------: | -----: | -----: | -----: |
| I feel happy today.        |   0.5719 | 0.5522 | 0.4478 | 0.0000 |
| I feel happy today\!       |   0.6114 | 0.5709 | 0.4291 | 0.0000 |
| I feel HAPPY today\!       |   0.6932 | 0.6117 | 0.3883 | 0.0000 |
| I feel HAPPY today\! :)    |   0.8283 | 0.7203 | 0.2797 | 0.0000 |
| But somtimes I feel sad :( | \-0.8402 | 0.0000 | 0.3333 | 0.6667 |
