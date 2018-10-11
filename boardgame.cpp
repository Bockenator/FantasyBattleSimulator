#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<queue>
#include<unordered_map>
#include<string>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include <cstdio>


using namespace std;

// Euclidean distance between two points a and b
float findDist (pair<int, int> a, pair<int, int> b) {
  float row_dist = pow((a.first - b.first), 2);
  float col_dist = pow((a.second - b.second), 2);
  return sqrt(row_dist+col_dist);
}

// Check that two monsters don't have the same team (unless its neutral)
bool checkValidTarget(string team1, string team2) {
  if (team1 == team2 && team1 != "Neutral") {
    return false;
  }
  return true;
}


/*
  Monster Class defines monsters that fight on a board

  Constructor:

  Monster m (attack, health, armour, movement, sight range);

  A Monster should also have explicitly set:
    -name
    -team
    -size of stack
    -location

  Example:

  Monster dragon(25, 40, 2, 2, 8);
  dragon.setName("Dragon");
  dragon.setTeam("Evil");
  dragon.setStackSize(5);
  dragon.setLocation(pair<int, int> (B_SIZE-1,B_SIZE-1));

*/
class Monster {
  pair<int, int> location;
  int attack;
  int health;
  int armour;
  int movement;
  int sight_range;
  int stack_size;
  string team;
  string name;

public:
  // Constructor
  Monster (int a, int h, int ar, int m, int sr) {
    attack = a;
    health = h;
    armour = ar;
    movement = m;
    sight_range = sr;
    stack_size = 0;
    team = "Neutral";
    name = "Unknown Entity";
  }

  // GETTERS AND SETTERS
  int getAttack () {
    return attack;
  }

  int getHealth () {
    return health;
  }

  int getArmour () {
    return armour;
  }

  int getMovement () {
    return movement;
  }

  int getStackSize () {
    return stack_size;
  }

  void setStackSize (int s) {
    stack_size = s;
  }

  // This method is only used on setup
  void setLocation (pair<int, int> newLoc) {
    location = newLoc;
  }

// Sets location to newLoc and ensures monster is not outside the board
// TODO: Make sure the locations aren't on walls
  void setLocation (pair<int, int> newLoc, vector<vector<int> > & board) {
    location = newLoc;
    if (location.first < 0) {
      location.first = 0;
    } else if (location.first > board.size()-1) {
      location.first = board.size()-1;
    }
    if (location.second < 0) {
      location.second = 0;
    } else if (location.second > board[0].size()-1) {
      location.second = board[0].size()-1;
    }
  }

  pair<int, int> getLocation () {
    return location;
  }

  string getName () {
    return name;
  }

  void setName (string n) {
    name = n;
  }

  string getTeam () {
    return team;
  }

  void setTeam (string t) {
    team = t;
  }


  // Add number to stack size of monster
  void addCreatures(int number) {
    stack_size+=number;
  }

  // Remove number from stack size of monster
  void removeCreatures(int number) {
    stack_size-=number;
  }

  // Move monster to target location and return true, if possible, false
  // otherwise
  bool moveTo (pair<int, int> target, vector<vector<int> > & board) {
    // Check that target location is in range
    if (findDist(location, target) <= movement) {
      // If it
      setLocation(target, board);

      cout << name << " moves to " << location.first << "," << location.second
       << "\n";

      return true;
    }
    return false;
  }

  // Take an amount of damage. Monsters will only die if they take damage
  // greater than or equal to the monsters health
  void takeDamage (int damage) {
    int before = stack_size;
    // TODO: fix armour
    stack_size -= (damage-armour)/health;

    // Calculate the number of monsters lost
    int lost = before-stack_size;
    if (lost > before) {
      lost = before;
    }

    // Print out the number of monsters lost to the damage taken
    if (lost == 1) {
      cout << lost << " " << name << " dies\n";
    } else {
      cout << lost << " " << name << "s die\n";
    }
    // If stack is less than 1 then the monster is considered destroyed
    if (stack_size < 1) {
      cout << name << "s from team " << team << " destroyed at " << location.first
          << "," << location.second << "\n";
    }
  }

