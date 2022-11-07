#include<bits/stdc++.h>
using namespace std;
pair<int,int> next_move (-1,-1);
int movesMadeBefore;

void read_input(int &myPlayer, vector<vector<int>> &previousBoard, vector<vector<int>> &currentBoard){

    ifstream inputFile("input.txt");

    if(inputFile.is_open()){
        string input;
        vector<int> tmp(5,-1);
        inputFile>>myPlayer;
        //cout<<myPlayer<<" ";
        inputFile.ignore();
        for(int i = 0; i < 5; i++) {
            getline(inputFile, input);
            for(int j = 0; j < 5; j++) {
                tmp[j]=int(input[j]-'0');
            }
            previousBoard.push_back(tmp);
        }
        for(int i = 0; i < 5; i++) {
            getline(inputFile, input);
            for(int j = 0; j < 5; j++) {
                tmp[j]=int(input[j]-'0');
            }
            currentBoard.push_back(tmp);
        }
    }
    inputFile.close();
}

void write_output(int i, int j, int movesMadeBefore){
    ofstream outputFile;
    outputFile.open ("output.txt");
    if(i==-1){
        outputFile<<"PASS";
    }
    else{
        outputFile<<i<<","<<j;
    }
    outputFile.close();
    outputFile.open("previousMoves.txt");
    outputFile<<movesMadeBefore;
    outputFile.close();
}

void print_board(vector<vector<int>> &board){

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            cout<<board[i][j]<<" ";
        }
        cout<<"\n";
    }
    return;
}

bool is_board_empty(vector<vector<int>> &board){

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(board[i][j]!=0){
                return false;
            }
        }
    }
    return true;
}

bool same_board(vector<vector<int>> &previousBoard, vector<vector<int>> &currentBoard){

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(previousBoard[i][j]!=currentBoard[i][j]){
                return false;
            }
        }
    }
    return true;
}

bool checkValidity(int i, int j){
    if(i<0 || j<0 || i>4 || j>4){
        return false;
    }
    return true;
}

void update_points(vector<vector<int>> &currentBoard, int value, vector<pair<int,int>> &points){
    for(int i=0; i<points.size(); i++){
        currentBoard[points[i].first][points[i].second] = value;
    }
}

void get_chain(int i, int j, vector<vector<int>> &board, vector<vector<bool>> &visited, vector<pair<int,int>> &chain, int myPlayer)
{
    chain.push_back(make_pair(i,j));
    visited[i][j] = true;

    if(checkValidity(i-1,j) && board[i-1][j]==myPlayer && !visited[i-1][j]){
        get_chain(i-1,j,board,visited,chain,myPlayer);
    }
    if(checkValidity(i,j-1) && board[i][j-1]==myPlayer && !visited[i][j-1]){
        get_chain(i,j-1,board,visited,chain,myPlayer);
    }
    if(checkValidity(i+1,j) && board[i+1][j]==myPlayer && !visited[i+1][j]){
        get_chain(i+1,j,board,visited,chain,myPlayer);
    }
    if(checkValidity(i,j+1) && board[i][j+1]==myPlayer && !visited[i][j+1]){
        get_chain(i,j+1,board,visited,chain,myPlayer);
    }
    return;
}

bool checkLiberty(int i, int j, vector<vector<int>> &board, int myPlayer){
    if(!checkValidity(i,j)){
        return false;
    }
    vector<vector<bool>> visited (5,vector<bool> (5,false));
    vector<pair<int,int>> chain;
    get_chain(i,j,board,visited,chain,myPlayer);
    
    for(int k=0; k<chain.size(); k++){
        int x = chain[k].first;
        int y = chain[k].second;
        
        if(checkValidity(x-1,y) && board[x-1][y]==0){
            return true;
        }
        if(checkValidity(x,y-1) && board[x][y-1]==0){
            return true;
        }
        if(checkValidity(x+1,y) && board[x+1][y]==0){
            return true;
        }
        if(checkValidity(x,y+1) && board[x][y+1]==0){
            return true;
        }
    }
    return false;
}

