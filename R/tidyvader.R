#' @useDynLib tidyvader, .registration = TRUE
#' @importFrom Rcpp sourceCpp
#' @importFrom Rcpp evalCpp
NULL


#' Tidy VADER
#'
#' @param data A dataframe or tibble with a column of sentences to score.
#' @param var The name of the column containing the sentences.
#'
#' @return The original input with four additional columns for each sentence's VADER scores: compound, pos, neu, neg.
#' @export
vader <- function(data, var){
  # make sure we get a data.frame
  if (!"data.frame" %in% class(data)) stop("Input data must be in a dataframe or tibble.")

  # make sure input has at least one row
  if (nrow(data) == 0) stop ("Input dataframe has no rows.")

  # extract the column
  # if it doesn't exist, dplyr::pull will throw an error. potential to provide a more informative error here

  text <- dplyr::pull(data, {{var}})

  # make sure the column is a character vector
  if (!typeof(text) == "character") stop("Input column does not contain character data.")

  results <- vaderdf(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value , dict_punct_word, dict_punct_value)

  results <- dplyr::bind_cols(data, results)

  return (results)

}

#'  VADER for a character vector
#'
#' @param text A sentence to be scored, in a single string (i.e. a non-empty length-1 character vector).
#'
#' @return A one-row data.frame with four columns for the sentence's VADER scores: compound, pos, neu, and neg.
#' @export
vader_chr <- function(text){
  if( !(typeof(text) == "character") | !(length(text) == 1) | (strlen(text) == 0)) stop ("vader_chr() works only on non-empty length-1 character vectors (i.e. single strings). For longer vectors consider using it with purrr::map, and for text in dataframes or tibbles consider using tidyvader::vader().")
  results <- vaderdf(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value , dict_punct_word, dict_punct_value)

  return (results)

}

#' Get VADER dictionaries in a nested tibble.
#'
#' This function returns a nested tibble containing all of the dictionaries used
#' to calculate VADER scores and canonical test sentences, so that users can
#' inspect them and understand what's happening under the hood. At present
#' end-users have read-only access to dictionaries; there is no way to add
#' custom dictionaries or make changes.
#'
#' @return A tibble containing all VADER dictionaries and canonical test
#'   sentences.
#' @export
get_vader_dictionaries <- function(){

  return (tibble::as_tibble(vader_dictionaries))

}


