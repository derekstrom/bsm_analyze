#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH1.h>
#include <TRint.h>

#include "input/interface/Reader.h"
#include "input/interface/Event.pb.h"
#include "interface/JetMonitor.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::Reader;
using bsm::Event;
using bsm::JetMonitor;

int main(int argc, char *argv[])
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    shared_ptr<JetMonitor> jets(new JetMonitor());

    {
        shared_ptr<Reader> reader(new Reader(argv[1]));
        uint32_t events_read = 0;
        for(shared_ptr<Event> event(new Event());
                reader->read(*event);
                ++events_read)
        {
            jets->fill(event->jets());
            event->Clear();
        }

        cout << "Events Read: " << events_read << endl;
    }

    {
        shared_ptr<TRint> app(new TRint("app", &argc, argv));

        shared_ptr<TCanvas> canvas(new TCanvas("jets", "Jets", 800, 480));
        canvas->Divide(3);

        canvas->cd(1);
        jets->multiplicity()->Draw();

        canvas->cd(2);
        jets->leading_jet_pt()->Draw();

        canvas->cd(3);
        jets->pt()->Draw();

        app->Run();
    }

    jets.reset();

    return 0;
}
