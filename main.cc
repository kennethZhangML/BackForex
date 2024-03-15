#include "StrategyBacktester.h"
#include <vector>
#include <string>

int main() {
    std::vector<std::string> tickers = {"AAPL", "MSFT", "GOOG"};
    StrategyBacktester<double> backtester(tickers, 10000.0, 0.0);

    std::vector<double> aaplPrices = {120.5, 121.0, 119.5, 122.0, 123.5, 125.0, 124.0, 126.5, 128.0};
    std::vector<double> msftPrices = {210.5, 211.0, 210.0, 212.5, 215.0, 217.0, 216.0, 218.5, 220.0};
    std::vector<double> googPrices = {1510.0, 1520.0, 1515.0, 1525.0, 1530.0, 1540.0, 1535.0, 1545.0, 1550.0};

    backtester.addData(aaplPrices);
    backtester.addData(msftPrices);
    backtester.addData(googPrices);

    int shortWindowSize = 3;
    int longWindowSize = 5;
    backtester.movingAverageCrossover(shortWindowSize, longWindowSize);
    backtester.backtest();

    return 0;
}
