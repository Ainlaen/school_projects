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
// Random number generator and distributions
// Part of the Boost C++ libraries from boost.org
//#include <boost/random.hpp>


using namespace std;

// Option to change this in main.
int seconds = 4;

// boardWidth really shouldn't be changed.
const unsigned boardWidth = 8;


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
double searchMin(vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
double searchMax(vector<vector<char> > &, char, unsigned, unsigned, clock_t, double, double);
double evaluate(const vector<vector<char> > &, char);
unsigned neuralAgent(const vector<vector<char> > &, char, vector<vector<vector<double> > > &);
unsigned responseAgent(const vector<vector<char> > &, char , vector<vector<vector<double> > > &);
unsigned neuralAgent2(const vector<vector<char> > &, char , vector<vector<vector<double> > > &);
double searchMaxNeural(vector<vector<char> > &, char , unsigned , unsigned, clock_t, double, double, vector<vector<vector<double> > > &);
double searchMinNeural(vector<vector<char> > &, char , unsigned , unsigned, clock_t, double, double, vector<vector<vector<double> > > &);


vector<double> feed(vector<vector<vector<double> > > &, vector<double>);
vector<double> getWeights(const vector<vector<vector<double> > > &);
void setWeights(vector<vector<vector<double> > > &, vector<double> &);
unsigned countWeights(vector<vector<vector<double> > > &);
vector<double> getInputs(const vector<vector<char> > &, char);
void writeResults(const vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[]);
void runTest(vector<pair<int, vector<vector<vector<double> > > > > &vec);
void loadResults(vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[]);

// All functions requiring part of the boost library have been commented out.
// These are only used for neural net creation and the genetic algorithm.
/*
vector<vector<vector<double> > > generateNetwork(unsigned inputs, unsigned inputParity, unsigned layers, vector<unsigned> neuronsPerHiddenLayer, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *, unsigned outputs = 1);
vector<double> makeNeuron(unsigned numWeights, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *);
vector<vector<double> > makeLayer(unsigned numNeurons, unsigned weightsPerNeuron, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *);
void replicate(vector<pair<int, vector<vector<vector<double> > > > > &, double, boost::mt19937 &, unsigned, unsigned, unsigned, bool);
*/

/*	The main function is currently set to allow play between any two of the agent functions.
 * 	Hopefully it will allow for most testing that you would want to do.
 * 	The only thing you might want to do that should require changing the code is if you want to have
 * 	separate time limits for two agents. This would require changing the individual agents, or creating
 * 	a copy of a specific agent if you want to have two of the same type of agent with different time limits.
 * 	It should be fairly obvious how to do this if you look at the agent code, so I'm not going to go into
 * 	detail on how to do that.
 * 	The human agent has been changed from the demonstration to only allow valid moves, so if you want to end
 * 	a game early just use ctrl+c.
 * 	Make sure networks.txt is in the same directory as the executable before playing.
*/
int main(int argc, char *argv[])
{

	unsigned i, j, agentb, agentw;//,h, size = 100;
/*	double min, max, sigma;
 *
 	// Sigma is the standard deviation of a normal distribution.
	sigma = 1.5;
	min = -1;
	max = 1;

	// Create and initialize mersenne twister PRNG
	boost::mt19937 rng;
	rng.seed(static_cast<unsigned>(time(0)));

	// Create uniform real distribution between min and max.
    boost::uniform_real<double> u(min, max);

    // Create front-end for using RNG with uniform distribution.
    boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > gen(rng, u);
*/
//    vector<vector<vector<double> > > network;
//    a specifies the number of neurons in each hidden layer beyond the first of a new network.
//    vector<unsigned> a;

    // vec hold networks along with their associated ratings
    vector<pair<int, vector<vector<vector<double> > > > > vec;


    // The following is code used in the creation and evolution of neural networks.
    /*
     // Create size many new neural networks to add to vec.
    for(i = 0; i < size; ++i)
    {
		network = generateNetwork(10, 64, 2, a, &gen);
    	vec.push_back(pair<int, vector<vector<vector<double> > > >(0, network));
    }


	// Number of generations to run for.
    for(i = 0; i < 500; ++i)
    {
		// Play each network against every other as black and white.
    	runTest(vec);

		// Output top five for every generation. Mostly used to track
		// what generation the process is up to.
		cout << "\nGeneration " << i << endl;
		for(j = 0; j < 5; ++j)
		{
			cout << j << ". " << vec[j].first << endl;
		}

		// Save results every so often just in case.
		if(i%15 == 14)
			writeResults(vec, argv);

		// Call genetic algorithm to create new networks.
		if(i < 50)
		{
			replicate(vec, sigma, rng, 66, size, 10, true);
		}
		else if(i + 1 < 500)
			replicate(vec, sigma, rng, 50, size, 16, true);

		// Reduce sigma over time. Faster at first.
		sigma -= 1.0/pow(static_cast<double>(i+2), 1.7) + 0.0007;
    }
	// Save final output.
	writeResults(vec, argv);

*/

    // Load neural networks from networks.txt into vec.
    loadResults(vec, argv);
    bool show = true;
    char winner;

    // All functions are passed a network, but only neural agents make use of them. This is just to
    // simplify how the game playing is handled.
    unsigned (*agents[5])(const vector<vector<char> > & , char, vector<vector<vector<double> > > &);
    agents[0] = &human;
    agents[1] = &neuralAgent2;
    agents[2] = &agent;
    agents[3] = &neuralAgent;
    agents[4] = &responseAgent;

    // The simple evaluation function is just a weighted piece differential. It's not intended to be a particularly good
    // evaluation function, but to be a sort of baseline for comparison.


    cout << "Player choices:\n1. Human\n2. Neural network with time limit\n3. Simple evaluation function with time limit\n";
    cout << "4. Neural network with no search\n5. Simple evaluation function with no search\n";
    cout << "Select who will play B\n";
    cin >> agentb;
    --agentb;
    cout << "Select who will play W\n";
    cin >> agentw;
    --agentw;

    // Details on each of the networks is included in the write-up.
    i = 0;
    if(agentb == 1 || agentb == 3)
    {
		for(i = 0; i < vec.size(); ++i)
		{
			cout << "Network " << i << " has " << vec[i].second.size() << " layers." << endl;
		}
		cout << "Which network will play B?" << endl;
		cin >> i;
		show = false;
    }
    j = 0;
    if(agentw == 1 || agentw == 3)
    {
    	if(show)
    	{
			for(j = 0; j < vec.size(); ++j)
			{
				cout << "Network " << j << " has " << vec[j].second.size() << " layers." << endl;
			}
    	}
    	cout << "Which network will play W?" << endl;
    	cin >> j;
    }
    if(agentw == 1 || agentb == 1 || agentw == 2 || agentb == 2)
    {
    	cout << "Enter time limit for computer moves in seconds\n";
    	cin >> seconds;
    }


    if(i < vec.size() && j < vec.size())
    {
    	winner = playNewGame(agents[agentb], agents[agentw], vec[i].second, vec[j].second);
    	cout << "Winner = " << winner << "\n\n";
    }
    else
    {
    	cout << "Network selection invalid.\n";
    }


    return EXIT_SUCCESS;
}
/*
  // This is the genetic algorithm.
void replicate(vector<pair<int, vector<vector<vector<double> > > > > &vec, double sigma, boost::mt19937 &rng, unsigned add, unsigned size, unsigned pairings, bool reproduce)
{
	unsigned i, j, k;
	vector<double> weights, weights2;
	// Create normal distribution centered at 0 with standard deviation of sigma.
    // A new distribution is created each time since sigma will change.
    boost::normal_distribution<double> nd(0, sigma);
    // Create front-end for RNG with normal distribution. The same RNG is used each time, as passed from main.
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<double> > norm(rng, nd);

    // Add a maximum of add more nets to vec.
	for(i = 0; i < size && vec.size() < (size + add); ++i)
	{
		// Roughly a 90% chance for selection.
		if(norm() < 1.3 * sigma)
		{
			// Take weights of selected net.
			weights = getWeights(vec[i].second);
			// Modify all weights according to normal distribution.
			for(j = 0; j < weights.size(); ++j)
			{
				weights[j] += norm();
			}
			// Copy net and set weights to modified values.
			vec.push_back(vec[i]);
			setWeights(vec.back().second, weights);
			// Set value of net high enough that it won't be eliminated immediately.
			vec.back().first = 100;
		}
	}

	// If there should be pairings between nets.
	if(reproduce)
	{
		for(i = 0; i < size && vec.size() < (size + add + pairings); ++i)
		{
			if(norm() < 1.3 * sigma)
			{
				for(j = 0; j < size; ++j)
				{
					// 50% chance.
					if(j != i && norm() < 0)
					{
						weights = getWeights(vec[i].second);
						weights2 = getWeights(vec[j].second);

						// 50% chance for each weight being replaced.
						for(k = 0; k < weights.size(); ++k)
						{
							if(norm() < 0)
							{
								weights[k] = weights2[k];
							}
						}

						vec.push_back(vec[i]);
						setWeights(vec.back().second, weights);
						vec.back().first = 100;
					}
				}
			}
		}
	}

	// Sort nets according to rating.
	stable_sort(vec.rbegin(), vec.rend());

	// Get rid of worst nets until only size many remain.
	while(vec.size() > size)
	{
		vec.pop_back();
	}
	// Reset ratings for next test.
	for(i = 0; i < vec.size(); ++i)
		vec[i].first = 0;

}
*/
// Loads saved networks into vec.
void loadResults(vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[])
{
	ifstream in;
	double num;
	string str;
    vector<double> neuron;
    vector<vector<double> > layer;
    vector<vector<vector<double> > > network;
    // Input file currently fixed.
    in.open("networks.txt");
    if(in.is_open())
    {
		while(!in.eof())
		{
			// Specific strings indicate structure of network.
			getline(in, str);
			if(stringstream(str) >> num)
			{
				neuron.push_back(num);
			}
			else if(str == "eonrn")
			{
				layer.push_back(neuron);
				neuron.clear();
			}
			else if(str == "eolyr")
			{
				network.push_back(layer);
				layer.clear();
			}
			else if(str == "break")
			{
				vec.push_back(pair<int, vector<vector<vector<double> > > >(0, network));
				network.clear();
			}
		}
    }
    else
    	cerr << "Input file could not be opened\n";

       in.close();

}
// Test all networks against each other.
void runTest(vector<pair<int, vector<vector<vector<double> > > > > &vec)
{
	unsigned i, j;
	char winner;
	vector<vector<char> > board;
	vector<unsigned> valid;
	for( i = 0; i < vec.size(); ++i)
	{
		for(j = 0; j < vec.size(); ++j)
		{
			// Don't bother playing networks against themselves.
			if(j != i)
			{
				resetBoard(board);
				winner = finishNeuralGame(&neuralAgent, &neuralAgent, vec[i].second, vec[j].second, board, 'B');

				// 1 point for a win, 0 points for a draw, -2 points for a loss.
				if(winner == 'B')
				{
					vec[i].first += 1;
					vec[j].first -= 2;
				}
				else if(winner == 'W')
				{
					vec[i].first -= 2;
					vec[j].first += 1;
				}

			}
		}
	}
	// Sort networks according to their rankings. Currently commented out so networks can be
	// kept track of with regard to their order on loading.
	//	stable_sort(vec.rbegin(), vec.rend());
}

// Save networks.
void writeResults(const vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[])
{
	unsigned i, j, k, h;
	ofstream out;
	out.precision(100);
	out.open(argv[2]);

	if(out.is_open())
	{
	    for(i = 0; i < vec.size(); ++i)
	    {
			for(j = 0; j < vec[i].second.size(); ++j)
			{
				for(k = 0; k < vec[i].second[j].size(); ++k)
				{
					for(h = 0; h < vec[i].second[j][k].size(); ++h)
					{
						out << vec[i].second[j][k][h] << endl;
					}

					out << "eonrn\n";
				}

				out << "eolyr\n";
			}


			out << "break\n";
	    }
	}
	else
		cerr << "File could not be opened" << endl;
	out.close();
}
/*
// Create new network.
// Names of the variables do not necessarily indicate their use at this point (messing with different structures has made it necessary to change their use over time.)
// Structures are not enforced to be usable within the framework of the rest of the program, so caution is required when selecting parameters.
vector<vector<vector<double> > > generateNetwork(unsigned inputs, unsigned inputParity, unsigned layers, vector<unsigned> neuronsPerHiddenLayer, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *gen, unsigned outputs)
{
	// Inputs is the number of neurons to be in the first layer. inputParity is the weights per neuron in the first layer.
	// Layers is actually the number of layers. neuronsPerHiddenLayer is the number of neurons in hidden layers after the first.
	// Outputs is the number of output neurons if there's more than one layer. Defaults to one.

	unsigned i, j = 0;
	vector<vector<vector<double> > > network;
	network.push_back(makeLayer(inputs, inputParity, gen));
	if(layers > 1)
	{
		for(i = 1; i < layers; ++i)
		{
			if(i+1 < layers)
			{
				network.push_back(makeLayer(neuronsPerHiddenLayer[j++], network.back().size(), gen));
			}
			else
			{
				network.push_back(makeLayer(outputs, network.back().size(), gen));
			}
		}
	}

	return network;
}
// Create new layer of neurons.
// Variable names are exactly what they sound like.
vector<vector<double> > makeLayer(unsigned numNeurons, unsigned weightsPerNeuron, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *gen)
{
	unsigned i;
	vector<vector<double> > layer;
	for(i = 0; i < numNeurons; ++i)
		layer.push_back(makeNeuron(weightsPerNeuron, gen));

	return layer;
}
// Create new neuron.
vector<double> makeNeuron(unsigned numWeights, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *gen)
{
	vector<double> neuron;
    unsigned i;
    for(i = 0; i < numWeights +1; ++i)
    {
    	neuron.push_back((*gen)());
    }
    return neuron;
}

*/

// Function for human player. Only allows valid moves.
unsigned human(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned move, i;
	vector<unsigned> valid;
	bool good = false;

	valid = movesAvailable(board, player, false);

	while(!good)
	{
		cout << "Enter move, player " << player << ".\n";
		cin >> move;
		for(i = 0; i < valid.size(); ++i)
		{
			if(move == valid[i])
				good = true;
		}
		if(!good)
		{
			cout << "Not a valid move.\n";
		}
	}
	return move;
}
// Agent function using simple evaluation function and time limit.
unsigned agent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i, unmake, plyLimit = 2;
//	int seconds = 5;
	double alpha = -200, beta = 200;
	clock_t limit;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;
	char op = (player == 'B' ? 'W' : 'B');

	cout << "Player " << player << endl;
	// Get valid moves.
	valid = movesAvailable(board, player, false);

	// If there's only one move available, just take it.
	if(valid.size() == 1)
	{
		cout << "Forced move at " << valid[0] << endl;
		return valid[0];
	}
	// Store valid moves as pairs to allow for ranking.
	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}

	// Set time limit.
	limit = clock() + seconds * CLOCKS_PER_SEC;
	// Copy board to newBoard
	newBoard = board;
	// If the best move isn't a definite loss or definite win, and while there's still time.
	while(clock() < limit && moves[0].first != -100 && moves[0].first != 100)
	{
		// Evaluation only occurs in Max function, so limit is increased by 2 each time.
		// I considered it important to have the evaluation always take place at the same
		// point (i.e. just after the opponent has moved) for consistency of comparison.
		plyLimit += 2;

		for(i = 0; i < moves.size() && clock() < limit; ++i)
		{
			makeMoveTest(newBoard, player, op, moves[i].second, unmake);
			moves[i].first = searchMin(newBoard, player, plyLimit, 1, limit, alpha, beta);
			unmakeMove(newBoard, player, op, moves[i].second, unmake);
		}

		stable_sort(moves.rbegin(), moves.rend());
	}
	cout << "Search cut off at depth " <<  plyLimit << endl;
	for(i = 0; i < moves.size(); ++i)
		cout << moves[i].second << " " << moves[i].first << endl;

	return moves[0].second;
}

