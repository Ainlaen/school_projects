// Matthew Bazzell
// CS 4318, spring 2011
// Agent Challenge 7: Wumpus World
//
// Rename the wumAgent() function, complete it and turn it in on Blackboard.
// Each submitted agent will play a large number of Wumpus World games; the
// total scores will determine the standings, which will be posted soon
// after the agents are due.

#include <sstream>
#include <algorithm>
#include <ctype.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <set>

using namespace std;

const unsigned numTries = 100000;
const unsigned worldSize = 4;
const unsigned moveLimit = 100 * worldSize * worldSize;

enum action {climbOut, doNothing, grab, moveEast, moveNorth, moveSouth, moveWest, shootArrowEast, shootArrowNorth, shootArrowSouth, shootArrowWest};

struct square
{
   bool hasGold, hasPit, hasWumpus;
};

struct sensor
{
   bool perceiveBreeze, perceiveBump, perceiveGlitter, perceiveScream, perceiveStench;
};

struct world
{
   bool doesPlayerHaveArrow, doesPlayerHaveGold, hasPlayerExited, isPlayerDead, isWumpusDead;
   int playerScore;
   square sq[worldSize][worldSize];
   unsigned whereX, whereY;
};

void applyAction(world &, sensor &, action);
unsigned randomInt(unsigned);
void randomizeWorld(world &);
void resetWorldAndSensor(world &, sensor &);

// The following are for my agent.
action wumNeville_Flynn(sensor, bool);
deque<action> findPathNeville(vector<vector<vector<unsigned> > > &, unsigned, unsigned, bool, bool);
pair<unsigned, deque<action> > searchHereNeville(vector<unsigned>, unsigned , unsigned , vector<vector<vector<unsigned> > > &, unsigned, unsigned, set<unsigned>, bool);
void isItSafe(vector<vector<vector<unsigned> > > &, vector<vector<unsigned> > &, set<unsigned> &, vector<vector<vector<unsigned> > > &, unsigned , vector<vector<unsigned> >, unsigned );
vector<vector<unsigned> > isItSafe(vector<vector<vector<unsigned> > > &);

// These are not part of my agent.
action wumPlayer(sensor, bool);
action wumNeville_Flynn3(sensor, bool);
deque<action> findPathNeville3(vector<vector<vector<unsigned> > > &, unsigned, unsigned, bool, bool, bool, bool &);
pair<unsigned, deque<action> > searchHereNeville3(vector<unsigned>, unsigned , unsigned , vector<vector<vector<unsigned> > > &, unsigned, unsigned, set<unsigned>, bool);
void isItSafe3(vector<vector<vector<unsigned> > > &, vector<vector<unsigned> > &, set<unsigned> &, vector<vector<vector<unsigned> > > &, unsigned , vector<vector<unsigned> >, unsigned );
vector<vector<unsigned> > isItSafe3(vector<vector<vector<unsigned> > > &, bool);


int main()
{
	time_t start,end;
	double diff;
	time(&start);
   int totalScore, totalScore2;
   action move;
   sensor s;
   unsigned i, numMoves, deaths1, deaths2, kills, kills2, shots, shots2, gold, gold2;
   world w;
	deaths1 = 0;
	deaths2 = 0;
	kills = kills2 = gold = gold2 = 0;
	shots = shots2 = 0;
   // Initialize random-number generator.
   srandom(time(0));

   totalScore = 0;
   totalScore2 = 0;
   for (i = 0; i < numTries; ++i)
   {
      randomizeWorld(w);
      resetWorldAndSensor(w, s);
      wumNeville_Flynn(s, true);
      numMoves = 0;
      do
      {
     	 move = wumNeville_Flynn(s, false);
         applyAction(w, s, move);
         if(s.perceiveScream)
         {
         	++kills2;
         }
  		 switch(move)
         {
   /*       case moveEast:
         		cout << "East ";
         		break;
         	case moveWest:
         		cout << "West ";
         		break;
         	case moveSouth:
         		cout << "South ";
         		break;
         	case moveNorth:
         		cout << "North ";
         		break;
         	case doNothing:
         		cout << "doNothing ";
         		break;
         	case climbOut:
         		cout << "climbOut ";
         		break;
         	case grab:
         		cout << "grab ";
         		break;     */
         	case shootArrowNorth:
         	//	cout << "shootNorth ";
         		++shots2;
         		break;
         	case shootArrowSouth:
         	//	cout << "shootSouth ";
         		++shots2;
         		break;
         	case shootArrowWest:
         	//	cout << "shootWest ";
         		++shots2;
         		break;
         	case shootArrowEast:
         	//	cout << "shootEast ";
         		++shots2;
         		break;
         	default:
         		break;
         
         }
    
         if(w.isPlayerDead)
         {
        //	 cout << "Neville Flynn died! " << endl;
        	 ++deaths1;
         }
      }
      while (++numMoves < moveLimit && !w.hasPlayerExited && !w.isPlayerDead);
      if(w.doesPlayerHaveGold)
      	++gold;
      totalScore += w.playerScore;
//	  cout << "\nNeville Flynn moved "  << numMoves << " times." << endl;
	 
      resetWorldAndSensor(w, s);
      wumNeville_Flynn3(s, true);
      numMoves = 0;

      do
      {
      	 move = wumNeville_Flynn3(s, false);
         applyAction(w, s, move);
         if(s.perceiveScream)
         {
         	++kills;
         }
        switch(move)
         { 
   /*       	case moveEast:
         		cout << "East ";
         		break;
         	case moveWest:
         		cout << "West ";
         		break;
         	case moveSouth:
         		cout << "South ";
         		break;
         	case moveNorth:
         		cout << "North ";
         		break;
         	case climbOut:
         		cout << "climbOut ";
         		break;
         	case doNothing:
         		cout << "doNothing ";
         		break;
         	case grab:
         		cout << "grab ";
         		break;*/
            case shootArrowNorth:
         	//	cout << "shootNorth ";
         		++shots;
         		break;
         	case shootArrowSouth:
         	//	cout << "shootSouth ";
         		++shots;
         		break;
         	case shootArrowWest:
         	//	cout << "shootWest ";
         		++shots;
         		break;
         	case shootArrowEast:
         	//	cout << "shootEast ";
         		++shots;
         		break;
         	default:
         		break;
         
         }
     
         if(w.isPlayerDead)
         {
    //    	 cout << "You died! " << endl;
        	 ++deaths2;
         }         
         
      }
      while (++numMoves < moveLimit && !w.hasPlayerExited && !w.isPlayerDead);
	  if(w.doesPlayerHaveGold)
	  	++gold2;
      totalScore2 += w.playerScore;
// 	  cout << "\nYou moved " << numMoves << " times." << endl; 
   }
   cout << "Neville Flynn's performance measure: " << totalScore << " / " << numTries << " = " << static_cast<double>(totalScore) / numTries << "\n";
   cout << "Neville Flynn died " << deaths1 << " times, took " << shots2 << " shots, killed the Wumpus " << kills2 << " times, and got the gold " << gold << " times." << endl;
   cout << "Your performance measure: " << totalScore2 << " / " << numTries << " = " << static_cast<double>(totalScore2) / numTries << "\n";
   cout << "You died " << deaths2 << " times, took " << shots << " shots, killed the Wumpus " << kills << " times, and got the gold " << gold2 << " times." << endl;
	time(&end);
	diff = difftime(end, start);
	cout << diff << endl;
   return 0;
}

void applyAction(world &w, sensor &s, action a)
{
   // Use the action to update the world and sensor.
   int i;
   --w.playerScore;
   s.perceiveBump = s.perceiveScream = false;
   switch (a)
   {
      case climbOut:
         if (w.whereX == 0 && w.whereY == 0)
         {
            if (w.doesPlayerHaveGold)
               w.playerScore += 1000;
            w.hasPlayerExited = true;
         }
         break;
      case doNothing:
         break;
      case grab:
         if (w.sq[w.whereX][w.whereY].hasGold)
            w.doesPlayerHaveGold = true;
         break;
      case moveEast:
         if (w.whereX < worldSize - 1)
            ++w.whereX;
         else
            s.perceiveBump = true;
         break;
      case moveNorth:
         if (w.whereY < worldSize - 1)
            ++w.whereY;
         else
            s.perceiveBump = true;
         break;
      case moveSouth:
         if (w.whereY > 0)
            --w.whereY;
         else
            s.perceiveBump = true;
         break;
      case moveWest:
         if (w.whereX > 0)
            --w.whereX;
         else
            s.perceiveBump = true;
         break;
      case shootArrowEast:
         if (w.doesPlayerHaveArrow)
         {
            w.playerScore -= 10;
            w.doesPlayerHaveArrow = false;
            for (i = w.whereX + 1; i < worldSize && !w.sq[i][w.whereY].hasWumpus; ++i);
            if (i < worldSize)
               w.isWumpusDead = s.perceiveScream = true;
         }
         break;
      case shootArrowNorth:
         if (w.doesPlayerHaveArrow)
         {
            w.playerScore -= 10;
            w.doesPlayerHaveArrow = false;
            for (i = w.whereY + 1; i < worldSize && !w.sq[w.whereX][i].hasWumpus; ++i);
            if (i < worldSize)
               w.isWumpusDead = s.perceiveScream = true;
         }
         break;
      case shootArrowSouth:
         if (w.doesPlayerHaveArrow)
         {
            w.playerScore -= 10;
            w.doesPlayerHaveArrow = false;
            for (i = w.whereY - 1; i >= 0 && !w.sq[w.whereX][i].hasWumpus; --i);
            if (i >= 0)
               w.isWumpusDead = s.perceiveScream = true;
         }
         break;
      case shootArrowWest:
         if (w.doesPlayerHaveArrow)
         {
            w.playerScore -= 10;
            w.doesPlayerHaveArrow = false;
            for (i = w.whereX - 1; i >= 0 && !w.sq[i][w.whereY].hasWumpus; --i);
            if (i >= 0)
               w.isWumpusDead = s.perceiveScream = true;
         }
         break;
   }
   if (w.isPlayerDead = w.sq[w.whereX][w.whereY].hasPit) //|| w.sq[w.whereX][w.whereY].hasWumpus && !w.isWumpusDead)
    {
    	
  //    cout << "The pit isn't really bottomless, as you will soon find out.\n";
      w.playerScore -= 1000;
      
     }
   if(!w.isPlayerDead)
   {
	   if(w.isPlayerDead =  w.sq[w.whereX][w.whereY].hasWumpus && !w.isWumpusDead)
		{
		  cout << "You boldly stride into the wumpus's lair and are promptly devoured. Yum!\n";
		  w.playerScore -= 1000; 
		 }
	}
   s.perceiveBreeze = w.sq[w.whereX][w.whereY].hasPit || w.whereX > 0 && w.sq[w.whereX - 1][w.whereY].hasPit || w.whereX < worldSize - 1 && w.sq[w.whereX + 1][w.whereY].hasPit || w.whereY > 0 && w.sq[w.whereX][w.whereY - 1].hasPit || w.whereY < worldSize - 1 && w.sq[w.whereX][w.whereY + 1].hasPit;
   s.perceiveGlitter = w.sq[w.whereX][w.whereY].hasGold && !w.doesPlayerHaveGold;
   s.perceiveStench = w.sq[w.whereX][w.whereY].hasWumpus || w.whereX > 0 && w.sq[w.whereX - 1][w.whereY].hasWumpus || w.whereX < worldSize - 1 && w.sq[w.whereX + 1][w.whereY].hasWumpus || w.whereY > 0 && w.sq[w.whereX][w.whereY - 1].hasWumpus || w.whereY < worldSize - 1 && w.sq[w.whereX][w.whereY + 1].hasWumpus;
}

