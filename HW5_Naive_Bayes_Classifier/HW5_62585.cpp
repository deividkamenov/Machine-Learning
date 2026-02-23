#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> dataset;

// 0 - yes, 1 - no, 2 - unknown
int republicans[3][16]; //  here we store the number of yes/no/unknown for each attribute of republicans
int democrats[3][16];   // for each attribute of democrats

double countDemoc = 0; // number of democrats
double countReub = 0;  // number of republicans

const int startPosRepub = 11; // here we store the position of the first attribute of republicans
const int startPosDemoc = 9;

int sizeTest; // here we store the size of the test set

// this function updates the tables with the new data which is read from the file
void updateTables(string line, int diff) // diff = 1 if we add new data, diff = -1 if we remove data line is the line which we read from the file
{
   bool isRep = line[0] == 'r';
   if (isRep == true)
   {
      countReub += diff;
   }
   else
   {
      countDemoc += diff;
   }

   int initPos;
   if (isRep == true)
   {
      initPos = startPosRepub;
   }
   else
   {
      initPos = startPosDemoc;
   }

   int counter = 0;
   for (int i = initPos; i < line.size(); i += 2)
   {
      switch (line[i])
      {
      case 'y': // y - yes
         if (isRep == true)
         {
            republicans[0][counter] += diff;
         }
         else
         {
            democrats[0][counter] += diff;
         }
         break;
      case 'n': // n - no
         // isRep ? republicans[1][counter] += diff : democrats[1][counter] += diff;
         if (isRep == true)
         {
            republicans[1][counter] += diff;
         }
         else
         {
            democrats[1][counter] += diff;
         }
         break;
      case '?': //? - unknown
         // isRep ? republicans[2][counter] += diff : democrats[2][counter] += diff;
         if (isRep == true)
         {
            republicans[2][counter] += diff;
         }
         else
         {
            democrats[2][counter] += diff;
         }

         break;
      }
      counter++;
   }
}

// this function prints the tables
void printObjects()
{
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 16; j++)
      {
         cout << "D " << i << " " << j << " " << democrats[i][j] << endl;
         cout << "R " << i << " " << j << " " << republicans[i][j] << endl;
      }
   }
}
// this function processes a line from the test set and returns 1 if the line is republican and 0 if it is democrat
// it does this by calculating the probability that the line is republican and the probability that the line is democrat
// and then comparing them
int processTestLine(string line)
{
   bool isRep = line[0] == 'r';
   int initPos;
   // this is the same as
   if (isRep == true)
   {
      initPos = startPosRepub;
   }
   else
   {
      initPos = startPosDemoc;
   }

   double probRepLog = 0; // here we store the logarithm of the probability that the line is republican
   double probDemLog = 0; // here we store the logarithm of the probability that the line is democrat

   int indexAttribute = 0;

   // here we use the logarithm of the probability because the probability is very small and we want to avoid underflow
   for (int i = initPos; i < line.size(); i += 2)
   {
      switch (line[i])
      {
      case 'y':
         // the formula here is (number of yes for the attribute + 1) / (number of yes + number of no + 2)
         // we add 1 to avoid the case when the number of yes is 0
         probRepLog += log((republicans[0][indexAttribute] + 1) / (countReub + 2));
         probDemLog += log((democrats[0][indexAttribute] + 1) / (countDemoc + 2));
         break;
      case 'n':
         probRepLog += log((republicans[1][indexAttribute] + 1) / (countReub + 2));
         probDemLog += log((democrats[1][indexAttribute] + 1) / (countDemoc + 2));
         break;
      case '?':
         probRepLog += log((republicans[2][indexAttribute] + 1) / (countReub + 2));
         probDemLog += log((democrats[2][indexAttribute] + 1) / (countDemoc + 2));
         break;
      }
      indexAttribute++;
   }

   probRepLog += log((countReub + 1) / (countReub + countDemoc + 2));
   probDemLog += log((countDemoc + 1) / (countReub + countDemoc + 2));

   if (isRep == true)
   {
      return probRepLog >= probDemLog;
   }
   else
   {
      return probRepLog <= probDemLog;
   }
}

// this function calculates the accuracy of the algorithm
double calculateAccuracy(int valTesting)
{
   string curLine;
   double rightPredictions = 0;

   for (int i = 0; i < sizeTest; i++)
   {
      rightPredictions += processTestLine(dataset[valTesting + i]);
   }

   return rightPredictions * 100 / sizeTest;
}

int runProgram()
{
   // here we read the data from the file and update the tables
   ifstream filein("house-votes-84.data", ios::in);
   string line;

   while (getline(filein, line))
   {
      dataset.push_back(line);
      updateTables(line, 1);
   }

   filein.close();

   // here we 10-fold cross-validation  which means that we split the dataset into 10 parts and we use 9 of them for training and 1 for testing
   sizeTest = dataset.size() / 10;
   int valTesting;
   double sumperc = 0;
   double accuracy;

   for (int j = 0; j < 10; j++)
   {
      valTesting = j * sizeTest;

      for (int i = valTesting; i < valTesting + sizeTest; i++)
      {
         if (valTesting != 0)
         {
            updateTables(dataset[i - sizeTest], 1);
         }

         updateTables(dataset[i], -1);
      }

      accuracy = calculateAccuracy(valTesting);
      cout << accuracy << "%" << endl;
      sumperc += accuracy;
   }

   cout << "---------" << endl;
   cout << sumperc / 10 << "%" << endl;
   // double result = calculateAccuracy(sumperc / 10);
   // cout << result <<"%"<< endl;
}

int main()
{
   runProgram();
   return 0;
}