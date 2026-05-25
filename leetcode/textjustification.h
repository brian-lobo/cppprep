#pragma once


class Solution {
public:
  std::vector<std::string> fullJustify(std::vector<std::string>& words, int maxWidth) {

    int len{0};
    int numWords{0};
    int rangeEnd{0};
    std::vector<std::string> result{};
    for (int wordIndex=0; wordIndex < words.size(); )
    {
        rangeEnd = wordIndex;

        len = 0;
        numWords=0;
        while (rangeEnd < words.size()){
          if (len + numWords + (int)words[rangeEnd].size() > maxWidth)
          {
            break;
          }
          len += words[rangeEnd].size();
          numWords++;
          rangeEnd++;
        }

        std::string line{};
        int remainingGap    = (maxWidth - len);
        int numWordsToJustify = (numWords==1)?1:(numWords-1);
        int regularSpaces   = (rangeEnd == words.size())? remainingGap : (remainingGap / numWordsToJustify);
        int spacesToJustify = (rangeEnd == words.size())? 0: (remainingGap % numWordsToJustify);


        for (; wordIndex < rangeEnd; wordIndex++)
        {
           line += words[wordIndex];
           int spaces = regularSpaces + spacesToJustify;
           if (rangeEnd == words.size())
           {
             if ((wordIndex+1) == rangeEnd)
             {
               while (spaces-- >0) { line += " ";}
             } else
             {
               line += " "; regularSpaces--;
             }
           }
           else if ((wordIndex < (rangeEnd-1)) || numWords == 1)
           {
             // int spaces = regularSpaces + spacesToJustify;
             while (spaces-- >0) { line += " ";}
             if (spacesToJustify > 0) spacesToJustify--;
           }
        }
       wordIndex = rangeEnd;
        result.push_back(line);
    }
    return result;
  }
};