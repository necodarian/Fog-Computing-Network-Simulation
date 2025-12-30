#include <omnetpp.h>
using namespace omnetpp;

#include "Cloud.h"

void Cloud::updateDisplay() {
    if (mode == "none") {
        getDisplayString().setTagArg("t", 0, "");
        return;
    }
    
    char buf[200];
    sprintf(buf, "sentCloudFast: %d rcvdCloudFast: %d sentCloudSlow: %d rcvdCloudSlow: %d",
            sentCloudFast, rcvdCloudFast, sentCloudSlow, rcvdCloudSlow);
    getDisplayString().setTagArg("t", 0, buf);
    getDisplayString().setTagArg("t", 1, "l");
}

void Cloud::initialize() {
    mode = getParentModule()->par("mode").stdstringValue();
    EV << "Cloud init. mode=" << mode << "\n";
    updateDisplay();
}

void Cloud::handleMessage(cMessage *msg) {
    const char* n = msg->getName();
    //inIx gets the index of the gate so we can determine if its fast or slowlink and update counter
    int inIx = msg->getArrivalGate()->getIndex();
    EV << "Cloud got: " << n << " on in[" << inIx << "]\n";

    if (inIx == 0) {
        rcvdCloudSlow++;
    } else {
        rcvdCloudFast++;
    }
    updateDisplay();

    if (strcmp(n, "7-Collect garbage") == 0) {
        send(new cMessage("8-OK"), "out", inIx);
        if (inIx == 0) {
            sentCloudSlow++;
        } else {
            sentCloudFast++;
        }
        updateDisplay();
    } else if (strcmp(n, "9-Collect garbage") == 0) {
        send(new cMessage("10-OK"), "out", inIx);
        if (inIx == 0) {
            sentCloudSlow++;
        } else {
            sentCloudFast++;
        }
        updateDisplay();
    }

    delete msg;
}

Define_Module(Cloud);