#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
//#include <boost/random.hpp>
#include <header.h>
using namespace std;

const unsigned boardWidth = 8;


char playNewGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w)
{
   // Begin and play a new game between given agents.
   vector<vector<char> > board;
   resetBoard(board);
   return finishGame(bAgent, wAgent, b, w, board, 'B');
}
char finishGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w, vector<vector<char> > &board, char player)
{
	printBoard(board);
	vector<unsigned> valid;
   // Continue and finish the started game.
   do
   {
      if (makeMove(board, player, (player == 'B' ? *bAgent : *wAgent)(board, player, (player == 'B' ? b : w))))
      {

         cout << "\n";
         printBoard(board);
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



char playNeuralGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w)
{
   // Begin and play a new game between given agents.
   vector<vector<char> > board;
   resetBoard(board);
   return finishNeuralGame(bAgent, wAgent, b, w, board, 'B');
}
char finishNeuralGame(unsigned (*bAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), unsigned (*wAgent)(const vector<vector<char> > &, char, vector<vector<vector<double> > > &), vector<vector<vector<double> > > &b, vector<vector<vector<double> > > &w, vector<vector<char> > &board, char player)
{
//	printBoard(board);
	vector<unsigned> valid;
   // Continue and finish the started game.
   do
   {
      if (makeMove(board, player, (player == 'B' ? *bAgent : *wAgent)(board, player, (player == 'B' ? b : w))))
      {

 //        cout << "\n";
  //       printBoard(board);
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



void printBoard(const vector<vector<char> > &board)
{
   // Print current state of board.
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

void resetBoard(vector<vector<char> > &board)
{
   // Reset board, removing all pieces.
   board.clear();
   board.resize(boardWidth);
   for(unsigned i = 0; i < boardWidth; ++i)
      board[i].resize(boardWidth, ' ');
   board[3][3] = 'B';
   board[3][4] = 'W';
   board[4][3] = 'W';
   board[4][4] = 'B';
}

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

bool makeMove(vector<vector<char> > &board, char player, unsigned move)
{
   unsigned row, column, i;
   bool flip = false;
   char op = (player == 'B' ? 'W' : 'B');
   row = move%10;
   column = move/10;

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


bool validMove(const vector<vector<char> > &board, char player , unsigned move)
{
	unsigned row, column, i;
	row = move % 10;
	column = move / 10;
	char op = (player == 'B' ? 'W' : 'B');

	if(board[row][column] != ' ')
		return false;

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


vector<double> feed(vector<vector<vector<double> > > &net, vector<double> inputs)
{
	vector<double> outputs;
	double sum, scale;
	unsigned i, j, k, h;
	scale = 2;

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

unsigned countWeights(vector<vector<vector<double> > > &net)
{
	unsigned i, num = 0;

	for(i = 0; i < net.size() ; ++i)
	{
		num += net[i].size() * net[i][0].size();
	}

	return num;
}



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









