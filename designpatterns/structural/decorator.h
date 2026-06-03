#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <chrono>

namespace designpatterns::structural
{
  // Decorator is a structural design pattern that lets you attach new behaviors to objects by placing these objects
  // inside special wrapper objects that contain the behaviors.


  // =============================================
  // Core Component
  // =============================================
  class OrderExecutor {
    public:
        virtual ~OrderExecutor() = default;
        virtual bool execute(const std::string& symbol, double quantity, double price, const std::string& side) = 0;
  };

  // =============================================
  // Concrete Component
  // =============================================
  class BasicOrderExecutor : public OrderExecutor {
    public:
        bool execute(const std::string& symbol, double quantity, double price, const std::string& side) override {
            std::cout << "[BASIC] Executing " << side << " " << quantity
                      << " " << symbol << " @ " << price << "\n";
            // Simulate ultra-low latency execution
            return true;
        }
  };

  // =============================================
  // Abstract Decorator
  // =============================================
  class OrderExecutorDecorator : public OrderExecutor {
    protected:
        std::unique_ptr<OrderExecutor> wrapped;

    public:
        explicit OrderExecutorDecorator(std::unique_ptr<OrderExecutor> exec)
            : wrapped(std::move(exec)) {}

        bool execute(const std::string& symbol, double quantity, double price, const std::string& side) override {
            return wrapped->execute(symbol, quantity, price, side);
        }
  };

  // =============================================
  // Concrete Decorators for HFT
  // =============================================

  class LoggingDecorator : public OrderExecutorDecorator {
    public:
        using OrderExecutorDecorator::OrderExecutorDecorator;

        bool execute(const std::string& symbol, double quantity, double price, const std::string& side) override {
            auto start = std::chrono::high_resolution_clock::now();

            std::cout << "[LOG] Order received: " << side << " " << quantity
                      << " " << symbol << " @ " << price << "\n";

            bool result = OrderExecutorDecorator::execute(symbol, quantity, price, side);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::micro>(end - start).count();

            std::cout << "[LOG] Order completed in " << duration << " μs\n";
            return result;
        }
  };

  class RiskCheckDecorator : public OrderExecutorDecorator {
    public:
        using OrderExecutorDecorator::OrderExecutorDecorator;

        bool execute(const std::string& symbol, double quantity, double price, const std::string& side) override {
            // HFT Risk Checks
            if (quantity > 10000.0) {
                std::cout << "[RISK] REJECTED: Quantity too large!\n";
                return false;
            }

            if (price <= 0.0) {
                std::cout << "[RISK] REJECTED: Invalid price!\n";
                return false;
            }

            std::cout << "[RISK] Checks passed\n";
            return OrderExecutorDecorator::execute(symbol, quantity, price, side);
        }
  };

  class LatencyMonitorDecorator : public OrderExecutorDecorator {
    public:
        using OrderExecutorDecorator::OrderExecutorDecorator;

        bool execute(const std::string& symbol, double quantity, double price, const std::string& side) override {
            auto start = std::chrono::high_resolution_clock::now();

            bool result = OrderExecutorDecorator::execute(symbol, quantity, price, side);

            auto end = std::chrono::high_resolution_clock::now();
            auto us = std::chrono::duration<double, std::micro>(end - start).count();

            if (us > 50.0) {  // HFT threshold example
                std::cout << "[LATENCY ALERT] Slow execution: " << us << " μs\n";
            }

            return result;
        }
  };

  // =============================================
  // Usage in HFT System
  // =============================================
  class DecoratorTest
  {
    public:
        void run_test() {
          // Stack decorators dynamically
          std::unique_ptr<OrderExecutor> executor = std::make_unique<BasicOrderExecutor>();

          executor = std::make_unique<RiskCheckDecorator>(std::move(executor));
          executor = std::make_unique<LoggingDecorator>(std::move(executor));

          // executor = std::make_unique<LatencyMonitorDecorator>(std::move(executor));
          // Execute order through decorated pipeline
          executor->execute("AAPL", 150.0, 225.75, "BUY");
          return;
        }
   };

}