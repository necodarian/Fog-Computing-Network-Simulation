#ifndef SMARTPHONE_H
#define SMARTPHONE_H

#include <omnetpp.h>
using namespace omnetpp;

class Smartphone : public cSimpleModule
{
  private:
    cMessage* pollEvent = nullptr;
    int attempt = 0;
    int phase = 0;
    bool waitingCloudAck = false;
    std::string mode;
    
    int sentHostFast = 0;
    int rcvdHostFast = 0;
    int sentHostSlow = 0;
    int rcvdHostSlow = 0;

  public:
    // Inline getters (ok in header)
    int getSentHostFast() const { return sentHostFast; }
    int getRcvdHostFast() const { return rcvdHostFast; }
    int getSentHostSlow() const { return sentHostSlow; }
    int getRcvdHostSlow() const { return rcvdHostSlow; }

  protected:
    void updateDisplay();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif