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
#include <header.h>

using namespace std;

const int seconds = 4;
const unsigned boardWidth = 8;


int main(int argc, char *argv[])
{
/*
	unsigned i, j, h, size = 100;
	double min, max, sigma;
	sigma = 1.5;
	min = -1;
	max = 1;
	boost::mt19937 rng;
	rng.seed(static_cast<unsigned>(time(0)));
    boost::uniform_real<double> u(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > gen(rng, u);
*/
    vector<vector<vector<double> > > network;
    vector<pair<int, vector<vector<vector<double> > > > > vec;

//    vector<unsigned> a;
//    a.push_back(20);

    loadResults(vec, argv);



    char winner;
    winner = playNewGame(&human, &neuralAgent2, vec[0].second, vec[0].second);
    cout << "Winner = " << winner << "\n\n";

    /*
    for(i = 0; i < size; ++i)
    {
    	network.clear();
    	network.push_back(makeLayer(6, 64, &gen));
    	network.push_back(makeLayer(1, 70, &gen));
    	vec.push_back(pair<int, vector<vector<vector<double> > > >(0, network));
    }



    for(i = 0; i < 500; ++i)
    {

    	runTest(vec);


		cout << "\nGeneration " << i << endl;
		for(j = 0; j < 5; ++j)
		{
			cout << j << ". " << vec[j].first << endl;
		}

		if(i%15 == 14)
			writeResults(vec, argv);

		if(i < 50)
		{
			replicate(vec, sigma, rng, 66, size, 10, true);
		}
		else if(i + 1 < 500)
			replicate(vec, sigma, rng, 50, size, 16, true);

		sigma -= 1.0/pow(static_cast<double>(i+2), 1.7) + 0.0007;
    }

	writeResults(vec, argv);
*/
/*    loadResults(vec, argv);
    network = vec[0].second;
   char winner;
   winner = playNewGame(&human, &neuralAgent2, network, network);
   cout << "Winner = " << winner << "\n\n";
*/
//   winner = playNewGame(&agent, &neuralAgent2, network, network);
//   cout << "Winner = " << winner << "\n\n";




    return EXIT_SUCCESS;
}
/*
void replicate(vector<pair<int, vector<vector<vector<double> > > > > &vec, double sigma, boost::mt19937 &rng, unsigned add, unsigned size, unsigned pairings, bool reproduce)
{
	unsigned i, j, k;
	vector<double> weights, weights2;
    boost::normal_distribution<double> nd(0, sigma);
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<double> > norm(rng, nd);

	for(i = 0; i < size && vec.size() < (size + add); ++i)
	{
		if(norm() < 1.3 * sigma)
		{
			weights = getWeights(vec[i].second);
			for(j = 0; j < weights.size(); ++j)
			{
				weights[j] += norm();
			}
			vec.push_back(vec[i]);
			setWeights(vec.back().second, weights);
			vec.back().first = 100;
		}
	}

	if(reproduce)
	{
		for(i = 0; i < size && vec.size() < (size + add + pairings); ++i)
		{
			if(norm() < 1.3 * sigma)
			{
				for(j = 0; j < size; ++j)
				{
					if(j != i && norm() < 0)
					{
						weights = getWeights(vec[i].second);
						weights2 = getWeights(vec[j].second);
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


	stable_sort(vec.rbegin(), vec.rend());


	while(vec.size() > size)
	{
		vec.pop_back();
	}

	for(i = 0; i < vec.size(); ++i)
		vec[i].first = 0;

}
*/
void loadResults(vector<pair<int, vector<vector<vector<double> > > > > &vec, char *argv[])
{
	ifstream in;
	double num;
	string str;
    vector<double> neuron;
    vector<vector<double> > layer;
    vector<vector<vector<double> > > network;
    in.open(argv[1]);
    if(in.is_open())
    {
		while(!in.eof())
		{

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
			if(j != i)
			{
				resetBoard(board);
				winner = finishNeuralGame(&neuralAgent, &neuralAgent, vec[i].second, vec[j].second, board, 'B');

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
//	stable_sort(vec.rbegin(), vec.rend());
}

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
vector<vector<vector<double> > > generateNetwork(unsigned inputs, unsigned inputParity, unsigned layers, vector<unsigned> neuronsPerHiddenLayer, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *gen, unsigned outputs)
{
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

vector<vector<double> > makeLayer(unsigned numNeurons, unsigned weightsPerNeuron, boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > *gen)
{
	unsigned i;
	vector<vector<double> > layer;
	for(i = 0; i < numNeurons; ++i)
		layer.push_back(makeNeuron(weightsPerNeuron, gen));

	return layer;
}

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

unsigned human(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned move;

	cout << "Enter move, player " << player << ".\n";
	cin >> move;

	return move;
}

unsigned agent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i, plyLimit = 2;
//	int seconds = 5;
	double alpha = -2 , beta = 2;
	clock_t limit;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;


	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
		return valid[0];

	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}

	limit = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < limit && moves[0].first != -1 && moves[0].first != 1) // && move < threshHold
	{
		plyLimit += 2;
		for(i = 0; i < moves.size() && clock() < limit; ++i)
		{
			newBoard = board;
			makeMove(newBoard, player, moves[i].second);


			moves[i].first = searchMin(newBoard, player, plyLimit, 1, limit, alpha, beta);

		}

		stable_sort(moves.rbegin(), moves.rend());
	for(i = 0; i < moves.size(); ++i)
		cout << moves[i].second << " " << moves[i].first << endl;
		//alpha = moves[0].first;
		cout << "Alpha for " << plyLimit << ": " << alpha << endl;
		//beta = moves.back().first;
		cout << "Beta for " << plyLimit << ": " << beta << endl;
		cout << endl;

	}

	cout << plyLimit << " " << player << " " << moves[0].second << " " << moves[0].first << endl;


	return moves[0].second;
}

double searchMin(const vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner, op = (player == 'B' ? 'W' : 'B');
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	double hold, min = 2;
	unsigned i;

//	if(clock() > timeLimit && ply >= plyLimit - 2)
//		return evaluate(board, player);

	valid = movesAvailable(board, op, false);
	if(valid.empty())
	{
		if(movesAvailable(board, ' ', true).empty())
		{
			winner = tally(board);
			if(winner == op)
				return -1;
			if(winner == player)
				return 1;
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
		newBoard = board;
		makeMove(newBoard, op, valid[i]);
		hold = searchMax(newBoard, player, plyLimit, ply+1, timeLimit, alpha, beta);
		min = min > hold ? hold : min;
		if(min < alpha)
			return min;
		beta = beta > min ? min : beta;
	}

//	cout << "Min" << alpha << " " << beta << endl;
	return min;
}

double searchMax(const vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	unsigned i;
	double hold, max = -2;

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
				return 1;
			if(winner == 'D')
				return 0;
			return -1;
		}
		else
		{
			return searchMin(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		}
	}

//	cout << "Max" << alpha << " " << beta << endl;
	for(i = 0; i < valid.size(); ++i)
	{
		newBoard = board;
		makeMove(newBoard, player, valid[i]);
		hold = searchMin(newBoard, player, plyLimit, ply+1, timeLimit, alpha, beta);
		max = max < hold ? hold : max;
		if(max > beta)
			return max;
		alpha = alpha < max ? max : alpha;
	}



	return max;
}


unsigned agent2(const vector<vector<char> > &board, char player)
{
	unsigned i, plyLimit = 0;
//	int seconds = 5;
	double alpha = -2 , beta = 2;
	clock_t limit;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;

	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
		return valid[0];

	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}

	limit = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < limit && moves[0].first != -1 && moves[0].first != 1) // && move < threshHold
	{
		plyLimit += 2;
		for(i = 0; i < moves.size() && clock() < limit; ++i)
		{
			newBoard = board;
			makeMove(newBoard, player, moves[i].second);


			moves[i].first = searchMin2(newBoard, player, plyLimit, 1, limit, alpha, beta);

		}

		stable_sort(moves.rbegin(), moves.rend());
	for(i = 0; i < moves.size(); ++i)
		cout << moves[i].second << " " << moves[i].first << endl;
		//alpha = moves[0].first;
		cout << "Alpha for " << plyLimit << ": " << alpha << endl;
		//beta = moves.back().first;
		cout << "Beta for " << plyLimit << ": " << beta << endl;
		cout << endl;

	}

	cout << plyLimit << " " << player << " " << moves[0].second << " " << moves[0].first << endl;

	return moves[0].second;
}

