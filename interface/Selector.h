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

#include <boost/shared_ptr.hpp>

class TLorentzVector;

namespace bsm
{
    class Electron;
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

                // Return Cut counter
                //
                operator uint32_t() const;

                // Get actual cut value
                //
                double value() const;

                // apply cut: implicitly count number of success
                //
                bool operator()(const double &);

            private:
                // isPass is the actual application of the cut
                //
                virtual bool isPass(const double &) = 0;

                double _value;
                uint32_t _count;
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
        std::ostream &operator <<(std::ostream &, const Selector &);
    }
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
