/* medianfilter.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef MEDIANFILTER_HH
#define MEDIANFILTER_HH

#include <vector>
#include <algorithm>

template<class T>
T getMedian(std::vector<T> scores) {
  size_t s = scores.size();
  size_t n = s / 2;
  nth_element(scores.begin(), scores.begin() + n, scores.end());
  T median = (s % 2 == 0) ? (scores[n - 1] + scores[n]) / 2 : scores[n];
  return median;
}

template<class T>
void medfilt2D(
    T* input, 
    T* output, 
    const int SZ, 
    const int nr, 
    const int nc) {

  int dsz = (SZ / 2);
  std::vector<T> scores;

  for (int y = 0; y < nr; y++) {
    for (int x = 0; x < nc; x++) {
      scores.clear();
      for (int dy = -dsz; dy <= dsz; dy++) {
        for (int dx = -dsz; dx <= dsz; dx++) {
          if ((y + dy) >= 0 && (y + dy) < nr && (x + dx) >= 0 && (x + dx) < nc)
            scores.push_back(input[y + dy + (x + dx) * nr]);
        }
      }
      output[y + x * nr] = getMedian(scores);
    }
  }
}

template<class T>
T *medfilt2D(
    T* input, 
    const int SZ,
    const int nr,
    const int nc) {

    T* output = new T[nr*nc]();

    int dsz = (SZ / 2);
    std::vector<T> scores;

    for (int y = 0; y < nr; y++) {
        for (int x = 0; x < nc; x++) {
            scores.clear();
            for (int dy = -dsz; dy <= dsz; dy++) {
                for (int dx = -dsz; dx <= dsz; dx++) {
                    if ((y + dy) >= 0 && (y + dy) < nr && (x + dx) >= 0 && (x + dx) < nc)
                        scores.push_back(input[y + dy + (x + dx) * nr]);
                }
            }
            output[y + x * nr] = getMedian(scores);
        }
    }

    return output;
}

#endif
