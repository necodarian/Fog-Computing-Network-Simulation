#include <omnetpp.h>
#include <sstream>
#include <iomanip>
#include "SmartPhone.h"
#include "GarbageCan.h"
#include "Cloud.h"

using namespace omnetpp;

class InfoDisplay : public cSimpleModule
{
  private:
    std::string mode;
    int fastDelay;
    int slowDelay;
    
  protected:
    virtual void initialize() override {
        mode = par("mode").stdstringValue();
        fastDelay = (int)(par("fastDelay").doubleValue() * 1000);
        slowDelay = (int)(par("slowDelay").doubleValue() * 1000);
        
        displayInitialMessage();
    }
    
    void displayInitialMessage() {
        std::ostringstream text;
        std::string title;

        if (mode == "cloud") {
            title = "Cloud-based solution with slow messages";
        } else if (mode == "fog") {
            title = "Fog-based solution with fast messages";
        } else {
            title = "No garbage solution";
        }

        text << "Simulation in progress...\n";
        text << "Statistics will be calculated after simulation completes.";

        cCanvas *canvas = getParentModule()->getCanvas();

        cTextFigure *titleFig = new cTextFigure("infoTitle");
        titleFig->setText(title.c_str());
        titleFig->setPosition(cFigure::Point(1060, 20));
        titleFig->setAnchor(cFigure::ANCHOR_N);
        titleFig->setFont(cFigure::Font("", 12, cFigure::FONT_BOLD));
        titleFig->setColor(cFigure::BLACK);
        canvas->addFigure(titleFig);

        cTextFigure *contentFig = new cTextFigure("infoContent");
        contentFig->setText(text.str().c_str());
        contentFig->setPosition(cFigure::Point(1060, 45));
        contentFig->setAnchor(cFigure::ANCHOR_N);
        contentFig->setFont(cFigure::Font("", 10));
        contentFig->setColor(cFigure::BLACK);
        canvas->addFigure(contentFig);
    }

    virtual void handleMessage(cMessage *msg) override {
        delete msg;
    }
    
    virtual void finish() override {
        cModule *network = getParentModule();
        Smartphone *host = check_and_cast<Smartphone*>(network->getSubmodule("host")->getSubmodule("app"));
        GarbageCan *can = check_and_cast<GarbageCan*>(network->getSubmodule("can"));
        GarbageCan *anotherCan = check_and_cast<GarbageCan*>(network->getSubmodule("anotherCan"));
        Cloud *cloud = check_and_cast<Cloud*>(network->getSubmodule("cloud"));
        
        int sentHostFast = host->getSentHostFast();
        int rcvdHostFast = host->getRcvdHostFast();
        int sentHostSlow = host->getSentHostSlow();
        int rcvdHostSlow = host->getRcvdHostSlow();
        
        int sentCanMessages = can->getSentMessages();
        int rcvdCanMessages = can->getRcvdMessages();
        
        int sentAnotherCanMessages = anotherCan->getSentMessages();
        int rcvdAnotherCanMessages = anotherCan->getRcvdMessages();
        
        int sentCloudFast = cloud->getSentCloudFast();
        int rcvdCloudFast = cloud->getRcvdCloudFast();
        int sentCloudSlow = cloud->getSentCloudSlow();
        int rcvdCloudSlow = cloud->getRcvdCloudSlow();
        
        int slowHostToOthers = sentHostSlow * slowDelay;
        int slowOthersToHost = rcvdHostSlow * slowDelay;
        int fastHostToOthers = sentHostFast * fastDelay;
        int fastOthersToHost = rcvdHostFast * fastDelay;
        
        int canToOthers = sentCanMessages * fastDelay;
        int othersToCan = rcvdCanMessages * fastDelay;
        
        int anotherCanToOthers = sentAnotherCanMessages * fastDelay;
        int othersToAnotherCan = rcvdAnotherCanMessages * fastDelay;
        
        int slowCloudToOthers = sentCloudSlow * slowDelay;
        int slowOthersToCloud = rcvdCloudSlow * slowDelay;
        int fastCloudToOthers = sentCloudFast * fastDelay;
        int fastOthersToCloud = rcvdCloudFast * fastDelay;
        
        updateFinalDisplay(slowHostToOthers, slowOthersToHost, fastHostToOthers, fastOthersToHost,
                          canToOthers, othersToCan, anotherCanToOthers, othersToAnotherCan,
                          slowCloudToOthers, slowOthersToCloud, fastCloudToOthers, fastOthersToCloud);
    }
    
    void updateFinalDisplay(int slowHostToOthers, int slowOthersToHost, int fastHostToOthers, int fastOthersToHost,
                           int canToOthers, int othersToCan, int anotherCanToOthers, int othersToAnotherCan,
                           int slowCloudToOthers, int slowOthersToCloud, int fastCloudToOthers, int fastOthersToCloud) {
        std::ostringstream text;
        std::string title;

        if (mode == "cloud") {
            title = "Cloud-based solution with slow messages";
        } else if (mode == "fog") {
            title = "Fog-based solution with fast messages";
        } else {
            title = "No garbage solution";
        }

        text << "Slow connection from the smartphone to others (time it takes) = " << slowHostToOthers << "\n";
        text << "Slow connection from others to the smartphone (time it takes) = " << slowOthersToHost << "\n";
        text << "Fast connection from the smartphone to others (time it takes) = " << fastHostToOthers << "\n";
        text << "Fast connection from others to the smartphone (time it takes) = " << fastOthersToHost << "\n\n";
        
        text << "Connection from the can to others (time it takes) = " << canToOthers << "\n";
        text << "Connection from others to the can (time it takes) = " << othersToCan << "\n\n";
        
        text << "Connection from the anotherCan to others (time it takes) = " << anotherCanToOthers << "\n";
        text << "Connection from others to the anotherCan (time it takes) = " << othersToAnotherCan << "\n\n";
        
        text << "Slow connection from the Cloud to others (time it takes) = " << slowCloudToOthers << "\n";
        text << "Slow connection from others to the Cloud (time it takes) = " << slowOthersToCloud << "\n";
        text << "Fast connection from the Cloud to others (time it takes) = " << fastCloudToOthers << "\n";
        text << "Fast connection from others to the Cloud (time it takes) = " << fastOthersToCloud;

        cCanvas *canvas = getParentModule()->getCanvas();
        
        cFigure *oldTitle = canvas->getFigure("infoTitle");
        if (oldTitle) canvas->removeFigure(oldTitle);
        
        cFigure *oldContent = canvas->getFigure("infoContent");
        if (oldContent) canvas->removeFigure(oldContent);

        cTextFigure *titleFig = new cTextFigure("infoTitle");
        titleFig->setText(title.c_str());
        titleFig->setPosition(cFigure::Point(1060, 20));
        titleFig->setAnchor(cFigure::ANCHOR_N);
        titleFig->setFont(cFigure::Font("", 12, cFigure::FONT_BOLD));
        titleFig->setColor(cFigure::BLACK);
        canvas->addFigure(titleFig);

        cTextFigure *contentFig = new cTextFigure("infoContent");
        contentFig->setText(text.str().c_str());
        contentFig->setPosition(cFigure::Point(1060, 45));
        contentFig->setAnchor(cFigure::ANCHOR_N);
        contentFig->setFont(cFigure::Font("", 10));
        contentFig->setColor(cFigure::BLACK);
        canvas->addFigure(contentFig);
        
        EV << "InfoDisplay: Final statistics calculated and displayed\n";
    }
};

Define_Module(InfoDisplay);