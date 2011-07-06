// Cut's framework
//
// Created by Samvel Khalatyan, Jun 02, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CUT
#define BSM_CUT

#include <boost/shared_ptr.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_core/interface/Object.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    typedef boost::shared_ptr<Counter> CounterPtr;

    // Simple counter of anything. There are two states defined: locked and
    // unlocked. If counter is locked, then any attempt to modify it will
    // silently be skipped. Counter may also automatically lock itself on
    // update
    //
    class Counter : public core::Object
    {
        public:
            Counter();

            // Get number of counts
            //
            operator uint32_t() const;

            bool isLocked() const;
            bool isLockOnUpdate() const;

            void lock();
            void lockOnUpdate();

            // Unlock counter: lockOnUpdate will be reset
            //
            void unlock();

            // Increase counter
            //
            void add();

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            virtual void merge(const ObjectPtr &);

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            Counter &operator =(const Counter &object);

            void update();

            uint32_t _count;

            bool _is_locked;
            bool _is_lock_on_update;
    };

    // Store cut value and count successfuly passed objects, events
    //
    class Cut : public core::Object
    {
        public:
            Cut(const double &value);
            Cut(const Cut &);

            const CounterPtr objects() const;
            const CounterPtr events() const;

            // Get actual cut value
            //
            double value() const;
            void setValue(const double &);

            // apply cut: implicitly count number of success
            //
            bool apply(const double &);

            bool isDisabled() const;

            void disable();
            void enable();

            // Object interface
            //
            virtual uint32_t id() const;

            // Only cuts with the same value can be merged
            //
            virtual void merge(const ObjectPtr &);

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            Cut &operator =(const Cut &);

            // isPass is the actual application of the cut
            //
            virtual bool isPass(const double &) = 0;

            double _value;
            bool _is_disabled;

            CounterPtr _objects;
            CounterPtr _events;
    };

    // Comparator has comparison policy defined with std functors:
    //  less, greater [http://goo.gl/bh9dl]
    //
    // Example:
    //  1.  boost::shared_ptr<Cut> default_cut(new Comparator<>(10));
    //      if ((*default_cut)(energy))
    //      {
    //          // Energy > 10
    //      }
    //
    //  2.  boost::shared_ptr<Cut> my_cut(
    //          new Comparator<std::less_equal<double> >(10));
    //      if ((*my_cut)(energy))
    //      {
    //          // Energy <= 10
    //      }
    //
    //  3.  cout << "Default cut (passes): " << *default_cut << endl;
    //      cout << "Default cut (> " << default_cut->value() << ")" << endl;
    //
    template<class Compare = std::greater<double> >
        class Comparator : public Cut
        {
            public:
                Comparator(const double &value);

                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;

                virtual void print(std::ostream &out) const;

            protected:
                virtual bool isPass(const double &number);

            private:
                Compare _functor;
        };

    class LockCounterOnUpdate
    {
        public:
            LockCounterOnUpdate(const CounterPtr &);
            ~LockCounterOnUpdate();

        private:
            // Prevent copying
            //
            LockCounterOnUpdate(const LockCounterOnUpdate &);
            LockCounterOnUpdate &operator =(const LockCounterOnUpdate &);

            CounterPtr _counter;
    };
}

// Template(s) implementation
//
template<class Compare>
    bsm::Comparator<Compare>::Comparator(const double &value):
        Cut(value)
{
}

template<class Compare>
    uint32_t bsm::Comparator<Compare>::id() const
{
    return core::ID<Comparator<Compare> >::get();
}

template<class Compare>
    typename bsm::Comparator<Compare>::ObjectPtr
        bsm::Comparator<Compare>::clone() const
{
    return ObjectPtr(new Comparator<Compare>(*this));
}

template<class Compare>
    bool bsm::Comparator<Compare>::isPass(const double &number)
{
    return _functor(number, value());
}

template<class Compare>
    void bsm::Comparator<Compare>::print(std::ostream &out) const
{
    out << _functor << " ";

    Cut::print(out);
}

#endif