double searchMin2(const vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner, op = (player == 'B' ? 'W' : 'B');
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	double hold, min = 2;
	unsigned i;

//	if(clock() > timeLimit && ply >= plyLimit - 2)
//		return evaluate(board, player);

	valid = movesAvailable(board, op, false);
	if(valid.empty())
	{
		if(movesAvailable(board, ' ', true).empty())
		{
			winner = tally(board);
			if(winner == op)
				return -1;
			if(winner == player)
				return 1;
			if(winner == 'D')
				return 0;
		}
		else
		{
			return searchMax2(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		}
	}

	for(i = 0; i < valid.size(); ++i)
	{
		newBoard = board;
		makeMove(newBoard, op, valid[i]);
		hold = searchMax2(newBoard, player, plyLimit, ply+1, timeLimit, alpha, beta);
		min = min > hold ? hold : min;
		if(min <= alpha)
			return min;
		beta = beta > min ? min : beta;
	}
//	cout << "Min" << alpha << " " << beta << endl;
	return min;
}

double searchMax2(const vector<vector<char> > &board, char player, unsigned plyLimit, unsigned ply, clock_t timeLimit, double alpha, double beta)
{
	char winner;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	unsigned i;
	double hold, max = -2;

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
				return 1;
			if(winner == 'D')
				return 0;
			return -1;
		}
		else
		{
			return searchMin2(board, player, plyLimit, ply+1, timeLimit, alpha, beta);
		}
	}

