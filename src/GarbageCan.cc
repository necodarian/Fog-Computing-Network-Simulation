#include <omnetpp.h>
using namespace omnetpp;

#include "GarbageCan.h"

void GarbageCan::updateDisplay() {
    char buf[200];
    bool top = strstr(getFullName(), "another") == nullptr;
    
    if (top) {
        sprintf(buf, "sentCanFast: %d rcvdCanFast: %d numberOfLostCanMsgs: %d",
                sentMessages, rcvdMessages, lostMessages);
    } else {
        sprintf(buf, "sentAnotherCanFast: %d rcvdAnotherCanFast: %d numberOfLostAnotherCanMsgs: %d",
                sentMessages, rcvdMessages, lostMessages);
    }
    
    getDisplayString().setTagArg("t", 0, buf);
    getDisplayString().setTagArg("t", 1, "l");
}

void GarbageCan::initialize() {
    mode = getParentModule()->par("mode").stdstringValue();
    EV << "GarbageCan init. mode=" << mode << " path=" << getFullPath() << endl;
    updateDisplay();
}

void GarbageCan::handleMessage(cMessage *msg) {
    const char* n = msg->getName();
    EV << getFullPath() << " got: " << n << endl;

    bool top = strstr(getFullName(), "another") == nullptr;

    if ((strcmp(n, "1-Is the can full?") == 0 && top) ||
        (strcmp(n, "4-Is the can full?") == 0 && !top))
    {
        dropCount++;
        if (dropCount <= 3) {
            EV << "Dropping probe #" << dropCount << endl;
            bubble("Message dropped!");
            lostMessages++;
            updateDisplay();
            delete msg;
            return;
        }

        rcvdMessages++;
        updateDisplay();

        if (mode == "none") {
            const char* noMsg = top ? "2-NO" : "5-NO";
            EV << "Replying " << noMsg << " (can is NOT full)" << endl;
            send(new cMessage(noMsg), "out", 0);
            sentMessages++;
            updateDisplay();
            delete msg;
            return;
        }

        const char* yesMsg = top ? "3-YES" : "6-YES";
        EV << "Replying " << yesMsg << " to smartphone" << endl;
        send(new cMessage(yesMsg), "out", 0);
        sentMessages++;
        updateDisplay();
        delete msg;

        if (mode == "fog") {
            const char* collectMsg = top ? "7-Collect garbage" : "9-Collect garbage";
            EV << "Sending " << collectMsg << " to cloud" << endl;
            send(new cMessage(collectMsg), "out", 1);
            sentMessages++;
            updateDisplay();
        }
        return;
    }

    rcvdMessages++;
    updateDisplay();
    EV << getFullPath() << " received from cloud: " << n << endl;
    delete msg;
}

Define_Module(GarbageCan);