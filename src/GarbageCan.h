#ifndef GARBAGECAN_H
#define GARBAGECAN_H

#include <omnetpp.h>
using namespace omnetpp;

class GarbageCan : public cSimpleModule
{
  private:
    std::string mode;
    int dropCount = 0;
    int sentMessages = 0;
    int rcvdMessages = 0;
    int lostMessages = 0;

  public:
    int getSentMessages() const { return sentMessages; }
    int getRcvdMessages() const { return rcvdMessages; }
    int getLostMessages() const { return lostMessages; }

  protected:
    void updateDisplay();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif