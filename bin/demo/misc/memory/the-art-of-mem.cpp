/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief GraVitoN socks5 client
 *
 */

/// Compile: g++ -I ../../../../code -o the-art-of-mem the-art-of-mem.cpp

#include <graviton.hpp>
#include <core/memory.hpp>

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace GraVitoN;
using namespace std;

#define PRINT(STR,XXX) cout << STR << "\t|\t"; printHexMemo(XXX);

template<class _M_>
void printHexMemo(const Core::Memory<_M_> &m)
{
    int i = 0, sz = m.size();

    printf("Base address: 0x%8x\n", (_M_*)m);

    cout << "Data: " << m << endl;
    
    while( i < sz )
    {
        printf("%2x ", (unsigned char)*(m + i));
        if( i%8 == 7 )
            cout << "\t";
        if( i%16 == 15 )
            cout << endl;
        ++i;
    }

    if( sz % 16 != 0 )
        cout << endl;
    cout<<endl;
}

int main()
{
    cout << "May the 'Base address' reveals the unknown..." << endl << endl;

    guchar * buf;
    Core::Memory<guchar> memo(8), tmp, memo2(32);

    memo.zero();
    PRINT("memo",memo);

    memo.set('G');
    PRINT("memo",memo);

    memo.resize(16);
    for(int i=8; i<16; ++i)
        memo[i] = 'C';
    PRINT("memo",memo);

    tmp = memo;
    PRINT("tmp",tmp);

    buf = tmp;
    printf("buf: Base address: %8x\n", buf);
    memo2.set('Z');
    memo2.copy(buf, tmp.size(), 24);
    PRINT("memo2", memo2);

    cout << "Testing istream - Enter something (no blanks!): ";
    cin >> tmp;
    cout << "Testing ostream: ";
    cout << tmp << endl;
    
    PRINT("tmp", tmp);

    return 0;
}
