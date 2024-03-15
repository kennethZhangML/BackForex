#ifndef STRATEGYBACKTESTER_H
#define STRATEGYBACKTESTER_H

#include <vector>
#include <string>

template<typename T>
class StrategyBacktester {
private:
    std::vector<std::string> tickers;
    std::vector<std::vector<T>> data;
    std::vector<std::vector<double>> movingAverages;
    std::vector<double> portfolio;

    std::function<void()> customStrategy;
    double cash;
    double commission;


public:
    StrategyBacktester(const std::vector<std::string>& tickers, double initialCash, double commission);

    void addData(const std::vector<T>& tickerData);
    void calculateMovingAverages(int windowSize);
    void movingAverageCrossover(int shortWindowSize, int longWindowSize);
    void backtest();

    void setCustomStrategy(const std::function<void()>& strategy) {
        customStrategy = strategy;
    }

    void executeCustomStrategy() {
        if(customStrategy) { 
            customStrategy();
        }
    }
};

#include "StrategyBacktester.cc"

#endif // STRATEGYBACKTESTER_H