double searchMin(vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner, op = (player == 'B' ? 'W' : 'B');
	vector<unsigned> valid;
	double hold, min = 200;
	unsigned i, unmake;


	valid = movesAvailable(board, op, false);
	if(valid.empty())
	{
		if(movesAvailable(board, ' ', true).empty())
		{
			winner = tally(board);
			if(winner == op)
				return -100;
			if(winner == player)
				return 100;
			if(winner == 'D')
				return 0;
		}
		else
		{
			return searchMax(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		}
	}

	for(i = 0; i < valid.size(); ++i)
	{
		makeMoveTest(board, op, player, valid[i], unmake);
		hold = searchMax(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		unmakeMove(board, op, player, valid[i], unmake);
		min = min > hold ? hold : min;
		if(min <= alpha)
			return alpha;
		beta = beta > min ? min : beta;
	}


	return min;
}

double searchMax(vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner, op = (player == 'B' ? 'W' : 'B');
	vector<unsigned> valid;
	unsigned i, unmake;
	double hold, max = -200;

	if(ply == plyLimit)
	{
		return evaluate(board, player);
	}
	if(clock() > timeLimit && ply >= plyLimit - 2)
		return evaluate(board, player);

	valid = movesAvailable(board, player, false);

	if(valid.empty())
	{
		if(movesAvailable(board, ' ', true).empty())
		{
			winner = tally(board);
			if(winner == player)
				return 100;
			if(winner == 'D')
				return 0;
			return -100;
		}
		else
		{
			return searchMin(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		}
	}

	for(i = 0; i < valid.size(); ++i)
	{
		makeMoveTest(board, player, op , valid[i], unmake);
		hold = searchMin(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		unmakeMove(board, player, op, valid[i], unmake);
		max = max < hold ? hold : max;
		if(max >= beta)
			return beta;
		alpha = alpha < max ? max : alpha;
	}

	return max;
}

// This is the simple evaluation function used by the agent function.
double evaluate(const vector<vector<char> > &board, char player)
{
	double us, them;
	unsigned i, j;
	char op = (player == 'B' ? 'W' : 'B');
	us = them = 0;

	for(i = 0; i < boardWidth; ++i)
	{
		for(j = 0; j < boardWidth; ++j)
		{
			// Corners are worth 4, edges 1.5, the rest 1.
			if((j == 0 || j == 7) && (i == 0 || i == 7))
			{
				if(board[i][j] == player)
				{

					us += 4;
				}
				else if(board[i][j] == op)
				{

					them += 4;
				}
			}
			else if(j == 0 || j == 7)
			{
				if(board[i][j] == player)
				{

					us +=1.5;
				}
				else if(board[i][j] == op)
				{
					them +=1.5;

				}
			}
			else if(i == 0 || i == 7)
			{
				if(board[i][j] == player)
				{

					us += 1.5;
				}
				else if(board[i][j] == op)
				{

					them += 1.5;
				}
			}
			else if(board[i][j] == player)
			{
				us += 1;

			}
			else if(board[i][j] == op)
			{
				them += 1;

			}
		}
	}
	// Return difference.
	return (us-them);
}

// Create inputs for neural network.
// 1 for own piece, -1 for opponent's piece, 0 for empty square.
vector<double> getInputs(const vector<vector<char> > &board, char player)
{
	vector<double> inputs;
	char op = (player == 'B' ? 'W' : 'B');
	for(unsigned i = 0; i < boardWidth; ++i)
	{
		for(unsigned j = 0; j < boardWidth; ++j)
		{
			if(board[i][j] == player)
				inputs.push_back(1);
			else if(board[i][j] == op)
				inputs.push_back(-1);
			else
				inputs.push_back(0);
		}
	}
	return inputs;
}
// Agent using neural networks for evaluation which only considers next move.
// Used during evolution of networks.
unsigned neuralAgent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	double score, best = -2;
	unsigned move;

	cout << "Player " << player << endl;

	valid = movesAvailable(board, player, false);
	if(valid.size() == 1)
	{
		cout << "Forced move at " << valid[0] << endl;
		return valid[0];
	}

	for(i = 0; i < valid.size(); ++i)
	{
		newBoard = board;
		makeMove(newBoard, player, valid[i]);
		score = (feed(net, getInputs(newBoard, player))).front();
		if(score > best)
		{
			best = score;
			move = valid[i];
		}
		cout << valid[i] << " " << score << endl;
	}
	cout << "Playing at " << move << " rated " << best << endl;
	return move;

}
// Agent using evaluation function and only considering next move.
unsigned responseAgent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;

	cout << "Player " << player << endl;

	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
	{
		cout << "Forced move at " << valid[0] << endl;
		return valid[0];
	}
	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}

	for(i = 0; i < moves.size(); ++i)
	{
		newBoard = board;
		makeMove(newBoard, player, moves[i].second);
		moves[i].first = evaluate(newBoard, player);
	}


	stable_sort(moves.rbegin(), moves.rend());

	for(i = 0; i < moves.size(); ++i)
		cout << moves[i].second << " " << moves[i].first << endl;

	return moves[0].second;
}

// Agent using neural network for evaluation with time limit.
unsigned neuralAgent2(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i, unmake, plyLimit = 1;
	double alpha = -2 , beta = 2;
	clock_t limit;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;
	char op = (player == 'B' ? 'W' : 'B');

	cout << "Player " << player << endl;
	// Get valid moves.
	valid = movesAvailable(board, player, false);

	// If there's only one move, take it.
	if(valid.size() == 1)
	{
		cout << "Forced move at " << valid[0] << endl;
		return valid[0];
	}
	// Store valid moves as pairs for ranking.
	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}
	// Copy board.
	newBoard = board;
	// Set time limit for search.
	limit = clock() + seconds * CLOCKS_PER_SEC;
	// While there's still time and the best move isn't a guaranteed loss or win.
	while(clock() < limit && moves[0].first != -2 && moves[0].first != 2)
	{
		// Limit increased by 2 each time.
		// Limit is increased at this point instead of at the end of the loop to allow for
		// a more accurate indication of how deep the search was.
		plyLimit += 2;
		for(i = 0; i < moves.size() && clock() < limit; ++i)
		{
			makeMoveTest(newBoard, player, op, moves[i].second, unmake);
			moves[i].first = searchMinNeural(newBoard, player, plyLimit, 1, limit, alpha, beta, net);
			unmakeMove(newBoard, player, op, moves[i].second, unmake);
		}
		stable_sort(moves.rbegin(), moves.rend());

	}

	cout << "Search cut off during depth " <<  plyLimit << endl;
	for(i = 0; i < moves.size(); ++i)
		cout << moves[i].second << " " << moves[i].first << endl;

	return moves[0].second;
}

