#ifndef CLOUD_H
#define CLOUD_H

#include <omnetpp.h>
using namespace omnetpp;

class Cloud : public cSimpleModule
{
  private:
    int sentCloudFast = 0;
    int rcvdCloudFast = 0;
    int sentCloudSlow = 0;
    int rcvdCloudSlow = 0;
    std::string mode;

  public:
    int getSentCloudFast() const { return sentCloudFast; }
    int getRcvdCloudFast() const { return rcvdCloudFast; }
    int getSentCloudSlow() const { return sentCloudSlow; }
    int getRcvdCloudSlow() const { return rcvdCloudSlow; }
    
  protected:
    void updateDisplay();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif