/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __AIGAME_SCENE_H__
#define __AIGAME_SCENE_H__
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"
#include "cocos2d.h"
#include "AI.h"





class AIGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(AIGame);

private:

        // called when the touch first begins
        bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
    // called when the user moves their finger
        void onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event );
        // called when the user lifts their finger
        void onTouchEnded( cocos2d::Touch *touch, cocos2d::Event *event );
        // called when the device goes to another application such as a phone call
        void onTouchCancelled( cocos2d::Touch *touch, cocos2d::Event *event );

        // Setup the grid rects so we can detect which space the user has clicked
        void InitGridRects( );
        void InitGridPieces( );

        // check if the user has clicked an empty grid space, if so place their piece
        void CheckAndPlacePiece( cocos2d::Touch *touch );
        // check if the user has won
        void CheckWin( int x, int y );
        // check a particular set of pieces
        void Check3PiecesForMatch( int x1, int y1, int x2, int y2, int x3, int y3, int pieceToCheck );

        void backButton();

        cocos2d::Sprite *gridSprite;
        cocos2d::Rect gridSpaces[3][3];
        cocos2d::Sprite *gridPieces[3][3];
        int gridArray[3][3];

        int turn;
        int gameState;
        AI *ai;
};

#endif  __AIGAME_SCENE_H__
