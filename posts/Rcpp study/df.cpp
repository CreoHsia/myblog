#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::export]]
DataFrame rcpp_df(){
  // Creating vector v
  NumericVector v = {1,2};
  // Creating DataFrame df
  DataFrame df = DataFrame::create( Named("V1") = v,         // simple assign
                                    Named("V2") = clone(v)); // using clone()
  // Changing vector v
  v = v * 2;
  return df;
}

