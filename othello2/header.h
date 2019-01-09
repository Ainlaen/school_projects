/*
 * header.h
 *
 *  Created on: Apr 25, 2011
 *      Author: Ainlaen
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
//#include <boost/random.hpp>


using namespace std;

#ifndef HEADER_H_
#define HEADER_H_

char finishGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w, vector<vector<char> > &board, char player);
bool makeMove(vector<vector<char> > &, char, unsigned);
char playNewGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w);
void unmakeMove(vector<vector<char> > &board, char &player, char &op, unsigned &move, unsigned &unmake);
bool makeMoveTest(vector<vector<char> > &board, char &player, char &op, unsigned &move, unsigned &unmake);
char finishNeuralGame(unsigned (*)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &),vector<vector<vector<double> > > &, vector<vector<vector<double> > > &, vector<vector<char> > &, char);
char playNeuralGame(unsigned (*)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &, vector<vector<vector<double> > > &);

bool validMove(const vector<vector<char> > &, char,  unsigned);
vector<unsigned> movesAvailable(const vector<vector<char> > &, char, bool);
void printBoard(const vector<vector<char> > &);
void resetBoard(vector<vector<char> > &);
char tally(const vector<vector<char> > &);
unsigned human(const vector<vector<char> > & , char, vector<vector<vector<double> > > &);
unsigned agent(const vector<vector<char> > & , char, vector<vector<vector<double> > > &);
double searchMin(const vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
double searchMax(const vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
unsigned agent2(const vector<vector<char> > & , char);
double searchMin2(const vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
double searchMax2(const vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
double evaluate(const vector<vector<char> > &, char);
unsigned neuralAgent(const vector<vector<char> > &, char, vector<vector<vector<double> > > &);


vector<double> feed(vector<vector<vector<double> > > &, vector<double>);
vector<double> getWeights(const vector<vector<vector<double> > > &);
void setWeights(vector<vector<vector<double> > > &, vector<double> &);
unsigned countWeights(vector<vector<vector<double> > > &);
vector<double> getInputs(const vector<vector<char> > &, char);
void writeResults(const vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[]);
void runTest(vector<pair<int, vector<vector<vector<double> > > > > &vec);
void loadResults(vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[]);
/*
vector<vector<vector<double> > > generateNetwork(unsigned inputs, unsigned inputParity, unsigned layers, vector<unsigned> neuronsPerHiddenLayer, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *, unsigned outputs = 1);
vector<double> makeNeuron(unsigned numWeights, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *);
vector<vector<double> > makeLayer(unsigned numNeurons, unsigned weightsPerNeuron, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *);
void replicate(vector<pair<int, vector<vector<vector<double> > > > > &, double, boost::mt19937 &, unsigned, unsigned, unsigned, bool);
*/
unsigned responseAgent(const vector<vector<char> > &, char , vector<vector<vector<double> > > &);
unsigned neuralAgent2(const vector<vector<char> > &, char , vector<vector<vector<double> > > &);
double searchMaxNeural(vector<vector<char> > &, char , unsigned , unsigned, clock_t, double, double, vector<vector<vector<double> > > &);
double searchMinNeural(vector<vector<char> > &, char , unsigned , unsigned, clock_t, double, double, vector<vector<vector<double> > > &);



#endif /* HEADER_H_ */
