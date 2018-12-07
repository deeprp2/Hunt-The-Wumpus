/* prog5Wumpus.cpp
 
 Description: Navigate a maze to kill the Wumpus.
 Implementation of a 1972 game by Gregory Yob.
 Author of this implementation: Dale Reed, for CS 141, UIC, Fall 2018
 
 Results of a program run are shown below:
 
     You are in room 13. You smell a stench.
 
     1. Enter your move (or 'D' for directions): d
           ______18______
          /      |       \
         /      _9__      \
        /      /    \      \
       /      /      \      \
     17     8        10     19
     | \   / \      /  \   / |
     |  \ /   \    /    \ /  |
     |   7     1---2     11  |
     |   |    /     \    |   |
     |   6----5     3---12   |
     |   |     \   /     |   |
     |   \       4      /    |
     |    \      |     /     |
     \     15---14---13     /
      \   /            \   /
       \ /              \ /
        16---------------20
 
     Hunt the Wumpus:
     The Wumpus lives in a completely dark cave of 20 rooms. Each
     room has 3 tunnels leading to other rooms.
 
     Hazards:
     1. Two rooms have bottomless pits in them.  If you go there you fall and die.
     2. Two other rooms have super-bats.  If you go there, the bats grab you and
        fly you to some random room, which could be troublesome.  Then those bats go
        to a new room different from where they came from and from the other bats.
     3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and
        is too heavy for bats to lift.  Usually he is asleep.  Two things wake
        him up: Anytime you shoot an arrow, or you entering his room.  When he
        wakes he moves if he is in an odd-numbered room, but stays still otherwise.
        After that, if he is in your room, he eats you and you die!
 
     Moves:
     On each move you can do the following, where input can be upper or lower-case:
     1. Move into an adjacent room.  To move enter 'M' followed by a space and
        then a room number.
     2. Shoot your guided arrow through a list of up to three adjacent rooms, which
        you specify.  Your arrow ends up in the final room.
        To shoot enter 'S' followed by the number of rooms (1..3), and then the
        list of the desired number (up to 3) of adjacent room numbers, separated
        by spaces. If an arrow can't go a direction because there is no connecting
        tunnel, it ricochets and moves to the lowest-numbered adjacent room and
        continues doing this until it has traveled the designated number of rooms.
        If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You
        automatically pick up the arrow if you go through a room with the arrow in
        it.
     3. Enter 'R' to reset the person and hazard locations, useful for testing.
     4. Enter 'C' to cheat and display current board positions.
     5. Enter 'D' to display this set of instructions.
     6. Enter 'P' to print the maze room layout.
     7. Enter 'X' to exit the game.
 
     Good luck!
 
 
     You are in room 13. You smell a stench.
 
     1. Enter your move (or 'D' for directions): P
          ______18______
         /      |       \
        /      _9__      \
       /      /    \      \
      /      /      \      \
     17     8        10     19
     | \   / \      /  \   / |
     |  \ /   \    /    \ /  |
     |   7     1---2     11  |
     |   |    /     \    |   |
     |   6----5     3---12   |
     |   |     \   /     |   |
     |   \       4      /    |
     |    \      |     /     |
     \     15---14---13     /
      \   /            \   /
       \ /              \ /
        16---------------20
 
     You are in room 13. You smell a stench.
 
     1. Enter your move (or 'D' for directions): m 12
     You are in room 12.
 
     2. Enter your move (or 'D' for directions): M 3
     You are in room 3. You hear rustling of bat wings.
 
     3. Enter your move (or 'D' for directions): c
     Cheating! Game elements are in the following rooms:
     Player Wumpus Bats1 Bats2 Pit1 Pit2 Arrow
     3     14      4     7   18   16    -1
 
     You are in room 3. You hear rustling of bat wings.
 
     3. Enter your move (or 'D' for directions): r
     (Remember arrow value of -1 means it is with the person.)
     Enter the 7 room locations (1..20) for player, wumpus, bats1, bats2, pit1, pit2, and arrow:
     3 14 4 2 18 16 2
 
     You are in room 3. You hear rustling of bat wings.
 
     3. Enter your move (or 'D' for directions): s
     Sorry, you can't shoot an arrow you don't have.  Go find it.
     You are in room 3. You hear rustling of bat wings.
 
     4. Enter your move (or 'D' for directions): m 2
     Woah... you're flying!
     You've just been transported by bats to room 9
     You are in room 9. You feel a draft.
 
     5. Enter your move (or 'D' for directions): m 10
     You are in room 10.
 
     6. Enter your move (or 'D' for directions): m 2
     Congratulations, you found the arrow and can once again shoot.
     You are in room 2. You hear rustling of bat wings.
 
     7. Enter your move (or 'D' for directions): s
     Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: 3 3 4 14
     Wumpus has just been pierced by your deadly arrow!
     Congratulations on your victory, you awesome hunter you.
 
     Exiting Program ...
 
 */
