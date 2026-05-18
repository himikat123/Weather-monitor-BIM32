// Dummy set so class user does not have to implement them all and
// if I add more functions then legacy sketches will still work...

// These are over-ridden by the users equivalent functions

#include "JSON_Listener.h"

//JsonListener::JsonListener() {} implicitly declared

  // Called when JSON document start detected e.g. "{"
  void JsonListener::startDocument() {};

  // Called at end of JSON document
  void JsonListener::endDocument() {};

  // Called at the start of each object, objects can be nested
  void JsonListener::startObject() {};

  // Called at the end of each object, objects can be nested
  // Increment array index here
  void JsonListener::endObject() {};

  // Called at begining of an array of N items
  // Reset array index to 0 here
  // Array path is currentObject:currentKey
  void JsonListener::startArray() {};

  // Called when all N items have ended
  void JsonListener::endArray() {};

  // Current key
  void JsonListener::key(const char *key) { key = key;};

  // Value associated with current key
  void JsonListener::value(const char *value) {value = value;};

  // Called when whitespace characters detected in stream
  void JsonListener::whitespace(char c) {c = c;};

  // Called when an error occurs during parsing - typically indicates bad JSON
  void JsonListener::error( const char *message ) {message = message;};
