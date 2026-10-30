#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

void runSimulation(string algoName, string filename, int totalPackets, double lossProb, const vector<bool>& lossPattern) {
    int cwnd = 1;
    int ssthresh = 16;

    int packetsSent = 0;
    int packetsReceived = 0;
    int packetsLost = 0;
    int retransmissions = 0;

    int packetNumber = 1;
    int round = 1;
    long long totalCwndSum = 0;

    ofstream file(filename);
    file << "Round,cwnd,ssthresh\n";

    cout << "\n============================================\n";
    cout << "   RUNNING " << algoName << " SIMULATION\n";
    cout << "============================================\n";

    while (packetNumber <= totalPackets) {
        file << round << "," << cwnd << "," << ssthresh << "\n";
        totalCwndSum += cwnd;

        cout << "\n--- Round (RTT) " << round << " | cwnd: " << cwnd << " | ssthresh: " << ssthresh << " ---\n";

        int lostThisRound = 0;

        for (int i = 0; i < cwnd && packetNumber <= totalPackets; i++) {
            packetsSent++;
            bool isLost = lossPattern[packetNumber - 1];

            cout << "Packet " << packetNumber << " : ";

            if (isLost) {
                cout << "LOST\n";
                packetsLost++;
                lostThisRound++;
            } else {
                cout << "RECEIVED - ACK\n";
                packetsReceived++;
            }

            packetNumber++;
        }

        if (lostThisRound > 0) {
            int detection = rand() % 2;
            retransmissions += lostThisRound;

            if (detection == 0) {
                cout << "*** LOSS DETECTED: 3 DUPLICATE ACKS ***\n";
                ssthresh = max(1, cwnd / 2);

                if (algoName == "TCP Reno") {
                    cwnd = ssthresh;
                    cout << "TCP Reno: Fast Recovery -> New cwnd = ssthresh (" << cwnd << ")\n";
                } else {
                    cwnd = 1;
                    cout << "TCP Tahoe: Reset cwnd to 1\n";
                }
            } else {
                cout << "*** LOSS DETECTED: TIMEOUT ***\n";
                ssthresh = max(1, cwnd / 2);
                cwnd = 1;
                cout << "Timeout -> Reset cwnd to 1, New ssthresh: " << ssthresh << "\n";
            }
        } else {
            if (cwnd < ssthresh) {
                cwnd *= 2;
                cout << "State: SLOW START -> Double cwnd to " << cwnd << "\n";
            } else {
                cwnd++;
                cout << "State: CONGESTION AVOIDANCE -> Increment cwnd to " << cwnd << "\n";
            }
        }

        round++;
    }

    file.close();

    int totalRounds = round - 1;
    double throughput = (double)packetsReceived / totalRounds;
    double lossRatio = ((double)packetsLost / packetsSent) * 100.0;
    double avgCwnd = (double)totalCwndSum / totalRounds;
    double efficiency = ((double)packetsReceived / packetsSent) * 100.0;

    cout << "\n--------------------------------------------\n";
    cout << " " << algoName << " FINAL SUMMARY\n";
    cout << "--------------------------------------------\n";
    cout << "Total RTT Rounds          : " << totalRounds << "\n";
    cout << "Packets Sent              : " << packetsSent << "\n";
    cout << "Packets Received          : " << packetsReceived << "\n";
    cout << "Packets Lost              : " << packetsLost << "\n";
    cout << "Retransmissions           : " << retransmissions << "\n";
    cout << "Throughput (Packets/RTT)  : " << throughput << "\n";
    cout << "Packet Loss Ratio         : " << lossRatio << "%\n";
    cout << "Average cwnd Size         : " << avgCwnd << " MSS\n";
    cout << "Network Efficiency        : " << efficiency << "%\n";
}

int main(int argc, char* argv[]) {
    int totalPackets = 100;
    double lossProb = 10.0;

    if (argc >= 3) {
        totalPackets = atoi(argv[1]);
        lossProb = atof(argv[2]);
    }

    // Seed once per execution
    srand(time(0));
    vector<bool> lossPattern(totalPackets);
    for (int i = 0; i < totalPackets; i++) {
        double randomValue = (double)rand() / RAND_MAX * 100;
        lossPattern[i] = (randomValue < lossProb);
    }

    // Both algorithms receive the exact same lossPattern vector
    runSimulation("TCP Tahoe", "tahoe_data.csv", totalPackets, lossProb, lossPattern);
    runSimulation("TCP Reno", "reno_data.csv", totalPackets, lossProb, lossPattern);

    cout << "\nSimulation completed successfully.\n";
    return 0;
}