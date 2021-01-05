# https://github.com/cjhutto/vaderSentiment
# VADER is smart, handsome, and funny.----------------------------- {'pos': 0.746, 'compound': 0.8316, 'neu': 0.254, 'neg': 0.0}
# VADER is smart, handsome, and funny!----------------------------- {'pos': 0.752, 'compound': 0.8439, 'neu': 0.248, 'neg': 0.0}
# VADER is very smart, handsome, and funny.------------------------ {'pos': 0.701, 'compound': 0.8545, 'neu': 0.299, 'neg': 0.0}
# VADER is VERY SMART, handsome, and FUNNY.------------------------ {'pos': 0.754, 'compound': 0.9227, 'neu': 0.246, 'neg': 0.0}
# VADER is VERY SMART, handsome, and FUNNY!!!---------------------- {'pos': 0.767, 'compound': 0.9342, 'neu': 0.233, 'neg': 0.0}
# VADER is VERY SMART, uber handsome, and FRIGGIN FUNNY!!!--------- {'pos': 0.706, 'compound': 0.9469, 'neu': 0.294, 'neg': 0.0}
# VADER is not smart, handsome, nor funny.------------------------- {'pos': 0.0, 'compound': -0.7424, 'neu': 0.354, 'neg': 0.646}
# The book was good.----------------------------------------------- {'pos': 0.492, 'compound': 0.4404, 'neu': 0.508, 'neg': 0.0}
# At least it isn't a horrible book.------------------------------- {'pos': 0.363, 'compound': 0.431, 'neu': 0.637, 'neg': 0.0}
# The book was only kind of good.---------------------------------- {'pos': 0.303, 'compound': 0.3832, 'neu': 0.697, 'neg': 0.0}
# The plot was good, but the characters are uncompelling and the dialog is not great. {'pos': 0.094, 'compound': -0.7042, 'neu': 0.579, 'neg': 0.327}
# Today SUX!------------------------------------------------------- {'pos': 0.0, 'compound': -0.5461, 'neu': 0.221, 'neg': 0.779}
# Today only kinda sux! But I'll get by, lol----------------------- {'pos': 0.317, 'compound': 0.5249, 'neu': 0.556, 'neg': 0.127}
# Make sure you :) or :D today!------------------------------------ {'pos': 0.706, 'compound': 0.8633, 'neu': 0.294, 'neg': 0.0}
# Catch utf-8 emoji such as 💘 and 💋 and 😁-------------------- {'pos': 0.279, 'compound': 0.7003, 'neu': 0.721, 'neg': 0.0}
# Not bad at all--------------------------------------------------- {'pos': 0.487, 'compound': 0.431, 'neu': 0.513, 'neg': 0.0}


vader_tests <- tibble::tribble(
  ~text,                                                                                ~pos,  ~compound, ~neu,  ~neg,
  "VADER is smart, handsome, and funny.",                                               0.746, 0.8316,    0.254, 0.0,
  "VADER is smart, handsome, and funny!",                                               0.752, 0.8439,    0.248, 0.0,
  "VADER is very smart, handsome, and funny.",                                          0.701, 0.8545,    0.299, 0.0,
  "VADER is VERY SMART, handsome, and FUNNY.",                                          0.754, 0.9227,    0.246, 0.0,
  "VADER is VERY SMART, handsome, and FUNNY!!!",                                        0.767, 0.9342,    0.233, 0.0,
  "VADER is VERY SMART, uber handsome, and FRIGGIN FUNNY!!!",                           0.706, 0.9469,    0.294, 0.0,
  "VADER is not smart, handsome, nor funny.",                                           0.0  ,-0.7424,    0.354, 0.646,
  "The book was good.",                                                                 0.492, 0.4404,    0.508, 0.0,
  "At least it isn't a horrible book.",                                                 0.363, 0.431 ,    0.637, 0.0,
  "The book was only kind of good.",                                                    0.303, 0.3832,    0.697, 0.0,
  "The plot was good, but the characters are uncompelling and the dialog is not great.",0.094,-0.7042,    0.579, 0.327,
  "Today SUX!",                                                                         0.0  ,-0.5461,    0.221, 0.779,
  "Today only kinda sux! But I'll get by, lol",                                         0.317, 0.5249,    0.556, 0.127,
  "Make sure you :) or :D today!",                                                      0.706, 0.8633,    0.294, 0.0,
  "Catch utf-8 emoji such as 💘 and 💋 and 😁",                                         0.279, 0.7003,    0.721, 0.0,
  "Not bad at all",                                                                     0.487, 0.431 ,    0.513, 0.0,

)
