/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
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
 * @brief GraVitoN::Core::Buffer
*/

#ifndef GRAVITON_MEMORY_H
#define GRAVITON_MEMORY_H

#include <graviton.hpp>
#include <istream>
#include <ostream>

namespace GraVitoN
{
    namespace Core
    {
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        class Memory
        {
        protected:
            Type *buffer;
            GraVitoN::gsize buff_size;
            bool is_executable;
            
        public:
            /// Default constgructor
            Memory(const GraVitoN::gsize size_ = 0, bool executable_ = false);

            /// Construct from a valid buffer
            Memory(const Type *buffer_, const GraVitoN::gsize size);
            
            /// Copy Memory object
            Memory(const Memory<Type> &a_);

            /// Copy string to memory
            Memory(const string &str);
            
            /// Destructor
            ~Memory();

            /// is executable
            bool isExecutable() const
                { return is_executable; }
            
            /// alloc
            bool alloc(GraVitoN::gsize size_, bool executable_ = false);

            /// resize without loosing data
            bool resize(GraVitoN::gsize size_);
            
            /// free
            void free();
            
            /// set memory
            void set(Type value_);
            
            /// fill buffer with
            void zero();
            
            /// copy new data on memory starting at position
            void copy(const Type *buffer_, GraVitoN::gsize size_, GraVitoN::gsize positon_ = 0);

            /// 'buffer address' + 'position_ bytes'
            Type * address(const GraVitoN::gsize position_ = 0) const;

            /// cool! char x = (memory + 5)
            Type *operator + (const gsize &index) const;
            
            /// cast operator Type *
            operator Type * () const;

            /// check if buffer is allocated
            bool operator ! () const;
            
            /// size of buffer in byte ( not in sizeof(Type) )
            GraVitoN::gsize size() const;

            /// Memory = Memory operator
            Memory<Type> & operator = (const Memory<Type> &a);

            /// Memory = Memory operator
            Memory<Type> & operator = (const std::string &str);

            /// index operator (cyclic: size = A -=> data[A-1] == data[-1], data[1] == data[A+1] )
            Type &operator [] (const gsize &index) const;
            
            /// get sub buffer
            bool sub(Type *&sub_buffer_,
                     const GraVitoN::gsize size_,
                     const GraVitoN::gsize position_ = 0) const;
            
            /// convert buffer to string
            std::string toString(const GraVitoN::gsize position_ = 0) const;

            /// print memory into an ostream as a string
            template<class MType>
            friend std::ostream& operator << (std::ostream& stream, const Memory<MType> &m);
            
            /// read memory from an istream
            template<class MType>
            friend std::istream& operator >> (std::istream& stream, Memory<MType> &m);
            
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const GraVitoN::gsize size_, bool executable_)
        {
            is_executable = executable_;
            Memory<Type>::alloc(size_, executable_);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const Memory<Type> &a_)
        {
            is_executable = a_.isExecutable();
            Memory<Type>::copy(a_.address(), a_.size());
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const string &str)
        {
            is_executable = false;
            Memory<Type>::copy((Type*)str.c_str(), str.size());
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const Type *buffer_, const GraVitoN::gsize size_)
        {
            is_executable = false;
            Memory<Type>::copy(buffer_, size_);
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::~Memory()
        {
            Memory<Type>::free();
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool Memory<Type>::alloc(GraVitoN::gsize size_, bool executable_)
        {
            is_executable = executable_;
            
            if(size_ <= 0)
            {
                buffer = _null_;
                buff_size = 0;
            }
            else
            {
                if( buff_size > 0 )
                    Memory::free();
                
                buff_size = size_;
                
#if defined(INFO_OS_LINUX)
                int flag = PROT_READ | PROT_WRITE;
                if( executable_ ) flag = flag | PROT_EXEC;
                buffer = (Type*) mmap(0, size_, flag, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
               
#elif defined(INFO_OS_WINDOWS)
                if( executable_ )
                    buffer = (Type*) VirtualAllocEx(GetCurrentProcess(), NULL, size_, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                else
                    buffer = (Type*) VirtualAllocEx(GetCurrentProcess(), NULL, size_, MEM_COMMIT, PAGE_READWRITE);
#else
                buffer = (Type*)malloc(size_);

                unsigned long page = ( unsigned long ) jumper & ~ ( unsigned long ) ( getpagesize() - 1 );
                
                int flag = (PROT_READ | PROT_WRITE);
                if( executable_ )
                    flag = flag | PROT_EXEC;
                mprotect ( ( unsigned char* ) page, getpagesize(), flag);
#endif
            }
            return buffer != _null_;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool Memory<Type>::resize(GraVitoN::gsize size_)
        {
            bool good = (buffer != _null_ && buff_size > 0);

            Memory<Type> tmp;
            
            if(good)
                tmp.copy(buffer, buff_size);
            
            Memory<Type>::free();
            Memory<Type>::alloc(size_, is_executable);

            if(good)
                memcpy( (void*) (buffer), (void *) tmp.address(), std::min(size_, buff_size));
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool Memory<Type>::sub(Type *&sub_buffer_, const GraVitoN::gsize size_, const GraVitoN::gsize position_) const
        {
            // cout << " size_ " << size_ << " posiiton_ " << position_ << " buff_size " << buff_size << endl;
            
            if( buffer && (position_ + size_) <= buff_size && position_ >= 0 )
            {
                // cout << "sub" << endl;

                /// char = 1 byte
                sub_buffer_ = (Type*)( new char[size_] );
                // cout << (int) ( buffer[position_] ) << (int) ( buffer[position_ + 1] ) << endl;
                memcpy( (void*) sub_buffer_, (void*)( buffer + position_ ), size_);
                // cout << (int) ( sub_buffer_[0] ) << (int) ( sub_buffer_[1] ) << endl;
                return true;
            }

            sub_buffer_ = _null_;
            return false;
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        void Memory<Type>::copy(const Type *buffer_, GraVitoN::gsize size_, GraVitoN::gsize position_)
        {
            // cout << "copying" << endl;
            
            if( size_ <= 0 || buffer_ == _null_ || position_ < 0 )
            {
                buffer = _null_;
                buff_size = 0;
                return;
            }
            else if( (position_ + size_) != buff_size )
            {
                Memory<Type>::resize(position_ + size_);
            }
            memcpy( (void*) (buffer + position_), (void *) buffer_, size_);

            // cout << "done" << endl;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type> & Memory<Type>::operator = (const Memory<Type> &a)
        {
            this->copy(a.address(), a.size());
            return *this;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type> & Memory<Type>::operator = (const std::string &str)
        {
            this->copy((Type*)str.c_str(), str.size());
            return *this;
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        void Memory<Type>::set(Type value)
        {
            if( buffer && buff_size > 0 )
                memset(buffer, value, buff_size);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        void Memory<Type>::zero()
        {
            Memory<Type>::set( 0x00 );
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Type* Memory<Type>::address(const GraVitoN::gsize position_) const
        {
            if( !buffer || position_ >=  buff_size || position_ < 0 )
            {
                // cout << "dead beef" << endl;
                return _null_;
            }
            // cout << "still alive" << endl;
            return (buffer + position_);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        GraVitoN::gsize Memory<Type>::size() const
        {
            return buff_size;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        void Memory<Type>::free()
        {
            // printf("%x\n", buffer);

            if( buffer )
            {
#if defined(INFO_OS_LINUX)
                munmap(buffer, buff_size);
#elif defined(INFO_OS_WINDOWS)
                VirtualFreeEx(GetCurrentProcess(), (LPVOID)buffer, buff_size, MEM_RELEASE);
#else
                ::free(buffer);
#endif
            }

            buffer = _null_;
            buff_size = 0;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        std::string Memory<Type>::toString(const GraVitoN::gsize position_) const
        {
            if ( buffer == _null_  || buff_size <= 0 || position_ < 0 ) //|| position_ >= buff_size )
            {
                return string("");
            }

            std::string res;
            for(gsize i = position_; i < buff_size; ++i)
                res += (char)*(buffer+i);

            return res;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Type * Memory<Type>::operator + (const gsize &index) const
        {
            return Memory<Type>::address(index);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::operator Type * () const
        {
            return this->address();
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        bool Memory<Type>::operator ! () const
        {
            return (buffer != _null_) && (buff_size > 0);
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Type & Memory<Type>::operator [] (const gsize &index) const
        {
            if( !buffer )
            {
                throw( std::logic_error("Memory: index operator - trying to access an empty buffer") );
            }
            return buffer[ ((index % buff_size) + buff_size) % buff_size ];
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        std::ostream& operator << (std::ostream& stream, const Memory<Type> &m)
        {
            gsize i = 0;
            while( (m + i) )
            {
                stream  << (char)*(m + i);
                ++i;
            }
            return stream;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        std::istream& operator >> (std::istream& stream, Memory<Type> &m)
        {
            string str;
            stream >> str;
            m.copy((Type*)str.c_str(), str.size());
            return stream;
        }
        
    } /// end of Core
} /// end of GraVitoN

#endif /// end of GRAVITON_MEMORY_H