double searchMinNeural(vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta, vector<vector<vector<double> > > &net)
{
	char op = (player == 'B' ? 'W' : 'B');
	vector<unsigned> valid;
	double hold, min = 2;
	unsigned i, unmake;

	// Evaluation is only applied in this function for consistency.
	if(ply == plyLimit)
	{
		return feed(net, getInputs(board, player)).front();
	}
	// Time limit enforced only at plyLimit of previous search.
	if(clock() > timeLimit && ply >= plyLimit - 2)
		return feed(net, getInputs(board, player)).front();


	valid = movesAvailable(board, op, false);
	if(valid.empty())
	{
		// If there are no valid moves, see if the game is over.
		if(movesAvailable(board, ' ', true).empty())
		{
			char winner;
			winner = tally(board);
			if(winner == op)
				return -2;
			if(winner == player)
				return 2;
			if(winner == 'D')
				return 0;
		}
		// Otherwise, continue search down a ply.
		else
		{
			return searchMaxNeural(board, player, plyLimit, ply+1, timeLimit, alpha, beta, net);
		}
	}

	for(i = 0; i < valid.size(); ++i)
	{
		makeMoveTest(board, op, player, valid[i], unmake);
		hold = searchMaxNeural(board, player, plyLimit, ply+1, timeLimit, alpha, beta, net);
		unmakeMove(board, op, player, valid[i], unmake);
		min = min > hold ? hold : min;
		if(min <= alpha)
			return alpha;
		beta = beta > min ? min : beta;
	}

	return min;
}

