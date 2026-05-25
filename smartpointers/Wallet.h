#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <memory>
#include <string>
#include <random>

namespace cppprep::smartpointers {

    struct Position {
        std::string symbol;
        double quantity = 0.0;
        double avgPrice = 0.0;
    };

    class WalletSnapshot {
    public:
        double equity = 0.0;
        double availableBalance = 0.0;
        double marginUsed = 0.0;
        std::vector<Position> positions;

        void print() const {
            std::cout << "Equity: $" << equity
                      << " | Available: $" << availableBalance
                      << " | Margin Used: $" << marginUsed
                      << " | Positions: " << positions.size() << "\n";
        }
    };

    class Wallet {
        std::atomic<const WalletSnapshot*> currentSnapshot{nullptr};

    public:
        Wallet() {
            auto init = std::make_unique<WalletSnapshot>();
            init->equity = 100000.0;
            init->availableBalance = 100000.0;
            currentSnapshot.store(init.release(), std::memory_order_relaxed);
        }

        ~Wallet() {
            delete currentSnapshot.load(std::memory_order_relaxed);
        }

        // === Used by Strategy Threads (Hot Path) ===
        const WalletSnapshot* getSnapshot() const {
            return currentSnapshot.load(std::memory_order_acquire);
        }

        // === Used by Fill / Risk / Update Thread ===
        void update(std::unique_ptr<WalletSnapshot> newSnapshot) {
            auto old = currentSnapshot.exchange(newSnapshot.release(), std::memory_order_acq_rel);
            delete old;  // In production, defer deletion to a background thread
        }
    };

    // ==================== Strategy Thread Example ====================

    void strategyThread(Wallet& wallet, int strategyId) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);

        for (int i = 0; i < 20; ++i) {
            const WalletSnapshot* snap = wallet.getSnapshot();

            if (snap) {
                std::cout << "[Strategy " << strategyId << "] Read snapshot -> ";
                snap->print();

                // Example decision making
                if (snap->availableBalance > 50000.0) {
                    std::cout << "[Strategy " << strategyId << "] Signal: Can take larger position\n";
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen) * 100));
        }
    }

    // ==================== Update Thread Example ====================

    void walletUpdateThread(Wallet& wallet) {
        for (int i = 0; i < 8; ++i) {
            auto newSnap = std::make_unique<WalletSnapshot>();

            newSnap->equity = 100000.0 + (i * 1250.0);
            newSnap->availableBalance = 95000.0 - (i * 800.0);
            newSnap->marginUsed = i * 3000.0;

            newSnap->positions = {
                {"BTCUSDT", 0.5 + i*0.1, 62000.0},
                {"ETHUSDT", 10.0 + i*2, 3400.0}
            };

            wallet.update(std::move(newSnap));
            std::cout << "[Wallet Update] Published new snapshot #" << i << "\n";

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // ==================== Main ====================

    class WalletTest
    {
      public:
        int run_test()
        {
            Wallet wallet;

            std::cout << "=== Starting Trading System Simulation ===\n\n";

            std::vector<std::thread> strategies;

            // Launch 4 strategy threads
            for (int i = 0; i < 4; ++i) {
                strategies.emplace_back(strategyThread, std::ref(wallet), i + 1);
            }

            // Launch wallet update thread
            std::thread updater(walletUpdateThread, std::ref(wallet));

            // Join all threads
            for (auto& t : strategies) t.join();
            updater.join();

            std::cout << "\n=== Simulation Finished ===\n";
            return 0;
        }
    };
}
