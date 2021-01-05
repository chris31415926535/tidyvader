test_that("multiplication works", {
  expect_equal(2 * 2, 4)
})


test_that("string splitting works",{
  # basic string split
  expect_equal(str_split_space("hi there"), c("hi", "there"))

  # ignores leading whitespace
  expect_equal(str_split_space("   hi there"), c("hi", "there"))

  # skips multiple spaces between words
  expect_equal(str_split_space("hi  there"), c("hi", "there"))

  # trims trailing spaces
  expect_equal(str_split_space("hi there   "), c("hi", "there"))

  # does all of these at once
  expect_equal(str_split_space(   "hi   there   buddy   "), c("hi","there","buddy"))
})


test_that("character counting works",{
  # counting one instance
  expect_equal(char_count("hi there!", "!"), 1)

  #counting many instances
  expect_equal(char_count("!!hi there!!","!"), 4)
})

test_that("removing punctuation works", {
  # get same input if no punctuation
  expect_equal(remove_punc("hello"), "hello")

  # removes punc from length-1 vector
  expect_equal(remove_punc("!hello!"), "hello")

  # removes punc from length-2 vector
  expect_equal(remove_punc(c("!h.i!", "!th!e??><><><><><,.re!" )),
               c("hi", "there"))
})

test_that("removing leading and trailing punctuation works",{
  # removes leading punc
  expect_equal(remove_punc_around("!hello"), "hello")

  # removes trailing punc
  expect_equal(remove_punc_around("hello!"), "hello")

  # leaves punc with letters around it
  expect_equal(remove_punc_around("he'llo"), "he'llo")

  # does all of these things at once
  expect_equal(remove_punc_around("!!he'llo!!"), "he'llo")

  # works for a vector
  expect_equal(remove_punc_around(c("!hello", "hello!", "!hello!", "!he'llo!")),
                                  c("hello",  "hello",  "hello",   "he'llo"))
  })

# test_that("match official vader tests",{
# #  expect_equal(vadercpp(vader_tests$text[[1]], mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value, dict_punct_word, dict_punct_value)[[1]] %>% round(digits = 4),
# #               vader_tests$compound[[1]])
#   expect_equal(vader_tests %>% dplyr::select(text) %>% vader(text) %>% dplyr::select(pos, compound, neu, neg),
#   vader_tests %>% dplyr::select(-text))
#
#   purrr::map_df(vader_tests$text, vaderchr) %>% tibble::as_tibble()
#   dplyr::select(vader_tests , compound, pos, neu, neg)
#   //vaderchr(vader_tests$text[1])
#   //dplyr::select(vader_tests[1,], compound, pos, neu, neg) %>% unlist()
# })
#
# ## testing the vadercpp function as i build it
# source('~/R/tidyvader/data-raw/dict_sentiments_sorted.R', encoding = 'UTF-8')
# source('~/R/tidyvader/data-raw/dict_negate.R')
# source('~/R/tidyvader/data-raw/dict_modifiers.R')
# source('~/R/tidyvader/data-raw/vader_tests.R')
# mod_bigrams <- c("kind of", "sort of")
#
# text <- "happy happy happy but sad sad sad"# and that's a fact whether you like it or not!!!!!!!??????"
# text <- "a A AA AAA AAAA"
# text <- "i am SO happy!"
# text <- "hey dude you are looking GREAT today!"
# text <- "hello good day good bad, GOOD!"
#
# ## FIXME TODO calculating pos/neg/neu when there are emojis or bigrams
# ## FIXME TODO test sentence #13:
# vadercpp(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value, dict_punct_word, dict_punct_value ) %>% round(digits=3)
#
# vader::get_vader(text)[c("compound","pos","neu","neg")] %>% as.numeric()
#
#
# bench::mark(vadercpp(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value , dict_punct_word, dict_punct_value))
# bench::mark(vader::get_vader(text))
#
# text <- rep("I am a happy dude", 1000)
# b <- bench::mark(purrr::map(text, vadercpp, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value , dict_punct_word, dict_punct_value))
#
#
#
# #### testing cpp wrapper
# text <- rep("I am a happy dude!!", 100000)
# testdf(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value , dict_punct_word, dict_punct_value)
#
# #### R WRAPPER WORKS
#
#
# text <- "lol"
# texttib <- tibble::tibble(text = "happy dude")
# vaderchr(text)
# tidyvader::vader(texttib, text)
#
# vader::get_vader(text)
#
# vader_tests %>%
#   dplyr::select(text) %>%
#   vader(text) %>%
#   dplyr::select(text, pos, compound, neu, neg)
#
# vader_tests
# ###########
# tests <- vader_tests$text
# testwrapper(tests) %>%
#   dplyr::bind_rows()
