#include "BacktestEngine.h"
#include "Statistiques.h"

StrategyContext::StrategyContext(const std::string& name,
	std::unique_ptr<Strategy> strategy,
	const TimeFrame& tf,
	const double initialCash) : name(name), strategy(std::move(strategy)), tf(tf), orderManager(initialCash) {
}

void BacktestEngine::setTickData(std::vector<Tick>& ticks) {
	data = ticks;
}

void BacktestEngine::setTickData(std::vector<Tick>&& ticks) {
	data = std::move(ticks);
}

void BacktestEngine::setTickData(std::vector<QuoteTick>& quoteTicks) {
	quoteData = quoteTicks;
}

void BacktestEngine::setTickData(std::vector<QuoteTick>&& quoteTicks) {
	quoteData = std::move(quoteTicks);
}

void BacktestEngine::addStrategy(const std::string& name, std::unique_ptr<Strategy> strategy, const TimeFrame& tf, double initialCash) {
	strategies.emplace_back(std::make_unique<StrategyContext>(name, std::move(strategy), tf, initialCash));
}

void BacktestEngine::runAll(const bool saveToCSV, const bool verbose) {
	for (auto& context : strategies) {
		context->strategy->setOrderManager(&context->orderManager);
		registerUserStats(context->statistics, context->tf);

		context->worker = std::thread([&, ctx = context.get()] {
			ctx->strategy->onStart();

			if (auto* quoteStrat = dynamic_cast<QuoteStrategy*>(ctx->strategy.get())) {
				if (!quoteData.empty()) {
					for (const QuoteTick& tick : quoteData) {
						quoteStrat->onTick(tick);
						ctx->orderManager.handleTick(tick);
						ctx->statistics.recordPnL(ctx->orderManager.getPnL((tick.bid + tick.ask) / 2.0));
					}
				}
			}
			else if (!data.empty()) {
				for (const Tick& tick : data) {
					ctx->strategy->onTick(tick);
					ctx->orderManager.handleTick(tick);
					ctx->statistics.recordPnL(ctx->orderManager.getPnL(tick.price));
				}
			} else throw std::runtime_error("No data available for backtest.");

			ctx->strategy->onEnd();
			auto stats = ctx->statistics.computeStats();

			if (verbose) {
				std::lock_guard<std::mutex> lock(globalPrintMutex);
				std::cout << "[" << ctx->name << "] Final PnL: " << ctx->orderManager.getPnL(0.0) << "\n";
				for (const auto& [name, value] : stats) std::cout << " - " + name << ": " << value << "\n";
			}

			if (saveToCSV) {
				ctx->statistics.exportPnLToCSV(ctx->name + "_pnl.csv");
				ctx->statistics.exportStatsToCSV(ctx->name + "_statistics.csv", stats);
			}
		});
	}

	for (auto& context : strategies) {
		if (context->worker.joinable()) context->worker.join();
	}
}
