In this project, I create an agent that plays the game Reversi (also known by the trademark Othello). Reversi is a two-person, full information game. Rules, and some basic strategy. 

My program contains the following 2 python functions:

nextMove(board, color, time)
where board is a representation of the board:
The board will be represented as a list of list of strings (that is, a 2D array of strings). Each string will be "B" for black, "W" for white, or "." for an empty square. Color is either the string "B" or the string "W", representing the color to play. Time is the time left on your tournament clock (in seconds).

nextMoveR(board, color, time)
Which is identical to doMove, except it plays the Reversed game (Reversed Reversi) -- when the object is to end with as few pieces of your color as possible.