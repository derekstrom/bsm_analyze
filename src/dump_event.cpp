// Dump Event content given the run/lumi and event numbers
// (multi-threads)
//
// Created by Samvel Khalatyan, Jul 07, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "interface/DumpEventAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;
using boost::lexical_cast;
using boost::regex;
using boost::smatch;

namespace po = boost::program_options;

using bsm::DumpEventAnalyzer;
using bsm::ThreadController;

typedef shared_ptr<DumpEventAnalyzer> DumpEventAnalyzerPtr;
typedef shared_ptr<ThreadController> ControllerPtr;

void run(const po::variables_map &, ControllerPtr &);

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

            ("event,e",
             po::value<vector<string> >(),
             "event selection [repeatable]. Format: event[:lumi[:run]]")
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

            run(arguments, controller);
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

void run(const po::variables_map &arguments, ControllerPtr &controller)
try
{
    // Prepare Analysis
    //
    DumpEventAnalyzerPtr analyzer(new DumpEventAnalyzer());

    if (arguments.count("event"))
    {
        const vector<string> &events = arguments["event"].as<vector<string> >();
        for(vector<string>::const_iterator event = events.begin();
                events.end() != event;
                ++event)
        {
            smatch matches;
            regex pattern("^(\\d+)(?::(\\d+)(?::(\\d+))?)?$");

            if (!regex_match(*event, matches, pattern))
            {
                cerr << "Didn't understand event: " << *event << endl;

                continue;
            }

            uint32_t id = lexical_cast<uint32_t>(matches[1]);
            uint32_t lumi = matches[2].matched
                ? lexical_cast<uint32_t>(matches[2])
                : 0;

            uint32_t run = matches[3].matched
                ? lexical_cast<uint32_t>(matches[3])
                : 0;

            analyzer->addEvent(id, lumi, run);
        }
    }

    // Process inputs
    //
    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;
}
catch(...)
{
}
