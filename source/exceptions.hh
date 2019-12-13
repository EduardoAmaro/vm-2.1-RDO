/*
 * exceptions.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef EXCEPTIONS_HH_
#define EXCEPTIONS_HH_

#include <string>
#include <exception>

using namespace std;

class MissingRequiredInputException : public exception {
  const char * what() const throw () {
    return "Mandatory command line parameter is missing.";
  }
};

class CodecNotSupportedException : public exception {
  const char * what() const throw () {
    return "Codec not supported. Only the options 'kakadu' or 'mule' are supported";
  }
};

class DifferentViewsDimensionsException : public exception {
  const char * what() const throw () {
    return "Invalid view sizes. Views with different dimensions cannot be synthesized.";
  }
};

class InvalidInputLightFieldFormatException : public exception {
 private:
  std::string msg;

 public:
  InvalidInputLightFieldFormatException(const string m)
      : msg(
          "Invalid format. Check the PGM names in " + m + " light-field path.") {

  }

  const char * what() const throw () {
    return msg.c_str();
  }
};

class FileDoesNotExistsException : public exception {
 private:
  std::string msg;

 public:
  FileDoesNotExistsException(const string m)
      : msg(m + " does not exists.") {
  }

  virtual const char * what() const throw () {
    return msg.c_str();
  }
};

#endif /* EXCEPTIONS_HH_ */