#include <iostream>
#include <iomanip>   // for setw()
#include <cctype>    // for toupper()
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // to seed random number generator with time(0)
using namespace std;

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
class GameInfo {
public:
    int moveNumber;  // Counts up from 1, incrementing for each move
    int personRoom;  // Room 1..20 the person currently is in
    int wumpusRoom;  // Room 1..20 the Wumpus is in
    int pitRoom1;    // Room 1..20 the first pit is in
    int pitRoom2;    // Room 1..20 the second pit is in
    int batsRoom1;   // Room 1..20 the first set of bats are in
    int batsRoom2;   // Room 1..20 the second set of bats are in
    int arrowRoom;   // -1 if arrow is with person, else room number 1..20

    GameInfo* pNext;
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(GameInfo &theGameInfo, bool &personIsAlive, int rooms[][3]);


//--------------------------------------------------------------------------------
void displayCave()
{
    cout<< "       ______18______             \n"
    << "      /      |       \\           \n"
    << "     /      _9__      \\          \n"
    << "    /      /    \\      \\        \n"
    << "   /      /      \\      \\       \n"
    << "  17     8        10     19       \n"
    << "  | \\   / \\      /  \\   / |    \n"
    << "  |  \\ /   \\    /    \\ /  |    \n"
    << "  |   7     1---2     11  |       \n"
    << "  |   |    /     \\    |   |      \n"
    << "  |   6----5     3---12   |       \n"
    << "  |   |     \\   /     |   |      \n"
    << "  |   \\       4      /    |      \n"
    << "  |    \\      |     /     |      \n"
    << "  \\     15---14---13     /       \n"
    << "   \\   /            \\   /       \n"
    << "    \\ /              \\ /        \n"
    << "    16---------------20           \n"
    << endl;
}


//--------------------------------------------------------------------------------
void displayInstructions()
{
    cout<< "Hunt the Wumpus:                                             \n"
    << "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    << "room has 3 tunnels leading to other rooms.                   \n"
    << "                                                             \n"
    << "Hazards:                                                     \n"
    << "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
    << "2. Two other rooms have super-bats.  If you go there, the bats grab you and    \n"
    << "   fly you to some random room, which could be troublesome.  Then those bats go\n"
    << "   to a new room different from where they came from and from the other bats.  \n"
    << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
    << "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake      \n"
    << "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
    << "   wakes he moves if he is in an odd-numbered room, but stays still otherwise. \n"
    << "   After that, if he is in your room, he eats you and you die!                 \n"
    << "                                                                               \n"
    << "Moves:                                                                         \n"
    << "On each move you can do the following, where input can be upper or lower-case: \n"
    << "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
    << "   then a room number.                                                         \n"
    << "2. Shoot your guided arrow through a list of up to three adjacent rooms, which \n"
    << "   you specify.  Your arrow ends up in the final room.                         \n"
    << "   To shoot enter 'S' followed by the number of rooms (1..3), and then the     \n"
    << "   list of the desired number (up to 3) of adjacent room numbers, separated    \n"
    << "   by spaces. If an arrow can't go a direction because there is no connecting  \n"
    << "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and     \n"
    << "   continues doing this until it has traveled the designated number of rooms.  \n"
    << "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You \n"
    << "   automatically pick up the arrow if you go through a room with the arrow in  \n"
    << "   it.                                                                         \n"
    << "3. Enter 'R' to reset the person and hazard locations, useful for testing.     \n"
    << "4. Enter 'C' to cheat and display current board positions.                     \n"
    << "5. Enter 'D' to display this set of instructions.                              \n"
    << "6. Enter 'P' to print the maze room layout.                                    \n"
    << "7. Enter 'X' to exit the game.                                                 \n"
    << "                                                                               \n"
    << "Good luck!                                                                     \n"
    << " \n"
    << endl;
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int randomNumbers[],  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
}


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[],   // Array of random numbers
                     int size)              // Size of random numbers array
{
    int randomValue = -1;
    
    for( int i = 0; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
}

//--------------------------------------------------------------------------------
void newNode (GameInfo &gameInfo, GameInfo *&pHead, GameInfo *&pTemp) {
  pTemp = new GameInfo;
  pTemp->batsRoom1 = gameInfo.batsRoom1;
  pTemp->batsRoom2 = gameInfo.batsRoom2;
  pTemp->wumpusRoom = gameInfo.wumpusRoom;
  pTemp->personRoom = gameInfo.personRoom;
  pTemp->arrowRoom = gameInfo.arrowRoom;
  pTemp->pitRoom1 = gameInfo.pitRoom1;
  pTemp->pitRoom2 = gameInfo.pitRoom2;
  pTemp->pNext = pHead;
  pHead = pTemp;
}


//--------------------------------------------------------------------------------
void deleteNode (GameInfo &gameInfo, GameInfo *&pHead) {
  GameInfo* discard;
  discard  = pHead;
  pHead = pHead->pNext;
  delete discard;
}


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(int rooms[][3],        // Array of room connections
                    GameInfo &gameInfo, GameInfo *&pHead, GameInfo *&pTemp)    // All other game settings variables
{
    // Array of 6 unique values 1..20, to be used in initializing cave hazards locations
    int randomNumbers[6];
    
    // Initialize cave room connections
    //       ______18______
    //      /      |       \
    //     /      _9__      \
    //    /      /    \      \
    //   /      /      \      \
    //  17     8        10    19
    // |  \   / \      /  \   / |
    // |   \ /   \    /    \ /  |
    // |    7     1---2     11  |
    // |    |    /     \    |   |
    // |    6----5     3---12   |
    // |    |     \   /     |   |
    // |    \       4      /    |
    // |     \      |     /     |
    //  \     15---14---13     /
    //   \   /            \   /
    //    \ /              \ /
    //    16---------------20
    
    // Room connection values could have also been set globally
    int roomsInitializeArray[ 21][ 3] = {
      //                   Room 0  is unused, to help avoid off-by-one indexing issues
                           {0,0,0},
      //                   Room 1   Room 2    Room 3    Room 4    Room 5
      /* Rooms 1-5   */    {2,5,8}, {1,3,10}, {2,4,12}, {3,5,14}, {1,4,6},
      //                    Room 6   Room 7    Room 8    Room 9    Room 10
      /* Rooms 6-10  */    {5,7,15}, {6,8,17}, {1,7,9}, {8,10,18}, {2,9,11},
      //                   Room 11    Room 12     Room 13    Room 14    Room 15
      /* Rooms 11-15 */  {10,12,19}, {3,11,13}, {12,14,20}, {4,13,15}, {6,14,16},
      //                   Room 16    Room 17     Room 18    Room 19    Room 20
      /* Rooms 16-20 */  {15,17,20}, {7,16,18}, {9,17,19}, {11,18,20}, {13,16,19},
    };
    
    // Copy from the hard-coded array values declared here into the array passed in as a
    // parameter to this function, used elsewhere in the program.
    for( int room=0; room < NUMBER_OF_ROOMS + 1;  room++) {
        // Copy values for each room
        for( int adjacentRoom = 0; adjacentRoom < 3; adjacentRoom++) {
            rooms[ room][ adjacentRoom] = roomsInitializeArray[ room][ adjacentRoom];
        }
    }
    
    // Select some unique random values 1..20 to be used for 2 bats rooms, 2
    // pits rooms, Wumpus room, and initial player room
    setUniqueValues(randomNumbers, 6);
    // Use the unique random numbers to set initial locations of hazards, which
    //    should be non-overlapping.
    gameInfo.arrowRoom = -1;    // -1 value indicates player has the arrow
    gameInfo.batsRoom1 = randomNumbers[0];  gameInfo.batsRoom2 = randomNumbers[1];
    gameInfo.pitRoom1 = randomNumbers[2];   gameInfo.pitRoom2 = randomNumbers[3];
    gameInfo.wumpusRoom = randomNumbers[4];
    gameInfo.personRoom = randomNumbers[5];
    
    gameInfo.moveNumber = 1;

    

}// end initializeBoard(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[ 3],     // Array of adjacent tunnels
                   int nextRoom)        // Desired room to move to
{
    return(tunnels[0] == nextRoom ||
           tunnels[1] == nextRoom ||
           tunnels[2] == nextRoom
           );
}


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(GameInfo gameInfo)
{
    cout << "Cheating! Game elements are in the following rooms: \n"
    << "Player Wumpus Bats1 Bats2 Pit1 Pit2 Arrow  \n"
    << setw( 4) << gameInfo.personRoom
    << setw( 7) << gameInfo.wumpusRoom
    << setw( 7) << gameInfo.batsRoom1
    << setw( 6) << gameInfo.batsRoom2
    << setw( 5) << gameInfo.pitRoom1
    << setw( 5) << gameInfo.pitRoom2
    << setw( 6) << gameInfo.arrowRoom
    << endl
    << endl;
}// end displayCheatInfo(...)


