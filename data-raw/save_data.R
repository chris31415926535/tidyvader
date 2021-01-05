source('~/R/tidyvader/data-raw/dict_sentiments_sorted.R', encoding = 'UTF-8')
source('~/R/tidyvader/data-raw/dict_negate.R')
source('~/R/tidyvader/data-raw/dict_modifiers.R')
source('~/R/tidyvader/data-raw/vader_tests.R')


vader_dictionaries <- tibble::tribble(~name, ~dictionary,
                                      "dict_sent_sorted", dict_sent_sorted,
                                      "dict_punct_sorted", dict_punct_sorted,
                                      "dict_negate", dict_negate,
                                      "dict_modifiers", dict_modifiers,
                                      "mod_bigrams", mod_bigrams,
                                      "vader_tests", vader_tests
                                      )

usethis::use_data(mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value, dict_punct_word, dict_punct_value, vader_dictionaries, internal = TRUE, overwrite = TRUE)
