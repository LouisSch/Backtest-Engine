#include <vector>
#include <mutex>
#include <chrono>

#include "Tick.h"
#include "TimeFrame.h"
#include "GBMJumpGenerator.h"
#include "QuoteGBMJumpGenerator.h"
#include "BacktestEngine.h"
#include "MeanReversionSimpleStrategy.h"
#include "BreakoutStrategy.h"
#include "SpreadStrategy.h"

std::mutex globalPrintMutex;

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::unique_ptr<GBMJumpGenerator> jumpGenerator = std::make_unique<GBMJumpGenerator>(1000, TimeFrame::MINUTE);
    std::vector<Tick> ticks = jumpGenerator->generateTicks();

    std::unique_ptr<QuoteGBMJumpGenerator> quoteJumpGenerator = std::make_unique<QuoteGBMJumpGenerator>(1000, TimeFrame::MINUTE);
    std::vector<QuoteTick> quoteTicks = quoteJumpGenerator->generateTicks();
    
    BacktestEngine engine;
    engine.setTickData(std::move(ticks));
    engine.setTickData(std::move(quoteTicks));
    
    engine.addStrategy("Mean_Reversion", std::make_unique<MeanReversionSimple>(), TimeFrame::MINUTE, 10000.0);
    engine.addStrategy("Breakout_Win20", std::make_unique<BreakoutStrategy<20>>(), TimeFrame::MINUTE, 10000.0);
    engine.addStrategy("Spread", std::make_unique<SpreadStrategy>(), TimeFrame::MINUTE, 10000.0);
    
    engine.runAll(true, true);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    {
        std::unique_lock<std::mutex> lock(globalPrintMutex);
        std::cout << "Total execution elapsed time: " << elapsed.count() << " seconds\n";
    }

    return 0;
}