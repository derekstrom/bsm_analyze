// Synchronization exercise July 2011
// (multi-threads)
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <TRint.h>
#include <TFile.h>

#include "bsm_input/interface/Event.pb.h"
#include "interface/MonitorCanvas.h"
#include "interface/SynchAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;

namespace po = boost::program_options;

using bsm::SynchJuly2011Analyzer;
using bsm::ThreadController;

typedef shared_ptr<SynchJuly2011Analyzer> SynchAnalyzerPtr;
typedef shared_ptr<ThreadController> ControllerPtr;

void run(char *[], const po::variables_map &, ControllerPtr &);

int main(int argc, char *argv[])
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

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

            ("mode,m",
             po::value<string>(),
             "Synchronizatin mode: muon, electron")

            ("interactive",
             "Interactive session: view plots")
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
        {
            ControllerPtr controller(new ThreadController());

            const vector<string> &inputs = arguments["input"].as<vector<string> >();
            for(vector<string>::const_iterator input = inputs.begin();
                    inputs.end() != input;
                    ++input)
            {
                cout << *input << endl;
                controller->push(*input);
            }

            run(argv, arguments, controller);
        }
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
        const po::variables_map &arguments,
        ControllerPtr &controller)
try
{
    using namespace bsm;

    SynchJuly2011Analyzer::LeptonMode mode = SynchJuly2011Analyzer::ELECTRON;

    if (arguments.count("mode"))
    {
        string arg = arguments["mode"].as<string>();
        boost::to_lower(arg);

        if ("muon" == arg)
            mode = SynchJuly2011Analyzer::MUON;
        else if ("electron" != arg)
            cerr << "Unsupported mode: use Electron channel" << endl;
    }

    // Prepare Analysis
    //
    SynchAnalyzerPtr analyzer(new SynchJuly2011Analyzer(mode));

    // Process inputs
    //
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

        shared_ptr<LorentzVectorCanvas> leading_jet(new LorentzVectorCanvas("Leading Jet P4"));
        leading_jet->draw(*analyzer->leadingJet());

        if (SynchJuly2011Analyzer::ELECTRON == mode)
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

        shared_ptr<LorentzVectorCanvas> leading_jet(new LorentzVectorCanvas("Leading Jet P4"));
        leading_jet->write(output.get(), *analyzer->leadingJet());

        if (SynchJuly2011Analyzer::ELECTRON == mode)
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
}
