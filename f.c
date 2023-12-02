#include <iostream>
#include <fstream>
#include <json/json.h>
#include <ctime>
#include <cmath>
#include <unistd.h> // for sleep

class Shock {
public:
    Shock();
    void loadConfiguration();
    Json::Value getKlineData();
    Json::Value getLastClosedData(const Json::Value& klineData);
    double calculateHighTrack(const Json::Value& klineData);
    double calculateLowTrack(const Json::Value& klineData);
    double calculateIntervalRange(double highTrack, double lowTrack);
    int getPositionQty();
    void createLimitOrder(const std::string& side, int qty, int price);

private:
    std::string api_key;
    std::string api_secret;
    std::string api_passphrase;
    bool sandbox;
    std::string symbol;
    int resolution;
    double valve;
    double leverage;
    int size;

public:
    Json::CharReaderBuilder jsonReaderBuilder;
    Json::Value config;
};

Shock::Shock() {
    loadConfiguration();
}

void Shock::loadConfiguration() {
    std::ifstream configFile("config.json");
    configFile >> config;

    api_key = config["api_key"].asString();
    api_secret = config["api_secret"].asString();
    api_passphrase = config["api_passphrase"].asString();
    sandbox = config["is_sandbox"].asBool();
    symbol = config["symbol"].asString();
    resolution = config["resolution"].asInt();
    valve = config["valve"].asDouble();
    leverage = config["leverage"].asDouble();
    size = config["size"].asInt();
}

Json::Value Shock::getKlineData() {
    // Placeholder for getting kline data in C++
    Json::Value klineData;
    // Replace this section with actual logic to fetch kline data
    return klineData;
}

Json::Value Shock::getLastClosedData(const Json::Value& klineData) {
    return klineData[klineData.size() - 1];
}

double Shock::calculateHighTrack(const Json::Value& klineData) {
    double highTrack = 0.0;
    for (int index = -30; index < 0; ++index) {
        double high = klineData[index][2].asDouble();
        if (high > highTrack) {
            highTrack = high;
        }
    }
    return highTrack;
}

double Shock::calculateLowTrack(const Json::Value& klineData) {
    double lowTrack = INFINITY;
    for (int index = -30; index < 0; ++index) {
        double low = klineData[index][3].asDouble();
        if (low < lowTrack) {
            lowTrack = low;
        }
    }
    return lowTrack;
}

double Shock::calculateIntervalRange(double highTrack, double lowTrack) {
    return (highTrack - lowTrack) / (highTrack + lowTrack);
}

int Shock::getPositionQty() {
    // Placeholder for getting position quantity in C++
    // Replace this section with actual logic to fetch position quantity
    return 0;
}

void Shock::createLimitOrder(const std::string& side, int qty, int price) {
    // Placeholder for creating a limit order in C++
    // Replace this section with actual logic to create a limit order
    std::cout << "Created " << side << " order for qty " << qty << " at price " << price << std::endl;
}

int main() {
    Shock shock;

    while (true) {
        Json::Value klineData = shock.getKlineData();
        Json::Value lastClosedData = shock.getLastClosedData(klineData);
        int nowPrice = lastClosedData[4].asInt();
        double highTrack = shock.calculateHighTrack(klineData);
        double lowTrack = shock.calculateLowTrack(klineData);
        double intervalRange = shock.calculateIntervalRange(highTrack, lowTrack);

        int positionQty = shock.getPositionQty();

        if (positionQty > 0 && nowPrice < lowTrack) {
            shock.createLimitOrder("sell", positionQty, nowPrice);
        } else if (positionQty < 0 && nowPrice > highTrack) {
            shock.createLimitOrder("buy", positionQty, nowPrice);
        }

        if (intervalRange < shock.valve && positionQty == 0) {
            if (nowPrice > highTrack) {
                shock.createLimitOrder("buy", shock.size, nowPrice);
            } else if (nowPrice < highTrack) {
                shock.createLimitOrder("sell", shock.size, nowPrice);
            }
        }

        sleep(1);
    }

    return 0;
}
