#include <Rcpp.h>
using namespace Rcpp;

// Get character length of string
//
// Need this to check for zero-length inputs without relying on other packages
// @export
// [[Rcpp::export]]
int strlen(CharacterVector text){
  return std::string(text[0]).length();
}

// Remove punctuation from each element of CharacterVector
//
// got this algorithm from here: https://stackoverflow.com/questions/19138983/c-remove-punctuation-from-string
// @param text A CharacterVector.
//[[Rcpp::export]]
CharacterVector remove_punc(CharacterVector text){
  int n = text.length();
  CharacterVector out(n);


  for (int i = 0; i < n; ++i){
    std::string result;
    std::remove_copy_if(text[i].begin(), text[i].end(),
                        std::back_inserter(result), //Store output
                        std::ptr_fun<int, int>(&std::ispunct)
    );
    out[i] = std::string(result);
  }

  return out;

}

// Remove punctuation from beginning and end of CharacterVector
//
// removing all punctuation is overkill and introduces errors: "I'll" becomes "ill" which is negative
// @param text A CharacterVector.
//[[Rcpp::export]]
CharacterVector remove_punc_around(CharacterVector text){
  // number of strings we're dealing with
  int n = text.size();
  int l;
  CharacterVector out(n);
  std::string result;

  for (int i = 0; i < n; ++i) {

    result = std::string (text[i]);

    // remove leading punctuation
    // loop max as many characters as there are, and keep removing the 0th if it's punctuation
    l = result.length();

    for (int j = 0; j < l; ++j){
      if (!std::ispunct(result[0])) break;
      result.erase(0,1);
    }

    //remove trailing punctuation
    // loop max as many characters as there are, and keep removing the last if it's punctuation
    l = result.length();

    for (int j = l; j > 0; --j){
      if (!std::ispunct(result[j-1])) break;
      result.erase(j-1,1);
    }

    out[i] = result;
  }
  return out;

}


// search/replace in a string
//
// learned how here: https://www.cplusplus.com/reference/string/string/find/
//
// @param str A single string
//[[Rcpp::export]]
std::string replace_text(std::string str, std::string pattern, std::string replacement){

  // find the index of the first instance
  std::size_t found = str.find(pattern);

  // keep looping while we're not at the end of the string
  while (found != std::string::npos){
    // do the replacement
    str.replace(str.find(pattern),pattern.length(),replacement);

    // find the next one
    found = str.find(pattern);
  }
  return (str);
}

// split string at spaces
//
// @param x A single integer.
//[[Rcpp::export]]
CharacterVector str_split_space(std::string text){
  // length of input string
  int n = text.length();
  // output vector: will be at most n characters long
  CharacterVector out(n);
  // count the number of words we find
  int words = 0;
  // two counters for our place in the input string
  int l = 0;

  for (int i = 0; i < n; ++i){

    // case 0: First character is a space
    //         Move the left-hand pointer ahead by one (i.e. trim leading spaces)
    if (i == 0 && text[i] == ' '){
      ++l;
    }

    // Case 1: current non-first character is not a space:
    //         may not need to do anything? maybe we can set it up in the next case?
    if (i != 0 && text[i] != ' ') {}//Rprintf("character %d is not a space\n", i);}

      // Case 2: Current non-first character is a space, previous character is not a space
      //         we have found a word boundary. extract the word from l to i-1, update l, count the word
      if ((i != 0) && (text[i] == ' ')){
       // Rprintf("character %d IS a space\n", i);

        if (text[i-1] != ' ') {
         // Rprintf("** and character %d is NOT a space, so we have found a word boundary\n", (i-1));
        //  Rprintf("** this is word number %d \n", words);

          out[words] = text.substr(l, (i-l));
          l = i+1;
          ++words;
        }

        // Case 3: Current non-first character is a space, previous character is also a space
        //         just move the left-hand pointer ahead to trim the whitespace
        if (text[i-1] == ' ' ){
          ++l;

        }
      }

      // Case 4: end of the string and it's not a space. this must be a word
      //         don't do ++words so that the final indexing will be correct.
      if((i == (n-1)) && text[i] != ' '){
        //Rprintf("we are at the last character, this must be a word too");
        out[words] = text.substr(l, (i+1));
      }

      // Case 5: end of the string and it's a space. this is not a word. reduce word count by 1 so it works out.
      if ((i == (n-1) && text[i] == ' ')){
        --words;
      }

  }

  // index the output to only contain the words
  // thank god for this explanation https://stackoverflow.com/questions/47246200/how-to-slice-rcpp-numericvector-for-elements-2-to-101
  return (out[Rcpp::Range(0, words)]);
}