vector<pair<int,int>> get_killed_points(vector<vector<int>> &currentBoard, int currPlayer){
    // Optimize this with number of chains in O(n)
    vector<pair<int,int>> killed_points;
    for(int i=0; i<5; i++){
        for(int j=0 ;j<5; j++){
            if(currentBoard[i][j]==currPlayer){
                if(!checkLiberty(i,j,currentBoard,currPlayer)){
                    killed_points.push_back(make_pair(i,j));
                }
            }
        }
    }
    return killed_points;
}

vector<pair<int,int>> get_opponent_coord(vector<vector<int>> &board, int player){
    vector<pair<int,int>> ans;
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(board[i][j]==player){
                ans.push_back(make_pair(i,j));
            }
        }
    }
    return ans;
}

double get_euler_number(vector<vector<int>> board_without_padding, int player){
    int q1=0, q3=0, qd=0;

    vector<vector<int>> board (7,vector<int> (7,0));
    // Copy board to this
    for(int i=1; i<6; i++){
        for(int j=1; j<6; j++){
            board[i][j] = board_without_padding[i-1][j-1];
        }
    }
    
    for(int i=0; i<6; i++){
        for(int j=0; j<6; j++){
            int count_0 = 0, count_player = 0;
            if(board[i][j]==0){count_0++;}
            else if(board[i][j]==player){count_player++;}

            if(board[i+1][j+1]==0){count_0++;}
            else if(board[i][j]==player){count_player++;}

            if(board[i+1][j]==0){count_0++;}
            else if(board[i][j]==player){count_player++;}

            if(board[i][j+1]==0){count_0++;}
            else if(board[i][j]==player){count_player++;}

            if(count_0==3 && count_player==1){q1++;}
            if(count_0==1 && count_player==3){q3++;}
            
            //Check Diagonal
            if(board[i][j]==board[i+1][j+1] && board[i][j+1]==board[i+1][j]){
                if(board[i][j]==0 && board[i+1][j]==player){qd++;}
                if(board[i][j]==player && board[i+1][j]==0){qd++;}
            }
        }
    }
    double euler = (q1 - q3 + (2*qd))/4;
    return euler;
}

pair<double,double> get_lib_and_piece_score(vector<vector<int>> &board, int currPlayer){
    int cw_edge = 0, cw_middle = 0;
    int cb_edge = 0, cb_middle = 0;
    int cb = 0, cw = 0;
    
    int first_second_b = 0, third_b = 0;
    int first_second_w = 0, third_w = 0;

    int x3[] = {-1,-2,-2,-1,1,2,2,1};
    int y3[] = {-2,-1,1,2,2,1,-1,-2};

    int x1_2[] = {-1,-1,0,1,1,1,0,-1,-2,0,2,0};
    int y1_2[] = {0,1,1,1,0,-1,-1,-1,0,2,0,-2};

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(i==0 || j==0 || i==4 || j==4){
                if(board[i][j]==1){
                    cb_edge++;cb++;
                }
                else if(board[i][j]==2){
                    cw_edge++;cw++;
                }
            }
            else{
                if(board[i][j]==1){
                    cb_middle++;cb++;
                }
                else if(board[i][j]==2){
                    cw_middle++;cw++;
                }
            }

            // Getting First and Second Order Liberties
            for(int k=0; k<12; k++){
                if(!checkValidity(i+x1_2[k],j+y1_2[k])){
                    continue;
                }
                if(board[i+x1_2[k]][j+y1_2[k]] == 0){
                    if(board[i][j]==1){first_second_b++;}
                    else if(board[i][j]==2){first_second_w++;}
                }
            }

            //Third Order Liberties
            for(int k=0; k<8; k++){
                if(!checkValidity(i+x3[k],j+y3[k])){
                    continue;
                }
                if(board[i+x3[k]][j+y3[k]] == 0){
                    if(board[i][j]==1){third_b++;}
                    else if(board[i][j]==2){third_w++;}
                }
            }
        }
    }

    if(currPlayer==1){ 
        double lib_black = (first_second_b + (0.5*third_b)) - (first_second_w + (0.5*third_w));
        return  make_pair(lib_black,(cb-cw));
    }
    double lib_white = (first_second_w + (0.5*third_w)) - (first_second_b + (0.5*third_b));
    return make_pair(lib_white,(cw-cb));
}