//	cout << "Max" << alpha << " " << beta << endl;
	for(i = 0; i < valid.size(); ++i)
	{
		newBoard = board;
		makeMove(newBoard, player, valid[i]);
		hold = searchMin2(newBoard, player, plyLimit, ply+1, timeLimit, alpha, beta);
		max = max < hold ? hold : max;
		if(max >= beta)
			return max;
		alpha = alpha < max ? max : alpha;
	}



	return max;
}


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

	return (us-them)/(88.0);
}

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

unsigned neuralAgent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
//	deque<pair<double, unsigned> > moves;
	double score, best = -2;
	unsigned move;

	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
		return valid[0];
/*
	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}
*/
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
//		moves[i].first = (feed(net, getInputs(newBoard, player))).front();
	}

//	stable_sort(moves.rbegin(), moves.rend());
	return move;

}

unsigned responseAgent(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;

	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
		return valid[0];

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
	return moves[0].second;
}




unsigned neuralAgent2(const vector<vector<char> > &board, char player, vector<vector<vector<double> > > &net)
{
	unsigned i, unmake, plyLimit = 1;

	double alpha = -2 , beta = 2;
	clock_t limit;
	vector<vector<char> > newBoard;
	vector<unsigned> valid;
	deque<pair<double, unsigned> > moves;
	char op = (player == 'B' ? 'W' : 'B');

	valid = movesAvailable(board, player, false);

	if(valid.size() == 1)
		return valid[0];

	for(i = 0; i < valid.size(); ++i)
	{
		moves.push_back(pair<double, unsigned>(0, valid[i]));
	}
	newBoard = board;
	limit = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < limit && moves[0].first != -2 && moves[0].first != 2) // && move < threshHold
	{
		plyLimit += 2;
		for(i = 0; i < moves.size() && clock() < limit; ++i)
		{

			makeMoveTest(newBoard, player, op, moves[i].second, unmake);
			moves[i].first = searchMinNeural(newBoard, player, plyLimit, 1, limit, alpha, beta, net);
			unmakeMove(newBoard, player, op, moves[i].second, unmake);

		}

		stable_sort(moves.rbegin(), moves.rend());
//	for(i = 0; i < moves.size(); ++i)
//		cout << moves[i].second << " " << moves[i].first << endl;

//		cout << endl;
	}

	cout << "Search at depth " <<  plyLimit << endl;
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

	if(ply == plyLimit)
	{
		return feed(net, getInputs(board, player)).front();
	}
	if(clock() > timeLimit && ply >= plyLimit - 2)
		return feed(net, getInputs(board, player)).front();


	valid = movesAvailable(board, op, false);
	if(valid.empty())
	{
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



