#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH1.h>
#include <TRint.h>

#include "input/interface/Reader.h"
#include "input/interface/Event.pb.h"
#include "interface/JetMonitor.h"
#include "interface/MuonMonitor.h"
#include "interface/ElectronMonitor.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::Reader;
using bsm::Event;
using bsm::JetMonitor;
using bsm::MuonMonitor;
using bsm::ElectronMonitor;

int main(int argc, char *argv[])
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    shared_ptr<JetMonitor> jets(new JetMonitor());
    shared_ptr<MuonMonitor> muons(new MuonMonitor());
    shared_ptr<ElectronMonitor> electrons(new ElectronMonitor());

    {
        shared_ptr<Reader> reader(new Reader(argv[1]));
        uint32_t events_read = 0;
        for(shared_ptr<Event> event(new Event());
                reader->read(*event);
                ++events_read)
        {
            jets->fill(event->jets());
            muons->fill(event->muons());
            electrons->fill(event->electrons());

            event->Clear();
        }

        cout << "Events Read: " << events_read << endl;
    }

    {
        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };
        shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

        shared_ptr<TCanvas> jet_canvas(new TCanvas("jets", "Jets", 800, 480));
        jet_canvas->Divide(3);

        jet_canvas->cd(1);
        jets->multiplicity()->Draw();

        jet_canvas->cd(2);
        jets->leading_jet_pt()->Draw();

        jet_canvas->cd(3);
        jets->pt()->Draw();

        shared_ptr<TCanvas> muon_canvas(new TCanvas("muons", "Muons", 800, 480));
        muon_canvas->Divide(3);

        muon_canvas->cd(1);
        muons->multiplicity()->Draw();

        muon_canvas->cd(2);
        muons->leading_muon_pt()->Draw();

        muon_canvas->cd(3);
        muons->pt()->Draw();

        shared_ptr<TCanvas> electron_canvas(new TCanvas("electrons", "Electrons", 800, 480));
        electron_canvas->Divide(3);

        electron_canvas->cd(1);
        electrons->multiplicity()->Draw();

        electron_canvas->cd(2);
        electrons->leading_electron_pt()->Draw();

        electron_canvas->cd(3);
        electrons->pt()->Draw();

        app->Run();
    }

    jets.reset();

    return 0;
}