// Sort a character vector using c++'s algorithm
//
// Allows us to then use c++ string comparisons so we can do a binary search.
// R's internal sort is different, so we can't sort in R and search in c++.
//
// @param strings A vector of strings to be sorted.
// [[Rcpp::export]]
std::vector< std::string > cpp_str_sort( std::vector< std::string > strings ) {

  int len = strings.size();

  for( int i=0; i < len; i++ ) {
    std::sort( strings[i].begin(), strings[i].end() );
  }

  return strings;
}

// Count instances of a single character in a text string. Takes ~8us--about half of str_count, which takes ~ 18us
//
// @param text Input text string.
// @param to_count Character to count.
// [[Rcpp::export]]
int char_count(std::string text, std::string to_count) {
 // String text(Rtext[0]);
 // String to_count(Rto_count[0]);
  int count = 0;

  for (long long unsigned int i = 0; i < text.length(); ++i){
    if (text[i] == to_count[0]) ++count;
  }

  return (count);
}



// This is the core of this implementation of VADER.

// [[Rcpp::export]]
NumericVector vaderC(NumericVector word_scores, NumericVector word_negations, NumericVector word_boosters, LogicalVector word_caps, int first_but){
  int n = word_scores.size();
  NumericVector out(n);
  NumericVector sign(n);      // for the sign of each word, to make addition/subtraction easier. could maybe be just a single double?
  double N_SCALAR = -0.74;
  double C_INCR = 0.733;


  for (int i = 3; i < n; ++i){

    // update the boosters for caps. put it here so it checks each word.
    if (word_boosters[i] > 0) word_boosters[i] += (C_INCR * word_caps[i]);
    if (word_boosters[i] < 0) word_boosters[i] -= (C_INCR * word_caps[i]);

    // update sentiment scores for sentiment-bearing words.
    if (word_scores[i] != 0){

      // using absolute value so that we can just add booster effects. we will put the sign back on at the end.
      out[i] = abs(word_scores[i]);
      sign[i] = (double(0) < word_scores[i]) - (word_scores[i] < double(0));

      // do caps: if this word is capitalized for emphasis, adjust it by adding or subtracting the caps differential
      if (word_caps[i] & (out[i] > 0)) {out[i] += C_INCR;}

      // do boosters: look at 3 preceding words. strength decays with each word back: 1, 0.95, 0.9
      // boosters are already adjusted for capitalization.
      out[i] = out[i] + word_boosters[i-1] + word_boosters[i-2] * 0.95 + word_boosters[i-3] * 0.9;

      // do negations: look back 3 words, multiply by N_SCALAR for each negator
      // FIXME full VADER code looks for special things like "never so" and "without doubt"
      out[i] = out[i] * pow(N_SCALAR, word_negations[i-1] + word_negations[i-2] + word_negations[i-3]);


      // but check
      // find if there is a but, and then half sentiments before it and 1.5 sentiments after it
      if (first_but != 0){
        if (first_but > (i)) out[i] *= 0.5;
        if (first_but < (i)) out[i] *= 1.5;
      }

      // put the polarity back on
      out[i] *= sign[i];

    }
  }

  return (out);
}


//
// This is really just a binary search. It takes as inputs a text vector and a sorted dictionary.
// It outputs an integer vector of  the same length as the input text vector.
// It loops through each word in the input vector and finds its index in the dictionary (if applicable).
// Words that aren't in the  dictionary get NA, which is what we want so we can use the results to index a vector in R.
// This is way faster than just doing the indexing in R, which was a bottleneck previously.
// [[Rcpp::export]]
IntegerVector get_index2(CharacterVector text, CharacterVector sorted_dictionary) {
  int n = text.length();
  IntegerVector out(n);

  int dict_length = sorted_dictionary.length();

  for (int i = 0; i < n; ++i){
    int l = 0;
    int r = dict_length - 1;
    String t(text[i]);

    // if no result, return NA. We want this (instead of, e.g., 0) because if you index an R vector with NA, you get
    // NA in response: this means that the length of the indexed vector is the same as that of our input index. If
    // you use 0 it seems to drop nonexistent entries, which causes problems.
    out[i] = NA_INTEGER;

    while (l <= r) {

      int m = l + (r-l) / 2;

      String comparator(sorted_dictionary[m]);

      // check if x is present at mid. if so, return m+1 (r is not 0-indexed)
      if (comparator == t){
        out[i] = m+1;
        break;
      }

      // if x greater, ignore left half
      if (t > comparator)
        l = m + 1;

      // if x less, ignore right half
      if (t < comparator)
        r = m - 1;
    }

  }

  return out;
}


