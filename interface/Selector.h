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
    class Jet;
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

                // Enable disable all cuts
                //
                virtual void enable() = 0;
                virtual void disable() = 0;
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

                virtual void enable();
                virtual void disable();

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

        class JetSelector : public Selector
        {
            public:
                typedef boost::shared_ptr<Cut> CutPtr;

                JetSelector();
                virtual ~JetSelector();

                // Test if object passes the selector
                //
                virtual bool operator()(const Jet &);

                // Cuts accessors
                //
                CutPtr pt() const;
                CutPtr eta() const;

                // Selector interface
                //
                virtual void print(std::ostream &) const;

                virtual SelectorPtr clone() const;
                virtual void merge(const SelectorPtr &);

                virtual void enable();
                virtual void disable();

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                CutPtr _pt;
                CutPtr _eta;

                // Temporary variable that is used to convert
                // bsm::LorentzVector to TLorentzVector
                //
                P4 _p4;
        };

        class WJetSelector : public Selector
        {
            public:
                typedef boost::shared_ptr<Cut> CutPtr;

                WJetSelector();
                virtual ~WJetSelector();

                // Test if object passes the selector
                //
                virtual bool operator()(const Jet &);

                // Cuts accessors
                //
                CutPtr children() const;
                CutPtr pt() const;
                CutPtr mass_drop() const;
                CutPtr mass_lower_bound() const;
                CutPtr mass_upper_bound() const;

                // Selector interface
                //
                virtual void print(std::ostream &) const;

                virtual SelectorPtr clone() const;
                virtual void merge(const SelectorPtr &);

                virtual void enable();
                virtual void disable();

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                CutPtr _children;
                CutPtr _pt;
                CutPtr _mass_drop;
                CutPtr _mass_lower_bound;
                CutPtr _mass_upper_bound;;

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
                CutPtr pt() const;
                CutPtr eta() const;
                CutPtr is_global() const;
                CutPtr is_tracker() const;
                CutPtr muon_segments() const;
                CutPtr muon_hits() const;
                CutPtr muon_normalized_chi2() const;
                CutPtr tracker_hits() const;
                CutPtr pixel_hits() const;
                CutPtr d0_bsp() const;
                CutPtr primary_vertex() const;

                // Selector interface
                //
                virtual void print(std::ostream &) const;

                virtual SelectorPtr clone() const;
                virtual void merge(const SelectorPtr &);

                virtual void enable();
                virtual void disable();

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                CutPtr _pt;
                CutPtr _eta;
                CutPtr _is_global;
                CutPtr _is_tracker;
                CutPtr _muon_segments;
                CutPtr _muon_hits;
                CutPtr _muon_normalized_chi2;
                CutPtr _tracker_hits;
                CutPtr _pixel_hits;
                CutPtr _d0_bsp;
                CutPtr _primary_vertex;

                // Temporary variable that is used to convert
                // bsm::LorentzVector to TLorentzVector
                //
                P4 _p4;
        };

        class MultiplicityCutflow
        {
            public:
                typedef boost::shared_ptr<MultiplicityCutflow> CutflowPtr;
                typedef boost::shared_ptr<selector::Cut> CutPtr;

                MultiplicityCutflow(const uint32_t &max);
                virtual ~MultiplicityCutflow();

                // Test if selector is passed
                //
                virtual void operator()(const uint32_t &);

                // Cuts accessors
                //
                CutPtr cut(const uint32_t &) const;

                // Cutflow interface
                //
                virtual void print(std::ostream &) const;

                virtual CutflowPtr clone() const;
                virtual void merge(const CutflowPtr &);

            private:
                typedef std::vector<CutPtr> Cuts;

                Cuts _cuts;
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
                LockSelectorEventCounterOnUpdate(JetSelector &);
                LockSelectorEventCounterOnUpdate(WJetSelector &);
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
        std::ostream &operator <<(std::ostream &, const MultiplicityCutflow &);
    }

    using selector::ElectronSelector;
    using selector::JetSelector;
    using selector::MuonSelector;
    using selector::MultiplicityCutflow;
    using selector::WJetSelector;
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
