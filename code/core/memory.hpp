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
            
        public:
            Memory(const GraVitoN::gsize size_ = 0, bool executable_ = false);
            Memory(const Type *buffer_, const GraVitoN::gsize size);
            /// Copy Memory object
            Memory(const Memory<Type> &a_);

            /// Copy string to memory
            Memory(const string &str);
            
            /// Destructor
            ~Memory();

            /// alloc
            bool alloc(GraVitoN::gsize size_, bool executable_ = false);
            
            /// free
            void free();
            
            /// set memory
            void set(Type value_);
            
            /// fill buffer with
            void zero();

            /// copy buffer
            void copy(const Type *buffer_, GraVitoN::gsize size_);

            /// get buffer address
            Type * address() const;
            
            /// size of buffer
            GraVitoN::gsize size() const;

            /// Memory = Memory operator
            Memory<Type> & operator = (const Memory<Type> &a);

            /// Memory = Memory operator
            Memory<Type> & operator = (const std::string &str);
            
            /// convert buffer to string
            std::string toString();
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const GraVitoN::gsize size_, bool executable_)
        {
            Memory<Type>::alloc(size_, executable_);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const Memory<Type> &a_)
        {
            Memory<Type>::copy(a_.address(), a_.size());
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const string &str)
        {
            Memory<Type>::copy((Type*)str.c_str(), str.size());
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Memory<Type>::Memory(const Type *buffer_, const GraVitoN::gsize size_)
        {
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
        void Memory<Type>::copy(const Type *buffer_, GraVitoN::gsize size_)
        {
            if( size_ <= 0 || buffer_ == _null_ )
            {
                buffer = _null_;
                buff_size = 0;
                return;
            }
            else if( size_ != buff_size )
            {
                Memory<Type>::free();
                Memory<Type>::alloc(size_);
            }
            memcpy( (void*) buffer, (void *) buffer_, size_);
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
            memset(buffer, value, buff_size);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        void Memory<Type>::zero()
        {
            memset(buffer, 0, buff_size);
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class Type>
        Type* Memory<Type>::address() const
        {
            return buffer;
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
        std::string Memory<Type>::toString()
        {
            if( buffer == _null_ || buff_size <= 0 )
                return "";
            return string((char*)buffer, buff_size);
        }
        
    } /// end of Core
} /// end of GraVitoN

#endif /// end of GRAVITON_MEMORY_H