//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo(int rooms[][3], GameInfo gameInfo)
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    cout << "You are in room " << currentRoom << ". ";
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = rooms[currentRoom][0];
    int room2 = rooms[currentRoom][1];
    int room3 = rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        cout << "You smell a stench. ";
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
       room2 == pit1Room || room2 == pit2Room ||
       room3 == pit1Room || room3 == pit2Room
       ) {
        cout << "You feel a draft. ";
    }
    
    // Display hazard detection message if bats are in an adjacent room
    int batsRoom1 = gameInfo.batsRoom1;
    int batsRoom2 = gameInfo.batsRoom2;
    if( room1 == batsRoom1 || room1 == batsRoom2 ||
       room2 == batsRoom1 || room2 == batsRoom2 ||
       room3 == batsRoom1 || room3 == batsRoom2
       ) {
        cout << "You hear rustling of bat wings. ";
    }
    
    cout << "\n" << endl;
}//end displayRoomInfo(...)


//--------------------------------------------------------------------------------
// When the person just moved into a room with bats, the bats transport the person to
//   a random room that is not where they just were, and is not a room that already
//   has bats in it, but it could be a room with the Wumpus or a pit.  The bats are then
//   placed in a random room that is not where the person is, is not where the other
//   bats are, and is not their original room.
void inRoomWithBats(
                    GameInfo &gameInfo,    // Hazards location and game info
                    bool &personIsAlive,   // Person starts as alive, but this could change from true to false
                    int rooms[][3])        // Gets passed on to check for death by pit or Wumpus
