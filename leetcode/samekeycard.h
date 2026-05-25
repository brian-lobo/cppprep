#pragma once

#include <string>
#include <vector>
#include <map>

class KeyCardSolution {
public:
  using StringList = std::vector<std::string>;


  int stringTimeToInt(const std::string& str) {
    std::string hour{str.substr(0, 2)};
    std::string min{str.substr(3, 2)};

    return (std::stoi(hour) * 60 + std::stoi(min));
  }

  std::vector<std::string> alertNames(StringList& keyName, StringList& keyTime) {
    std::map<std::string, std::vector<int>> entries;
    std::vector<std::string> result{};

    // Read the names and times into a map keyed on name and a vector containing the times
    for (int i=0; i< keyName.size(); i++)
    {
      auto employee = entries.find(keyName[i]);
      int timeInInt{stringTimeToInt(keyTime[i])};
      if (employee == entries.end())
      {
        entries.emplace(keyName[i], std::vector<int>{timeInInt});
      }
      else
      {
        entries[keyName[i]].push_back(timeInInt);
      }
    }

    // Sort the vector of timings
    for (auto& [key, value] : entries)
    {
      std::sort(value.begin(), value.end());

      for (int i=0; i<value.size(); i++)
      {
        int timeWindow = value[i]+60;
        if (
             (i < value.size()) && (value[i] <= timeWindow) &&
             ((i +1) < value.size()) && (value[i+1] <= timeWindow)
             && ((i +2) < value.size()) && (value[i+2] <= timeWindow)
             )
        {
          result.emplace_back(key);
          break;
        }
      }
    }
    return result;
  }
};


class KeyCardSolution
{
    public:
        void run_test() {
          {
            KeyCardSolution solution;
            KeyCardSolution::StringList keyName{"daniel","daniel","daniel","luis","luis","luis","luis"};
            KeyCardSolution::StringList keyTime{"10:00","10:40","11:00","09:00","11:00","13:00","15:00"};

            solution.alertNames(keyName, keyTime);
          }

          {
            KeyCardSolution solution;
            KeyCardSolution::StringList keyName = {"alice","alice","alice","bob","bob","bob","bob"};
            KeyCardSolution::StringList keyTime = {"12:01","12:00","18:00","21:00","21:20","21:30","23:00"};
          }
        }

};