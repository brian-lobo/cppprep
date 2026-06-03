#pragma once
#include <string>
#include <memory>


// Strategy is a behavioral design pattern that lets you define a family of algorithms, put each of them into a
// separate class, and make their objects interchangeable.
namespace designpatterns::behavioural
{
  // ==================== Order Data Structure ====================
  struct Order {
    std::string symbol;
    std::string side;      // "BUY" or "SELL"
    double quantity;
    std::string status = "NEW";
  };

  // ===== Strategy Interface =====
  class ExecutionStrategy
  {
  public:
    virtual ~ExecutionStrategy() {}

    virtual void execute(Order& order, double currentMarketPrice,
                         const std::string& symbol)= 0;

    virtual std::string getName() const = 0;
  };

  // ===== Concrete Implemenation =====
  class AggressiveExecution : public ExecutionStrategy
  {
    void execute(Order& order, double currentMarketPrice,
                 const std::string& symbol) override {
      std::cout << "[Aggressive] Sending MARKET order for "
                    << order.quantity << " " << symbol
                    << " @ " << currentMarketPrice << std::endl;
      // In real HFT: Use IOC/Fill-or-Kill market orders + low latency venue
      order.status = "FILLED";
    }

    std::string getName() const override{

    }
  };

  class PassiveExecution : public ExecutionStrategy {
  public:
    void execute(Order& order, double currentMarketPrice,
                const std::string& symbol) override {
      double limitPrice = (order.side == "BUY")
                        ? currentMarketPrice * 0.9995   // 5 ticks below
                        : currentMarketPrice * 1.0005;  // 5 ticks above

      std::cout << "[Passive] Posting LIMIT order for "
                << order.quantity << " " << symbol
                << " @ " << limitPrice << std::endl;
      // In real HFT: Use limit orders + order book monitoring
      order.status = "POSTED";
    }

    std::string getName() const override { return "Passive"; }
  };

  class TWAPExecution : public ExecutionStrategy {
  private:
    int slices = 5;
    int currentSlice = 0;

  public:
    void execute(Order& order, double currentMarketPrice, const std::string& symbol) override {
      double sliceQty = order.quantity / slices;
      currentSlice++;

      std::cout << "[TWAP] Slice " << currentSlice << "/" << slices
                << ": Executing " << sliceQty << " " << symbol
                << " @ " << currentMarketPrice << std::endl;

      if (currentSlice >= slices) {
        order.status = "FILLED";
      }
    }

    std::string getName() const override { return "TWAP"; }
  };

  // ==================== Context (Trading Engine) ====================
  class TradingEngine
  {
    private:
      std::unique_ptr<ExecutionStrategy> _executionStrategy;

    public:
      TradingEngine(std::unique_ptr<ExecutionStrategy>&& executionStrategy)
      : _executionStrategy(std::move(executionStrategy)){

      }

      void processOrder(Order order, double marketPrice) {
        _executionStrategy->execute(order, marketPrice, order.symbol);
      }
  };


  class StrategyTest
  {
    public:
      void run_test() {

        TradingEngine engine{std::move(std::make_unique<AggressiveExecution>())};
        engine.processOrder(Order{"AAPL", "B", 100, "OPEN"}, 53.71);

      }
  };

}
