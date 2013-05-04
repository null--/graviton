/* The MIT License:

Copyright (c) 2012 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://ting.googlecode.com



/**
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief utf8 utilities.
 */

#pragma once

#include "types.hpp"



namespace ting{
namespace utf8{



//TODO: doxygen
class Iterator{
	ting::u32 c;
	const ting::u8* n;

public:
	Iterator() :
			c(0)
	{}

	Iterator(const char* begin) :
			n(reinterpret_cast<const ting::u8*>(begin))
	{
		if(this->n == 0){
			this->c = 0;
		}else{
			this->operator++();
		}
	}

	inline ting::u32 Char()const throw(){
		return this->c;
	}

	//no operator*() because it usually returns reference, don't want to break this contract.
	
	//prefix ++
	//if ++ end iterator then undefined behavior
	Iterator& operator++()throw(){
		ting::u8 b = *this->n;
//		TRACE(<< "utf8::Iterator::operator++(): b = " << std::hex << unsigned(b) << std::endl)
		++this->n;
		if((b & 0x80) == 0){
			this->c = ting::u32(b);
			return *this;
		}

//		TRACE(<< "utf8::Iterator::operator++(): *n = " << std::hex << unsigned(*this->n) << std::endl)
		
		this->c = (*this->n) & 0x3f;
//		TRACE(<< "utf8::Iterator::operator++(): initial c = " << std::hex << c << std::endl)

		++this->n;

		unsigned i = 2;
		for(; (ting::u8(b << i) >> 7); ++i, ++this->n){
//			TRACE(<< "utf8::Iterator::operator++(): ting::u8(b << i) = " << std::hex << unsigned(ting::u8(b << i)) << std::endl)
//			TRACE(<< "utf8::Iterator::operator++(): ((b << i) >> 7) = " << std::hex << unsigned(ting::u8(b << i) >> 7) << std::endl)
//			TRACE(<< "utf8::Iterator::operator++(): *n = " << std::hex << unsigned(*this->n) << std::endl)
			this->c <<= 6;
//			TRACE(<< "utf8::Iterator::operator++(): c = " << std::hex << c << std::endl)
			this->c |= (*this->n) & 0x3f;
//			TRACE(<< "utf8::Iterator::operator++(): c = " << std::hex << c << std::endl)
		}
		this->c |= (ting::u32(ting::u8(b << (i + 1)) >> (i + 1)) << (6 * (i - 1)));
//		TRACE(<< "utf8::Iterator::operator++(): c = " << std::hex << c << std::endl)

		return *this;
	}

	//no postfix ++ operator, there is no need in it.

	inline bool IsEnd()const throw(){
		return this->c == 0;
	}

	inline bool IsNotEnd()const throw(){
		return !this->IsEnd();
	}
};



}//~namespace
}//~namespace