  // Attack a monster by moving to its location and dealing damage equal to the
  // the stack_size*attack. Returns true if the attack was
  bool attackMonster (Monster* target, vector<vector<int> > & board) {
    // Check the target is not friendly
    if (checkValidTarget(team, target->getTeam())) {
      // Check the target is in range
      //TODO: Change to check that there is a valid path
      if (findDist(location, target->getLocation()) <= movement) {
        // Check that distance is larger than 0 and if so move there
        if (findDist(location, target->getLocation()) > 0) {
          moveTo(target->getLocation(), board);
        }

        // Print the attack
        cout << stack_size << " " << name << "s attack " << target->getStackSize()
         << " " << target->getName() << "s at " << location.first << "," <<
         location.second << "\n";

        // Target monster takes the damage
        target->takeDamage(attack*stack_size);
        return true;
      }
    }
    return false;
  }

  // Move as close as possible to a target monster by first closing row distance
  // or if that isnt possinble closing column distance
  // TODO: integrate this with the setLocation code
  void moveTowardsTarget (Monster* target, vector<vector<int> > & board) {
    int row_dist = location.first - target->getLocation().first;
    int column_dist = location.second - target->getLocation().second;
    int steps = movement;

    while (steps > 0) {
      if (row_dist > 0 && (location.first > 0) && (board[location.first-1][location.second] == 0)) {
        location.first--;
        row_dist--;
      } else if (row_dist < 0 && (location.first < board.size()-1) && (board[location.first+1][location.second] == 0)) {
        location.first++;
        row_dist++;
      } else if (column_dist > 0 && (location.second > 0) && (board[location.first][location.second-1] == 0)) {
        location.second--;
        column_dist--;
      } else if (column_dist < 0 && (location.second < board[0].size()-1) && (board[location.first][location.second+1] == 0)) {
        location.second++;
        column_dist++;
      }
      steps--;
    }

    // Check bounds
    if (location.first < 0) {
      location.first = 0;
    } else if (location.first > board.size()-1) {
      location.first = board.size()-1;
    }
    if (location.second < 0) {
      location.second = 0;
    } else if (location.second > board[0].size()-1) {
      location.second = board[0].size()-1;
    }

    // Print move
    cout << name << " moves towards " <<target->getName() << " to " <<
      location.first << "," << location.second << "\n";
  }

  // Find the monster with the closest euclidean distance that is in sight range
  // Returns the closest monster in range (if there are none return nullptr)
  Monster* findClosestMonster (unordered_map<string, vector<Monster*> > & monsters, set<string> & teams) {
    //TODO: Make this a real minimum thing...
    int min_dist = 10000;
    Monster* min_mon = nullptr;

    // Loop over every monster in every other team
    for (auto t : teams) {
      if (t != team) {
        for (Monster* m : monsters[t]) {
          // Min dist
          int dist = findDist(m->getLocation(), location);
          if ((dist < min_dist) && (m->getStackSize() > 0) && (dist <= sight_range)){
            min_dist = dist;
            min_mon = m;
          }
        }
      }
    }

    return min_mon;
  }


  // Move monster in a random direction
  void moveRandom (vector<vector<int> > & board) {

    // Choose direction
    int dir = (rand() % 100) + 1;
    int steps = movement;

    // Move as many steps in the chosen direction as possible or choose a new
    // direction if no mor steps are possible in the original direction
    while (steps > 0) {
      // Move North
      if (dir < 25) {
        while ((location.first > 0) && (board[location.first-1][location.second] == 0) && (steps > 0)) {
          location.first--;
          steps--;
        }
      // Move East
    } else if (dir < 50) {
        while ((location.second < board[0].size()-1) && (board[location.first][location.second+1] == 0) && (steps > 0)) {
          location.second++;
          steps--;
        }
      // Move South
    } else if (dir < 75) {
        while ((location.first < board.size()-1) && (board[location.first+1][location.second] == 0) && (steps > 0)) {
          location.first++;
          steps--;
        }
      // Move West
      } else {
        while ((location.second > 0) && (board[location.first][location.second-1] == 0) && (steps > 0)) {
          location.second--;
          steps--;
        }
      }
      // Choose new direction
      dir = (rand() % 100) + 1;
    }

    // Print move
    cout << name << " moves to " << location.first << "," << location.second << "\n";
  }