double calc_score(vector<vector<int>> &board, int currPlayer){
    
    vector<pair<int,int>> points = get_opponent_coord(board,1);
    update_points(board,0,points);
    double euler_white = get_euler_number(board,2);
    update_points(board,1,points); 

    points = get_opponent_coord(board,2);
    update_points(board,0,points);
    double euler_black = get_euler_number(board,1);
    update_points(board,2,points);
    
    double euler_num;
    if(currPlayer==1){
        euler_num = -4*(euler_black - euler_white);
    }
    else{
        euler_num = -4*(euler_white - euler_black);
    }

    pair<double,double> lib_and_piece = get_lib_and_piece_score(board,currPlayer);
    double lib = lib_and_piece.first;
    double piece_score = lib_and_piece.second;

    if(currPlayer==1){ 
        return  min(max(lib,-4.0),4.0) + (euler_num) + piece_score*10;
        //return piece_score - 2.5;
    }
    return min(max(lib,-4.0),4.0) + (euler_num) + piece_score*10;
}

bool is_neighbour(vector<vector<int>> &Board, int i, int j, int player){

    int x[] = {-1,-1,0,1,1,1,0,-1};
    int y[] = {0,1,1,1,0,-1,-1,-1};

    for(int w=0; w<8; w++){
        int x1 = i + x[w];
        int y1 = j + y[w];
        if(!checkValidity(x1,y1)){
            continue;
        }
        else if(Board[x1][y1]==player){
            return true;
        }
    }
    return false;
}

vector<pair<int,int>> get_next_possible_moves(vector<vector<int>> &previousBoard, vector<vector<int>> &currentBoard, int currPlayer, int myPlayer, bool flag){
    
    vector<pair<int,int>> next_pm;

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(currentBoard[i][j]!=0){
               continue; 
            }
            else{
                currentBoard[i][j]=currPlayer;
                if(checkLiberty(i,j,currentBoard,currPlayer)){
                    if(flag){       
                        if(is_neighbour(currentBoard,i,j,myPlayer)&&is_neighbour(currentBoard,i,j,3-myPlayer)){
                            next_pm.push_back(make_pair(i,j));
                        }
                    }
                    else{       
                        next_pm.push_back(make_pair(i,j));
                    }
                }
                else{
                    // FInd Dead Pieces
                    vector<pair<int,int>> killed_points = get_killed_points(currentBoard,3-currPlayer);
                    // Remove Dead Pieces
                    update_points(currentBoard, 0, killed_points);
                    if(killed_points.size()>0){
                        // Check for KO
                        if(!same_board(currentBoard,previousBoard)){       
                            next_pm.push_back(make_pair(i,j));
                        }
                    }    
                    // revert the captured coordinates
                    update_points(currentBoard, 3-currPlayer, killed_points);
                }
                currentBoard[i][j]=0;
            }
        }
    }
    return next_pm;
}

