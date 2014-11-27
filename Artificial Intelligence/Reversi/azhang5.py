__author__ = 'anglies'

import gameplay
import sys
from copy import deepcopy


def value(board, reverse):
    valueBoard = ((100, -5, 10, 5, 5, 10, -5, 100), (-5, -45, 1, 1, 1, 1, -45, -5), (10, 1, 3, 2, 2, 3, 1, 10),
                  (5, 1, 2, 1, 1, 2, 1, 5), (5, 1, 2, 1, 1, 2, 1, 5), (10, 1, 3, 2, 2, 3, 1, 10),
                  (-5, -45, 1, 1, 1, 1, -45, -5), (100, -5, 10, 5, 5, 10, -5, 100))
    value = 0
    for i in range(8):
        for j in range(8):
            if board[i][j] == "B":
                value = value + valueBoard[i][j]
            elif board[i][j] == "W":
                value = value - valueBoard[i][j]

    return value


def betterThan(val1, val2, color, reversed):
    if color == "W":
        retVal = val1 < val2
    else:
        retVal = val2 < val1
    if reversed:
        return not retVal
    else:
        return retVal


def alphaBeta(board, color, reverse, depth, alpha=-sys.maxint, beta=sys.maxint):
    if depth == 0:
        return value(board, reverse)

    moves = []
    for i in range(8):
        for j in range(8):
            if gameplay.valid(board, color, (i, j)):
                moves.append((i, j))

    if (color == "B" and not reverse) or (color == "W" and reverse):
        for move in moves:
            newBoard = deepcopy(board)
            gameplay.doMove(newBoard, color, move)
            alpha = max(alpha, alphaBeta(newBoard, gameplay.opponent(color), reverse, depth - 1, alpha, beta))

            if beta < alpha:
                break
        return alpha
    else:
        for move in moves:
            newBoard = deepcopy(board)
            gameplay.doMove(newBoard, color, move)
            beta = min(beta, alphaBeta(newBoard, gameplay.opponent(color), reverse, depth - 1, alpha, beta))
            if beta < alpha:
                break
        return beta


def nextMoveR(board, color, time):
    return nextMove(board, color, time, True)


def nextMove(board, color, time, reversed=False):
    if time > 150:
        depth = 5
    else:
        depth = 3
    moves = []
    for i in range(8):
        for j in range(8):
            if gameplay.valid(board, color, (i, j)):
                moves.append((i, j))
    if len(moves) == 0:
        return "pass"
    best = None
    for move in moves:
        newBoard = deepcopy(board)
        gameplay.doMove(newBoard, color, move)
        moveVal = alphaBeta(newBoard, gameplay.opponent(color), reversed, depth)
        if best == None or betterThan(moveVal, best, color, reversed):
            bestMove = move
            best = moveVal
    return bestMove