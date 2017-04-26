/**
 * =====================================================================================
 *       @file  Singleton.hpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  09/14/2011 20:37:21 PM 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  TaoMee.Inc, ShangHai.
 *   Copyright  Copyright (c) 2011, TaoMee.Inc, ShangHai.
 *
 *     @author  kevinwu(), kevinwu@taomee.com
 * This source code is wrote for TaoMee,Inc. ShangHai CN.
 * =====================================================================================
 */
#ifndef FAIRY_SINGLETON_HPP
#define FAIRY_SINGLETON_HPP

template <class T >
class Singleton 
{
    public:
        static T& get_instance() { 
            if(!m_singleton)
                m_singleton = new T();
            return *m_singleton;
        }
		static T* get_instance_ptr(){
			if(!m_singleton)
				m_singleton = new T();
			return m_singleton;
		}
        static void del_instance(){
            if(m_singleton) {
                delete m_singleton;
                m_singleton = NULL;
            }
        }
    private:
        static T* m_singleton;
};
template<class T >
T* Singleton<T>::m_singleton;

#endif