//   which 75% of the time just moves the Wumpus
{
    int randomNewRoom = -1;    // Used to transport person to a new room
    
    // Person is transported by bats and dropped off in some random room.
    // Find new random room location that is not where we started
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo.personRoom);
    
    // Move the person to the new room.  Keep track of which room they came from, to help us
    // know which set of bats to adjust afterwards
    int personOldRoom = gameInfo.personRoom;
    gameInfo.personRoom = randomNewRoom;
    cout << "Woah... you're flying! \n";
    cout << "You've just been transported by bats to room " << gameInfo.personRoom << endl;
    
    // Find new random room location that is not where the person is now, and is not
    //   where the other bats are, and is not where the bats came from.
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo.personRoom ||
             randomNewRoom == gameInfo.batsRoom1  ||
             randomNewRoom == gameInfo.batsRoom2 );
    
    // Move the bats to this newly selected room.  Figure out which set of bats to move by
    // comparing their current location to the room the person came from.
    if( personOldRoom == gameInfo.batsRoom1) {
        // Change batsRoom1, since that is where the person came from
        gameInfo.batsRoom1 = randomNewRoom;
    }
    else {
        // Change batsRoom2, since that is where the person came from
        gameInfo.batsRoom2 = randomNewRoom;
    }
    
    // Handle pit or Wumpus hazards in person's new room, if any of those hazards are present.
    // If person is dropped in a room with a pit, or the room with the Wumpus, then they could die.
    int personRoom = gameInfo.personRoom;
    if( personRoom == gameInfo.pitRoom1 || personRoom == gameInfo.pitRoom2 ||
       personRoom == gameInfo.wumpusRoom) {
        checkForHazards(gameInfo, personIsAlive, rooms);
    }
}//end inRoomWithBats(...)


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(
                     GameInfo &gameInfo,    // Hazards location and game info
                     bool &personIsAlive,   // Person is alive, but could die depending on the hazards
                     int rooms[][ 3])
{
    // retrieve the room the person is in
    int personRoom = gameInfo.personRoom;
    
    // Check whether there is a pit
    if( personRoom == gameInfo.pitRoom1 || personRoom == gameInfo.pitRoom2) {
        // Person falls into pit
        cout << "Aaaaaaaaahhhhhh....   " << endl;
        cout << "    You fall into a pit and die. \n";
        personIsAlive = false;
        return;
    }
    
    // Check for the Wumpus
    if( personRoom == gameInfo.wumpusRoom) {
        // To make it easier to test, in this version of the program the Wumpus always
        // moves if it is currently in an odd-numbered room, and it moves into the
        // lowest-numbered adjacent room.  In the version that is more fun to play
        // (but harder to test), the Wumpus has a 75% chance of moving, and a 25%
        // chance of staying and killing you.  The "more fun" code is commented out below.
        
        // Wumpus is there. 75% change of Wumpus moving, 25% chance of it killing you
        // Generate a random number 1..100
        // if(  (rand() % 100) < 75) {
        if( gameInfo.wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            cout << "You hear a slithering sound, as the Wumpus slips away. \n"
            << "Whew, that was close! " << endl;
            // Choose a random adjacent room for the Wumpus to go into
            // gameInfo.wumpusRoom = rooms[ personRoom][ rand() % 3];
            gameInfo.wumpusRoom = rooms[ personRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            cout << "You briefly feel a slimy tentacled arm as your neck is snapped. \n"
            << "It is over." << endl;
            personIsAlive = false;
            return;
        }
    }
    
    // Check for bats, which if present move you to a new room
    if( gameInfo.personRoom == gameInfo.batsRoom1 || gameInfo.personRoom == gameInfo.batsRoom2) {
        inRoomWithBats(gameInfo, personIsAlive, rooms);
    }
}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// User shot arrow, providing up to 3 rooms for the arrow to fly through
void shootArrow(int rooms[][3],          // Array of connected rooms
                GameInfo &gameInfo,      // Locations of hazards, user, Wumpus and arrow
                bool &personIsAlive,     // Whether or not person is alive
                bool &wumpusIsAlive)     // Whether or not Wumpus is alive
{
    int roomsForArrowTravel[3];          // Array of up to 3 rooms for arrow to travel through
    int howManyRoomsForArrow = 0;        // How many rooms the user supplies for arrow to travel through
    
    // Successively scan the user input string, storing room numbers into an array
    cout << "Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ";
    cin >> howManyRoomsForArrow;
    if( howManyRoomsForArrow > 3) {
        cout << "Sorry, the max number of rooms is 3.  Setting that value to 3." << endl;
        howManyRoomsForArrow = 3;
    }
    for( int i=0; i<howManyRoomsForArrow; i++) {
        cin >> roomsForArrowTravel[ i];
    }
    
    // Move the arrow through those rooms, ensuring they are adjacent
    gameInfo.arrowRoom = gameInfo.personRoom;  // arrow starts in person room
    for( int i = 0; i < howManyRoomsForArrow; i++) {
        // Ensure room is adjacent before traveling through it
        if( roomIsAdjacent( rooms[ gameInfo.arrowRoom], roomsForArrowTravel[i]) ) {
            // Room was adjacent, so move arrow there
            gameInfo.arrowRoom = roomsForArrowTravel[i];
            // Kill Wumpus if it is there
            if( gameInfo.arrowRoom == gameInfo.wumpusRoom) {
                wumpusIsAlive = false;
                cout << "Wumpus has just been pierced by your deadly arrow! \n"
                << "Congratulations on your victory, you awesome hunter you.\n";
                return;
            }
            // Kill person if path went to a second room and then came right back
            if( gameInfo.arrowRoom == gameInfo.personRoom) {
                personIsAlive = false;
                cout << "You just shot yourself.  \n"
                << "Maybe Darwin was right.  You're dead.\n";
                return;
            }
        }
        else {
            // Error in user input, as indicated room is not adjacent.
            cout << "Room " << roomsForArrowTravel[i] << " is not adjacent.  Arrow ricochets..." << endl;
            // Move arrow to lowest numbered adjacent rooms
            gameInfo.arrowRoom = rooms[ gameInfo.arrowRoom][ 0];
            // The game is more interesting if you move the arrow to a random adjacent room, but
            // this feature was removed to make it easier to test your program.
            // gameInfo.arrowRoom = rooms[gameInfo.arrowRoom][rand() % 3];
            
            // If it richochets into room where user is, user dies
            if( gameInfo.arrowRoom == gameInfo.personRoom) {
                personIsAlive = false;
                cout << "You just shot yourself, and are dying.\n"
                << "It didn't take long, you're dead." << endl;
                return;
            }
            
            // If it ricochets into room where Wumpus is, Wumpus dies
            if( gameInfo.arrowRoom == gameInfo.wumpusRoom) {
                wumpusIsAlive = false;
                cout << "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                << "Accidental victory, but still you win!" << endl;
                return;
            }
        }//end else
        
    }//end for( int i = 0; i < howManyRoomsForArrow
    
    // Since arrow was shot, Wumpus hears it.  Choose a random adjacent room for the Wumpus to go into
    // The line below has the Wumpus go to a random room, but this has been disabled for testing,
    // instead having the Wumpus move to the lowest-numbered adjacent room.
    // gameInfo.wumpusRoom = rooms[ gameInfo.wumpusRoom][ rand() % 3];
    gameInfo.wumpusRoom = rooms[ gameInfo.wumpusRoom][ 0];
}//end shootArrow(...)


//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions( GameInfo &theGameInfo)
{
    cout << "(Remember arrow value of -1 means it is with the person.)" << endl;
    cout << "Enter the 7 room locations (1..20) for player, wumpus, bats1, bats2, pit1, pit2, and arrow:"
    << endl;
    cin >> theGameInfo.personRoom
    >> theGameInfo.wumpusRoom
    >> theGameInfo.batsRoom1 >> theGameInfo.batsRoom2
    >> theGameInfo.pitRoom1 >> theGameInfo.pitRoom2
    >> theGameInfo.arrowRoom;
    cout << endl;
}


//--------------------------------------------------------------------------------
int main()
{
    int rooms[21][3];           // 3 tunnel index values for the 20 rooms. Indexed 0..20, but
    //    we will ignore 0 and only use 1..20
    GameInfo gameInfo;          // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    GameInfo* pHead = NULL;
    GameInfo* pTemp;

    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);
    
    // Set random initial values for person, Wumpus, bats and pits
    initializeGame(rooms, gameInfo, pHead, pTemp);
    newNode(gameInfo, pHead, pTemp);  //create the new node

    
    // Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) {
        
        // Handle user input inside a loop to allow retrying move after invalid user input
        while ( true) {
            
            // Display current room information: Room number, hazards detected
            displayRoomInfo(rooms, gameInfo);
            
            // Prompt for and handle move
            cout << gameInfo.moveNumber << ". Enter your move (or 'D' for directions): ";
            cin >> typeOfMove;
            typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
            
            // Check all types of user input and handle them.  This uses if-else-if code
            //   rather than switch-case, so that we can take advantage of break and continue.
            if( typeOfMove == 'D') {
                displayCave();
                displayInstructions();
                continue;       // Loop back up to reprompt for the same move
            }
            else if( typeOfMove == 'P') {
                // To assist with play, display the cave layout
                displayCave();
                continue;       // Loop back up to reprompt for the same move
            }
            else if( typeOfMove == 'M') {
                // Move to an adjacent room
                cin >> nextRoom;
                if( roomIsAdjacent( rooms[ gameInfo.personRoom], nextRoom)) {
                    gameInfo.personRoom = nextRoom;        // move to a new room
                    
                    // Check for pit, Wumpus or bats present in this new room
                    checkForHazards(gameInfo, personIsAlive, rooms);
                    
                    // Pick up arrow if it is in this new room
                    if( gameInfo.arrowRoom == gameInfo.personRoom) {
                        cout << "Congratulations, you found the arrow and can once again shoot." << endl;
                        gameInfo.arrowRoom = -1;   // -1 indicates player has arrow
                    }
                    break;   // break out of the move validation loop
                }
                else {
                    cout << "Invalid move.  Please retry. \n";
                    continue;
                }
            }
            else if( typeOfMove == 'S') {
                // arrowRoom value of -1 means it is with the person, which must be true for arrow to be shot
                if( gameInfo.arrowRoom == -1) {
                    shootArrow(rooms, gameInfo, personIsAlive, wumpusIsAlive);
                }
                else {
                    cout << "Sorry, you can't shoot an arrow you don't have.  Go find it.\n";
                }
                break;   // break out of the move validation loop
            }
            else if( typeOfMove == 'C') {
                // Display Cheat information
                displayCheatInfo(gameInfo);
                continue;        // doesn't count as a move, so retry same move.
            }
            else if( typeOfMove == 'R') {
                // Specify resetting the hazards and person positions, useful for testing
                resetPositions( gameInfo);
                continue;        // Doesn't count as a move, so retry same move.
            }
            else if( typeOfMove == 'X') {
                // Exit program
                personIsAlive = false;           // Indicate person is dead as a way to exit playing loop
                break;
            }
            else if ( typeOfMove == 'U') {

              gameInfo.moveNumber--;

              if (pHead->pNext == NULL) {
                cout << "Sorry, you can't undo past the beginning of the game.  Please retry.\n";
                break;
              }
              else {
                 deleteNode (gameInfo, pHead);
                 gameInfo.personRoom = pHead->personRoom;
              }
            }
            
        }//end while( stillValidatingMove)
        
        // Increment the move number
        gameInfo.moveNumber++;
        newNode(gameInfo, pHead, pTemp);
        
    }//end while( true)
    
    cout << "\nExiting Program ..." << endl;
}//end main()
