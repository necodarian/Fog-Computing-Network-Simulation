package in5700groupprojectclean;

import ned.DatarateChannel;
import inet.visualizer.canvas.integrated.IntegratedCanvasVisualizer;
import inet.mobility.single.TurtleMobility;
import inet.mobility.contract.IMobility;


channel FastLink extends DatarateChannel
{
    delay = default(35ms);
    datarate = default(150Mbps);
}

channel SlowLink extends DatarateChannel
{
    delay = default(300ms);
    datarate = default(5Mbps);
}


simple GarbageCan
{
    parameters:
        @display("i=block/bucket");
        @display("p=50,79;r=130");
    gates:
        input in[];
        output out[];
}

simple Smartphone
{
    parameters:
        @display("i=device/cellphone");
    gates:
        input in[];
        output out[];
}

simple Cloud
{
    parameters:
        @display("i=block/app2");
        @display("p=50,79;r=500");
    gates:
        input in[];
        output out[];
}

simple InfoDisplay
{
    parameters:
        @display("");
        string mode;
        double fastDelay @unit(s);
        double slowDelay @unit(s);
}

module SmartphoneWithMobility
{
    parameters:
        @display("i=device/cellphone");
        @display("p=50,79;r=130");
        string mobilityType = default("TurtleMobility");
        string mode = default("cloud");  // Add mode parameter here
    gates:
        input in[];
        output out[];
    submodules:
        app: Smartphone {
            gates:
                in[sizeof(parent.in)];
                out[sizeof(parent.out)];
        }
        mobility: <mobilityType> like IMobility {
            @display("p=50,150;is=s");
        }
    connections:
        for i=0..sizeof(in)-1 {
            in[i] --> app.in[i];
        }
        for i=0..sizeof(out)-1 {
            app.out[i] --> out[i];
        }


}


network GarbageCollectionSystem
{
    parameters:
        string mode = default("cloud"); // "cloud" | "fog" | "none"
        int gsize = default(3);

        double fastDelay @unit(s) = default(10ms);
        double slowDelay @unit(s) = default(200ms);
        double fastRate @unit(bps) = default(100Mbps);
        double slowRate @unit(bps) = default(1Mbps);






		        // Baner (svarte linjer)
        @figure[topLane1](type=line; points=80,80,700,80; lineWidth=3);
        @figure[topLane2](type=line; points=180,180,700,180; lineWidth=3);

        @figure[dropLane](type=line; points=80,80,80,460; lineWidth=3);
        @figure[dropLane2](type=line; points=180,180,180,360; lineWidth=3);

        @figure[botLane1](type=line; points=80,460,700,460; lineWidth=3);
        @figure[botLane2](type=line; points=180,360,700,360; lineWidth=3);



        @display("bgb=1328,533");
    submodules:
        can: GarbageCan {
            @display("p=182,60");
            gates:
                in[parent.gsize];
                out[parent.gsize];
        }
        anotherCan: GarbageCan {
            @display("p=248,310");
            gates:
                in[parent.gsize];
                out[parent.gsize];
        }
        host: SmartphoneWithMobility {
            @display("p=618,500");
            mode = parent.mode;  // Pass mode from network to host
            gates:
                in[parent.gsize];
                out[parent.gsize];
        }
        cloud: Cloud {
            @display("p=705,271");
            gates:
                in[parent.gsize];
                out[parent.gsize];
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=555,36");
        }
        infoDisplay: InfoDisplay {
            @display("p=1250,448");
            mode = parent.mode;
            fastDelay = parent.fastDelay;
            slowDelay = parent.slowDelay;
        }

    connections allowunconnected:
        host.out[0] --> FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } --> can.in[0];
        host.in[0] <-- FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } <-- can.out[0];

        host.out[1] --> FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } --> anotherCan.in[0];
        host.in[1] <-- FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } <-- anotherCan.out[0];


        if (mode == "cloud") {
            host.out[2] --> SlowLink {  delay = parent.slowDelay; datarate = parent.slowRate; } --> cloud.in[0];
            host.in[2] <-- SlowLink {  delay = parent.slowDelay; datarate = parent.slowRate; } <-- cloud.out[0];
        }

        if (mode == "fog") {
            can.out[1] --> FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } --> cloud.in[1];
            can.in[1] <-- FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } <-- cloud.out[1];

            anotherCan.out[1] --> FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } --> cloud.in[2];
            anotherCan.in[1] <-- FastLink {  delay = parent.fastDelay; datarate = parent.fastRate; } <-- cloud.out[2];
        }
}


