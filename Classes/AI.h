//
// Created by Rahul Yadav on 2020-01-19.
//

#ifndef PROJ_ANDROID_AI_H
#define PROJ_ANDROID_AI_H

#include <array>
#include <vector>
#include "cocos2d.h"


class AI
{
public:
    AI( int playerPiece );

    void PlacePiece( int ( *gridArray )[3][3], cocos2d::Sprite *gridPieces[3][3], int *gameState );

private:
    void CheckSection( int x1, int y1, int x2, int y2, int X, int Y, int pieceToCheck, int ( *gridArray )[3][3], cocos2d::Sprite *gridPieces[3][3] );
    void CheckIfPieceIsEmpty( int X, int Y, int ( *gridArray )[3][3], cocos2d::Sprite *gridPieces[3][3] );

    int aiPiece;
    int playerPiece;

    std::vector<std::array<int, 6>> checkMatchVector;

};

#endif //PROJ_ANDROID_AI_H
