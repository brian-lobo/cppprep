#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <memory>

// Template Method is a behavioral design pattern that defines the skeleton of an algorithm in the superclass but
// lets subclasses override specific steps of the algorithm without changing its structure.

namespace designpatterns::behavioural
{
    // Simple types for HFT context
    struct MarketData {
        double bid = 0.0;
        double ask = 0.0;
        double lastPrice = 0.0;
        uint64_t timestamp = 0;
        std::string symbol;
    };

    struct Signal {
        enum class Type { BUY, SELL, HOLD };
        Type type = Type::HOLD;
        double confidence = 0.0;
        double targetPrice = 0.0;
    };

    class TradingStrategy {
    public:
        // Template Method - This is the "skeleton" that never changes
        void runTradingCycle() {
            auto start = std::chrono::high_resolution_clock::now();

            log("=== Starting trading cycle ===");

            preTradeRiskCheck();                    // Step 1

            MarketData data = fetchMarketData();    // Step 2
            logMarketData(data);

            Signal signal = generateSignal(data);   // Step 3

            if (validateSignal(signal)) {           // Step 4
                executeOrder(signal);               // Step 5
                postTradeUpdate(signal);            // Step 6
            } else {
                log("Signal rejected by validation");
            }

            auto end = std::chrono::high_resolution_clock::now();
            logLatency(start, end);
        }

        virtual ~TradingStrategy() = default;

    protected:
        // === Steps that can/must be overridden ===

        // 1. Common risk checks (can be extended)
        virtual void preTradeRiskCheck() {
            std::cout << "[Risk] Position limits, kill switches, daily loss check - OK\n";
        }

        // 2. Market data fetch - usually implemented differently per venue/feed
        virtual MarketData fetchMarketData() = 0;

        // 3. Core strategy logic - this is where strategies differ most
        virtual Signal generateSignal(const MarketData& data) = 0;

        // 4. Common validation (can be overridden)
        virtual bool validateSignal(const Signal& signal) {
            if (signal.confidence < 0.65) {
                log("Low confidence: " + std::to_string(signal.confidence));
                return false;
            }
            return true;
        }

        // 5. Order execution (can be venue-specific)
        virtual void executeOrder(const Signal& signal) = 0;

        // 6. Post-trade actions (P&L update, position tracking, etc.)
        virtual void postTradeUpdate(const Signal& signal) {
            std::cout << "[Post] Updated position. Signal: "
                      << (signal.type == Signal::Type::BUY ? "BUY" : "SELL")
                      << "\n";
        }

        // Helper methods (not part of the template)
        void log(const std::string& msg) {
            std::cout << "[LOG] " << msg << "\n";
        }

        void logMarketData(const MarketData& data) {
            std::cout << "[Data] " << data.symbol << " | Bid: " << data.bid
                      << " Ask: " << data.ask << " Last: " << data.lastPrice << "\n";
        }

        void logLatency(auto start, auto end) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "[Latency] Cycle took " << duration.count() << " μs\n\n";
        }
    };

    class MomentumStrategy : public TradingStrategy {
    protected:
        MarketData fetchMarketData() override {
            // In real system: get from UDP/multicast feed
            return {100.05, 100.10, 100.07, 0, "AAPL"};
        }

        Signal generateSignal(const MarketData& data) override {
            // Simple momentum logic
            Signal s;
            if (data.lastPrice > data.bid * 1.0005) {  // Strong upward tick
                s.type = Signal::Type::BUY;
                s.confidence = 0.82;
                s.targetPrice = data.ask * 1.001;
            }
            return s;
        }

        void executeOrder(const Signal& signal) override {
            std::cout << "[ORDER] Aggressive BUY on momentum! Target: "
                      << signal.targetPrice << "\n";
            // Real impl: Send to low-latency venue (FIX, ITCH, etc.)
        }
    };


    class TemplateTest
    {
      public:
        void run_test() {
            std::unique_ptr<TradingStrategy> strat1 = std::make_unique<MomentumStrategy>();
            std::unique_ptr<TradingStrategy> strat2 = std::make_unique<MarketMakingStrategy>();

            std::cout << "Running Momentum Strategy:\n";
            strat1->runTradingCycle();

            std::cout << "Running Market Making Strategy:\n";
            strat2->runTradingCycle();

            return;
        }
    };

}