// another binary search. this one is rewritten to work on the internal cppvader implementation
// gives -1 if index isn't found
// [[Rcpp::export]]
NumericVector get_index3(CharacterVector text, CharacterVector sorted_dictionary) {
  int n = text.length();
  NumericVector out(n);

  int dict_length = sorted_dictionary.length();

  for (int i = 0; i < n; ++i){
    int l = 0;
    int r = dict_length - 1;
    String t(text[i]);

    // if no result, return NA. We want this (instead of, e.g., 0) because if you index an R vector with NA, you get
    // NA in response: this means that the length of the indexed vector is the same as that of our input index. If
    // you use 0 it seems to drop nonexistent entries, which causes problems.
    out[i] = -1;

    while (l <= r) {

      int m = l + (r-l) / 2;

      String comparator(sorted_dictionary[m]);

      // check if x is present at mid. if so, return m (c++ is  0-indexed)
      if (comparator == t){
        out[i] = m;
        break;
      }

      // if x greater, ignore left half
      if (t > comparator)
        l = m + 1;

      // if x less, ignore right half
      if (t < comparator)
        r = m - 1;
    }

  }

  return out;
}


// ugly growing vader implementation
//
// @param text A single string
//[[Rcpp::export]]
NumericVector vadercpp(std::string text, CharacterVector mod_bigrams, CharacterVector dict_sent_word, NumericVector dict_sent_value, CharacterVector dict_negate, CharacterVector dict_mod_word, NumericVector dict_mod_value, CharacterVector dict_punct_word, NumericVector dict_punct_value){
  std::string text_edit = text;

  // at present all bigram modifiers are attenuators. the implementation only looks at single-word tokens, so
  // replace each bigram modifier with a unigram "kindof".
  // TODO FIXME: should it replace them with two tokens so the pos/neg/neu scores are the same? e.g. "kind of" -> "the kindof" ?
  // apparently regexes are hard to import to Rcpp so here's a fast, dumb, probably unoptimized linear search function
  int n_bigrams = mod_bigrams.size();
  std::string replacement = "kindof";
  for (int i = 0; i < n_bigrams; ++i){
    text_edit = replace_text(text_edit, std::string(mod_bigrams[i]), "kindof");
  }

  // "least" is a negator UNLESS in bigram constructions "at least" and "very least".
  // so again do a search/replace for those.
 // text_edit = replace_text(text_edit, "at least", "kindof");

  // split the string into a CharacterVector at spaces
  CharacterVector text_split = str_split_space(text_edit);

  // keep the original for comparing caps
  // FIXME TODO this is a slow way of doing it: we could also create and populate a new vector in the tolower loop. would be faster
  CharacterVector text_split_orig (clone(text_split));

  // get the length here once since we'll use it a lot below
  int text_split_len = text_split.length();

  /* FIND EMOJI SCORES: SEARCH UNEDITED TEXT TOKENS IN DICT_PUNCT FOR EMOJIS ETC. */
  // set up numericVector for word scores. NB it's length of text_split + 3, since we need 3 leading 0s for the main loop
  // we will use these again when we look for the lowercase and punct-removed words
  IntegerVector word_index(text_split_len);

  NumericVector word_scores(text_split_len + 3);

  // first get the indices
  word_index =  get_index3(text_split, dict_punct_word);

  // then change it to use the scores, starting at the third index so there are 3 leading zeroes
  for (int i = 0; i < word_index.length(); ++i){
    if (word_index[i] != -1) word_scores[i+3] = dict_punct_value[word_index[i]];
    if (word_index[i] == -1) word_scores[i+3] = 0;
  }

  /* REMOVE LEADING AND TRAILING PUNCTUATION */
  // Removing all punctuation was overkill: it turns neutral words like "I'll" into "ill" which is negative
  text_split = remove_punc_around(text_split);

  /* CONVERT TO LOWER CASE */
  // loops along each element of the CharacterVector
  // we can't do it on the original string because we need to check each word to see if it's capitalized
  // FIXMETODO!! this has two negative effects!
  // a) it means emojis with capital letters like ":D" won't get caught, b) it will mess up with unicode
  std::locale loc;
  for (int j = 0; j < text_split_len; ++j){
    std::string str = std::string(text_split[j]);

    for (std::string::size_type i=0; i<str.length(); ++i)
      str[i] = std::tolower(str[i],loc);

    text_split[j] = str;
  }


  /* CONVERT MULTI-WORD TOKENS TO UNIGRAMS */
  // VADER uses a few multi-word constructions where words differ from their usage alone:
  // For example, "never"'s meaning differs in "never so good" and "never good".
  // Since the main loop here works on one-word tokens, here we convert multi-word tokens to single-word tokens.
  // There is no perfect solution here. My rationale is that it's easier to think of the main VADER algorithm
  // as a moving-window function over comparable vectors, so then we need to do this preprocessing to get comparable vectors.
  // NumericVector never_so (text_split_len + 3);
  // for (int i = 3; i < text_split_len, ++i) {
  //   if ((text_split[i-2] == "never") & (text_split[i-1] == "so") )
  //
  //
  // }
  //

   /* FIND WORD SCORES */

  // first get the indices
  word_index =  get_index3(text_split, dict_sent_word);

  // then change it to use the scores, starting at the third index so there are 3 leading zeroes
  // we don't zero-out anything not found, since we already did that and don't want to overwrite emoji scores
  for (int i = 0; i < word_index.length(); ++i){
    if (word_index[i] != -1) word_scores[i+3] = dict_sent_value[word_index[i]];
    //if (word_index[i] == -1) word_scores[i+3] = 0;
  }

  /* FIND NEGATIONS */
  // using simple linear search for now, which works. move to binary search for speed later FIXME TODO
  NumericVector word_negations(text_split_len + 3);

  for (int i = 0; i < text_split_len; ++i){
    // "least" is a negator UNLESS in bigram constructions "at least" and "very least".
    // so we look for those separately.
    // TODO FIXME: can we do a search/replace to turn the bigram into a unigram and not have this special case?
    //             I considered doing it earlier but we might get tripped up if we do it before converting to lower case.
    if ((text_split[i] == "least") & (i == 0)){
      word_negations[i+3] = 1;
      continue;
    }
    if ((text_split[i] == "least") & (i > 0)){
      if ((text_split[i-1] != "at") & (text_split[i-1] != "very")){
        word_negations[i+3] = 1;
        continue;
      }
    }

    // if we didn't find a "least" construction then look for the rest of the negators
    for (int j = 0; j < dict_negate.length(); ++j) {
      if (dict_negate[j] == text_split[i]){
        word_negations[i+3] = 1;
      }
    }
  }

  /* FIND MODIFIERS */
  // using simple linear search for now, which works. move to binary search for speed later FIXME TODO
  // consider switching to IntegerVector and just passing sign in dict--could multiply sign by the value in the assignment
  NumericVector word_modifiers(text_split_len + 3);

  for (int i = 0; i < text_split_len; ++i){
    for (int j = 0; j < dict_mod_word.length(); ++j) {
      if (dict_mod_word[j] == text_split[i]){
        word_modifiers[i+3] = dict_mod_value[j];
      }
    }
  }

  /* FIND CAPITALIZED WORDS IF APPLICABLE */

  LogicalVector word_caps(text_split_len + 3);
  bool two_lowercase = 0;
  bool two_uppercase = 0;
  bool caps_diff = 0;

  // FIRST find if there is a caps difference
  // look for two adjacent lowercase, then two adjacent uppercase, if we find both then there is a caps difference
  for (long long unsigned int i = 1; i < text.length(); ++i){
    if ((text[i] == std::tolower(text[i])) & (text[i-1] == std::tolower(text[i-1]))) two_lowercase = 1;

    if ((text[i] == std::toupper(text[i])) & (text[i-1] == std::toupper(text[i-1]))) two_uppercase = 1;

    if (two_lowercase & two_uppercase){
      caps_diff = 1;
      break;
    }
  }

  // THEN find if each word is capitalized or not.
  // NOTE this loop and previous loop could probably be brought into one loop to be faster
  if (caps_diff == 1){
    for (int i = 0; i < text_split_len; ++i){
      for (int j = 1; j < text_split_orig[i].size(); ++j){
        if ((text_split_orig[i][j] == std::toupper(text_split[i][j])) & (text_split_orig[i][j-1] == std::toupper(text_split[i][j-1])) ){
          word_caps[i+3] = 1;

        }
      }
    }
  }



  /* COUNT PUNCTUATION AND DETERMINE MODIFIER */
  double mod_excl = std::min(char_count(text, "!"), 4) * 0.292;
  double mod_qm = 0;

  int qm_count = char_count(text, "?");

  if (qm_count > 1) {
    if (qm_count <= 3) {
      mod_qm = qm_count * 0.18;
      } else (mod_qm = 0.96);
  }

  double mod_punc = mod_excl + mod_qm;

  /* FIND THE FIRST "BUT" IF THERE IS ONE */
  int first_but = 0;
  for (int i = 0; i < text_split_len; ++i){
    if (text_split[i] == "but"){
      first_but = i+3;
      break;
    }
  }

  /* LOOP THROUGH EACH WORD USING VADERC() */
  word_scores = vaderC(word_scores, word_negations, word_modifiers, word_caps, first_but);


  /* CALCULATE TOTAL SUM OF SCORES */
      double sum_scores = sum(word_scores);


  /* ADJUST FOR GLOBAL PUNCTUATION EFFECTS: make it either more positive or more negative */
  if(sum_scores > 0) {
    sum_scores = sum_scores + mod_punc;
  } else if(sum_scores < 0) {
    sum_scores = sum_scores - mod_punc;
    }


  /* GET COMPOUND SCORE, POSITIVE SCORE, NEGATIVE SCORE, NEUTRAL SCORE */
  double compound = sum_scores / sqrt((sum_scores * sum_scores) + 15);

  NumericVector temp = word_scores[word_scores > 0];
  double pos = sum( temp ) + temp.length();

  temp = word_scores[word_scores < 0];
  double neg = sum(temp) - temp.length();

  // get neutral sum: don't count the three leading 0s!
  temp = word_scores[word_scores == 0];
  double neu = temp.length() - 3;

  // apply pucntuation modifier to whichever has greatest absolute value
  if (pos > abs(neg)) pos += mod_punc;
  if (pos < abs(neg)) neg -= mod_punc;

  // get total of absolute values (so we subtract negative)
  double total = pos - neg + neu;

  // now get the scores
  // this is where we flip neg to its absolute value
  pos /= total;
  neg /= -total;
  neu /= total;

  NumericVector results(4);

  results[0] = compound;
  results[1] = pos;
  results[2] = neu;
  results[3] = neg;

  results.names() = CharacterVector({"compound", "pos", "neu", "neg"});
  return round(results, 4);
}


// VADER return a dataframe
//
// @param text A character vector
//[[Rcpp::export]]
DataFrame vaderdf(CharacterVector text_vec, CharacterVector mod_bigrams, CharacterVector dict_sent_word, NumericVector dict_sent_value, CharacterVector dict_negate, CharacterVector dict_mod_word, NumericVector dict_mod_value, CharacterVector dict_punct_word, NumericVector dict_punct_value){
  int n(text_vec.size());

  NumericVector comps(n);
  NumericVector poss(n);
  NumericVector neuss(n);
  NumericVector negs(n);

  for (int i = 0; i < n; ++i){

  std::string text = std::string(text_vec[i]);

    NumericVector results = vadercpp(text, mod_bigrams, dict_sent_word, dict_sent_value, dict_negate, dict_mod_word, dict_mod_value, dict_punct_word, dict_punct_value );

    comps[i] = results[0];
    poss[i]  = results[1];
    neuss[i] = results[2];
    negs[i]  = results[3];

  }

  DataFrame out = DataFrame::create (Named("compound") = comps,
                                     Named("pos")      = poss,
                                     Named("neu")      = neuss,
                                     Named("neg")      = negs);
  return out;

}

