#include <omnetpp.h>
using namespace omnetpp;

#include "SmartPhone.h"

void Smartphone::updateDisplay() {
    char buf[200];
    sprintf(buf, "sentHostFast: %d rcvdHostFast: %d sentHostSlow: %d rcvdHostSlow: %d",
            sentHostFast, rcvdHostFast, sentHostSlow, rcvdHostSlow);
    
    getParentModule()->getDisplayString().setTagArg("t", 0, buf);
    getParentModule()->getDisplayString().setTagArg("t", 1, "l");
}

void Smartphone::initialize() {
    mode = getParentModule()->par("mode").stdstringValue();
    EV << "Smartphone init. mode=" << mode << "\n";
    pollEvent = new cMessage("pollEvent");
    scheduleAt(simTime() + 1, pollEvent);
    updateDisplay();
}

void Smartphone::handleMessage(cMessage *msg) {
    if (msg == pollEvent) {
        if (waitingCloudAck) return;

        if (phase == 0) {
            EV << "Smartphone polling TOP can, attempt " << (attempt+1) << "\n";
            send(new cMessage("1-Is the can full?"), "out", 0);
            sentHostFast++;
            updateDisplay();
        } else {
            EV << "Smartphone polling BOTTOM can, attempt " << (attempt+1) << "\n";
            send(new cMessage("4-Is the can full?"), "out", 1);
            sentHostFast++;
            updateDisplay();
        }

        attempt++;
        scheduleAt(simTime() + 2, pollEvent);
        return;
    }

    const char* n = msg->getName();
    EV << "Smartphone received: " << n << "\n";

    if (strcmp(n, "3-YES") == 0 && phase == 0) {
        rcvdHostFast++;
        updateDisplay();
        cancelEvent(pollEvent);
        if (mode == "cloud") {
            EV << "TOP can full -> requesting collection via SLOW link\n";
            waitingCloudAck = true;
            send(new cMessage("7-Collect garbage"), "out", 2);
            sentHostSlow++;
            updateDisplay();
        } else {
            phase = 1; attempt = 0;
            scheduleAt(simTime() + 3, pollEvent);
        }
    }
    else if (strcmp(n, "6-YES") == 0 && phase == 1) {
        rcvdHostFast++;
        updateDisplay();
        cancelEvent(pollEvent);
        if (mode == "cloud") {
            EV << "BOTTOM can full -> requesting collection via SLOW link\n";
            waitingCloudAck = true;
            send(new cMessage("9-Collect garbage"), "out", 2);
            sentHostSlow++;
            updateDisplay();
        } else {
            EV << "Polling finished in fog mode.\n";
        }
    }
    else if (strcmp(n, "8-OK") == 0 && waitingCloudAck && phase == 0) {
        rcvdHostSlow++;
        updateDisplay();
        EV << "Cloud OK for TOP can. Moving on.\n";
        waitingCloudAck = false;
        phase = 1; attempt = 0;
        scheduleAt(simTime() + 3, pollEvent);
    }
    else if (strcmp(n, "10-OK") == 0 && waitingCloudAck && phase == 1) {
        rcvdHostSlow++;
        updateDisplay();
        EV << "Cloud OK for BOTTOM can. All done.\n";
        waitingCloudAck = false;
    }
    else if (strcmp(n, "2-NO") == 0 && phase == 0) {
        rcvdHostFast++;
        updateDisplay();
        EV << "TOP can says NO (no-garbage); moving to bottom can\n";
        cancelEvent(pollEvent);
        phase = 1;
        attempt = 0;
        scheduleAt(simTime() + 3, pollEvent);
    }
    else if (strcmp(n, "5-NO") == 0 && phase == 1) {
        rcvdHostFast++;
        updateDisplay();
        EV << "BOTTOM can says NO (no-garbage); finished\n";
        cancelEvent(pollEvent);
    }

    delete msg;
}

void Smartphone::finish() {
    if (pollEvent) { cancelAndDelete(pollEvent); pollEvent = nullptr; }
}

Define_Module(Smartphone);