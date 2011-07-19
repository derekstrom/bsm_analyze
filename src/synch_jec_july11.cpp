// Synchronization exercise with Jet Energy Corrections July 2011
// (multi-threads)
//
// Created by Samvel Khalatyan, Jul 14, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <TRint.h>
#include <TFile.h>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "interface/MonitorCanvas.h"
#include "interface/SynchAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using bsm::SynchJECJuly2011Analyzer;
using bsm::Reader;
using bsm::Event;
using bsm::LorentzVectorCanvas;
using bsm::ThreadController;

typedef shared_ptr<SynchJECJuly2011Analyzer> AnalyzerPtr;

void run(char *[], const po::variables_map &);

int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    int result = 0;
    try
    {
        // Prepare options
        //
        po::options_description generic_options("Allowed Options");
        generic_options.add_options()
            ("help,h",
             "Help message")

            ("interactive",
             "Interactive session")

            ("l1",
             po::value<string>(),
             "Level 1 corrections")

            ("l2",
             po::value<string>(),
             "Level 2 corrections")

            ("l3",
             po::value<string>(),
             "Level 3 corrections")

            ("mode,m",
             po::value<string>(),
             "Synchronizatin mode: muon, electron")
        ;

        po::options_description hidden_options("Hidden Options");
        hidden_options.add_options()
            ("input,i",
             po::value<vector<string> >(),
             "input file(s)")
        ;

        po::options_description cmdline_options;
        cmdline_options.add(generic_options).add(hidden_options);

        po::positional_options_description positional_options;
        positional_options.add("input", -1);

        po::variables_map arguments;
        po::store(po::command_line_parser(argc, argv).
                options(cmdline_options).
                positional(positional_options).
                run(),
                arguments);
        po::notify(arguments);

        // Use options
        //
        if (arguments.count("help"))
            cout << generic_options << endl;
        else if (!arguments.count("input"))
            cout << "Input file(s) are not specified" << endl;
        else
            run(argv, arguments);
    }
    catch(...)
    {
        cerr << "Unknown error" << endl;

        result = 1;
    }

    // Clean Up any memory allocated by libprotobuf
    //
    google::protobuf::ShutdownProtobufLibrary();

    return result;
}

void run(char *argv[],
        const po::variables_map &arguments)
try
{
    SynchJECJuly2011Analyzer::LeptonMode mode = SynchJECJuly2011Analyzer::ELECTRON;

    if (arguments.count("mode"))
    {
        string arg = arguments["mode"].as<string>();
        boost::to_lower(arg);

        if ("muon" == arg)
            mode = SynchJECJuly2011Analyzer::MUON;
        else if ("electron" != arg)
            cerr << "Unsupported mode: use Electron channel" << endl;
    }

    // Prepare Analysis
    //
    AnalyzerPtr analyzer(new SynchJECJuly2011Analyzer(mode));

    // Load corrections
    //
    {
        SynchJECJuly2011Analyzer::Corrections corrections;

        if (arguments.count("l1"))
        {
            const string &l1 = arguments["l1"].as<string>();
            if (!fs::exists(l1.c_str()))
            {
                cerr << "Level 1 corrections file does not exist: " << l1 << endl;

                return;
            }

            cout << "load l1 corrections: " << l1 << endl;
            corrections.push_back(JetCorrectorParameters(l1));
        }

        if (arguments.count("l2"))
        {
            const string &l2 = arguments["l2"].as<string>();
            if (!fs::exists(l2.c_str()))
            {
                cerr << "Level 2 corrections file does not exist: " << l2 << endl;

                return;
            }

            cout << "load l2 corrections: " << l2 << endl;
            corrections.push_back(JetCorrectorParameters(l2));
        }

        if (arguments.count("l3"))
        {
            const string &l3 = arguments["l3"].as<string>();
            if (!fs::exists(l3.c_str()))
            {
                cerr << "Level 3 corrections file does not exist: " << l3 << endl;

                return;
            }

            cout << "load l3 corrections: " << l3 << endl;
            corrections.push_back(JetCorrectorParameters(l3));
        }


        if (corrections.empty())
        {
            cerr << "jet energy corrections are not loaded" << endl;

            return;
        }

        analyzer->setJetEnergyCorrections(corrections);
    }

    shared_ptr<ThreadController> controller(new ThreadController());
    const vector<string> &inputs = arguments["input"].as<vector<string> >();
    for(vector<string>::const_iterator input = inputs.begin();
            inputs.end() != input;
            ++input)
    {
        cout << *input << endl;
        controller->push(*input);
    }

    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;

    if (arguments.count("interactive"))
    {
        // Start ROOT interactive session
        //
        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };
        shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

        if (SynchJECJuly2011Analyzer::ELECTRON == mode)
        {
            shared_ptr<LorentzVectorCanvas> electron_before_veto(new LorentzVectorCanvas("Electron Before Veto P4"));
            electron_before_veto->draw(*analyzer->electronBeforeVeto());

            shared_ptr<LorentzVectorCanvas> muon_to_veto(new LorentzVectorCanvas("Muon To Veto P4"));
            muon_to_veto->draw(*analyzer->electronToVeto());

            shared_ptr<LorentzVectorCanvas> electron_after_veto(new LorentzVectorCanvas("Electron After Veto P4"));
            electron_after_veto->draw(*analyzer->electronAfterVeto());
        }
        else
        {
            shared_ptr<LorentzVectorCanvas> muon_before_veto(new LorentzVectorCanvas("Muon Before Veto P4"));
            muon_before_veto->draw(*analyzer->electronBeforeVeto());

            shared_ptr<LorentzVectorCanvas> electron_to_veto(new LorentzVectorCanvas("Electron To Veto P4"));
            electron_to_veto->draw(*analyzer->muonToVeto());

            shared_ptr<LorentzVectorCanvas> muon_after_veto(new LorentzVectorCanvas("Muon After Veto P4"));
            muon_after_veto->draw(*analyzer->electronAfterVeto());
        }
        
        app->Run();
    }
    else
    {
        shared_ptr<TFile> output(new TFile("output.root", "RECREATE"));
        if (!output->IsOpen())
        {
            cerr << "failed to open output file" << endl;

            return;
        }

        if (SynchJECJuly2011Analyzer::ELECTRON == mode)
        {
            shared_ptr<LorentzVectorCanvas> electron_before_veto(new LorentzVectorCanvas("Electron Before Veto P4"));
            electron_before_veto->write(output.get(), *analyzer->electronBeforeVeto());

            shared_ptr<LorentzVectorCanvas> muon_to_veto(new LorentzVectorCanvas("Muon To Veto P4"));
            muon_to_veto->write(output.get(), *analyzer->muonToVeto());

            shared_ptr<LorentzVectorCanvas> electron_after_veto(new LorentzVectorCanvas("Electron After Veto P4"));
            electron_after_veto->write(output.get(), *analyzer->electronAfterVeto());
        }
        else
        {
            shared_ptr<LorentzVectorCanvas> muon_before_veto(new LorentzVectorCanvas("Muon Before Veto P4"));
            muon_before_veto->write(output.get(), *analyzer->muonBeforeVeto());

            shared_ptr<LorentzVectorCanvas> electron_to_veto(new LorentzVectorCanvas("Electron To Veto P4"));
            electron_to_veto->write(output.get(), *analyzer->electronToVeto());

            shared_ptr<LorentzVectorCanvas> muon_after_veto(new LorentzVectorCanvas("Muon After Veto P4"));
            muon_after_veto->write(output.get(), *analyzer->muonAfterVeto());
        }
    }
}
catch(...)
{
    cerr << "error" << endl;
}
