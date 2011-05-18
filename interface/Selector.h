// Common Selectors Classes/functions
//
// Physics objects selectors: muons, electrons, jets
//
// Created by Samvel Khalatyan, May 16, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_SELECTOR
#define BSM_SELECTOR

#include <algorithm>
#include <iosfwd>
#include <vector>

#include <boost/shared_ptr.hpp>

class TLorentzVector;

namespace bsm
{
    class Electron;
    class Muon;
    class PrimaryVertex;

    namespace selector
    {
        // Common interface for all selectors. Each selector knows how to:
        //
        //  1. print
        //  2. clone
        //  3. merge
        //
        // the last two are required for the proper use in threads
        //
        class Selector
        {
            public:
                typedef boost::shared_ptr<Selector> SelectorPtr;

                virtual ~Selector() {}

                virtual void print(std::ostream &) const = 0;

                virtual SelectorPtr clone() const = 0;
                virtual void merge(const SelectorPtr &) = 0;
        };

        class Cut;

        class ElectronSelector : public Selector
        {
            public:
                typedef boost::shared_ptr<Cut> CutPtr;

                ElectronSelector();
                virtual ~ElectronSelector();

                // Test if electron passes the selector
                //
                virtual bool operator()(const Electron &, const PrimaryVertex &);

                // Cuts accessors
                //
                CutPtr et() const;
                CutPtr eta() const;
                CutPtr primary_vertex() const;

                // Selector interface
                //
                virtual void print(std::ostream &) const;

                virtual SelectorPtr clone() const;
                virtual void merge(const SelectorPtr &);

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                CutPtr _et;
                CutPtr _eta;
                CutPtr _primary_vertex;

                // Temporary variable that is used to convert
                // bsm::LorentzVector to TLorentzVector
                //
                P4 _p4;
        };

        class MuonSelector : public Selector
        {
            public:
                typedef boost::shared_ptr<Cut> CutPtr;

                MuonSelector();
                virtual ~MuonSelector();

                // Test if muon passes the selector
                //
                virtual bool operator()(const Muon &, const PrimaryVertex &);

                // Cuts accessors
                //
                CutPtr is_global() const;
                CutPtr is_tracker() const;
                CutPtr muon_segments() const;
                CutPtr muon_hits() const;
                CutPtr muon_normalized_chi2() const;
                CutPtr tracker_hits() const;
                CutPtr pixel_hits() const;
                CutPtr d0_bsp() const;
                CutPtr primary_vertex() const;
                CutPtr pt() const;
                CutPtr eta() const;

                // Selector interface
                //
                virtual void print(std::ostream &) const;

                virtual SelectorPtr clone() const;
                virtual void merge(const SelectorPtr &);

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                CutPtr _is_global;
                CutPtr _is_tracker;
                CutPtr _muon_segments;
                CutPtr _muon_hits;
                CutPtr _muon_normalized_chi2;
                CutPtr _tracker_hits;
                CutPtr _pixel_hits;
                CutPtr _d0_bsp;
                CutPtr _primary_vertex;
                CutPtr _pt;
                CutPtr _eta;

                // Temporary variable that is used to convert
                // bsm::LorentzVector to TLorentzVector
                //
                P4 _p4;
        };

        class Counter
        {
            public:
                Counter();

                Counter &operator +=(const Counter &);

                operator uint32_t() const;

                bool isLocked() const;
                bool isLockOnUpdate() const;

                void lock();
                void lockOnUpdate();

                void unlock();

                Counter &operator ++(); // prefix

            private:
                uint32_t _count;

                bool _is_locked;
                bool _is_lock_on_update;
        };

        class CounterLock
        {
            public:
                CounterLock(Counter &);
                ~CounterLock();

            private:
                Counter &_counter;
        };

        class CounterLockOnUpdate
        {
            public:
                CounterLockOnUpdate(Counter &);
                ~CounterLockOnUpdate();

            private:
                Counter &_counter;
        };

        class LockSelectorEventCounterOnUpdate
        {
            public:
                LockSelectorEventCounterOnUpdate(ElectronSelector &);
                LockSelectorEventCounterOnUpdate(MuonSelector &);

            private:
                typedef boost::shared_ptr<CounterLockOnUpdate> Locker;

                std::vector<Locker> _lockers;
        };

        // Cut Base: hold cut value and count success
        //
        class Cut
        {
            public:
                Cut(const double &value);
                virtual ~Cut() {}

                // Add two cut counters
                //
                Cut &operator +=(const Cut &cut);

                const Counter &objects() const;
                const Counter &events() const;

                Counter &objects();
                Counter &events();

                // Get actual cut value
                //
                double value() const;

                // apply cut: implicitly count number of success
                //
                bool operator()(const double &);

                bool isDisabled() const;

                void disable();
                void enable();

            private:
                // isPass is the actual application of the cut
                //
                virtual bool isPass(const double &) = 0;

                double _value;
                Counter _objects;
                Counter _events;

                bool _is_disabled;
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

                protected:
                    virtual bool isPass(const double &number);

                private:
                    Compare _functor;
            };

        // Helpers
        //
        std::ostream &operator <<(std::ostream &, const Cut &);
        std::ostream &operator <<(std::ostream &, const Selector &);
    }

    using selector::ElectronSelector;
    using selector::MuonSelector;
}

// Template(s) implementation
//
template<class Compare>
    bsm::selector::Comparator<Compare>::Comparator(const double &value):
        Cut(value)
{
}

template<class Compare>
    bool bsm::selector::Comparator<Compare>::isPass(const double &number)
{
    return _functor(number, value());
}

#endif