#'
#' #' Get VADER score for a length-1 character vector (i.e. a sentence)
#' #'
#' #' @param text
#' #' @param word_results
#' #'
#' #' @return
#' vader_rcpp <- function(text, word_results = FALSE){
#'
#'   # currently: replace 2-gram boosters (there are only 3) with 1-grams;
#'   # then split based on " " ( ~15us faster than splitting based on regex "\\s" and should be fine for these use cases);
#'   # then convert to vector.
#'   text_split <- text %>%
#'     gsub(x = ., pattern = "kind of|sort of|just enough", replacement = "kindof") %>%
#'     stringi::stri_split_fixed(pattern = " ", simplify = TRUE) %>%
#'     as.vector()
#'
#'   # trim leading and trailing punctuation
#'   # TO CONSIDER: insteatry doing split, then word scores, then remove punct, then word scores again, then take whichever has a score
#'   # RATIONALE: removing leading and trailing punctuation is expensive
#'   text_nopunc_lower <- gsub(x = text_split, pattern = "((?<=[[:alpha:]])[[:punct:]]*$|^[[:punct:]]*(?=[[:alpha:]]))", replacement = "", perl = TRUE) %>%
#'     tolower()
#'
#'   # indexing with c++ binary search function get_index2()
#'   # for each word in the input text, finds its index in the sentiment dictionary (returns NA if it's not there)
#'   # then uses this set of indices to index the values vector.
#'   # this is the fastest way I've found so far: from my experiments, this < indexing in R < left-join < for loop
#'   word_scores <- c(0,0,0, dict_sent_value[get_index2(text_nopunc_lower, dict_sent_word)], 0,0,0)
#'   word_scores[is.na(word_scores)] <- 0
#'
#'   # set up word negations
#'   # haven't rewritten these to use the c++ binary search: not sure how much faster it would be?
#'   # these ditionaries are very small so it may not be worth the effort.
#'   # this is an example of straightforward indexing in R, which took ~100us to do for the much larger full VADER dictionary.
#'   # here it takes like ~3us
#'   word_negations <- c(0,0,0, negate_dict[text_nopunc_lower], 0,0,0)
#'   word_negations[is.na(word_negations)] <- 0
#'
#'
#'   # set up word boosters
#'   word_boosters <- c(0,0,0,booster_dict[text_nopunc_lower],0,0,0)
#'   word_boosters[is.na(word_boosters)] <- 0
#'
#'   # set up caps detection: similarly, maybe this can be more efficient
#'   # a logical vector of length n + 6 where TRUE if that word is capped for emphasis
#'   # in practice we check to see if there are two adjacent caps and two adjacent lower-case letters, then
#'   # look for words with 2 or more caps, taking this as a proxy for all caps.
#'   if (grepl(x = text, pattern = "[A-Z]{2,}") & grepl(x=text, pattern = "[a-z]{2,}")) {
#'     #message("caps diff")
#'     word_caps <- c(FALSE, FALSE, FALSE, grepl(x = text_split, pattern = "[A-Z]{2,}"), FALSE, FALSE, FALSE)
#'   } else {word_caps <- rep(FALSE, length(text_split) + 6)}
#'
#'   ########################################
#'   # PUNCTUATION EFFECTS
#'
#'   # EXCLAMATION POINTS
#'   # count frequency of ! in text and assign appropriate scaling
#'   # each exclamation point (up to a maximum of 4) gives 0.292
#'   amp_p <- min(str_count(text, "!"), 4) * 0.292
#'
#'   # QUESTION MARKS
#'   # count frequency of ? in text and assign appropriate scaling
#'   qm_count <- str_count(text, "\\?")
#'   amp_qm <- 0
#'   if(qm_count > 1) {
#'     if(qm_count <= 3) {amp_qm <- qm_count * 0.18}
#'     else(amp_qm <- 0.96)
#'   }
#'
#'   # combine exclamation and question mark factors
#'   punc_amp <- amp_p + amp_qm
#'   ############################################
#'
#'   ############################################
#'   # FIND IF THERE IS A "BUT" and where it is.
#'   # if there isn't a "but", we set first_but to integer 0.
#'
#'   first_but <- which(text_nopunc_lower == "but") + 2
#'   if (length(first_but) == 0) first_but <- 0L
#'
#'   ############################################
#'   # CPP LOOP THROUGH TEXT TO GET WORD SCORES
#'   word_scores_mod <- vaderC(word_scores, word_negations, word_boosters, word_caps, first_but)
#'
#'   ############################################
#'   # COMPOUND SCORE
#'   sum_scores <- sum (word_scores_mod, na.rm = TRUE)
#'
#'   # Adjust for punctuation: make it either more positive or more negative
#'   if(sum_scores > 0) {sum_scores <- sum_scores + punc_amp
#'   } else if(sum_scores < 0) {sum_scores <- sum_scores - punc_amp}
#'
#'
#'   ##############
#'   # NORMALIZE SCORE
#'   # Normalize the score to be between -1 and 1 using an alpha that approximates the max expected value
#'   alpha <- 15
#'   compound <- sum_scores / sqrt((sum_scores * sum_scores) + alpha)
#'   if(compound < (-1.0)) {compound <- (-1.0)
#'   } else if(compound > 1.0) {compound <- (1.0)}
#'
#'   #####################
#'
#'   #return (list("word_scores" = (word_scores_mod[4:(length(word_scores_mod)-3)]), "compound" = compound))
#'
#'   results <- round(compound, digits = 4)
#'
#'   # if (!word_results) results <- tibble(compound = round(compound, digits = 4))
#'   #
#'   # if (word_results) results <-  tibble(compound = round(compound, digits = 4),
#'   #                                   word_scores = list(tibble(word = text_split, score = (word_scores_mod[4:(length(word_scores_mod)-3)]))))
#'   return (results)
#' }