double searchMaxNeural(vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta, vector<vector<vector<double> > > &net)
{
	char op = (player == 'B' ? 'W' : 'B');
	vector<unsigned> valid;
	unsigned i, unmake;
	double hold, max = -2;


	valid = movesAvailable(board, player, false);

	if(valid.empty())
	{
		if(movesAvailable(board, ' ', true).empty())
		{
			char winner;
			winner = tally(board);
			if(winner == player)
				return 2;
			if(winner == 'D')
				return 0;
			return -2;
		}
		else
		{
			return searchMinNeural(board, player, plyLimit, ply+1, timeLimit, alpha, beta, net);
		}
	}

	for(i = 0; i < valid.size(); ++i)
	{
		makeMoveTest(board, player, op, valid[i], unmake);
		hold = searchMinNeural(board, player, plyLimit, ply+1, timeLimit, alpha, beta, net);
		unmakeMove(board, player, op, valid[i], unmake);
		max = max < hold ? hold : max;
		if(max >= beta)
			return beta;
		alpha = alpha < max ? max : alpha;
	}

	return max;
}

// Initialize board and start game.
char playNewGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w)
{
   vector<vector<char> > board;
   resetBoard(board);
   return finishGame(bAgent, wAgent, b, w, board, 'B');
}
// Play game.
char finishGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w, vector<vector<char> > &board, char player)
{
	printBoard(board);
	vector<unsigned> valid;
	char last = ' ';
   do
   {
	   if(player == last)
	   {
		   cout << (player == 'B' ? 'W' : 'B') << " has no valid moves and must pass.\n";
	   }
      if (makeMove(board, player, (player == 'B' ? *bAgent : *wAgent)(board, player, (player == 'B' ? b : w))))
      {

         cout << "\n";
         printBoard(board);
      }
      else
         return player == 'B' ? 'W' : 'B';

      last = player;
      if(movesAvailable(board, (player == 'B' ? 'W' : 'B'), true).size() > 0)
      {
      	  player = player == 'B' ? 'W' : 'B';
      }
      valid = movesAvailable(board, ' ', true);
   }
   while (!valid.empty());
   return tally(board);
}