unsigned randomInt(unsigned n)
{
   // Return a random unsigned int between 0 and n - 1, distributed uniformly.
   const unsigned randomMax = (unsigned) (1 << 8 * sizeof(unsigned) - 1) - 1;
   unsigned r;
   while ((r = random()) >= randomMax / n * n);
   return r / (randomMax / n);
}

void randomizeWorld(world &w)
{
   // Create a new and random wumpus world.
   unsigned i, j;
   for (i = 0; i < worldSize; ++i)
      for (j = 0; j < worldSize; ++j)
      {
         w.sq[i][j].hasGold = w.sq[i][j].hasWumpus = false;
         w.sq[i][j].hasPit = randomInt(5) == 0; // 20% chance of pit
      }
   w.sq[0][0].hasPit = false;
   w.sq[randomInt(worldSize)][randomInt(worldSize)].hasGold = true;
   do
   {
      i = randomInt(worldSize);
      j = randomInt(worldSize);
   }
   while (i == 0 && j == 0);
   w.sq[i][j].hasWumpus = true;
/*
   cout << "Map \n";
      for (j = (worldSize -1); j < worldSize; --j)
  	  {    
  		  for (i = 0; i < worldSize; ++i)
  		  {

        	if(w.sq[i][j].hasGold && w.sq[i][j].hasWumpus && w.sq[i][j].hasPit)
        		cout << "9 ";
         	else if(w.sq[i][j].hasPit && w.sq[i][j].hasWumpus)
         		cout << "8 ";
         	else if(w.sq[i][j].hasPit && w.sq[i][j].hasGold)
         		cout << "7 ";
         	else if(w.sq[i][j].hasWumpus && w.sq[i][j].hasGold)
         		cout << "6 ";
         	else if(w.sq[i][j].hasGold)
         		cout << "5 ";
         	else if(w.sq[i][j].hasWumpus)
         		cout << "4 ";
         	else if(w.sq[i][j].hasPit)
         		cout << "1 ";
         	else
         		cout << "0 ";
   		  }
  	 	  cout << endl;
   		}
  */
   
   
}

void resetWorldAndSensor(world &w, sensor &s)
{
   // Reset the wumpus world and the sensor for a new agent to start.
   w.doesPlayerHaveArrow = true;
   w.doesPlayerHaveGold = w.hasPlayerExited = w.isPlayerDead = w.isWumpusDead = false;
   w.playerScore = w.whereX = w.whereY = 0;
   s.perceiveBump = s.perceiveScream = false;
   s.perceiveBreeze = w.sq[0][1].hasPit || w.sq[1][0].hasPit;
   s.perceiveGlitter = w.sq[0][0].hasGold;
   s.perceiveStench = w.sq[0][1].hasWumpus || w.sq[1][0].hasWumpus;
}





