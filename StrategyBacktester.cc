#include "StrategyBacktester.h"
#include <iostream>
#include <numeric>

template<typename T>
StrategyBacktester<T>::StrategyBacktester(const std::vector<std::string>& tickers, double initialCash, double commission)
    : tickers(tickers), cash(initialCash), commission(commission) {
    portfolio.resize(tickers.size(), 0.0);
}

template<typename T>
void StrategyBacktester<T>::addData(const std::vector<T>& tickerData) {
    data.push_back(tickerData);
}

template<typename T>
void StrategyBacktester<T>::calculateMovingAverages(int windowSize) {
    movingAverages.resize(tickers.size());
    for (size_t i = 0; i < data.size(); ++i) {
        const std::vector<T>& tickerData = data[i];
        std::vector<double>& ma = movingAverages[i];
        ma.resize(tickerData.size() - windowSize + 1);

        for (size_t j = 0; j < tickerData.size() - windowSize + 1; ++j) {
            ma[j] = std::accumulate(tickerData.begin() + j, tickerData.begin() + j + windowSize, 0.0) / windowSize;
        }
    }
}

template<typename T>
void StrategyBacktester<T>::movingAverageCrossover(int shortWindowSize, int longWindowSize) {
    if (shortWindowSize >= longWindowSize) {
        std::cerr << "Error: Short window size should be less than long window size." << std::endl;
        return;
    }

    calculateMovingAverages(shortWindowSize);
    calculateMovingAverages(longWindowSize);

    for (size_t i = 0; i < data[0].size(); ++i) {
        for (size_t j = 0; j < tickers.size(); ++j) {
            if (i >= longWindowSize - 1) {
                double shortMA = movingAverages[j][i - shortWindowSize + 1];
                double longMA = movingAverages[j][i - longWindowSize + 1];

                if (shortMA > longMA && (portfolio[j] == 0)) {
                    double price = data[j][i];
                    portfolio[j] = cash / price;
                    cash -= portfolio[j] * price;
                }
                else if (shortMA < longMA && (portfolio[j] != 0)) {
                    double price = data[j][i];
                    cash += portfolio[j] * price;
                    portfolio[j] = 0;
                }
            }
        }
    }

    for (size_t j = 0; j < tickers.size(); ++j) {
        if (portfolio[j] != 0) {
            cash += portfolio[j] * data[j].back();
            portfolio[j] = 0;
        }
    }

    std::cout << "Final Portfolio Value (Moving Average Crossover): " << cash << std::endl;
}

template<typename T>
void StrategyBacktester<T>::backtest() {
    for (size_t i = 0; i < data[0].size(); ++i) {

        for (size_t j = 0; j < tickers.size(); ++j) {
            double price = data[j][i];
            if (i == 0) {
                portfolio[j] = cash / price;
                cash -= portfolio[j] * price;
            } else {
                // std::cout << "Nothing to do...proceeding!" << std::endl;
            }
        }
    }

    double portfolioValue = cash;
    for (size_t i = 0; i < tickers.size(); ++i) {
        portfolioValue += portfolio[i] * data[i].back();
    }
    
    std::cout << "Final Portfolio Value: " << portfolioValue << std::endl;
}