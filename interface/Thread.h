// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CORE_THREAD
#define BSM_CORE_THREAD

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace bsm
{
    namespace core
    {
        class Condition
        {
            public:
                typedef boost::weak_ptr<boost::mutex> MutexPtr;
                typedef boost::weak_ptr<boost::condition_variable> VariablePtr;

                Condition() throw();

                MutexPtr mutex() const;
                VariablePtr variable() const;

            private:
                typedef boost::shared_ptr<boost::mutex> SharedMutexPtr;
                typedef boost::shared_ptr<boost::condition_variable> SharedVariablePtr;

                SharedMutexPtr _mutex;
                SharedVariablePtr _variable;
        };

        class Thread
        {
            public:
                Thread();
                virtual ~Thread();

                virtual void operator()() = 0;
        };
    }
}

#endif