// Neville Flynn will systematically explore the world using an internal representation of its knowledge of each square.
// It will never move into a square it does not know is safe if there is an alternative. 
// Special effort is made to determine the location of the wumpus. If all other safe moves are exhausted
// before the gold is found, the agent will attempt to kill the wumpus.
// If there are no safe moves and the gold has not been found, the agent will take a chance moving into a square
// which is not guaranteed to be safe, but is believed to be unlikely to have a pit (~ a 20% chance given available information).
// The agent will die about 75 out of every 1000 trials, but will also get to the gold more often. This balances out to be slightly
// better than never taking the risk over enough trials. If no "unlikely pits" are found, the agent will 
// exit the map. Movement is handled in two ways: initial exploration is made with a simple series of 
// if statements. After a dead-end is reached or the gold is found, the agent will utilize an A*-style 
// search to determine the optimum path to an automatically determined goal. (I say A*-style since I don't think it quite meets the
// requirements for a true A* search.)
// The agent currently keeps track of more information than is strictly necessary, but I feel the 
// added information may prove handly in future iterations, so it has been retained. Exploration movements are 
// not always optimal, but improvement in this area is difficult and will do little to improve the agent's score.
action wumNeville_Flynn(sensor s, bool shouldReset)
{
	// whereX and whereY keep track of the agent's current coordinates.
	static unsigned whereX, whereY;
	// myMap stores all knowledge about the world.
	static vector<vector<vector<unsigned> > > myMap;
	// The bools are mostly self-explanatory. updated keeps track of whether the map has been updated to reflect
	// the wumpus being found.
	static bool wumpusDead, haveArrow, haveGold, wumpusFound, updated;
	// breadCrumbs stores movements returned by the path-finding function. Its name reflects my initial approach of 
	// using backtracking to explore the map.
	static deque<action> breadCrumbs;
	// shot is used to store shoot actions.
	static action shot;
	// move returns the agent's action in most cases.
	action move = doNothing;


   // Reset static variables for new world.
   if (shouldReset)
   {
	   whereX = 0;
	   whereY = 0;
	   shot = doNothing;
	   updated = false;
	   wumpusDead = false;
	   wumpusFound = false;
	   haveArrow = true;
	   haveGold = false;
	   breadCrumbs.clear();
	   myMap.clear();
	   myMap.resize(worldSize);
	   for(unsigned i = 0; i < worldSize; ++i)
	   {
		   myMap[i].resize(worldSize);
		   for(unsigned j = 0; j < worldSize; ++j)
			   myMap[i][j].resize(5);
	   }

      return doNothing;
   }
   // myMap stores information about each coordinate in the world in its own vector. For each square:
   // [0] is what's in a space that has been visited, 0 otherwise. 1 is empty, 2 is breeze, 3 is stench, 4 is breeze and stench.
   // [1] is whether a space has been visited before. 0 is false, 1 is true.
   // [2] is whether a space is safe. 0 is unknown, 1 is safe, 2 is suspected pit, 3 is suspected wumpus, 4 is both pit and wumpus.
   // [3] is certainty of a space having a pit. 0 is both unknown and none, 1 or more is possible. This is only used to determine if
   //     it is worthwhile to kill a wumpus currently. (Killing a wumpus in a pit doesn't help anything)
   // [4] is certainty of a space having the wumpus. 0 is uknown or none, 1 is possible, 4 is definite.
   else
   {	   	
   		// Boom, headshot!
   		// Update map and variables if the wumpus is dead.
	   if(s.perceiveScream)
	   {

	   	   haveArrow = false;
		   wumpusDead = true;
		   
		   for(unsigned i = 0; i < myMap.size() ; ++i)
		   	for(unsigned j = 0; j < myMap.size() ; ++j)
		   	{
		   		if(myMap[i][j][2] == 3)
		   			myMap[i][j][2] = 1;
		   		
		   		if(myMap[i][j][2] == 4)
		   			myMap[i][j][2] = 2;	
		   		myMap[i][j][4] = 0;
		   		
		   	}
		   	shot = doNothing;
	   } // If a shot was taken and missed, update map and variables.
 	   else if(shot != doNothing)
	   {
	   		haveArrow = false;
	   		// Update map according to direction of shot. 
	   		// Shots are only taken from adjacent squares to the 
	   		// wumpus, but all squares in a direction will be updated.
	   		if(shot == shootArrowNorth)
	   		{
	   			for(unsigned i = whereY + 1; i < worldSize; ++i)
	   			{
	   				if(myMap[whereX][i][2] == 3)
	   					myMap[whereX][i][2] = 1;
					if(myMap[whereX][i][2] == 4)
						myMap[whereX][i][2] = 2;
	   				myMap[whereX][i][4] = 0;	
	   			}
	   		}
	   		else if(shot == shootArrowSouth)
	   		{
	   			for(unsigned i = whereY - 1; i < worldSize; --i)
	   			{
	   				if(myMap[whereX][i][2] == 3)
	   					myMap[whereX][i][2] = 1;
	   				if(myMap[whereX][i][2] == 4)
	   					myMap[whereX][i][2] = 2;
	   				myMap[whereX][i][4] = 0;
	   			}
	   		}
	   		else if(shot == shootArrowEast)
	   		{
	   			for(unsigned i = whereX + 1; i < worldSize; ++i)
	   			{
	   				if(myMap[i][whereY][2] == 3)
	   					myMap[i][whereY][2] = 1;
	   				if(myMap[i][whereY][2] == 4)
	   					myMap[i][whereY][2] = 2;
	   				myMap[i][whereY][4] = 0;
	   			}
	   		
	   		}
	   		else if(shot == shootArrowWest)
	   		{
	   			for(unsigned i = whereX - 1; i < worldSize; --i)
	   			{
	   				if(myMap[i][whereY][2] == 3)
	   					myMap[i][whereY][2] = 1;
	   				if(myMap[i][whereY][2] == 4)
	   					myMap[i][whereY][2] = 2;
	   				myMap[i][whereY][4] = 0;
	   			}
	   		}
	   		// Shots should be 50/50 at worst. Update map to reflect 
	   		// knowledge of the wumpus's true location.
	   		for(unsigned i = 0; i < worldSize; ++i)
	   		{
	   			for(unsigned j = 0; j < worldSize; ++j)
	   			{
	   				if(myMap[i][j][4] > 0)
	   				{
	   					myMap[i][j][4] = 4;
	   				}
	   			}
	   		
	   		}

	   		wumpusFound = true;
	   		updated = true;
			shot = doNothing;
	   }

	   // If the current square hasn't been explored before, determine what you can learn from 
	   // percepts. 
	   if(myMap[whereX][whereY][1] == 0)
	   {
		   if(s.perceiveBreeze)
		   { // Double checking square hasn't been explored. Should always be true.
			   if(myMap[whereX][whereY][0] == 0)
			   {  // Record a breeze in this square.
			   		myMap[whereX][whereY][0]=2;
			   		// If there is only a breeze or if stench can be ignored.
				   if(!s.perceiveStench || wumpusDead)
				   {  
					   // Update knowledge of adjacent squares that are not known to be safe.
					   if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] != 1)
					   {
							// If the adjacent square is already suspected of being a 
							// wumpus, it must be safe. Update it accordingly.
					   	   if(myMap[whereX+1][whereY][2] == 3)
					   	   {
					   	   		myMap[whereX+1][whereY][2] = 1;
					   	   		myMap[whereX+1][whereY][3] = 0;
					   	   }
					   	   else
					   	   { // Otherwise, the square might be a pit.
							   myMap[whereX+1][whereY][2] = 2;
							   ++myMap[whereX+1][whereY][3];
						   }
						   // In either case, there's no wumpus nearby.
						   myMap[whereX+1][whereY][4] = 0;
					   }
					   if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					   {
					   
					   	   if(myMap[whereX-1][whereY][2] == 3)
					   	   {
					   	   		myMap[whereX-1][whereY][2] = 1;
					   	   		myMap[whereX-1][whereY][3] = 0;
					   	   }
					   	   else
					   	   {
							   myMap[whereX-1][whereY][2] = 2;
							   ++myMap[whereX-1][whereY][3];
						   }
						   myMap[whereX-1][whereY][4] = 0;
					   }
					   if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY+1][2] == 3)
					   	   {
					   	   		myMap[whereX][whereY+1][2] = 1;
					   	   		myMap[whereX][whereY+1][3] = 0;
					   	   }
					   	   else
					   	   {
							   myMap[whereX][whereY+1][2] = 2;
							   ++myMap[whereX][whereY+1][3];
						   }
						   myMap[whereX][whereY+1][4] = 0;
					   }
					   if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY-1][2] == 3)
					   	   {
					   	   		myMap[whereX][whereY-1][2] = 1;
					   	   		myMap[whereX][whereY-1][3] = 0;
					   	 
					   	   }
					   	   else
					   	   {
							   myMap[whereX][whereY-1][2] = 2;
							   ++myMap[whereX][whereY-1][3];
						   }
						   myMap[whereX][whereY-1][4] = 0;
					   }
				   } 
				   else if(wumpusFound)
				   {
					   // Update knowledge of adjacent squares that are not known to be safe or contain the wumpus.
					   if(whereX + 1 < worldSize && (myMap[whereX+1][whereY][2] == 0 || myMap[whereX+1][whereY][2] == 2))
					   {
						   myMap[whereX+1][whereY][2] = 2;
						   ++myMap[whereX+1][whereY][3];
						   myMap[whereX+1][whereY][4] = 0;
					   }
					   if(whereX - 1 < worldSize && (myMap[whereX-1][whereY][2] == 0 || myMap[whereX-1][whereY][2] == 2))
					   {
						   myMap[whereX-1][whereY][2] = 2;
						   ++myMap[whereX-1][whereY][3];
						   myMap[whereX-1][whereY][4] = 0;
					   }
					   if(whereY + 1 < worldSize && (myMap[whereX][whereY+1][2] == 0 || myMap[whereX][whereY+1][2] == 2))
					   {
						   myMap[whereX][whereY+1][2] = 2;
						   ++myMap[whereX][whereY+1][3];					  
						   myMap[whereX][whereY+1][4] = 0;
					   }
					   if(whereY -1 < worldSize && (myMap[whereX][whereY-1][2] == 0 || myMap[whereX][whereY-1][2] == 2))
					   {
						   myMap[whereX][whereY-1][2] = 2;
  						   ++myMap[whereX][whereY-1][3];
						   myMap[whereX][whereY-1][4] = 0;
					   }					   
				   }
				   // If the wumpus is still alive and its location is unknown, take into account combination of breeze and stench.
				   else if(!wumpusFound)
				   {	// Update map to reflect both breeze and stench.
					   myMap[whereX][whereY][0]=4;
						// Update adjacent squares not known to be safe.
					   if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] != 1)
					   {
					   	   if(myMap[whereX+1][whereY][2] == 0)
					   		   myMap[whereX+1][whereY][2] = 4;
					   		
					   	   if(myMap[whereX+1][whereY][2] != 3)
							   ++myMap[whereX+1][whereY][3];
						   // Update certainty of adjacent square having a wumpus.
						   if(myMap[whereX+1][whereY][2] != 2)
						   {
							   if(myMap[whereX+1][whereY][4] == 0)
							   {
							   		++myMap[whereX+1][whereY][4];
							   }
							   else if(myMap[whereX+1][whereY][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX+1][whereY][4] = 4;
							   }
							}
					   }
					   if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					   {
					   
					   	   if(myMap[whereX-1][whereY][2] == 0)
					   			myMap[whereX-1][whereY][2] = 4;
					   		
					   	   if(myMap[whereX-1][whereY][2] != 3)
							   ++myMap[whereX-1][whereY][3];
						   if(myMap[whereX-1][whereY][2] != 2)
						   {
							   if(myMap[whereX-1][whereY][4] == 0)
							   {
							   		++myMap[whereX-1][whereY][4];
							   }
							   else if(myMap[whereX-1][whereY][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX-1][whereY][4] = 4;
							   }
							}					   

					   }
					   if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					   {

					   	   if(myMap[whereX][whereY+1][2] == 0)
					   			myMap[whereX][whereY+1][2] = 4;
					   		
					   	   if(myMap[whereX][whereY+1][2] != 3)
							   ++myMap[whereX][whereY+1][3];
						   if(myMap[whereX][whereY+1][2] != 2)
						   {
							   if(myMap[whereX][whereY+1][4] == 0)
							   {
							   		++myMap[whereX][whereY+1][4];
							   }
							   else if(myMap[whereX][whereY+1][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX][whereY+1][4] = 4;
							   }
							}						   

					   }
					   if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY-1][2] == 0)
					   			myMap[whereX][whereY-1][2] = 4;
					   		
					   	   if(myMap[whereX][whereY-1][2] != 3)
							   ++myMap[whereX][whereY-1][3];
						   if(myMap[whereX][whereY-1][2] != 2)
						   {
							   if(myMap[whereX][whereY-1][4] == 0)
							   {
							   		++myMap[whereX][whereY-1][4];
							   }
							   else if(myMap[whereX][whereY-1][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX][whereY-1][4] = 4;
							   }
							}						   					   		  
					   }
				   }
			   }
		   }
		   // If there's a stench and no breeze, and if the stench still needs to be paid attention to.
		   if(s.perceiveStench && !s.perceiveBreeze && !wumpusDead && !wumpusFound)
		   {
			   	// Update current square.
					myMap[whereX][whereY][0]=3;
					// Update adjacent squares which are not known to be safe.
					if(whereX + 1 < worldSize && (myMap[whereX+1][whereY][2] != 1))
					{
						// If a square is already suspected of a pit, then it's neither.
						if(myMap[whereX+1][whereY][2] == 2)
						{
							myMap[whereX+1][whereY][2] = 1;
							myMap[whereX+1][whereY][4] = 0;
						}
						else
						{	// Otherwise, suspect wumpus
							myMap[whereX+1][whereY][2] = 3;


							// Update certainty of a wumpus being there.
							if(myMap[whereX+1][whereY][4] == 0)
							{
							   	++myMap[whereX+1][whereY][4];
							}
							else if(myMap[whereX+1][whereY][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX+1][whereY][4] = 4;
							}	
						}	
						// No breeze => no pit										
						myMap[whereX+1][whereY][3] = 0;					
					}
					if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					{
						if(myMap[whereX-1][whereY][2] == 2)
						{
							myMap[whereX-1][whereY][2] = 1;
							myMap[whereX-1][whereY][4] = 0;
						}
						else
						{
							myMap[whereX-1][whereY][2] = 3;
							if(myMap[whereX-1][whereY][4] == 0)
							{
							   	++myMap[whereX-1][whereY][4];
							}
							else if(myMap[whereX-1][whereY][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX-1][whereY][4] = 4;
							}
						}							
						myMap[whereX-1][whereY][3] = 0;
						
					}
					if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					{
						if(myMap[whereX][whereY+1][2] == 2)
						{
							myMap[whereX][whereY+1][2] = 1;
							myMap[whereX][whereY+1][4] = 0;
						}
						else
						{
							myMap[whereX][whereY+1][2] = 3;
							if(myMap[whereX][whereY+1][4] == 0)
							{
							   	++myMap[whereX][whereY+1][4];
							}
							else if(myMap[whereX][whereY+1][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX][whereY+1][4] = 4;
							}	
					
						}	
											
						myMap[whereX][whereY+1][3] = 0;												
						
					}
					if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					{
						if(myMap[whereX][whereY-1][2] == 2)
						{
							myMap[whereX][whereY-1][2] = 1;
							myMap[whereX][whereY-1][4] = 0;
						}
						else
						{
							myMap[whereX][whereY-1][2] = 3;					
							if(myMap[whereX][whereY-1][4] == 0)
							{
							   	++myMap[whereX][whereY-1][4];
							}
							else if(myMap[whereX][whereY-1][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX][whereY-1][4] = 4;
							}							
						}
						myMap[whereX][whereY-1][3] = 0;						
						
					}

		   }
		   // If there's nothing to worry about, update accordingly.
		   if(!(s.perceiveStench || s.perceiveBreeze) || (wumpusDead && !s.perceiveBreeze) || (wumpusFound && !s.perceiveBreeze))
		   {   // Nothing to be seen here.
			   myMap[whereX][whereY][0] = 1;
			   // Adjacent squares, how's it going?
			   if(whereX + 1 < worldSize)
			   {   // If it's not the square with the wumpus, it's safe.
				   myMap[whereX+1][whereY][2] = (myMap[whereX+1][whereY][4] == 4 ? 3 : 1);
				   // If it's not already certain, there's no wumpus nearby.
				   if(myMap[whereX+1][whereY][4] == 1)
				   	myMap[whereX+1][whereY][4] = 0;
					// And there's definitely no pit.
				   myMap[whereX+1][whereY][3] = 0;
			   }
			   if(whereX - 1 < worldSize)
			   {
				   myMap[whereX-1][whereY][2] = (myMap[whereX-1][whereY][4] == 4 ? 3 : 1);
				   
				   if(myMap[whereX-1][whereY][4] == 1)
				   	myMap[whereX-1][whereY][4] = 0;
				   
				   myMap[whereX-1][whereY][3] = 0;
			   }
			   if(whereY + 1 < worldSize)
			   {
				   myMap[whereX][whereY+1][2] = (myMap[whereX][whereY+1][4] == 4 ? 3 : 1);
				   				   
				   if(myMap[whereX][whereY+1][4] == 1)
				   	myMap[whereX][whereY+1][4] = 0;

				   myMap[whereX][whereY+1][3] = 0;
			   }
			   if(whereY - 1 < worldSize)
			   {		
				   myMap[whereX][whereY-1][2] = (myMap[whereX][whereY-1][4] == 4 ? 3 : 1);

				   if(myMap[whereX][whereY-1][4] == 1)
				   	myMap[whereX][whereY-1][4] = 0;
				  
				   myMap[whereX][whereY-1][3] = 0;
		 	   }
		   }
		   
		   // Mark the current square as safe and visited.
		   myMap[whereX][whereY][2] = 1;
		   myMap[whereX][whereY][1] = 1;
		   // Once the wumpus has been found, make sure the map is updated properly.
		   if(wumpusFound && !wumpusDead && !updated)
		   {
		   	 updated = true;
		   	 
		  	 for(unsigned i = 0; i < myMap.size() ; ++i)
		 	  	for(unsigned j = 0; j < myMap.size() ; ++j)
			   	{
					if(myMap[i][j][4] != 4)
					{
						myMap[i][j][4] = 0;
						if(myMap[i][j][2] == 3)
							myMap[i][j][2] = 1;
						if(myMap[i][j][2] == 4)
							myMap[i][j][2] = 2;
							
					}
			   	}		 
		   }

	   } // End map update.


	   // Everything that glitters is gold.
	   if(s.perceiveGlitter)
	   {
		   haveGold = true;
		   return grab;
	   }
	   // Move to exit if you have the gold.
	   if(haveGold)
	   {
			// If you don't know what to do, ask!
		   if(breadCrumbs.empty())
		   {
			   breadCrumbs = findPathNeville(myMap, whereX, whereY, haveGold, haveArrow);
		   }
		   // The path finding function should never return an empty deque, but just to be sure...
		   if(!breadCrumbs.empty())
		   {
				move = breadCrumbs.back();
				breadCrumbs.pop_back();
		   }
		   
		   // Pathfinding has proven effective enough that keeping track of current location here
		   // is unecessary.

		   return move;
	   }
	   // No gold, no problem. Let's have a look around.
	   // Move in the first safe, unexplored square found. This movement priority seems to be optimal for
	   // this agent.
	   	if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] == 1 && myMap[whereX-1][whereY][1] == 0)
		{
			move = moveWest;
			--whereX;
			breadCrumbs.clear();
		}	
		else if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] == 1 && myMap[whereX][whereY-1][1] == 0)
		{
			move = moveSouth;
			--whereY;
			breadCrumbs.clear();
		}			
		else if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] == 1 && myMap[whereX+1][whereY][1] == 0)
		{
			move = moveEast;
			++whereX;
			breadCrumbs.clear();
		}
		else if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] == 1 && myMap[whereX][whereY+1][1] == 0)
		{
			move = moveNorth;
			++whereY;
			breadCrumbs.clear();
		}


		// No safe and unexplored adjacent squares? Let's see what else we can find...
		if(move == doNothing)
		{
		
			if(breadCrumbs.empty())
			{
				breadCrumbs = findPathNeville(myMap, whereX, whereY, haveGold, haveArrow);
			}
			
			if(!breadCrumbs.empty())
			{
				move = breadCrumbs.back();
				breadCrumbs.pop_back();
			}
			// Keep track of what's being done.
			if(move == moveWest)
				--whereX;
			else if(move == moveEast)
				++whereX;
			else if(move == moveSouth)
				--whereY;
			else if(move == moveNorth)
				++whereY;
			else if(move == shootArrowNorth)
				shot = shootArrowNorth;
			else if(move == shootArrowSouth)
				shot = shootArrowSouth;
			else if(move == shootArrowEast)
				shot = shootArrowEast;
			else if(move == shootArrowWest)
				shot = shootArrowWest;
		}



      return move;
   }
}
// This is the control function for the A*-style search. It determines the location and type of goals to be found.
deque<action> findPathNeville(vector<vector<vector<unsigned> > > &myMap, unsigned whereX, unsigned whereY, bool haveGold, bool haveArrow)
{
	// Searched keeps track of squares already on a path.
	set<unsigned> searched;
	// Coordinates to go to.
	vector<unsigned> goal;
	// All goals are added to ideas.
	vector<vector<unsigned> > ideas;
	multimap<unsigned, vector<unsigned> >::iterator it, second;
	// frontier holds possible squares to move to, ranked by their function value.
	multimap<unsigned, vector<unsigned> > frontier;
	// A proposal is a sequence of actions to the goal. Will be judged by size.
	deque<action> proposal;
	unsigned value;
	// Is wumpus on the menu?
	bool hunting = false;
	pair<unsigned, deque<action> > hold;
	unsigned limit;
	
	
	// Don't have the gold, look for places to go.
	if(!haveGold)
	{
		for(unsigned i = 0; i < worldSize; ++i)
		{
			for(unsigned j = 0; j < worldSize; ++j)
			{	// If there's a safe unexplored square, add it as a potential goal.
				if(myMap[i][j][1] == 0 && myMap[i][j][2] == 1)
				{
					goal.push_back(i);
					goal.push_back(j);
					ideas.push_back(goal);
					goal.clear();
				}
			}
		}
		// If no safe, unexplored squares are found and the arrow hasn't been shot,
		// look for the wumpus.
		if(ideas.empty() && haveArrow)
		{
			for(unsigned i = 0; i < worldSize; ++i)
			{
				for(unsigned j = 0; j < worldSize; ++j)
				{	// Add all possible locations of the wumpus to ideas as long as
					// there's no pit suspected in the same square.
					if(myMap[i][j][4] > 0 && myMap[i][j][3] == 0)
					{
						goal.push_back(i);
						goal.push_back(j);
						ideas.push_back(goal);
						goal.clear();
					}
				}
			}

		}
		// If there was no wumpus to kill, see if there's a suspected pit that's 
		// probably safe (explained in the isItSafe function).
		if(ideas.empty())
		{
			ideas = isItSafe(myMap);
			// Clear out squares that might have the wumpus if the arrow has already
			// been used.
			if(!haveArrow)
			{			
				for(unsigned i = 0; i < ideas.size(); ++i)
				{
					goal = ideas[i];
					if(myMap[goal[0]][goal[1]][4] > 0)
					{
						ideas.erase(ideas.begin() + i);
						--i;
					}
					goal.clear();
				}
			}	
		}
	}
	// If there's nothing else to do, or if the gold has been found, head to the exit.
	if(ideas.empty())
	{
		goal.push_back(0);
		goal.push_back(0);
		ideas.push_back(goal);
		goal.clear();
		// Maybe you can just wrap a rock in some gold colored foil?
		haveGold = true;
	}
	// Add goals in ideas to frontier along with heuristic value based on city block distance, plus 10 for needing to kill the
	// wumpus. Usually when the wumpus is going to be killed, it will be the only option anyway, but isItSafe may return the 
	// wumpus space as one of other options. (This may seem a bit redundant here, but I'd already written the rest of the
	// code when I decided to add it. Ideas will never be very large, so the amount of time lost here is pretty insignificant.)
	for(unsigned i = 0; i < ideas.size(); ++i)
	{
		value = (whereY < ideas[i][1] ? (ideas[i][1] - whereY) : (whereY - ideas[i][1])) + (whereX < ideas[i][0] ? (ideas[i][0] - whereX) : (whereX - ideas[i][0]));
		if(myMap[ideas[i][0]][ideas[i][1]][4]>0)
			value += 10;
		frontier.insert(pair<unsigned, vector<unsigned> >(value, ideas[i]));
	}
	// Add the current square to those searched.
	// Squares stored as two-digit numbers (YX)
	searched.insert(whereX + (10*whereY));
	it = frontier.begin();
	// Consider goals based on their value.
	while(it != frontier.end() && proposal.empty())
	{
		goal = (*it).second;
		
		if(myMap[goal[0]][goal[1]][4] > 0)
		{// Enough is enough! I have had it with these monkey fighting wumpuses on this Monday through Friday map! 
			hunting = true;
		}
		else
		{
			hunting = false;
		}
		
		if(frontier.size() > 1)
		{
			second = it;
			++second;
			if(second != frontier.end())
				limit = (*second).first;
			else 
				limit = 100;
		}
		else
		{
			limit = 100;
		}
		
		// If not already at the goal, call recursive search to find a path there.
		if(goal[0] != whereX || goal[1] != whereY)
		{
			hold = searchHereNeville(goal,whereX, whereY,  myMap, 1, limit, searched, hunting);
			if(hold.first == 0)
				proposal = hold.second;
			else
			{
				frontier.erase(it);
				frontier.insert(pair<unsigned, vector<unsigned> > (hold.first, goal));
				it = frontier.begin();
			}

		}
		
		// Once you get to the exit, climb out.
		if(haveGold)
		{
			proposal.push_front(climbOut);
		}
		
		
	}

	return proposal;

}
// This is the recursive A*-style search.
pair<unsigned, deque<action> > searchHereNeville(vector<unsigned> goal, unsigned whereX, unsigned whereY, vector<vector<vector<unsigned> > > &myMap, unsigned path, unsigned limit, set<unsigned> searched, bool hunting)
{
	
	multimap<unsigned, vector<unsigned> >::iterator it, second;
	// frontier holds possible squares to move to, ranked by their function value.
	multimap<unsigned, vector<unsigned> > frontier;
	// coord stores coordinates
	vector<unsigned> coord;
	// hold is used to insert elements into the frontier
	pair<unsigned, vector<unsigned> > hold;
	unsigned newLimit = 100;
	// plan is a series of actions to the goal.
	pair<unsigned, deque<action> > plan;
	unsigned currentX, currentY, value, goalX, goalY;

	// Store goal coordinates for more convenient reference.
	goalX = goal[0];
	goalY = goal[1];
	
		// Base case: if one away from the goal, move there.
		if(!hunting)
		{
			if(whereX - 1 == goalX && whereY == goalY)

			{
				plan.second.push_back(moveWest);
			}
			else if(whereX + 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveEast);
			}
			else if(whereY - 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveSouth);
			}
			else if(whereY + 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveNorth);
			}
		}
		// Unless hunting, then shoot first.
		else
		{
			if(whereX - 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveWest);
				plan.second.push_back(shootArrowWest);
			}
			else if(whereX + 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveEast);
				plan.second.push_back(shootArrowEast);
			}
			else if(whereY - 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveSouth);
				plan.second.push_back(shootArrowSouth);
			}
			else if(whereY + 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveNorth);
				plan.second.push_back(shootArrowNorth);
			}		
		}
		// Return base case once encountered.
		if(!plan.second.empty())
		{
			plan.first = 0;
			return plan;
		}

		// Otherwise, add all available moves to the frontier.
		if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] == 1)
		{
			coord.clear();
			currentX = whereX + 1;
			currentY = whereY;
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{// Squares are evaluated according to city block distance to the goal plus path cost.
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

			
				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(0); // Used to identify which direction the move is in.
				hold.first = value; 
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] == 1)
		{
			coord.clear();
			currentX = whereX -1;
			currentY = whereY;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(1);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);

			}
		}
		if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] == 1)
		{
			coord.clear();
			currentX = whereX;
			currentY = whereY + 1;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(2);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		if(whereY - 1 < worldSize && myMap[whereX][whereY-1][2] == 1)
		{
			coord.clear();
			currentX = whereX;
			currentY = whereY -1;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))			
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;
			
				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(3);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		// Initialize iterator to the beginning of frontier.
		// multimaps sort elements by size, so the first element will always
		// have the smallest possible value.
		it = frontier.begin();		
		// Keep looking until a good enough plan has been found or there are no more
		// options.
		if(!frontier.empty())
		{
			
			while(plan.second.empty() && it != frontier.end())
			{		

				if((*it).first <= limit)
				{
				
					if(frontier.size() > 1)
					{
						second = it;
						++second;
						if(second != frontier.end())
						{
							newLimit = (*second).first;
						}
					}
					plan = searchHereNeville(goal, (*it).second[0] , (*it).second[1], myMap, path + 1, (newLimit < limit ? newLimit : limit), searched, hunting);
					// If the search fails, plan will be empty. Otherwise...
					if(plan.first == 0)
					{
						// Add move to plan based on what square was considered.
						if((*it).second[2] == 0)
						{
							plan.second.push_back(moveEast);
						}
						else if((*it).second[2] == 1)
						{
							plan.second.push_back(moveWest);
						}
						else if((*it).second[2] == 2)
						{
							plan.second.push_back(moveNorth);
						}
						else if((*it).second[2] == 3)
						{
							plan.second.push_back(moveSouth);
						}
					}
					else
					{
						hold.second = (*it).second;
						hold.first = plan.first;
						frontier.erase(it);
						frontier.insert(hold);
						it = frontier.begin();
					}
				}
				else
				{
					plan.first = (*it).first;
					return plan;
				}
			}
			
		}
		else
		{
			plan.first = 101;
		}

		return plan;
}
// This function identifies potential pits that are not necessarily actually pits.
// It can look through all subsets of the set of pits to determine whether they completely
// account for all observed breezes, then return the coordinates of pits which were not included
// in the subsets. It's currently set to look at only subsets of size one less than the size of 
// of the full set. This reduces the number of risks the agent will take. 
vector<vector<unsigned> > isItSafe(vector<vector<vector<unsigned> > > &myMap)
{
	// All subsets of pits which account for all breezes are added to consistent.
	vector<vector<vector<unsigned> > > consistent;
	// Pits contains the coordinates of all pits. Hypothesis is used to hold a subset of pits.
	vector<vector<unsigned> > pits, hypothesis;
	// coord is used as a holder.
	vector<unsigned> coord;
	// Breeze is the set of all squares with breezes. Accounted is all pits accounted for by a subset.
	set<unsigned> breeze, accounted;
	// currentSize is the size of the subsets currently being considered.
	unsigned currentSize;
	
	// Find all pits and breezes and add them to their respective containers.
	for(unsigned i = 0; i < myMap.size(); ++i)
	{
		for(unsigned j = 0; j < myMap.size(); ++j)
		{
			if(myMap[i][j][2] == 2 || myMap[i][j][2] == 4)
			{
				coord.push_back(i);
				coord.push_back(j);
				pits.push_back(coord);
				coord.clear();
			}
			if(myMap[i][j][0] == 2 || myMap[i][j][0] == 4)
			{
				breeze.insert(i + (j*10));
			}
		}
	}
	
	if(pits.size() < 2)//|| pits.size() > 3)
		return hypothesis;
	if(pits.size() >= 3 && breeze.size() <= pits.size())
		return hypothesis;
	
	// The original idea was to find the smallest size subsets of pits which would explain all breezes.
	// Though this worked fairly well, it took too many risks to outperform an agent which did not take
	// any risks. By only considering subsets of size one smaller than pits, the agent takes small
	// enough risks to outperform an agent which never takes any risks at all.
	currentSize = 1;
	
	// This loop is currently not necessary (since the function only needs to be called once from here in this case), 
	// but I've left it in for future reference and to demonstrate how it would work in general. 
	while(currentSize < pits.size() && consistent.empty())
	{
		isItSafe(myMap, pits, breeze, consistent, currentSize, hypothesis, 0);	
		++currentSize;
	}
	// If subsets which explain all breezes have been found, go through and add all included pits to 
	// accounted.
	if(!consistent.empty())
	{
		for(unsigned j = 0; j < consistent.size(); ++j)
		{
			for(unsigned i = 0; i < consistent[j].size(); ++i)
			{
				if(!(accounted.count(consistent[j][i][0]+(consistent[j][i][1]*10))))
					accounted.insert(consistent[j][i][0]+(consistent[j][i][1]*10));
			}
		}
		// If there are pits not included in accounted, find them and add to hypothesis.
		if(accounted.size() < pits.size())
		{
			for(unsigned i = 0; i < pits.size(); ++i)
			{
				if(!(accounted.count(pits[i][0]+(pits[i][1]*10))))
				{
					hypothesis.push_back(pits[i]);
				}
			}		
		}
	}	
	// Hypothesis will be empty if no consistent subsets are found or if all pits are included
	// in accounted. In this case, the path finding function will direct the agent to climb out.
	return hypothesis;
}
// This is the recursive portion of the search.
void isItSafe(vector<vector<vector<unsigned> > > &myMap, vector<vector<unsigned> > &pits, set<unsigned> &breeze, vector<vector<vector<unsigned> > > &consistent, unsigned currentSize, vector<vector<unsigned> > hypothesis, unsigned num)
{
	vector<unsigned> coord;
	// Here, accounted is all breezes accounted for by a subset.
	set<unsigned> accounted;
	
	for(; num < pits.size(); ++num)
	{	
		hypothesis.push_back(pits[num]);
		// If hypothesis is smaller than the subset size being considered, go down another level.
		if(hypothesis.size() < currentSize)
		{
			isItSafe(myMap, pits, breeze, consistent, currentSize, hypothesis, num+1);
		}
		// Otherwise, see if hypothesis is consistent.
		else
		{
			for(unsigned j = 0; j < hypothesis.size(); ++j)
			{
				coord = hypothesis[j];
				// If an adjacent square has been explored and isn't accounted for, add it to accounted.
				// Any adjacent, explored square will have a breeze, so there's no seperate checking for that.
				if(coord[0] - 1 < worldSize && myMap[coord[0]-1][coord[1]][1] && !(accounted.count((coord[0]-1)+(coord[1]*10))))
				{
					accounted.insert((coord[0]-1)+(coord[1]*10));
				}
				if(coord[0] + 1 < worldSize && myMap[coord[0]+1][coord[1]][1] && !(accounted.count((coord[0]+1)+(coord[1]*10))))
				{
					accounted.insert((coord[0]+1)+(coord[1]*10));
				}
				if(coord[1] - 1 < worldSize && myMap[coord[0]][coord[1]-1][1] && !(accounted.count(coord[0]+((coord[1]-1)*10))))
				{
					accounted.insert(coord[0]+((coord[1]-1)*10));
				}
				if(coord[1] + 1 < worldSize && myMap[coord[0]][coord[1]+1][1] && !(accounted.count(coord[0]+((coord[1]+1)*10))))
				{
					accounted.insert(coord[0]+((coord[1]+1)*10));
				}
			}
			// If all breezes are accounted for, add hypothesis to consistent.
			if(accounted.size() == breeze.size())
			{
				consistent.push_back(hypothesis);
			}
			
			accounted.clear();	
		}
		
		hypothesis.pop_back();			
	}
}


