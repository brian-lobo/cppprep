#pragma once



namespace designpatterns::behavioural
{
  template<typename D>
  class ExecutionStrategy
  {
    public:
      ExecutionStrategy() {

      }

      void display()
      {
        D& strategy{static_cast<D&>(*this)};
        strategy.displayImpl();
      }
  };

  class AlgorithmicStrategy : public ExecutionStrategy<AlgorithmicStrategy>
  {
    public:
      AlgorithmicStrategy() :  ExecutionStrategy<AlgorithmicStrategy>() {
      }
      void displayImpl()
      {
        std::cout << "Inside AlgorithmicStrategy::displayImpl" << std::endl;
      }
  };



  class ExecutionStrategyTest
  {
    public:

      template<typename T>
      void display(T& strategy) {
        strategy.display();
      }

      void run_test() {
        AlgorithmicStrategy as;
        display(as);
      }
  };

}