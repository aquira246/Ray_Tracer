#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void printBar(int current, int max, int length) {
   static int r = 0;
   double percent = ((double)current/(double)max);
   int NumEquals = percent*length;
      cout << "[";
      for (int i = 1; i <= length; ++i)
      {
         if (i <= NumEquals) {
            cout << "=";
         } else if (i == NumEquals + 1) {
            switch(r++%8) {
               case 0:
               cout << "/";
               break;
               case 1:
               cout << "-";
               break;
               case 2:
               cout << "\\";
               break;
               case 3:
               cout << "|";
               break;
               case 4:
               cout << "/";
               break;
               case 5:
               cout << "-";
               break;
               case 6:
               cout << "\\";
               break;
               case 7:
               cout << "|";
               break;
            }
         } else {
            cout << " ";
         }
      }

      cout << "]   " << percent*100 << "%                         \r";
      std::cout.flush();
}