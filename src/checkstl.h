/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */


//---------------------------------------------------------------------------
#ifndef checkstlH
#define checkstlH
//---------------------------------------------------------------------------

#include "check.h"

class Token;

/** @brief %Check STL usage */
class CheckStl : public Check
{
public:
    /** This constructor is used when registering the CheckClass */
    CheckStl() : Check()
    { }

    /** This constructor is used when running checks.. */
    CheckStl(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
            : Check(tokenizer, settings, errorLogger)
    { }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
    {
        CheckStl checkStl(tokenizer, settings, errorLogger);

        checkStl.stlOutOfBounds();
        checkStl.iterators();
        checkStl.erase();
        checkStl.pushback();
        checkStl.stlBoundries();
    }


    /**
     * Finds errors like this:
     * for (unsigned ii = 0; ii <= foo.size(); ++ii)
     */
    void stlOutOfBounds();

    /**
     * Finds errors like this:
     * for (it = foo.begin(); it != bar.end(); ++it)
     */
    void iterators();

    /** Dereferencing an erased iterator */
    void dereferenceErasedError(const Token *tok, const std::string &itername);

    /**
     * Dangerous usage of erase
     */
    void erase();

    /**
     * Dangerous usage of push_back
     */
    void pushback();

    /**
     * bad condition.. "it < alist.end()"
     */
    void stlBoundries();

private:

    /**
     * Helper function used by the 'erase' function
     * This function parses a loop
     * @param it iterator token
     */
    void eraseCheckLoop(const Token *it);

    void stlOutOfBoundsError(const Token *tok, const std::string &num, const std::string &var);
    void iteratorsError(const Token *tok, const std::string &container1, const std::string &container2);
    void eraseError(const Token *tok);
    void pushbackError(const Token *tok, const std::string &iterator_name);
    void invalidPointerError(const Token *tok, const std::string &pointer_name);
    void stlBoundriesError(const Token *tok);

    void getErrorMessages()
    {
        std::cout << "===stl===" << "\n";
        iteratorsError(0, "container1", "container2");
        dereferenceErasedError(0, "iter");
        stlOutOfBoundsError(0, "i", "foo");
        eraseError(0);
        pushbackError(0, "iterator");
        invalidPointerError(0, "pointer");
        stlBoundriesError(0);
    }

    std::string name() const
    {
        return "STL usage";
    }

    std::string classInfo() const
    {
        return "Check for invalid usage of STL:\n"
               " * out of bounds errors\n"
               " * misuse of iterators when iterating through a container\n"
               " * dereferencing an erased iterator\n"
               " * for vectors: using iterator/pointer after push_back has been used\n";
    }
};

//---------------------------------------------------------------------------
#endif

