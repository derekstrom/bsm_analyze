// Event decay plots
//
// Two decay level plots are generated. It is assumed the decay starts from
// the t, tbar. Plots are 2D: Yaxis is the decaying particle, Xaxis is the decay
// product
//
// Created by Samvel Khalatyan, Jun 29, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH2.h>
#include <TLatex.h>
#include <TRint.h>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "bsm_stat/interface/Utility.h"
#include "interface/DecayAnalyzer.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using namespace bsm;

typedef shared_ptr<DecayAnalyzer> DecayAnalyzerPtr;
typedef boost::shared_ptr<TLatex> LatexPtr;

LatexPtr createLabel();

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
        DecayAnalyzerPtr analyzer(new DecayAnalyzer());
        for(int i = 1; argc > i; ++i)
        {
            cout << argv[i] << endl;
            shared_ptr<Reader> reader(new Reader(argv[i]));
            reader->open();
            if (!reader->isOpen())
                continue;

            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(event);
                    ++events_read)
            {
                analyzer->process(event.get());

                event->Clear();
            }
        }

        cout << endl;
        cout << *analyzer << endl;

        {
            int empty_argc = 1;
            char *empty_argv[] = { argv[0] };

            boost::shared_ptr<TRint>
                app(new TRint("app", &empty_argc, empty_argv));

            boost::shared_ptr<TCanvas> canvas(new TCanvas("decays",
                        "Decays L1, L2", 1024, 480));
            canvas->Divide(2, 1);

            typedef stat::TH1Ptr TH1Ptr;

            canvas->cd(1);
            TH1Ptr decay_level_1 = convert(*analyzer->decay_level_1());
            decay_level_1->SetTitle("Decay L1");
            decay_level_1->GetYaxis()->SetTitle("Parent ID_{PDG}");
            decay_level_1->GetXaxis()->SetTitle("Product ID_{PDG}");
            decay_level_1->Draw("colz");

            LatexPtr label1 = createLabel();
            label1->Draw();

            LatexPtr label1_2(new TLatex(3.570061, 23.08044, 
                "Decay L1"));
            label1_2->SetNDC();
            label1_2->SetTextAlign(13);
            label1_2->SetX(0.3);
            label1_2->SetY(0.758);
            label1_2->Draw();

            canvas->cd(2);
            TH1Ptr decay_level_2 = convert(*analyzer->decay_level_2());
            decay_level_2->SetTitle("Decay L2");
            decay_level_2->GetYaxis()->SetTitle("Parent ID_{PDG}");
            decay_level_2->GetXaxis()->SetTitle("Product ID_{PDG}");
            decay_level_2->Draw("colz");

            LatexPtr label2 = createLabel();
            label2->Draw();

            LatexPtr label2_2(new TLatex(3.570061, 23.08044, 
                "Decay L2"));
            label2_2->SetNDC();
            label2_2->SetTextAlign(13);
            label2_2->SetX(0.3);
            label2_2->SetY(0.758);
            label2_2->Draw();

            app->Run();
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

LatexPtr createLabel()
{
    LatexPtr label(new TLatex(3.570061, 23.08044, 
        "#splitline{CMS: Work in Progress}{230 pb^{-1} at #sqrt{s} = 7 TeV}"));
    label->SetNDC();
    label->SetTextAlign(13);
    label->SetX(0.3);
    label->SetY(0.908);

    return label;
}
