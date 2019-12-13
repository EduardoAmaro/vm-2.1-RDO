/*
 * contains.hpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */
#include <list>
#include <string>
#include <algorithm>

#ifndef THIRDPARTLIBS_CONTAINS_HPP_
#define THIRDPARTLIBS_CONTAINS_HPP_

/*
 * Generic function to find if an element of any type exists in list
 */
template<typename T>
bool contains(std::list<T> & listOfElements, const T & element)
{
  // Find the iterator if element in list
  auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
  //return if iterator points to end or not. It points to end then it means element
  // does not exists in list
  return it != listOfElements.end();
}


#endif /* THIRDPARTLIBS_CONTAINS_HPP_ */