  // The AI method
  void act (unordered_map<string, vector<Monster*> > & monsters, set<string> & teams, vector<vector<int> > & board) {
    Monster* target = findClosestMonster(monsters, teams);
    if (target == nullptr) {
      moveRandom(board);
      return;
    }
    if (attackMonster(target, board)) {
      return;
    } else {
      moveTowardsTarget(target, board);
    }
  }

};








class Board {
  vector<vector<int> > board_layout;
  unordered_map<string, vector<Monster*> > monsters;
  set<string> teams;
public:

  Board (int size) {
    vector<int> row;
    for (int i = 0; i < size; i++) {
      row.push_back(0);
    }
    for (int i = 0; i < size; i++) {
      board_layout.push_back(row);
    }
    randomBoard();
  }

  // Init a random board layout
  void randomBoard () {
    for (int j = 1; j < board_layout.size()-1; j++) {
      // Leave first and last two spaces open for spawning creatures
      for (int i = 1; i < board_layout[j].size()-1; i++) {
        int r = (rand() % 100)+1;
        if (r >= 80) {
          board_layout[j][i] = 1;
        }
      }
    }
  }

  void addTeam (string s) {
    if (teams.find(s) == teams.end()) {
      teams.insert(s);
      vector<Monster*> m;
      monsters[s] = m;
    } else {
      cout << "TEAM " << s << " IS ALREADY IN PLAY\n";
    }
  }

  void addMonster (Monster* m) {
    if (teams.find(m->getTeam()) == teams.end()) {
      addTeam(m->getTeam());
    }
    monsters[m->getTeam()].push_back(m);
  }

  void reset () {
    teams.clear();
    monsters.clear();
  }

  string simulate () {

    for (auto s : teams) {
      cout << s << "\n";
    }
    int count = 0;

    while ((teams.size() > 1) && count < 200) {
      count++;
      cout << "===============TURN " << count << "===============\n";
      for (auto s : teams) {
        cout << "=====Team " << s << "=====\n";
        for (int i = 0; i < monsters[s].size(); i++) {
          cout << "+++Monster " << monsters[s][i]->getName() << "+++\n";
          if (monsters[s][i]->getStackSize() > 0) {
            //cout << "Acting\n";
            monsters[s][i]->act(monsters, teams, board_layout);
            //cout << "Finished Acting\n";
          } else {
            monsters[s].erase(monsters[s].begin()+i);

            if (monsters[s].empty()) {
              monsters.erase(s);
              teams.erase(s);
              cout << ">>>>>" << s << " has been eliminated!<<<<<\n\n";
              break;
            }
            //i--;
          }
          cout << "\n";
        }
      }
      cout << "=============END TURN " << count << "=============\n";
      cout << "\n\n";
    }
    string winner;
    if (count < 200) {
      winner = (*teams.begin());
    } else {
      winner = "NA";
    }
    cout << ">>>>>>>>>>" << winner << " Wins!<<<<<<<<<<\n\n";
    reset();
    return winner;
  }

  void printBoard () {
    for (vector<int> v : board_layout) {
      for (int i = 0; i < v.size(); i++) {
        cout << "+ - ";
      }
      cout << "+\n";
      for (int i = 0; i < v.size(); i++) {
        cout << "| " << v[i] << " ";
      }
      cout << "|\n";
    }
    for (int i = 0; i < board_layout[0].size(); i++) {
      cout << "+ - ";
    }
    cout << "+\n";
  }
};

