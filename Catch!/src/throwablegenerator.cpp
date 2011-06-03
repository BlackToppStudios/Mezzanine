#ifndef _throwablegenerator_cpp
#define _throwablegenerator_cpp

#include "throwablegenerator.h"
#include <cassert>

ThrowableGenerator* ThrowableGenerator::TheRealThrowableGenerator = 0;

ThrowableGenerator::ThrowableGenerator()
{
    assert(0==ThrowableGenerator::TheRealThrowableGenerator);
    TheRealThrowableGenerator = this;
}

ThrowableGenerator::~ThrowableGenerator()
{

}

ThrowableGenerator* ThrowableGenerator::GetThrowableGenerator()
{
    return TheRealThrowableGenerator;
}

#endif
