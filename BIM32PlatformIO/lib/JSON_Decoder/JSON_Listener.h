/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


See more at http://blog.squix.ch and https://github.com/squix78/json-streaming-parser

*/

// altrunner forker and refactored to use char arrays and add error reporting:
// https://github.com/altrunner/json-streaming-parser

// altrunner version of library forked and name changed by Bodmer to avoid IDE
// name conflicts with Arduino IDE Library manager version:
// https://github.com/Bodmer/JSON_Decoder

#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "MockArduino.h"
#endif

class JsonListener {
  private:

  public:

    virtual ~JsonListener() {}

    // Member functions changed to a more logical order
    virtual void startDocument();

    virtual void endDocument();


    virtual void startObject();

    virtual void endObject();


    virtual void startArray();

    virtual void endArray();


    virtual void key(const char *key);

    virtual void value(const char *value);


    virtual void whitespace(char c);


    virtual void error( const char *message );

};