// Here there be dragons



// This function allows you to manually play through the world. It keeps a map
// which is automatically updated and printed to show where you are, where you've been,
// and what was perceived in each square. The main and apply action functions have been 
// slightly modified to add to the experience.
action wumPlayer(sensor s, bool shouldReset)
{

	static unsigned whereX, whereY;
	static vector<vector<vector<unsigned> > > myMap;
	static bool haveArrow, haveGold;
	action move = doNothing;
	string command;
	bool accepted = false;

   if (shouldReset)
   {
	   whereX = 0;
	   whereY = 0;
	   haveArrow = true;
	   haveGold = false;
	   myMap.clear();
	   myMap.resize(worldSize);
	   for(unsigned i = 0; i < worldSize; ++i)
	   {
		   myMap[i].resize(worldSize);
		   for(unsigned j = 0; j < worldSize; ++j)
			   myMap[i][j].resize(5);
	   }
	   cout << "Map Legend\n0: Unexplored\n1: Empty\n2: Breeze\n3: Stench\n4: Breeze & Stench\nP: Current position";
	
      return doNothing;
   }
   // myMap stores information about each coordinate in the world in its own vector. For each square:
   // [0] is what's in a space that has been visited, 0 otherwise. 1 is empty, 2 is breeze, 3 is stench, 4 is breeze and stench.
   // [1] is whether a space has been visited before. 0 is false, 1 is true.

   else
   {	   	

	   // If the current square hasn't been explored before, determine what you can learn from 
	   // percepts. 
	   if(myMap[whereX][whereY][1] == 0)
	   {
		   if(s.perceiveBreeze)
		   { // Double checking square hasn't been explored. Should always be true.
			   if(myMap[whereX][whereY][0] == 0)
			   {	// If there is only a breeze
				   if(!s.perceiveStench)
				   {  // Record a breeze in this square.
					   myMap[whereX][whereY][0]=2;
				
				   } 
				   else
				   {	// Update map to reflect both breeze and stench.
					   myMap[whereX][whereY][0]=4;
					}
			
				}
		   }
		   
		   if(s.perceiveStench && !s.perceiveBreeze)
		   {
			   if(myMap[whereX][whereY][0] == 0)
			   {
					myMap[whereX][whereY][0]=3;
				}
		   }
		   if(!(s.perceiveStench || s.perceiveBreeze))
		   {
			   myMap[whereX][whereY][0] = 1;
	
		   }

		   myMap[whereX][whereY][1] = 1;

		  }
		   


	   
		   	cout << "\nmyMap\n";
		   	for(unsigned j = myMap.size()-1; j < myMap.size(); --j)
		   	{
		   		for(unsigned i = 0; i < myMap.size(); ++i)
		   		{
		   			if(whereX == i && whereY == j)
		   				cout << "P ";
		   			else
		   				cout << myMap[i][j][0] << " ";
		   			
		   		}
		   		
		   		cout << endl;	
		   	}


	if(!haveGold && !s.perceiveGlitter && myMap[whereX][whereY][0] == 1)
		cout << "There doesn't seem to be anything here.\n";

	if(s.perceiveBump)
		cout << "Ow!\n";	
	else
	{
		if(haveGold)
		{
			cout << "This gold is heavy. You'd better get out of here.\n";
			haveGold = false;
		}
		if(s.perceiveGlitter)
			cout << "Something in the corner glitters in the torchlight.\n";
		if(s.perceiveStench)
			cout << "It stinks in here!\n";
		if(s.perceiveBreeze)
			cout << "You feel a slight breeze.\n";
		if(s.perceiveScream)
			cout << "A bloodcurdling scream echoes through the dungeon!\n";
	}
	
	

	while(!accepted)
	{
		cout << "What will you do?\n" << "Move, shoot, climb out, grab\n";	
		getline(cin, command);
		command.erase(remove(command.begin(), command.end(), ' '), command.end());
		transform(command.begin(), command.end(), command.begin(), ::tolower);
		if(command == "movenorth" || command == "north" || command == "moven" || command == "n")
			move = moveNorth;
		else if(command == "movesouth" || command == "south" || command == "moves" || command == "s")
			move = moveSouth;
		else if(command == "movewest" || command == "west" || command == "movew" || command == "w")
			move = moveWest;
		else if(command == "moveeast" || command == "east" || command == "movee" || command == "e")
			move = moveEast;

		else if(command == "move" || command == "m")
		{
			cout << "Which way will you move?\nN, S, E, W:";
			getline(cin, command);
			transform(command.begin(), command.end(), command.begin(), ::tolower);

			if(command == "n" || command == "north")
				move = moveNorth;
			else if(command == "s" || command == "south")
				move = moveSouth;
			else if(command == "e" || command == "east")
				move = moveEast;
			else if(command == "w" || command == "west")
				move = moveWest;
		}
		else if(command == "shoot" && haveArrow)
		{
			cout << "Which way will you shoot?\nN, S, E, W:";
			getline(cin, command);
			transform(command.begin(), command.end(), command.begin(), ::tolower);
			if(command == "n" || command == "north")
				move = shootArrowNorth;
			else if(command == "s" || command == "south")
				move = shootArrowSouth;
			else if(command == "e" || command == "east")
				move = shootArrowEast;
			else if(command == "w" || command == "west")
				move = shootArrowWest;
		}
		else if(command == "shoot" && !haveArrow)
			cout << "You don't have anything to shoot with!\n";
		if(command == "climbout" || command == "climb" || command == "c")
			move = climbOut;
		if(command == "grab" || command == "g")
			move = grab;
		
		
		if(move != doNothing)
			accepted = true;
		else
			cout << "You can't just sit here all day.\n";
		
	}



	if(move == moveWest && whereX - 1 < worldSize)
		--whereX;
	else if(move == moveEast && whereX + 1 < worldSize)
		++whereX;
	else if(move == moveSouth && whereY - 1 < worldSize)
		--whereY;
	else if(move == moveNorth && whereY + 1 < worldSize)
		++whereY;
	else if(move == shootArrowNorth)
		haveArrow = false;
	else if(move == shootArrowSouth)
		haveArrow = false;
	else if(move == shootArrowEast)
		haveArrow = false;
	else if(move == shootArrowWest)
		haveArrow = false;
	else if(move == grab && s.perceiveGlitter)
		haveGold = true;
	else if(move == grab)
		cout << "You grab at thin air, but it dodges!\n";
	else if(move == climbOut && (whereX != 0 || whereY != 0))
		cout << "You try to climb up the wall, but you just slide right back down.\n";
	else if(move == climbOut)
		cout << "Fresh air at last!\n";

      return move;
   }
}