// Same as above two functions with board output suppressed. Used for runTest.
char playNeuralGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w)
{
   vector<vector<char> > board;
   resetBoard(board);
   return finishNeuralGame(bAgent, wAgent, b, w, board, 'B');
}
char finishNeuralGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w, vector<vector<char> > &board, char player)
{
	vector<unsigned> valid;
   do
   {
      if (makeMove(board, player, (player == 'B' ? *bAgent : *wAgent)(board, player, (player == 'B' ? b : w))))
      {
      }
      else
         return player == 'B' ? 'W' : 'B';
      if(movesAvailable(board, (player == 'B' ? 'W' : 'B'), true).size() > 0)
      	  player = player == 'B' ? 'W' : 'B';
      valid = movesAvailable(board, ' ', true);
   }
   while (!valid.empty());
   return tally(board);
}

// Print current board.
void printBoard(const vector<vector<char> > &board)
{
   unsigned column, row;
   cout << "  ";
	for(row = 0; row < boardWidth; ++row)
		cout << " " << row;
	cout << endl << "  ";
    for (column = 0; column < boardWidth; ++column)
    {
       cout << "__";
    }
    cout << endl;
   for (row = boardWidth - 1; row < boardWidth; --row)
   {
	  cout << row << " ";
      for (column = 0; column < boardWidth; ++column)
      {
         cout << "|" << board[row][column];
      }
      cout << "|\n" << "  ";
      for (column = 0; column < boardWidth; ++column)
      {
         cout << "__";
      }
      cout << "\n";
   }
}
// Reset board for a new game.
void resetBoard(vector<vector<char> > &board)
{
   board.clear();
   board.resize(boardWidth);
   for(unsigned i = 0; i < boardWidth; ++i)
      board[i].resize(boardWidth, ' ');
   board[3][3] = 'B';
   board[3][4] = 'W';
   board[4][3] = 'W';
   board[4][4] = 'B';
}
// Count up pieces to determine the winner of a finished game.
char tally(const vector<vector<char> > &board)
{
   char winner;
   unsigned column, row, w, b;
   w = b = 0;

   for(row = 0; row < boardWidth ; ++row)
   {
	   for(column = 0; column < boardWidth; ++column)
	   {
		   if(board[row][column] == 'B')
			   ++b;
		   else if(board[row][column] == 'W')
			   ++w;
	   }
   }

   if(b > w)
	   winner = 'B';
   else if(w > b)
	   winner = 'W';
   else
	   winner = 'D';

   return winner;
}
// Make specified move.
bool makeMove(vector<vector<char> > &board, char player, unsigned move)
{
   unsigned row, column, i;
   bool flip = false;
   char op = (player == 'B' ? 'W' : 'B');
   row = move%10;
   column = move/10;

   // Check if the move is valid.
   if (row < boardWidth && column < boardWidth && validMove(board, player, move))
   {
	   // Place piece.
	   board[row][column] = player;
	   // Check for which pieces to flip.
	   // left
	   if(column - 1 < boardWidth && board[row][column-1] == op)
	   {
		   for(i = column - 2 ; i < boardWidth && !flip; --i)
		   {
			   if(board[row][i] == player)
			   {
				   flip = true;
			   }
			   else if(board[row][i] == ' ')
				   break;

		   }
		   for(i = column - 1; i < boardWidth && flip; --i)
		   {
			   if(board[row][i] == op)
				   board[row][i] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // right
	   if(column + 1 < boardWidth && board[row][column+1] == op)
	   {
		   for(i = column + 2 ; i < boardWidth && !flip; ++i)
		   {
			   if(board[row][i] == player)
			   {
				   flip = true;
			   }
			   else if(board[row][i] == ' ')
				   break;
		   }
		   for(i = column + 1; i < boardWidth && flip; ++i)
		   {
			   if(board[row][i] == op)
				   board[row][i] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // up
	   if(row + 1 < boardWidth && board[row+1][column] == op)
	   {
		   for(i = row + 2 ; i < boardWidth && !flip; ++i)
		   {
			   if(board[i][column] == player)
			   {
				   flip = true;
			   }
			   else if(board[i][column] == ' ')
				   break;
		   }
		   for(i = row + 1; i < boardWidth && flip; ++i)
		   {
			   if(board[i][column] == op)
				   board[i][column] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // down
	   if(row - 1 < boardWidth && board[row-1][column] == op)
	   {
		   for(i = row - 2 ; i < boardWidth && !flip; --i)
		   {
			   if(board[i][column] == player)
			   {
				   flip = true;
			   }
			   else if(board[i][column] == ' ')
				   break;
		   }
		   for(i = row - 1; i < boardWidth && flip; --i)
		   {
			   if(board[i][column] == op)
				   board[i][column] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // diagonally down and left
	   if(row - 1 < boardWidth && column - 1 < boardWidth && board[row-1][column-1] == op)
	   {
		   for(i = 2; row - i < boardWidth && column - i < boardWidth && !flip ; ++i)
		   {
			   if(board[row-i][column-i] == player)
				   flip = true;
			   else if(board[row-i][column-i] == ' ')
				   break;
		   }
		   for(i = 1; row -i < boardWidth && column - i < boardWidth && flip ; ++i)
		   {
			   if(board[row-i][column-i] == op)
				   board[row-i][column-i] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // diagonally down and right
	   if(row - 1 < boardWidth && column + 1 < boardWidth && board[row-1][column+1] == op)
	   {
		   for(i = 2; row - i < boardWidth && column + i < boardWidth && !flip ; ++i)
		   {
			   if(board[row-i][column+i] == player)
				   flip = true;
			   else if(board[row-i][column+i] == ' ')
				   break;
		   }
		   for(i = 1; row -i < boardWidth && column + i < boardWidth && flip ; ++i)
		   {
			   if(board[row-i][column+i] == op)
				   board[row-i][column+i] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // diagonally up and right
	   if(row + 1 < boardWidth && column + 1 < boardWidth && board[row+1][column+1] == op)
	   {
		   for(i = 2; row + i < boardWidth && column + i < boardWidth && !flip ; ++i)
		   {
			   if(board[row+i][column+i] == player)
				   flip = true;
			   else if(board[row+i][column+i] == ' ')
				   break;
		   }
		   for(i = 1; row +i < boardWidth && column + i < boardWidth && flip ; ++i)
		   {
			   if(board[row+i][column+i] == op)
				   board[row+i][column+i] = player;
			   else
				   flip = false;
		   }
	   }
	   flip = false;
	   // diagonally up and left
	   if(row + 1 < boardWidth && column - 1 < boardWidth && board[row+1][column-1] == op)
	   {
		   for(i = 2; row + i < boardWidth && column - i < boardWidth && !flip ; ++i)
		   {
			   if(board[row+i][column-i] == player)
				   flip = true;
			   else if(board[row+i][column-i] == ' ')
				   break;
		   }
		   for(i = 1; row +i < boardWidth && column - i < boardWidth && flip ; ++i)
		   {
			   if(board[row+i][column-i] == op)
				   board[row+i][column-i] = player;
			   else
				   flip = false;
		   }
	   }
	   return true;
   }
   return false;
}
// Return all of player's valid moves on board. If test is true, just
// check to see if there are any valid moves at all. If player is blank,
// check if the game is over.
vector<unsigned> movesAvailable(const vector<vector<char> > &board, char player, bool test)
{
	vector<unsigned> moves;
	unsigned column, row, move;

	for(row = 0; row < boardWidth; ++row)
	{
		for(column = 0; column < boardWidth ; ++column)
		{
			move = row + (10*column);
			if(player == ' ')
			{
				if(validMove(board, 'B', move) || validMove(board, 'W', move))
					moves.push_back(move);
			}
			else
			{
				if(validMove(board, player, move))
					moves.push_back(move);
			}
			if(test && !moves.empty())
				return moves;
		}

	}

	return moves;
}

// Check if move is valid.
bool validMove(const vector<vector<char> > &board, char player , unsigned move)
{
	unsigned row, column, i;
	row = move % 10;
	column = move / 10;
	char op = (player == 'B' ? 'W' : 'B');

	// Check if another piece is already in square.
	if(board[row][column] != ' ')
		return false;
	// If not, check to see if there are any pieces to flip.
	// left
	if(column - 1 < boardWidth && board[row][column-1] == op)
	{
	   for(i = column - 2 ; i < boardWidth; --i)
	   {
		   if(board[row][i] == player)
			   return true;
		   else if(board[row][i] == ' ')
			   break;
	   }
	}
	// right
	if(column + 1 < boardWidth && board[row][column+1] == op)
	{
	   for(i = column + 2 ; i < boardWidth; ++i)
	   {
		   if(board[row][i] == player)
			   return true;
		   else if(board[row][i] == ' ')
			   break;
	   }
	}

	// up
	if(row + 1 < boardWidth && board[row+1][column] == op)
	{
	   for(i = row + 2 ; i < boardWidth; ++i)
	   {
		   if(board[i][column] == player)
			   return true;
		   else if(board[i][column] == ' ')
			   break;
	   }

	}

	// down
	if(row - 1 < boardWidth && board[row-1][column] == op)
	{
	   for(i = row - 2 ; i < boardWidth; --i)
	   {
		   if(board[i][column] == player)
			   return true;
		   else if(board[i][column] == ' ')
			   break;
	   }

	}

	// diagonally down and left
	if(row - 1 < boardWidth && column - 1 < boardWidth && board[row-1][column-1] == op)
	{
	   for(i = 2; row - i < boardWidth && column - i < boardWidth; ++i)
	   {
		   if(board[row-i][column-i] == player)
			   return true;
		   else if(board[row-i][column-i] == ' ')
			   break;
	   }

	}

	// diagonally down and right
	if(row - 1 < boardWidth && column + 1 < boardWidth && board[row-1][column+1] == op)
	{
	   for(i = 2; row - i < boardWidth && column + i < boardWidth; ++i)
	   {
		   if(board[row-i][column+i] == player)
			   return true;
		   else if(board[row-i][column+i] == ' ')
			   break;
	   }

	}

	// diagonally up and right
	if(row + 1 < boardWidth && column + 1 < boardWidth && board[row+1][column+1] == op)
	{
	   for(i = 2; row + i < boardWidth && column + i < boardWidth; ++i)
	   {
		   if(board[row+i][column+i] == player)
			   return true;
		   else if(board[row+i][column+i] == ' ')
			   break;
	   }

	}

	// diagonally up and left
	if(row + 1 < boardWidth && column - 1 < boardWidth && board[row+1][column-1] == op)
	{
	   for(i = 2; row + i < boardWidth && column - i < boardWidth; ++i)
	   {
		   if(board[row+i][column-i] == player)
			   return true;
		   else if(board[row+i][column-i] == ' ')
			   break;
	   }

	}

	return false;
}

// Feed inputs forward through network.
vector<double> feed(vector<vector<vector<double> > > &net, vector<double> inputs)
{
	vector<double> outputs;
	double sum, scale;
	unsigned i, j, k, h;
	scale = 2;
	// Various rules for different network structures.
	if(net.size() == 2  && net.back()[0].size() > net.front()[0].size())
	{
		for(i = 0; i < net.size(); ++i)
		{
			outputs.clear();
			k = 0;
			for(j = 0; j < net[i].size(); ++j)
			{
				for(h = 1, sum = net[i][j][0]; h < net[i][j].size(); ++h)
				{
					sum += inputs[k++] * net[i][j][h];
				}
				if(i == 0)
					inputs.push_back(tanh(sum/scale));
				else
					outputs.push_back(tanh(sum/scale));

				k = 0;
			}
		}
	}
	else
	{
		for(i = 0; i < net.size(); ++i)
		{
			outputs.clear();
			k = 0;
			for(j = 0; j < net[i].size(); ++j)
			{
				for(h = 1, sum = net[i][j][0]; h < net[i][j].size(); ++h)
				{
					sum += inputs[k++] * net[i][j][h];
				}
				outputs.push_back(tanh(sum/scale));
				if(i > 0 || net[0][0].size() > inputs.size())
					k = 0;
			}
			inputs = outputs;
		}
	}

	return outputs;
}

// Returns all weights from given network.
vector<double> getWeights(const vector<vector<vector<double> > > &net)
{
	vector<double> weights;
	unsigned i, j, k;

	for(i = 0; i < net.size() ; ++i)
	{
		for(j = 0; j < net[i].size(); ++j)
		{
			for(k = 0; k < net[i][j].size(); ++k)
			{
				weights.push_back(net[i][j][k]);
			}
		}
	}

	return weights;
}

// Sets weights of given network to new weights.
void setWeights(vector<vector<vector<double> > > &net, vector<double> &weights)
{
	unsigned i, j, k, h = 0;
	if(weights.size() == countWeights(net))
	{
		for(i = 0; i < net.size() ; ++i)
		{
			for(j = 0; j < net[i].size(); ++j)
			{
				for(k = 0; k < net[i][j].size(); ++k)
				{
					net[i][j][k] = weights[h++];
				}
			}
		}
	}
}

// Returns number of weights in network.
unsigned countWeights(vector<vector<vector<double> > > &net)
{
	unsigned i, num = 0;

	for(i = 0; i < net.size() ; ++i)
	{
		num += net[i].size() * net[i][0].size();
	}

	return num;
}

// Unmake last move.
void unmakeMove(vector<vector<char> > &board, char &player, char &op, unsigned &move, unsigned &unmake)
{
   unsigned row, column, i;
   row = move%10;
   column = move/10;

	--unmake;
	for(i = 1; i <= unmake%10; ++i)
	{
		board[row+i][column-i] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake%10; ++i)
	{
		board[row+i][column+i] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake%10; ++i)
	{
		board[row-i][column+i] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake%10; ++i)
	{
		board[row-i][column-i] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake%10; ++i)
	{
		board[row-i][column] = op;
	}
	unmake = unmake /10 - 1;

	for(i = 1; i <= unmake%10; ++i)
	{
		board[row+i][column] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake%10 ; ++i)
	{

		board[row][column + i] = op;
	}
	unmake = unmake / 10 - 1;
	for(i = 1; i <= unmake; ++i)
	{
		board[row][column - i] = op;
	}

	board[row][column] = ' ';

}
// Make move function used with unmake move. Uses unsigned unmake to keep track of which pieces were flipped.
bool makeMoveTest(vector<vector<char> > &board, char &player, char &op, unsigned &move, unsigned &unmake)
{
   unsigned row, column, i;
   bool flip = false;
   row = move%10;
   column = move/10;

  unmake = 1;

   if (row < boardWidth && column < boardWidth && validMove(board, player, move))
   {
	   board[row][column] = player;
	   // left
	   if(column - 1 < boardWidth && board[row][column-1] == op)
	   {
		   for(i = column - 2 ; i < boardWidth && !flip; --i)
		   {
			   if(board[row][i] == player)
			   {
				   flip = true;
			   }
			   else if(board[row][i] == ' ')
				   break;

		   }
		   for(i = column - 1; i < boardWidth && flip; --i)
		   {
			   if(board[row][i] == op)
			   {
				   board[row][i] = player;
					++unmake;
				}
			   else
				   flip = false;
		   }
	   }
		unmake = unmake * 10 + 1;
	   flip = false;
	   // right
	   if(column + 1 < boardWidth && board[row][column+1] == op)
	   {
		   for(i = column + 2 ; i < boardWidth && !flip; ++i)
		   {
			   if(board[row][i] == player)
			   {
				   flip = true;
			   }
			   else if(board[row][i] == ' ')
				   break;
		   }
		   for(i = column + 1; i < boardWidth && flip; ++i)
		   {
			   if(board[row][i] == op)
			   {
				   board[row][i] = player;
				   ++unmake;
				}
			   else
				   flip = false;
		   }
	   }
       unmake = unmake * 10 + 1;
	   flip = false;
	   // up
	   if(row + 1 < boardWidth && board[row+1][column] == op)
	   {
		   for(i = row + 2 ; i < boardWidth && !flip; ++i)
		   {
			   if(board[i][column] == player)
			   {
				   flip = true;
			   }
			   else if(board[i][column] == ' ')
				   break;
		   }
		   for(i = row + 1; i < boardWidth && flip; ++i)
		   {
			   if(board[i][column] == op)
			   {
				   board[i][column] = player;
					++unmake;
				}
			   else
				   flip = false;
		   }
	   }
       unmake = unmake * 10 + 1;
	   flip = false;
	   // down
	   if(row - 1 < boardWidth && board[row-1][column] == op)
	   {
		   for(i = row - 2 ; i < boardWidth && !flip; --i)
		   {
			   if(board[i][column] == player)
			   {
				   flip = true;
			   }
			   else if(board[i][column] == ' ')
				   break;
		   }
		   for(i = row - 1; i < boardWidth && flip; --i)
		   {
			   if(board[i][column] == op)
			   {
				   board[i][column] = player;
				   ++unmake;
			   }
			   else
				   flip = false;
		   }
	   }
       unmake = unmake * 10 + 1;
	   flip = false;
	   // diagonally down and left
	   if(row - 1 < boardWidth && column - 1 < boardWidth && board[row-1][column-1] == op)
	   {
		   for(i = 2; row - i < boardWidth && column - i < boardWidth && !flip ; ++i)
		   {
			   if(board[row-i][column-i] == player)
				   flip = true;
			   else if(board[row-i][column-i] == ' ')
				   break;
		   }
		   for(i = 1; row -i < boardWidth && column - i < boardWidth && flip ; ++i)
		   {
			   if(board[row-i][column-i] == op)
			   {
				   board[row-i][column-i] = player;
					++unmake;
				}
			   else
				   flip = false;
		   }
	   }
       unmake = unmake * 10 + 1;

	   flip = false;

	   // diagonally down and right
	   if(row - 1 < boardWidth && column + 1 < boardWidth && board[row-1][column+1] == op)
	   {
		   for(i = 2; row - i < boardWidth && column + i < boardWidth && !flip ; ++i)
		   {
			   if(board[row-i][column+i] == player)
				   flip = true;
			   else if(board[row-i][column+i] == ' ')
				   break;
		   }
		   for(i = 1; row -i < boardWidth && column + i < boardWidth && flip ; ++i)
		   {
			   if(board[row-i][column+i] == op)
			   {
				   board[row-i][column+i] = player;
				   ++unmake;

				}
			   else
				   flip = false;
		   }
	   }
	   unmake = unmake * 10 + 1;
	   flip = false;
	   // diagonally up and right
	   if(row + 1 < boardWidth && column + 1 < boardWidth && board[row+1][column+1] == op)
	   {
		   for(i = 2; row + i < boardWidth && column + i < boardWidth && !flip ; ++i)
		   {
			   if(board[row+i][column+i] == player)
				   flip = true;
			   else if(board[row+i][column+i] == ' ')
				   break;
		   }
		   for(i = 1; row +i < boardWidth && column + i < boardWidth && flip ; ++i)
		   {
			   if(board[row+i][column+i] == op)
			   {
				   board[row+i][column+i] = player;
				   ++unmake;
			   }
			   else
				   flip = false;
		   }
	   }
	   unmake = unmake * 10 + 1;
	   flip = false;
	   // diagonally up and left
	   if(row + 1 < boardWidth && column - 1 < boardWidth && board[row+1][column-1] == op)
	   {
		   for(i = 2; row + i < boardWidth && column - i < boardWidth && !flip ; ++i)
		   {
			   if(board[row+i][column-i] == player)
				   flip = true;
			   else if(board[row+i][column-i] == ' ')
				   break;
		   }
		   for(i = 1; row +i < boardWidth && column - i < boardWidth && flip ; ++i)
		   {
			   if(board[row+i][column-i] == op)
			   {
				   board[row+i][column-i] = player;
				   ++unmake;
				}
			   else
				   flip = false;
		   }
	   }

	   return true;
   }
   return false;
}

