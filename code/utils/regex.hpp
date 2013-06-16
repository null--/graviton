#ifndef _GVN_REGEX_HEAD_
#define _GVN_REGEX_HEAD_

#include <graviton.hpp>
#include <external/trex/trex.hpp>
#include <core/logger.hpp>

#include <string>
#include <vector>
using namespace std;

/**
   ----------------------------------------------------------------------
TRex implements the following expressions

\	Quote the next metacharacter
^	Match the beginning of the string
.	Match any character
$	Match the end of the string
|	Alternation
()	Grouping (creates a capture)
[]	Character class  

==GREEDY CLOSURES==
*	   Match 0 or more times
+	   Match 1 or more times
?	   Match 1 or 0 times
{n}    Match exactly n times
{n,}   Match at least n times
{n,m}  Match at least n but not more than m times  

==ESCAPE CHARACTERS==
\t		tab                   (HT, TAB)
\n		newline               (LF, NL)
\r		return                (CR)
\f		form feed             (FF)

==PREDEFINED CLASSES==
\l		lowercase next char
\u		uppercase next char
\a		letters
\A		non letters
\w		alphanimeric [0-9a-zA-Z]
\W		non alphanimeric
\s		space
\S		non space
\d		digits
\D		non nondigits
\x		exadecimal digits
\X		non exadecimal digits
\c		control charactrs
\C		non control charactrs
\p		punctation
\P		non punctation
\b		word boundary
\B		non word boundary

 */
namespace GraVitoN
{
    namespace Utils
    {
        class Regex
        {
        private:
            TRex *rex;
            
        public:
            Regex();
            ~Regex();
            
            bool    compile(const string &pattern);
            bool    match(const string &text);
            bool    search(const string &text, vector<string> &matches);

            bool    isValid() { return rex != _null_; }
        };

        Regex::Regex()
        {
            rex = _null_;
        }

        Regex::~Regex()
        {
            if(rex)
                trex_free(rex);
        }
        
        bool Regex::compile(const string &pattern)
        {
            if( rex )
                trex_free(rex);
            
            const TRexChar *err;
            rex = trex_compile(_TREXC( (pattern.c_str()) ),&err);
            if(!rex)
            {
                Core::Logger::logVariable("[Regex Error] ", err?err:_TREXC("undefined"));
                return false;
            }

            return true;
        }

        
        bool Regex::match(const string &text)
        {
            const TRexChar *begin, *end;
            TRexChar * p_pos = (TRexChar*)_TREXC( (text).c_str() );

            return trex_search(rex, p_pos, &begin, &end);
        }

        bool Regex::search(const string &text, vector<string> &matches)
        {
            if( !rex)
            {
                return false;
            }

            matches.clear();
            
            if( !match(text) )
            {
                Core::Logger::logVariable("[REGEX] Not Matched: ", text);
                return false;
            }

            int i,j,n = trex_getsubexpcount(rex);
            TRexMatch match;
            for(i = 0; i < n; i++)
            {
                trex_getsubexp(rex,i,&match);
                // trex_sprintf(t,_TREXC("[%%d]%%.%ds\n"),match.len);
                // trex_printf(t,i,match.begin);

                string tmp;
                for(j=0; j<match.len; ++j)
                {
                    tmp = tmp + *((char*)(match.begin + j));
                }
                matches.push_back(tmp);
            }
            // trex_printf(_TREXC("match! %d sub matches\n"),trex_getsubexpcount(x));

            return matches.size() > 0;
        }
    }
}

#endif
