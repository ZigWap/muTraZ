/*
 * Copyright (c) 2008-2014 David Schor <david@zigwap.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "vm.h"
#include "muTraZ.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
/* {{
 *      ID:
 *           $len()
 *  Syntax:
 *           $len(<string>)
 *    Desc:
 *           Returns the length of the string.
 *  Throws:
 *           - muErr_TooManyParams
 * }}
 */
MSL_IDENTIFIER_L(len)
{
    if (MSL_ARG_COUNT > 1)
        MSL_COMMAND_THROW(muErr_TooManyParams);
    
    MSL_RETURN_PINT(strlen(MSL_GET_ARG(0)));
}
/* {{
 *      ID:
 *           $str()
 *  Syntax:
 *           $str(<string>,<repeat>)
 *    Desc:
 *           Returns the length of the string.
 *  Throws:
 *           - muErr_TooManyParams
 *           - muErr_TooFewParams
 *           - muErr_InvalidParams
 *           - muErr_OOM
 * }}
 */
MSL_IDENTIFIER_L(str)
{
    size_t len;
    long repeat;
    char *ret, *tmp, *str;
    
    if (MSL_ARG_COUNT > 2)
        MSL_COMMAND_THROW(muErr_TooManyParams);
    if (MSL_ARG_COUNT < 2)
        MSL_COMMAND_THROW(muErr_TooFewParams);
    
    repeat = strtol(MSL_GET_ARG(1), &tmp, 10);
    if (*tmp != '\0' || repeat < 0)
        MSL_COMMAND_THROW(muErr_InvalidParams);
    
    str = MSL_GET_ARG(0);
    len = strlen(str);
    
    if (!(ret = malloc(len * repeat + 1)))
        MSL_COMMAND_THROW(muErr_OOM);
        
    for (tmp = ret; repeat--;)
    {
        memcpy(tmp, str, len); 
        tmp += len;
    }
    *tmp = '\0';
    
    MSL_RETURN(ret);
}
/* {{
 *      ID:
 *           $left()
 *  Syntax:
 *           $left(<string>, <N>)
 *    Desc:
 *           Returns a substring from the left side of string
 *           based on <N>. Where:
 * 
 *           If <N> >= 0: return <N> characters from the left.
 *           If <N> <  0: return everything from the left minus
 *                         <N> characters.
 *  Throws:
 *           - muErr_TooManyParams
 *           - muErr_TooFewParams
 *           - muErr_InvalidParams
 *           - muErr_OOM
 * }}
 */
MSL_IDENTIFIER_L(left)
{
    size_t len;
    char *ret, *tmp, *str;
    long n;
    
    if (MSL_ARG_COUNT > 2)
        MSL_COMMAND_THROW(muErr_TooManyParams);
    if (MSL_ARG_COUNT < 2)
        MSL_COMMAND_THROW(muErr_TooFewParams);
    
    n = strtol(MSL_GET_ARG(1), &tmp, 10);
    if (*tmp != '\0')
        MSL_COMMAND_THROW(muErr_InvalidParams);
    
    str = MSL_GET_ARG(0);
    len = strlen(str);
    
    if (n < 0)
    { /* subtract N characters from the end */
        n = -n;
        if (len >= n)
            len -= n;
        else
            len = 0;
    }
    else
    { /* restrict string to N characters */
        if (len >= n)
            len = n;
    }
    
    if (!(ret = malloc(len + 1)))
        MSL_COMMAND_THROW(muErr_OOM);
    
    memcpy(ret, str, len);
    ret[len] = '\0';
    
    MSL_RETURN(ret);
}
/* {{
 *      ID:
 *           $right()
 *  Syntax:
 *           $right(<string>, <N>)
 *    Desc:
 *           Returns a substring from the right side of string
 *           based on <N>. Where:
 * 
 *           If <N> >= 0: return <N> characters from the right.
 *           If <N> <  0: return everything from the right minus
 *                         <N> characters.
 *  Throws:
 *           - muErr_TooManyParams
 *           - muErr_TooFewParams
 *           - muErr_InvalidParams
 *           - muErr_OOM
 * }}
 */
