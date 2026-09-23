#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

struct Position {
    int r, c;
    bool operator==(const Position& rhs) const { 
        return r == rhs.r && c == rhs.c; 
    }
    bool operator!=(const Position& rhs) const { 
        return r != rhs.r || c != rhs.c; 
    }
};

class Robot {

    private:
        vector<string> grid;
        int rows, cols;
        Position robotPosition;
        int totalmovees = 0;

        int dy[4] = {-1, 1, 0, 0};
        int dx[4] = {0, 0, -1, 1};
        string dn[4] = {"UP", "DOWN", "LEFT", "RIGHT"};

    public:

        Robot(const vector<string>& MapOri) {
            for (const string& line : MapOri) {
                string lineclean = "";
                for (char c : line) {
                    if (c != ' ') {
                        lineclean += c;
                    }
                }
                if (!lineclean.empty()) {
                    grid.push_back(lineclean);
                }
            }
            rows = grid.size();
            cols = grid[0].size();
            robotPosition = findSymbol('S');
        }

        Position findSymbol(char ch) const {
            for (int r = 0; r < rows; ++r)
                for (int c = 0; c < cols; ++c)
                    if (grid[r][c] == ch) 
                        return {r, c};
            return {-1, -1};
        }

        bool amankah(Position me) const {
            if (me.r < 0 || me.c < 0 || me.r >= rows || me.c >= cols) 
                return false;
            char tile = grid[me.r][me.c];
            return tile != '#' && tile != 'X';
        }

        void display() const {
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (r == robotPosition.r && c == robotPosition.c) {
                        cout << 'R' << ' ';
                    }
                    else {
                        cout << grid[r][c] << ' ';
                    }
                }
                cout << "\n";
            }
        }

        vector<pair<string, Position>> getPath(Position start, Position target) {
            queue<Position> q;
            vector<vector<bool>> visited(rows, vector<bool>(cols, false));
            vector<vector<Position>> parent(rows, vector<Position>(cols, {-1, -1}));
            vector<vector<int>> moveDir(rows, vector<int>(cols, -1));

            q.push(start);
            visited[start.r][start.c] = true;

            bool found = false;
            while (!q.empty()) {
                Position curr = q.front();
                q.pop();

                if (curr == target) { found = true; break; }

                for (int i = 0; i < 4; ++i) {
                    Position next = {curr.r + dy[i], curr.c + dx[i]};
                    if (amankah(next) && !visited[next.r][next.c]) {
                        visited[next.r][next.c] = true;
                        parent[next.r][next.c] = curr;
                        moveDir[next.r][next.c] = i;
                        q.push(next);
                    }
                }
            }

            vector<pair<string, Position>> path;
            if (!found) return path;

            Position curr = target;
            while (curr != start) {
                Position p = parent[curr.r][curr.c];
                int d = moveDir[curr.r][curr.c];
                path.push_back({dn[d], curr});
                curr = p;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        void solve() {

            Position start = findSymbol('S');
            Position flag  = findSymbol('F');
            Position goal  = findSymbol('G');

            auto toFlag = getPath(start, flag);
            auto toGoal = getPath(flag, goal);


            cout << "-------------------------------------------\n";
            cout << "MAP LOADED     : " << rows << " x " << cols << "\n";
            cout << "START POSITION : (" << start.r << "," << start.c << ")\n";
            cout << "PATH TO FLAG   : ";
            for (size_t i = 0; i < toFlag.size(); ++i) {
                cout << toFlag[i].first << (i + 1 < toFlag.size() ? ", " : "");
            }
            cout << "\n-------------------------------------------\n";
            display();
            cout << "-------------------------------------------\n";
            for (const auto& step : toFlag) {
                robotPosition = step.second;
                totalmovees++;
                cout << "STEP " << totalmovees << " - MOVE " << step.first << " - Position (" << robotPosition.r << "," << robotPosition.c << ")\n";
                display();
                cout << "-------------------------------------------\n";
            }
            cout << "FLAG CAPTURED  : (" << robotPosition.r << "," << robotPosition.c << ")\n";
            cout << "PATH TO BASE   : ";
            for (size_t i = 0; i < toGoal.size(); ++i) {
                cout << toGoal[i].first << (i + 1 < toGoal.size() ? ", " : "");
            }
            cout << "\n-------------------------------------------\n";
            for (const auto& step : toGoal) {
                robotPosition = step.second;
                totalmovees++;
                cout << "STEP " << totalmovees << " - MOVE " << step.first << " - Position (" << robotPosition.r << "," << robotPosition.c << ")\n";
                display();
                cout << "-------------------------------------------\n";
            }

            cout << "BASE REACHED       : (" << robotPosition.r << "," << robotPosition.c << ")\n";
            cout << "MISSION COMPLETE\n";
            cout << "TOTAL MOVES        : " << totalmovees << "\n";
        }
};

int main() {

    vector<string> Maze1 = {
        "# # # # # # #",
        "# S # . . . #",
        "# . # . # G #",
        "# . . . # . #", 
        "# # # . . . #", 
        "# X . . F . #", 
        "# # # # # # #",
    };

    vector<string> Maze2 = {
        "# # # # # # # # #",
        "# S . . # . . . #", 
        "# . # . # . # G #", 
        "# . # . . . # . #", 
        "# . . . # . . . #", 
        "# # # . # . . # #", 
        "# X . . # . . F #", 
        "# . # # # # X . #", 
        "# # # # # # # # #",
    };

    vector<string> Maze3 = {
        "# # # # # # # # # # #",
        "# S . . # . . . . . #",
        "# . # . # . # # # . #", 
        "# . # . . . # G # . #", 
        "# . . . # . # . # . #", 
        "# # # . # . . . # . #", 
        "# . . . # # # . # . #", 
        "# . # . . X . . # . #", 
        "# . # . # # # . # F #", 
        "# X . . . . X . . . #", 
        "# # # # # # # # # # #",
    };
    cout << "-------------------------------------------\n";
    cout << "===========================================\n";
    cout << "                 SOAL 1                    \n";
    cout << "===========================================\n";
    Robot solveee1(Maze1);
    solveee1.solve();

    cout << "-------------------------------------------\n";
    cout << "===========================================\n";
    cout << "                 SOAL 2                    \n";
    cout << "===========================================\n";
    Robot solveee2(Maze2);
    solveee2.solve();
    
    cout << "-------------------------------------------\n";
    cout << "===========================================\n";
    cout << "                 SOAL 3                    \n";
    cout << "===========================================\n";
    Robot solveee3(Maze3);
    solveee3.solve();

    return 0;
}