#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

// output of running : tools::package_native_routine_registration_skeleton(".", character_only = FALSE)
/* FIXME:
  Check these declarations against the C/Fortran source code.
*/

  /* .Call calls */
  extern SEXP _tidyvader_char_count(SEXP, SEXP);
extern SEXP _tidyvader_cpp_str_sort(SEXP);
extern SEXP _tidyvader_get_index2(SEXP, SEXP);
extern SEXP _tidyvader_get_index3(SEXP, SEXP);
extern SEXP _tidyvader_remove_punc(SEXP);
extern SEXP _tidyvader_remove_punc_around(SEXP);
extern SEXP _tidyvader_replace_text(SEXP, SEXP, SEXP);
extern SEXP _tidyvader_str_split_space(SEXP);
extern SEXP _tidyvader_strlen(SEXP);
extern SEXP _tidyvader_vaderC(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _tidyvader_vadercpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _tidyvader_vaderdf(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
  {"_tidyvader_char_count",         (DL_FUNC) &_tidyvader_char_count,         2},
  {"_tidyvader_cpp_str_sort",       (DL_FUNC) &_tidyvader_cpp_str_sort,       1},
  {"_tidyvader_get_index2",         (DL_FUNC) &_tidyvader_get_index2,         2},
  {"_tidyvader_get_index3",         (DL_FUNC) &_tidyvader_get_index3,         2},
  {"_tidyvader_remove_punc",        (DL_FUNC) &_tidyvader_remove_punc,        1},
  {"_tidyvader_remove_punc_around", (DL_FUNC) &_tidyvader_remove_punc_around, 1},
  {"_tidyvader_replace_text",       (DL_FUNC) &_tidyvader_replace_text,       3},
  {"_tidyvader_str_split_space",    (DL_FUNC) &_tidyvader_str_split_space,    1},
  {"_tidyvader_strlen",             (DL_FUNC) &_tidyvader_strlen,             1},
  {"_tidyvader_vaderC",             (DL_FUNC) &_tidyvader_vaderC,             5},
  {"_tidyvader_vadercpp",           (DL_FUNC) &_tidyvader_vadercpp,           9},
  {"_tidyvader_vaderdf",            (DL_FUNC) &_tidyvader_vaderdf,            9},
  {NULL, NULL, 0}
};

void R_init_tidyvader(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