MSL_IDENTIFIER_L(right)
{
    size_t len, start;
    char *ret, *tmp, *str;
    long n;
    
    if (MSL_ARG_COUNT > 2)
        MSL_COMMAND_THROW(muErr_TooManyParams);
    if (MSL_ARG_COUNT < 2)
        MSL_COMMAND_THROW(muErr_TooFewParams);
    
    n = strtol(MSL_GET_ARG(1), &tmp, 10);
    if (*tmp != '\0')
        MSL_COMMAND_THROW(muErr_InvalidParams);
    
    str = MSL_GET_ARG(0);
    len = strlen(str);
    
    if (n >= 0)
    { /* start on Nth character from left */
        if (n > len)
            start = 0;
        else
            start = len - n;
    }
    else
    { /* subtract N characters from the start */
        n = -n;
        if (n > len)
            start = len;
        else
            start = n;
    }
    
    len = len - start;
    if (!(ret = malloc(len + 1)))
        MSL_COMMAND_THROW(muErr_OOM);
    
    memcpy(ret, str + start, len);
    ret[len] = '\0';
    
    MSL_RETURN(ret);
}
/* {{
 *      ID:
 *           $mid()
 *  Syntax:
 *           $mid(<string>, <S>[, <L>])
 *    Desc:
 *           Returns a substring starting at position <S> for
 *           a length of <L>, where:
 * 
 *           If <S> >= 0: the start is from the <S>th character.
 *           If <S> <  0: the start if from the -<S>th character from the end.
 * 
 *           If <L> > 0: return <L> characters.
 *           If <L> < 0: return <L> characters from the end of the substring.
 *           If <L> = 0: return length (WHY WTF?)
 *  Throws:
 *           - muErr_TooManyParams
 *           - muErr_TooFewParams
 *           - muErr_InvalidParams
 *           - muErr_OOM
 * }}
 */
MSL_IDENTIFIER_L(mid)
{
    size_t len;
    char *ret, *tmp, *str;
    long start;
    
    if (MSL_ARG_COUNT > 3)
        MSL_COMMAND_THROW(muErr_TooManyParams);
    if (MSL_ARG_COUNT < 1)
        MSL_COMMAND_THROW(muErr_TooFewParams);
    
    start = strtol(MSL_GET_ARG(1), &tmp, 10);
    if (*tmp != '\0')
        MSL_COMMAND_THROW(muErr_InvalidParams);   
    
    str = MSL_GET_ARG(0);
    len = strlen(str);
    
    /* $mid( , 1) AND $mid( , 0) are the same, so we just shift
     * everything and zero-based it. */
    if (start > 0) --start;
    if (start < 0)
    { /* the start is number of character from the end */
        start = -start;
        if (start <= len)
            start = len - start;
        else
            start = len;
    }
    if (start > len)
        start = len;
    
    /* narrow down the length to the new length */
    len -= start;
    
    /* if the length is provided, apply it on top of the previous
     * substring */
    if (MSL_ARG_COUNT == 3)
    {
        int newlen;
        
        newlen = strtol(MSL_GET_ARG(2), &tmp, 10);
        if (*tmp != '\0')
            MSL_COMMAND_THROW(muErr_InvalidParams);
        
        /* this is serious a wtf? why dos mIRC even do this? */
        if (newlen == 0) /* on N=0, it actually returns the LENGTH, just wtf? */
            MSL_RETURN_PINT(len);
        
        if (newlen > 0)
        { /* reduce the size down to the newlen */
            if (newlen < len)
                len = newlen;
        }
        else
        {
            newlen = -newlen;
            if (newlen < len)
                len -= newlen;
            else /* -N > len of substring */
                len = start;
        }
    }
    
    if (!(ret = malloc(len + 1)))
        MSL_COMMAND_THROW(muErr_OOM);
    
    memcpy(ret, str + start, len);
    ret[len] = '\0';
    
    MSL_RETURN(ret);
}