// TODO: Create monster factories
int main (int argc, char *argv[]) {
  // Default values
  int B_SIZE = 8;
  int RUNS = 100;
  // Command line arguement passing
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-s") {
      if (i+1 = argc) {
        cerr << "Require Board Size Integer after -s\n";
      }
      B_SIZE = stoi(string(argv[i+1]));
    } else if (string(argv[i]) == "-r") {
      if (i+1 = argc) {
        cerr << "Require Number of Runs Integer after -r\n";
      }
      RUNS = stoi(string(argv[i+1]));
    }
  }

  ios::sync_with_stdio(false);
  srand((int) time(0));
  Board b(B_SIZE);

  unordered_map<string, int> wins;

  // Evil Creatures
  Monster dragon(25, 40, 2, 2, 8);
  dragon.setName("Dragon");
  dragon.setTeam("Evil");
  Monster* dragonp = &dragon;

  Monster skeleton(5, 5, 0, 1, 2);
  skeleton.setName("Skeleton");
  skeleton.setTeam("Evil");
  Monster* skeletonp = &skeleton;

  // Good Creatures
  Monster eagle(10, 20, 2, 2, 10);
  eagle.setName("Eagle");
  eagle.setTeam("Good");
  Monster* eaglep = &eagle;

  Monster elf(8, 7, 1, 1, 4);
  elf.setName("Elf");
  elf.setTeam("Good");
  Monster* elfp = &elf;

  // Humans
  Monster knight(15, 15, 5, 2, 3);
  knight.setName("Knight");
  knight.setTeam("Human");
  Monster* knightp = &knight;

  Monster pikeman(10, 10, 5, 2, 3);
  pikeman.setName("Pikeman");
  pikeman.setTeam("Human");
  Monster* pikemanp = &pikeman;

  // Chaos Creatures
  Monster demon(20, 30, 5, 1, 6);
  demon.setName("Demon");
  demon.setTeam("Chaos");
  Monster* demonp = &demon;

  Monster spawn(2, 2, 0, 3, 2);
  spawn.setName("Spawn");
  spawn.setTeam("Chaos");
  Monster* spawnp = &spawn;


  wins["Evil"] = 0;
  wins["Good"] = 0;
  wins["Human"] = 0;
  wins["Chaos"] = 0;
  wins["NA"] = 0;

  for (int i = 0; i < RUNS; i++) {
    // Reset
    b.addMonster(dragonp);
    dragon.setStackSize(5);
    dragon.setLocation(pair<int, int> (B_SIZE-1,B_SIZE-1));
    b.addMonster(skeletonp);
    skeleton.setStackSize(50);
    skeleton.setLocation(pair<int, int> (B_SIZE-1,B_SIZE-2));
    b.addMonster(eaglep);
    eagle.setStackSize(15);
    eagle.setLocation(pair<int, int> (0,0));
    b.addMonster(elfp);
    elf.setStackSize(30);
    elf.setLocation(pair<int, int> (0,1));
    b.addMonster(knightp);
    knight.setStackSize(10);
    knight.setLocation(pair<int, int> (0,B_SIZE-1));
    b.addMonster(pikemanp);
    pikeman.setStackSize(15);
    pikeman.setLocation(pair<int, int> (0,B_SIZE-2));
    b.addMonster(demonp);
    demon.setStackSize(6);
    demon.setLocation(pair<int, int> (B_SIZE-1,0));
    b.addMonster(spawnp);
    spawn.setStackSize(100);
    spawn.setLocation(pair<int, int> (B_SIZE-2,1));

    wins[b.simulate()]++;
    cout << "*************  " << i << "  *************\n";
  }

  int max_wins = 0;
  string winner = "NA";
  cout << "\n========== SCORES ==========\n";
  for (pair<string, int> p : wins) {
    cout << p.first << " : " << p.second << "\n";
    if (p.second > max_wins) {
      max_wins = p.second;
      winner = p.first;
    }
  }

  cout << "\n\n+=+=+=||| FINAL WINNER - " << winner << " with " << max_wins << "/" << RUNS <<" wins |||=+=+=+\n\n";

  //b.printBoard();
  return 0;
}