double alpha_beta(vector<vector<int>> previousBoard, vector<vector<int>> currentBoard, int depth, int currDepth, int myPlayer, int currPlayer, double alpha, double beta){

    if(currDepth == 0){
        //Calculate Score and return score
        return calc_score(currentBoard,myPlayer);
    }

    //Get Possible Moves
    vector<pair<int,int>> next_possible_moves = get_next_possible_moves(previousBoard, currentBoard, currPlayer, myPlayer, true);
    if(movesMadeBefore + depth - currDepth < 9){
        next_possible_moves = get_next_possible_moves(previousBoard, currentBoard, currPlayer, myPlayer, true);
    }
    else{
        next_possible_moves = get_next_possible_moves(previousBoard, currentBoard, currPlayer, myPlayer, false);
    }
    next_possible_moves.push_back(make_pair(-1,-1));

    bool maxPlayer = (myPlayer==currPlayer);
    vector<vector<int>> nextPreviousBoard = currentBoard;
    
    if(maxPlayer){
        double best_val = INT32_MIN;

        for(auto move: next_possible_moves){   
            vector<pair<int,int>> killed_points;
            if(move.first != -1){
                currentBoard[move.first][move.second] = currPlayer;
                // Killing the Opponent
                // MAKE SURE TO UPDATE THE BOARD TO ORIGINAL
                killed_points = get_killed_points(currentBoard,3-currPlayer);
                update_points(currentBoard, 0, killed_points);
            }

            double val = alpha_beta(nextPreviousBoard,currentBoard,depth,currDepth-1,myPlayer,3-currPlayer,alpha,beta);

            if(val>best_val){
                if(depth==currDepth){
                    next_move = move;
                }
                best_val = val;
            }
            alpha = max(alpha,best_val);
            if(beta<=alpha){
                break;
            }
            if(move.first != -1){
                currentBoard[move.first][move.second] = 0;
                // Updating Original
                update_points(currentBoard, 3-currPlayer, killed_points);
            }
        }
        return best_val;
    }
    else{
        double best_val = INT32_MAX;

        for(auto move: next_possible_moves){
            vector<pair<int,int>> killed_points;
            if(move.first != -1){
                currentBoard[move.first][move.second] = currPlayer;
                // Killing the Opponent
                // MAKE SURE TO UPDATE THE BOARD TO ORIGINAL
                killed_points = get_killed_points(currentBoard,3-currPlayer);
                update_points(currentBoard, 0, killed_points);
            }

            double val = alpha_beta(nextPreviousBoard,currentBoard,depth,currDepth-1,myPlayer,3-currPlayer,alpha,beta);

            best_val = min(best_val,val);
            beta = min(beta,best_val);
            if(beta<=alpha){
                //cout<<" PRUNED "<<"\n";
                break;
            }
            if(move.first != -1){
                currentBoard[move.first][move.second] = 0;
                // Updating Original
                update_points(currentBoard, 3-currPlayer, killed_points);
            }
        }
        return best_val;
    }
}

int main(){

    vector<vector<int>> previousBoard;
    vector<vector<int>> currentBoard;
    int myPlayer;
    int depth;

    read_input(myPlayer, previousBoard, currentBoard);

    if(is_board_empty(previousBoard)&&is_board_empty(currentBoard)){
        movesMadeBefore = 0;
        write_output(2,2,movesMadeBefore);
    }
    else if(is_board_empty(previousBoard)){
        movesMadeBefore = 1;
        ofstream outputFile;
        if(currentBoard[2][2]==0){
            write_output(2,2,movesMadeBefore);
        }
        else{
            write_output(1,2,movesMadeBefore);
        }
    }
    else{
        ifstream inputFile("previousMoves.txt");
        inputFile>>movesMadeBefore;
        movesMadeBefore+=2;

        if(movesMadeBefore<6){
            depth = 4;
        }
        else if(movesMadeBefore>=6 and movesMadeBefore<14){
            depth = 5;
        }
        else if(movesMadeBefore>=14 and movesMadeBefore<18){
            depth = 7;
        }
        else{
            depth = 24 - movesMadeBefore;
        }

        alpha_beta(previousBoard,currentBoard,depth,depth,myPlayer,myPlayer,INT32_MIN,INT32_MAX);
        write_output(next_move.first,next_move.second,movesMadeBefore);
    }
    return 0;
}