// Read plot from file, scale it, and write back
//
// Created by Samvel Khalatyan, Jun 08, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <TFile.h>
#include <TH1.h>

using namespace std;
using namespace boost;

// usage: merge LUMI CROSS_SECTION EVENTS FILTER_EFF file.root:/path/to/histogram
//
int main(int argc, char *argv[])
{
    if (6 != argc)
    {
        cout << argc << endl;
        cerr << "Usage: " << argv[0]
            << " lumi x-section events eff_filter file.root:/path/to/histogram"
            << endl;

        return 1;
    }

    const double lumi = lexical_cast<double>(argv[1]);
    const double xsec = lexical_cast<double>(argv[2]);
    const uint32_t events = lexical_cast<uint32_t>(argv[3]);
    if (!events)
    {
        cerr << "number of events can not be Zero" << endl;

        return 1;
    }

    const double filter = lexical_cast<double>(argv[4]);
    const double scale = lumi * xsec / events * filter;

    smatch matches;
    string file(argv[5]);
    if (!regex_match(file, matches, regex("^(.+?\\.root):(.+)$")))
    {
        cerr << "didn't understand file argument: " << argv[5] << endl;

        return 1;
    }

    TFile *input = TFile::Open(matches[1].str().c_str(), "update");
    if (!input->IsOpen())
    {
        cerr << "failed to open file: " << matches[1] << endl;

        return 1;
    }

    TH1 *hist = dynamic_cast<TH1 *>(input->Get(matches[2].str().c_str()));
    if (!hist)
    {
        cerr << "failed to extract histogram: " << matches[2] << endl;

        return 1;
    }

    hist->Scale(scale);

    hist->Write();

    input->Close();

    return 0;
}