// This is a copy of my agent with a few differences for testing purposes. 
// Its performance is currently identical to the main agent.
action wumNeville_Flynn3(sensor s, bool shouldReset)
{
	// whereX and whereY keep track of the agent's current coordinates.
	static unsigned whereX, whereY;
	// myMap stores all knowledge about the world.
	static vector<vector<vector<unsigned> > > myMap;
	// The bools are mostly self-explanatory. updated keeps track of whether the map has been updated to reflect
	// the wumpus being found.
	static bool wumpusDead, haveArrow, haveGold, wumpusFound, updated, risk;
	// breadCrumbs stores movements returned by the path-finding function. Its name reflects my initial approach of 
	// using backtracking to explore the map.
	static deque<action> breadCrumbs;
	// shot is used to store shoot actions.
	static action shot;
	// move returns the agent's action in most cases.
	action move = doNothing;


   // Reset static variables for new world.
   if (shouldReset)
   {
	   whereX = 0;
	   whereY = 0;
	   shot = doNothing;
	   updated = false;
	   wumpusDead = false;
	   wumpusFound = false;
	   haveArrow = true;
	   haveGold = false;
	   risk = true;
	   breadCrumbs.clear();
	   myMap.clear();
	   myMap.resize(worldSize);
	   for(unsigned i = 0; i < worldSize; ++i)
	   {
		   myMap[i].resize(worldSize);
		   for(unsigned j = 0; j < worldSize; ++j)
			   myMap[i][j].resize(5);
	   }

      return doNothing;
   }
   // myMap stores information about each coordinate in the world in its own vector. For each square:
   // [0] is what's in a space that has been visited, 0 otherwise. 1 is empty, 2 is breeze, 3 is stench, 4 is breeze and stench.
   // [1] is whether a space has been visited before. 0 is false, 1 is true.
   // [2] is whether a space is safe. 0 is unknown, 1 is safe, 2 is suspected pit, 3 is suspected wumpus, 4 is both pit and wumpus.
   // [3] is certainty of a space having a pit. 0 is both unknown and none, 1 or more is possible. This is only used to determine if
   //     it is worthwhile to kill a wumpus currently. (Killing a wumpus in a pit doesn't help anything)
   // [4] is certainty of a space having the wumpus. 0 is uknown or none, 1 is possible, 4 is definite.
   else
   {	   	
   		// Boom, headshot!
   		// Update map and variables if the wumpus is dead.
	   if(s.perceiveScream)
	   {

	   	   haveArrow = false;
		   wumpusDead = true;
		   
		   for(unsigned i = 0; i < myMap.size() ; ++i)
		   	for(unsigned j = 0; j < myMap.size() ; ++j)
		   	{
		   		if(myMap[i][j][2] == 3)
		   			myMap[i][j][2] = 1;
		   		
		   		if(myMap[i][j][2] == 4)
		   			myMap[i][j][2] = 2;	
		   		myMap[i][j][4] = 0;
		   		
		   	}
		   	shot = doNothing;
	   } // If a shot was taken and missed, update map and variables.
	   else if(shot != doNothing)
	   {
	   		haveArrow = false;
	   		// Update map according to direction of shot. 
	   		// Shots are only taken from adjacent squares to the 
	   		// wumpus, but all squares in a direction will be updated.
	   		if(shot == shootArrowNorth)
	   		{
	   			for(unsigned i = whereY + 1; i < worldSize; ++i)
	   			{
	   				if(myMap[whereX][i][2] == 3)
	   					myMap[whereX][i][2] = 1;
					if(myMap[whereX][i][2] == 4)
						myMap[whereX][i][2] = 2;
	   				myMap[whereX][i][4] = 0;	
	   			}
	   		
	   		
	   		}
	   		else if(shot == shootArrowSouth)
	   		{
	   			for(unsigned i = whereY - 1; i < worldSize; --i)
	   			{
	   				if(myMap[whereX][i][2] == 3)
	   					myMap[whereX][i][2] = 1;
	   				if(myMap[whereX][i][2] == 4)
	   					myMap[whereX][i][2] = 2;
	   				myMap[whereX][i][4] = 0;
	   			}
	   		
	   		
	   		}
	   		else if(shot == shootArrowEast)
	   		{
	   			for(unsigned i = whereX + 1; i < worldSize; ++i)
	   			{
	   				if(myMap[i][whereY][2] == 3)
	   					myMap[i][whereY][2] = 1;
	   				if(myMap[i][whereY][2] == 4)
	   					myMap[i][whereY][2] = 2;
	   				myMap[i][whereY][4] = 0;
	   			}
	   		
	   		}
	   		else if(shot == shootArrowWest)
	   		{
	   			for(unsigned i = whereX - 1; i < worldSize; --i)
	   			{
	   				if(myMap[i][whereY][2] == 3)
	   					myMap[i][whereY][2] = 1;
	   				if(myMap[i][whereY][2] == 4)
	   					myMap[i][whereY][2] = 2;
	   				myMap[i][whereY][4] = 0;
	   			}
	   		}
	   		// Shots are 50/50 at worst. Update map to reflect 
	   		// knowledge of the wumpus's true location.
	   		for(unsigned i = 0; i < worldSize; ++i)
	   		{
	   			for(unsigned j = 0; j < worldSize; ++j)
	   			{
	   				if(myMap[i][j][4] > 0)
	   				{
	   					myMap[i][j][4] = 4;

	   				}
	   			}
	   		
	   		}
	   		
	   		
	   		wumpusFound = true;
	   		updated = true;
			shot = doNothing;
	   }
	   
	   
	   
	   // If the current square hasn't been explored before, determine what you can learn from 
	   // percepts. 
	   if(myMap[whereX][whereY][1] == 0)
	   {
		   if(s.perceiveBreeze)
		   { // Double checking square hasn't been explored. Should always be true.
			   if(myMap[whereX][whereY][0] == 0)
			   {  // Record a breeze in this square.
			   		myMap[whereX][whereY][0]=2;
			   		// If there is only a breeze or if stench can be ignored.
				   if(!s.perceiveStench || wumpusDead)
				   {  
					   // Update knowledge of adjacent squares that are not known to be safe.
					   if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] != 1)
					   {
							// If the adjacent square is already suspected of being a 
							// wumpus, it must be safe. Update it accordingly.
					   	   if(myMap[whereX+1][whereY][2] == 3)
					   	   {
					   	   		myMap[whereX+1][whereY][2] = 1;
					   	   		myMap[whereX+1][whereY][3] = 0;
					   	   }
					   	   else
					   	   { // Otherwise, the square might be a pit.
							   myMap[whereX+1][whereY][2] = 2;
							   ++myMap[whereX+1][whereY][3];
						   }
						   // In either case, there's no wumpus nearby.
						   myMap[whereX+1][whereY][4] = 0;
					   }
					   if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					   {
					   
					   	   if(myMap[whereX-1][whereY][2] == 3)
					   	   {
					   	   		myMap[whereX-1][whereY][2] = 1;
					   	   		myMap[whereX-1][whereY][3] = 0;
					   	   }
					   	   else
					   	   {
							   myMap[whereX-1][whereY][2] = 2;
							   ++myMap[whereX-1][whereY][3];
						   }
						   myMap[whereX-1][whereY][4] = 0;
					   }
					   if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY+1][2] == 3)
					   	   {
					   	   		myMap[whereX][whereY+1][2] = 1;
					   	   		myMap[whereX][whereY+1][3] = 0;
					   	   }
					   	   else
					   	   {
							   myMap[whereX][whereY+1][2] = 2;
							   ++myMap[whereX][whereY+1][3];
						   }
						   myMap[whereX][whereY+1][4] = 0;
					   }
					   if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY-1][2] == 3)
					   	   {
					   	   		myMap[whereX][whereY-1][2] = 1;
					   	   		myMap[whereX][whereY-1][3] = 0;
					   	 
					   	   }
					   	   else
					   	   {
							   myMap[whereX][whereY-1][2] = 2;
							   ++myMap[whereX][whereY-1][3];
						   }
						   myMap[whereX][whereY-1][4] = 0;
					   }
				   } 
				   else if(wumpusFound)
				   {
					   // Update knowledge of adjacent squares that are not known to be safe or contain the wumpus.
					   if(whereX + 1 < worldSize && (myMap[whereX+1][whereY][2] == 0 || myMap[whereX+1][whereY][2] == 2))
					   {
						   myMap[whereX+1][whereY][2] = 2;
						   ++myMap[whereX+1][whereY][3];
						   myMap[whereX+1][whereY][4] = 0;
					   }
					   if(whereX - 1 < worldSize && (myMap[whereX-1][whereY][2] == 0 || myMap[whereX-1][whereY][2] == 2))
					   {
						   myMap[whereX-1][whereY][2] = 2;
						   ++myMap[whereX-1][whereY][3];
						   myMap[whereX-1][whereY][4] = 0;
					   }
					   if(whereY + 1 < worldSize && (myMap[whereX][whereY+1][2] == 0 || myMap[whereX][whereY+1][2] == 2))
					   {
						   myMap[whereX][whereY+1][2] = 2;
						   ++myMap[whereX][whereY+1][3];					  
						   myMap[whereX][whereY+1][4] = 0;
					   }
					   if(whereY -1 < worldSize && (myMap[whereX][whereY-1][2] == 0 || myMap[whereX][whereY-1][2] == 2))
					   {
						   myMap[whereX][whereY-1][2] = 2;
  						   ++myMap[whereX][whereY-1][3];
						   myMap[whereX][whereY-1][4] = 0;
					   }					   
				   }
				   // If the wumpus is still alive and its location is unknown, take into account combination of breeze and stench.
				   else if(!wumpusFound)
				   {	// Update map to reflect both breeze and stench.
					   myMap[whereX][whereY][0]=4;
						// Update adjacent squares not known to be safe.
					   if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] != 1)
					   {
					   	   if(myMap[whereX+1][whereY][2] == 0)
					   		   myMap[whereX+1][whereY][2] = 4;
					   		
					   	   if(myMap[whereX+1][whereY][2] != 3)
							   ++myMap[whereX+1][whereY][3];
						   // Update certainty of adjacent square having a wumpus.
						   if(myMap[whereX+1][whereY][2] != 2)
						   {
							   if(myMap[whereX+1][whereY][4] == 0)
							   {
							   		++myMap[whereX+1][whereY][4];
							   }
							   else if(myMap[whereX+1][whereY][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX+1][whereY][4] = 4;
							   }
							}
					   }
					   if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					   {
					   
					   	   if(myMap[whereX-1][whereY][2] == 0)
					   			myMap[whereX-1][whereY][2] = 4;
					   		
					   	   if(myMap[whereX-1][whereY][2] != 3)
							   ++myMap[whereX-1][whereY][3];
						   if(myMap[whereX-1][whereY][2] != 2)
						   {
							   if(myMap[whereX-1][whereY][4] == 0)
							   {
							   		++myMap[whereX-1][whereY][4];
							   }
							   else if(myMap[whereX-1][whereY][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX-1][whereY][4] = 4;
							   }
							}					   
					   
						   
					   }
					   if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					   {
					   
					   
					   	   if(myMap[whereX][whereY+1][2] == 0)
					   			myMap[whereX][whereY+1][2] = 4;
					   		
					   	   if(myMap[whereX][whereY+1][2] != 3)
							   ++myMap[whereX][whereY+1][3];
						   if(myMap[whereX][whereY+1][2] != 2)
						   {
							   if(myMap[whereX][whereY+1][4] == 0)
							   {
							   		++myMap[whereX][whereY+1][4];
							   }
							   else if(myMap[whereX][whereY+1][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX][whereY+1][4] = 4;
							   }
							}						   
					   
					   
						   
					   }
					   if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					   {
					   	   if(myMap[whereX][whereY-1][2] == 0)
					   			myMap[whereX][whereY-1][2] = 4;
					   		
					   	   if(myMap[whereX][whereY-1][2] != 3)
							   ++myMap[whereX][whereY-1][3];
						   if(myMap[whereX][whereY-1][2] != 2)
						   {
							   if(myMap[whereX][whereY-1][4] == 0)
							   {
							   		++myMap[whereX][whereY-1][4];
							   }
							   else if(myMap[whereX][whereY-1][4] == 1)
							   {
							   		wumpusFound = true;
							   		myMap[whereX][whereY-1][4] = 4;
							   }
							}						   
					   		   
					   }


				   }
			   }
		   }
		   // If there's a stench and no breeze, and if the stench still needs to be paid attention to.
		   if(s.perceiveStench && !s.perceiveBreeze && !wumpusDead && !wumpusFound)
		   {
			   	// Update current square.
					myMap[whereX][whereY][0]=3;
					// Update adjacent squares which are not known to be safe.
					if(whereX + 1 < worldSize && (myMap[whereX+1][whereY][2] != 1))
					{
						// If a square is already suspected of a pit, then it's neither.
						if(myMap[whereX+1][whereY][2] == 2)
						{
							myMap[whereX+1][whereY][2] = 1;
							myMap[whereX+1][whereY][4] = 0;
						}
						else
						{	// Otherwise, suspect wumpus
							myMap[whereX+1][whereY][2] = 3;


							// Update certainty of a wumpus being there.
							if(myMap[whereX+1][whereY][4] == 0)
							{
							   	++myMap[whereX+1][whereY][4];
							}
							else if(myMap[whereX+1][whereY][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX+1][whereY][4] = 4;
							}	
						}	
						// No breeze => no pit										
						myMap[whereX+1][whereY][3] = 0;					
					}
					if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] != 1)
					{
						if(myMap[whereX-1][whereY][2] == 2)
						{
							myMap[whereX-1][whereY][2] = 1;
							myMap[whereX-1][whereY][4] = 0;
						}
						else
						{
							myMap[whereX-1][whereY][2] = 3;
							if(myMap[whereX-1][whereY][4] == 0)
							{
							   	++myMap[whereX-1][whereY][4];
							}
							else if(myMap[whereX-1][whereY][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX-1][whereY][4] = 4;
							}
						}							
						myMap[whereX-1][whereY][3] = 0;
						
					}
					if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] != 1)
					{
						if(myMap[whereX][whereY+1][2] == 2)
						{
							myMap[whereX][whereY+1][2] = 1;
							myMap[whereX][whereY+1][4] = 0;
						}
						else
						{
							myMap[whereX][whereY+1][2] = 3;
							if(myMap[whereX][whereY+1][4] == 0)
							{
							   	++myMap[whereX][whereY+1][4];
							}
							else if(myMap[whereX][whereY+1][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX][whereY+1][4] = 4;
							}	
					
						}	
											
						myMap[whereX][whereY+1][3] = 0;												
						
					}
					if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] != 1)
					{
						if(myMap[whereX][whereY-1][2] == 2)
						{
							myMap[whereX][whereY-1][2] = 1;
							myMap[whereX][whereY-1][4] = 0;
						}
						else
						{
							myMap[whereX][whereY-1][2] = 3;					
							if(myMap[whereX][whereY-1][4] == 0)
							{
							   	++myMap[whereX][whereY-1][4];
							}
							else if(myMap[whereX][whereY-1][4] == 1)
							{
							   	wumpusFound = true;
							   	myMap[whereX][whereY-1][4] = 4;
							}							
						}
						myMap[whereX][whereY-1][3] = 0;						
						
					}

			   
		   }
		   // If there's nothing to worry about, update accordingly.
		   if(!(s.perceiveStench || s.perceiveBreeze) || (wumpusDead && !s.perceiveBreeze) || (wumpusFound && !s.perceiveBreeze))
		   {   // Nothing to be seen here.
			   myMap[whereX][whereY][0] = 1;
			   // Adjacent squares, how's it going?
			   if(whereX + 1 < worldSize)
			   {   // If it's not the square with the wumpus, it's safe.
				   myMap[whereX+1][whereY][2] = (myMap[whereX+1][whereY][4] == 4 ? 3 : 1);
				   // If it's not already certain, there's no wumpus nearby.
				   if(myMap[whereX+1][whereY][4] == 1)
				   	myMap[whereX+1][whereY][4] = 0;
					// And there's definitely no pit.
				   myMap[whereX+1][whereY][3] = 0;
			   }
			   if(whereX - 1 < worldSize)
			   {
				   myMap[whereX-1][whereY][2] = (myMap[whereX-1][whereY][4] == 4 ? 3 : 1);
				   
				   if(myMap[whereX-1][whereY][4] == 1)
				   	myMap[whereX-1][whereY][4] = 0;
				   
				   myMap[whereX-1][whereY][3] = 0;
			   }
			   if(whereY + 1 < worldSize)
			   {
				   myMap[whereX][whereY+1][2] = (myMap[whereX][whereY+1][4] == 4 ? 3 : 1);
				   				   
				   if(myMap[whereX][whereY+1][4] == 1)
				   	myMap[whereX][whereY+1][4] = 0;

				   myMap[whereX][whereY+1][3] = 0;
			   }
			   if(whereY - 1 < worldSize)
			   {		
				   myMap[whereX][whereY-1][2] = (myMap[whereX][whereY-1][4] == 4 ? 3 : 1);

				   if(myMap[whereX][whereY-1][4] == 1)
				   	myMap[whereX][whereY-1][4] = 0;
				  
				   myMap[whereX][whereY-1][3] = 0;
		 	   }
		   }
		   
		   // Mark the current square as safe and visited.
		   myMap[whereX][whereY][2] = 1;
		   myMap[whereX][whereY][1] = 1;
		   // Once the wumpus has been found, make sure the map is updated properly.
		   if(wumpusFound && !wumpusDead && !updated)
		   {
		   	 updated = true;
		   	 
		  	 for(unsigned i = 0; i < myMap.size() ; ++i)
		 	  	for(unsigned j = 0; j < myMap.size() ; ++j)
			   	{
					if(myMap[i][j][4] != 4)
					{
						myMap[i][j][4] = 0;
						if(myMap[i][j][2] == 3)
							myMap[i][j][2] = 1;
						if(myMap[i][j][2] == 4)
							myMap[i][j][2] = 2;
							
					}
			   	}		 
		   }

	   } // End map update.
	/*
			cout << "\nmyMap\n";
		   	for(unsigned j = myMap.size()-1; j < myMap.size(); --j)

		   	{




		   		for(unsigned i = 0; i < myMap.size(); ++i)
		   		{


		   				cout << myMap[i][j][0] << " ";
		   			
		   		}
		   		cout << "    ";
		   		for(unsigned i = 0; i < myMap.size(); ++i)
		   		{

		   				cout << myMap[i][j][2] << " ";
		   			
		   		}
		   		cout << "    ";
		   		for(unsigned i = 0; i < myMap.size(); ++i)
		   		{

		   				cout << myMap[i][j][4] << " ";
		   			
		   		}
		   		
		   		
		   		cout << endl;	
		   	}

		*/
	   // Everything that glitters is gold.
	   if(s.perceiveGlitter)
	   {
		   haveGold = true;
		   return grab;
	   }
	   // Move to exit if you have the gold.
	   if(haveGold)
	   {
			// If you don't know what to do, ask!
		   if(breadCrumbs.empty())
		   {
			   breadCrumbs = findPathNeville3(myMap, whereX, whereY, haveGold, haveArrow, wumpusFound, risk);
		   }
		   // The path finding function should never return an empty deque, but just to be sure...
		   if(!breadCrumbs.empty())
		   {
				move = breadCrumbs.back();
				breadCrumbs.pop_back();
		   }
		   
		   // Pathfinding has proven effective enough that keeping track of current location here
		   // is unecessary.

		   return move;
	   }
	   // No gold, no problem. Let's have a look around.
	   // Move in the first safe, unexplored square found.
	   
		if(whereY -1 < worldSize && myMap[whereX][whereY-1][2] == 1 && myMap[whereX][whereY-1][1] == 0)
		{
			move = moveSouth;
			--whereY;
			breadCrumbs.clear();
		}	   
	   	else if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] == 1 && myMap[whereX-1][whereY][1] == 0)
		{
			move = moveWest;
			--whereX;
			breadCrumbs.clear();
		}			
		else if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] == 1 && myMap[whereX+1][whereY][1] == 0)
		{
			move = moveEast;
			++whereX;
			breadCrumbs.clear();
		}
		else if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] == 1 && myMap[whereX][whereY+1][1] == 0)
		{
			move = moveNorth;
			++whereY;
			breadCrumbs.clear();
		}
		

		// No safe and unexplored adjacent squares? Let's see what else we can find...
		if(move == doNothing)
		{
		
			if(breadCrumbs.empty())
			{
				breadCrumbs = findPathNeville3(myMap, whereX, whereY, haveGold, haveArrow, wumpusFound, risk);
			}
			
			if(!breadCrumbs.empty())
			{
				move = breadCrumbs.back();
				breadCrumbs.pop_back();
			}
			// Keep track of what's being done.
			if(move == moveWest)
				--whereX;
			else if(move == moveEast)
				++whereX;
			else if(move == moveSouth)
				--whereY;
			else if(move == moveNorth)
				++whereY;
			else if(move == shootArrowNorth)
				shot = shootArrowNorth;
			else if(move == shootArrowSouth)
				shot = shootArrowSouth;
			else if(move == shootArrowEast)
				shot = shootArrowEast;
			else if(move == shootArrowWest)
				shot = shootArrowWest;
		}



      return move;
   }
}
// This is the control function for the A*-style search. It determines the location and type of goals to be found.
deque<action> findPathNeville3(vector<vector<vector<unsigned> > > &myMap, unsigned whereX, unsigned whereY, bool haveGold, bool haveArrow, bool wumpusFound, bool &risk)
{
	// Searched keeps track of squares already on a path.
	set<unsigned> searched;
	// Coordinates to go to.
	vector<unsigned> goal;
	// All goals are added to ideas.
	vector<vector<unsigned> > ideas;
	multimap<unsigned, vector<unsigned> >::iterator it, second;
	// frontier holds possible squares to move to, ranked by their function value.
	multimap<unsigned, vector<unsigned> > frontier;
	// A proposal is a sequence of actions to the goal. Will be judged by size.
	deque<action> proposal;
	unsigned value;
	// Is wumpus on the menu?
	bool hunting = false;
	pair<unsigned, deque<action> > hold;
	unsigned limit;
	
	// Don't have the gold, look for places to go.
	if(!haveGold)
	{
			for(unsigned i = 0; i < worldSize; ++i)
			{
				for(unsigned j = 0; j < worldSize; ++j)
				{	// If there's a safe unexplored square, add it as a potential goal.
					if(myMap[i][j][1] == 0 && myMap[i][j][2] == 1)
					{
						goal.push_back(i);
						goal.push_back(j);
						ideas.push_back(goal);
						goal.clear();
					}
				}
			}
		
			// If no safe, unexplored squares are found and the arrow hasn't been shot,
			// look for the wumpus.
			if(ideas.empty() && haveArrow)
			{
				for(unsigned i = 0; i < worldSize; ++i)
				{
					for(unsigned j = 0; j < worldSize; ++j)
					{	// Add all possible locations of the wumpus to ideas as long as
						// there's no pit suspected in the same square.
						if(myMap[i][j][4] > 0 && myMap[i][j][3] == 0)
						{
							goal.push_back(i);
							goal.push_back(j);
							ideas.push_back(goal);
							goal.clear();
						}
					}
				}

			}
			// If there was no wumpus to kill, see if there's a suspected pit that's 
			// probably safe (explained in the isItSafe function).
			if(ideas.empty())
			{
				ideas = isItSafe3(myMap, false);
				// Clear out squares that might have the wumpus if the arrow has already
				// been used.
				if(!haveArrow)
				{			
					for(unsigned i = 0; i < ideas.size(); ++i)
					{
						goal = ideas[i];
						if(myMap[goal[0]][goal[1]][4] > 0)
						{
							ideas.erase(ideas.begin() + i);
							--i;
						}
					}
				}	
			}
	}
	// If there's nothing else to do, or if the gold has been found, head to the exit.
	if(ideas.empty())
	{
		goal.clear();
		goal.push_back(0);
		goal.push_back(0);
		ideas.push_back(goal);
		goal.clear();
		// Maybe you can just wrap a rock in some gold colored foil?
		haveGold = true;
	}
	// Add goals in ideas to frontier along with heuristic value based on city block distance, plus 10 for needing to kill the
	// wumpus. Usually when the wumpus is going to be killed, it will be the only option anyway, but isItSafe may return the 
	// wumpus space as one of other options. (This may seem a bit redundant here, but I'd already written the rest of the
	// code when I decided to add it. Ideas will never be very large, so the amount of time lost here is pretty insignificant.)
	for(unsigned i = 0; i < ideas.size(); ++i)
	{
		value = (whereY < ideas[i][1] ? (ideas[i][1] - whereY) : (whereY - ideas[i][1])) + (whereX < ideas[i][0] ? (ideas[i][0] - whereX) : (whereX - ideas[i][0]));
		if(myMap[ideas[i][0]][ideas[i][1]][4] > 0)
			value += 10;
		frontier.insert(pair<unsigned, vector<unsigned> >(value, ideas[i]));
	}
	// Add the current square to those searched.
	// Squares stored as two-digit numbers (YX)
	searched.insert(whereX + (10*whereY));
	it = frontier.begin();
	// Consider goals based on their value.
	while(it != frontier.end() && proposal.empty())
	{
		goal = (*it).second;
		
		if(myMap[goal[0]][goal[1]][4] > 0)
		{// Enough is enough! I have had it with these monkey fighting wumpuses on this Monday through Friday map! 
			hunting = true;
		}
		else
		{
			hunting = false;
		}
		
		if(frontier.size() > 1)
		{
			second = it;
			++second;
			if(second != frontier.end())
				limit = (*second).first;
			else 
				limit = 100;
		}
		else
		{
			limit = 100;
		}
		
		// If not already at the goal, call recursive search to find a path there.
		if(goal[0] != whereX || goal[1] != whereY)
		{
			hold = searchHereNeville3(goal,whereX, whereY,  myMap, 1, limit, searched, hunting);
			if(hold.first == 0)
				proposal = hold.second;
			else
			{
				frontier.erase(it);
				frontier.insert(pair<unsigned, vector<unsigned> > (hold.first, goal));
				it = frontier.begin();
			}

		}
		
		// Once you get to the exit, climb out.
		if(haveGold)
		{
			proposal.push_front(climbOut);
		}
		
		
	}
	
	// Look for the shortest proposal in plans.
	
	return proposal;

}
// This is the recursive A*-style search.
pair<unsigned, deque<action> > searchHereNeville3(vector<unsigned> goal, unsigned whereX, unsigned whereY, vector<vector<vector<unsigned> > > &myMap, unsigned path, unsigned limit, set<unsigned> searched, bool hunting)
{
	
	multimap<unsigned, vector<unsigned> >::iterator it, second;
	// frontier holds possible squares to move to, ranked by their function value.
	multimap<unsigned, vector<unsigned> > frontier;
	// coord stores coordinates
	vector<unsigned> coord;
	// hold is used to insert elements into the frontier
	pair<unsigned, vector<unsigned> > hold;
	unsigned newLimit = 100;
	// plan is a series of actions to the goal.
	pair<unsigned, deque<action> > plan;
	unsigned currentX, currentY, value, goalX, goalY;

	// Store goal coordinates for more convenient reference.
	goalX = goal[0];
	goalY = goal[1];
	
		// Base case: if one away from the goal, move there.
		if(!hunting)
		{
			if(whereX - 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveWest);
			}
			else if(whereX + 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveEast);
			}
			else if(whereY - 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveSouth);
			}
			else if(whereY + 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveNorth);
			}
		}
		// Unless hunting, then shoot first.
		else
		{
			if(whereX - 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveWest);
				plan.second.push_back(shootArrowWest);
			}
			else if(whereX + 1 == goalX && whereY == goalY)
			{
				plan.second.push_back(moveEast);
				plan.second.push_back(shootArrowEast);
			}
			else if(whereY - 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveSouth);
				plan.second.push_back(shootArrowSouth);
			}
			else if(whereY + 1 == goalY && whereX == goalX)
			{
				plan.second.push_back(moveNorth);
				plan.second.push_back(shootArrowNorth);
			}		
		}
		// Return base case once encountered.
		if(!plan.second.empty())
		{
			plan.first = 0;
			return plan;
		}

		// Otherwise, add all available moves to the frontier.
		if(whereX + 1 < worldSize && myMap[whereX+1][whereY][2] == 1)
		{
			coord.clear();
			currentX = whereX + 1;
			currentY = whereY;
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{// Squares are evaluated according to city block distance to the goal plus path cost.
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

			
				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(0); // Used to identify which direction the move is in.
				hold.first = value; 
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		if(whereX - 1 < worldSize && myMap[whereX-1][whereY][2] == 1)
		{
			coord.clear();
			currentX = whereX -1;
			currentY = whereY;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(1);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);

			}
		}
		if(whereY + 1 < worldSize && myMap[whereX][whereY+1][2] == 1)
		{
			coord.clear();
			currentX = whereX;
			currentY = whereY + 1;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;

				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(2);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		if(whereY - 1 < worldSize && myMap[whereX][whereY-1][2] == 1)
		{
			coord.clear();
			currentX = whereX;
			currentY = whereY -1;
			
			value = (currentX + (currentY * 10));
			if(!searched.count(value))			
			{
				searched.insert(value);
				value = (currentY < goalY ? (goalY - currentY) : (currentY - goalY)) + (currentX < goalX ? (goalX - currentX): (currentX - goalX));
				value += path;
			
				coord.push_back(currentX);
				coord.push_back(currentY);
				coord.push_back(3);
				hold.first = value;
				hold.second = coord;

				frontier.insert(hold);
			}
		}
		// Initialize iterator to the beginning of frontier.
		// multimaps sort elements by size, so the first element will always
		// have the smallest possible value.
		it = frontier.begin();		
		// Keep looking until a good enough plan has been found or there are no more
		// options.
		if(!frontier.empty())
		{
			
			while(plan.second.empty() && it != frontier.end())
			{		

				if((*it).first <= limit)
				{
				
					if(frontier.size() > 1)
					{
						second = it;
						++second;
						if(second != frontier.end())
						{
							newLimit = (*second).first;
						}
					}
					plan = searchHereNeville3(goal, (*it).second[0] , (*it).second[1], myMap, path + 1, (newLimit < limit ? newLimit : limit), searched, hunting);
					// If the search fails, plan will be empty. Otherwise...
					if(plan.first == 0)
					{
						// Add move to plan based on what square was considered.
						if((*it).second[2] == 0)
						{
							plan.second.push_back(moveEast);
						}
						else if((*it).second[2] == 1)
						{
							plan.second.push_back(moveWest);
						}
						else if((*it).second[2] == 2)
						{
							plan.second.push_back(moveNorth);
						}
						else if((*it).second[2] == 3)
						{
							plan.second.push_back(moveSouth);
						}
					}
					else
					{
						hold.second = (*it).second;
						hold.first = plan.first;
						frontier.erase(it);
						frontier.insert(hold);
						it = frontier.begin();
					}
				}
				else
				{
					plan.first = (*it).first;
					return plan;
				}
			}
			
		}
		else
		{
			plan.first = 101;
		}

		return plan;
}
// This function identifies potential pits that are not necessarily actually pits.
// It can look through all subsets of the set of pits to determine whether they completely
// account for all observed breezes, then return the coordinates of pits which were not included
// in the subsets. It's currently set to look at only subsets of size one less than the size of 
// of the full set. This reduces the number of risks the agent will take. 
vector<vector<unsigned> > isItSafe3(vector<vector<vector<unsigned> > > &myMap, bool risk)
{
	// All subsets of pits which account for all breezes are added to consistent.
	vector<vector<vector<unsigned> > > consistent;
	// Pits contains the coordinates of all pits. Hypothesis is used to hold a subset of pits.
	vector<vector<unsigned> > pits, hypothesis;
	// coord is used as a holder.
	vector<unsigned> coord;
	// Breeze is the set of all squares with breezes. Accounted is all pits accounted for by a subset.
	set<unsigned> breeze, accounted;
	// currentSize is the size of the subsets currently being considered.
	unsigned currentSize, unexplored,limit, count = 0;
	unexplored = 0;
	if(risk)
	{
		limit = 1;
	}
	else
		limit = 1;
	
	// Find all pits and breezes and add them to their respective containers.
	for(unsigned i = 0; i < myMap.size(); ++i)
	{
		for(unsigned j = 0; j < myMap.size(); ++j)
		{
			if(myMap[i][j][2]==2 || myMap[i][j][2] == 4)
			{
				coord.push_back(i);
				coord.push_back(j);
				pits.push_back(coord);
				coord.clear();
			}
			if(myMap[i][j][0] == 2 || myMap[i][j][0] == 4)
			{
				breeze.insert(i + (j*10));
			}
			if(myMap[i][i][1] == 0)
				++unexplored;
		}
	}
	// The original idea was to find the smallest size subsets of pits which would explain all breezes.
	// Though this worked fairly well, it took too many risks to outperform an agent which did not take
	// any risks. By only considering subsets of size one smaller than pits, the agent takes small
	// enough risks to outperform an agent which never takes any risks at all.

	if(pits.size() < 2)//|| pits.size() > 3)
		return hypothesis;
	
	if(unexplored > 6)
	{
		if(pits.size() > 2 && breeze.size() <= pits.size())
			return hypothesis;
	}
	else
	{
		limit = 1;	
	}
	currentSize = pits.size() > 1 ? pits.size() - 1 : 1;

	//currentSize = 1;
	//currentSize = pits.size();
	// This loop is currently not necessary (since the function only needs to be called once from here in this case), 
	// but I've left it in for future reference and to demonstrate how it would work in general. 
	while(currentSize > 0 && consistent.empty() && count < limit)
	{
		isItSafe3(myMap, pits, breeze, consistent, currentSize, hypothesis, 0);	

	
	// If subsets which explain all breezes have been found, go through and add all included pits to 
	// accounted.
		if(!consistent.empty())
		{
			for(unsigned j = 0; j < consistent.size(); ++j)
			{
				for(unsigned i = 0; i < consistent[j].size(); ++i)
				{
					if(!(accounted.count(consistent[j][i][0]+(consistent[j][i][1]*10))))
						accounted.insert(consistent[j][i][0]+(consistent[j][i][1]*10));
				}
			}
			// If there are pits not included in accounted, find them and add to hypothesis.
			if(accounted.size() < pits.size())
			{
				for(unsigned i = 0; i < pits.size(); ++i)
				{
					if(!(accounted.count(pits[i][0]+(pits[i][1]*10))))
					{
						hypothesis.push_back(pits[i]);
					}
				}		
			}
		}
		if(hypothesis.empty())
		{
			consistent.clear();
			accounted.clear();
		}
		
		--currentSize;
		++count;
	}	
	// Hypothesis will be empty if no consistent subsets are found or if all pits are included
	// in accounted. In this case, the path finding function will direct the agent to climb out.
	return hypothesis;
}
// This is the recursive portion of the search.
void isItSafe3(vector<vector<vector<unsigned> > > &myMap, vector<vector<unsigned> > &pits, set<unsigned> &breeze, vector<vector<vector<unsigned> > > &consistent, unsigned currentSize, vector<vector<unsigned> > hypothesis, unsigned num)
{
	vector<unsigned> coord;
	// Here, accounted is all breezes accounted for by a subset.
	set<unsigned> accounted;
	
	for(; num < pits.size(); ++num)
	{	
		hypothesis.push_back(pits[num]);
		// If hypothesis is smaller than the subset size being considered, go down another level.
		if(hypothesis.size() < currentSize)
		{
			isItSafe3(myMap, pits, breeze, consistent, currentSize, hypothesis, num+1);
		}
		// Otherwise, see if hypothesis is consistent.
		else
		{
			for(unsigned j = 0; j < hypothesis.size(); ++j)
			{
				coord = hypothesis[j];
				// If an adjacent square has been explored and isn't accounted for, add it to accounted.
				// Any adjacent, explored square will have a breeze, so there's no seperate checking for that.
				if(coord[0] - 1 < worldSize && myMap[coord[0]-1][coord[1]][1] && !(accounted.count((coord[0]-1)+(coord[1]*10))))
				{
					accounted.insert((coord[0]-1)+(coord[1]*10));
				}
				if(coord[0] + 1 < worldSize && myMap[coord[0]+1][coord[1]][1] && !(accounted.count((coord[0]+1)+(coord[1]*10))))
				{
					accounted.insert((coord[0]+1)+(coord[1]*10));
				}
				if(coord[1] - 1 < worldSize && myMap[coord[0]][coord[1]-1][1] && !(accounted.count(coord[0]+((coord[1]-1)*10))))
				{
					accounted.insert(coord[0]+((coord[1]-1)*10));
				}
				if(coord[1] + 1 < worldSize && myMap[coord[0]][coord[1]+1][1] && !(accounted.count(coord[0]+((coord[1]+1)*10))))
				{
					accounted.insert(coord[0]+((coord[1]+1)*10));
				}
			}
			// If all breezes are accounted for, add hypothesis to consistent.
			if(accounted.size() == breeze.size())
			{
				consistent.push_back(hypothesis);
			}
			
			accounted.clear();	
		}
		
		hypothesis.pop_back();			
	}
}
