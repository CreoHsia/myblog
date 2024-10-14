#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::plugins("cpp11")]]
// [[Rcpp::export]]
NumericMatrix process_v2(NumericMatrix P) {
  // Number of rows and columns
  int n = P.nrow();
  int m = P.ncol();
  
  // Flatten the matrix P and store:
  // 1) value of P, 2) row index
  NumericMatrix P_flat(n * m, 2);
  int idx = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      P_flat(idx, 0) = P(i, j);  // value of P
      P_flat(idx, 1) = i + 1;    // row index (R is 1-indexed)
      idx++;
    }
  }
  
  // Sort the flattened matrix by value
  std::sort(P_flat.begin(), P_flat.end(),
            [](const NumericMatrix::Row& a, const NumericMatrix::Row& b) {
              return a[0] < b[0];
            });
  
  // Initialize result matrix to store frequencies of counts
  NumericMatrix result(n, m + 1);
  
  // Initialize a vector
  IntegerVector element_counts(n);
  
  // Initialize another count tracker; this tracker counts how many elements in each "bin"
  IntegerVector count_tracker(m + 1);
  count_tracker[0] = n;
  
  // Iterate through sorted matrix and update counts based on threshold markers
  for (int i = 0; i < P_flat.nrow(); ++i) {
    int rowidx = P_flat(i, 1) - 1;  // Row index is stored in 1-index, so subtract 1
    if (element_counts[rowidx] == m - 1) {
      // Update the result matrix for the current row
      for (int k = 0; k <= m; ++k) {
        result(rowidx, k) = count_tracker[k];
      }
    }
    
    // Update the count tracker and element count for the current row
    count_tracker[element_counts[rowidx]]--;
    element_counts[rowidx]++;
    count_tracker[element_counts[rowidx]]++;
  }
  
  return result;
}