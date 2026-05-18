#pragma once

#include "JsonListener.h"

class ExampleListener: public JsonListener {

  public:
    virtual void whitespace(char c);
  
    virtual void startDocument();

    virtual void key(const char *key);

    virtual void value(const char *value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();

    virtual void error( const char *message );
};
