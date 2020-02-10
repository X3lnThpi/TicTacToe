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

#include "SinglePlayerScene.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;



Scene* SinglePlayer::createScene()
{
    CCLOG("SinglePlayer createScene");

    return SinglePlayer::create();

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainMenuScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SinglePlayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    CCLOG("visibleSize:%.1f,%.1f",visibleSize.width,visibleSize.height);
    CCLOG("origin:%.1f,%.1f",origin.x,origin.y);




    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label




    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    auto bg = Sprite::create(GAME_BACKGROUND);
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(bg);


    ui::Button* backButton = ui::Button::create(BACK_BUTTON);
    backButton->setPosition(Vec2(visibleSize.width * 0.18 + origin.x, visibleSize.height * 0.92+ origin.y ));
    backButton->addTouchEventListener( CC_CALLBACK_0(SinglePlayer::backButton, this) );
    this->addChild(backButton);



    //The Rectangular 3X3 grid
    gridSprite = Sprite::create(GRID_BACKGROUND);
    gridSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(gridSprite);


    gameState = STATE_PLACING_PIECE;

    InitGridRects( );
    InitGridPieces( );

        for ( int x = 0; x < 3; x++ )
        {
            for ( int y = 0; y < 3; y++ )
            {
                gridArray[x][y] = EMPTY_PIECE;
            }
        }


    turn = X_PIECE;



        EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create( );
            listener->setSwallowTouches( true );

            listener->onTouchBegan = CC_CALLBACK_2( SinglePlayer::onTouchBegan, this );
            listener->onTouchMoved = CC_CALLBACK_2( SinglePlayer::onTouchMoved, this );
            listener->onTouchEnded = CC_CALLBACK_2( SinglePlayer::onTouchEnded, this );
            //listener->onTouchCancelled = CC_CALLBACK_2( SinglePlayer::onTouchCancelled, this );

            Director::getInstance( )->getEventDispatcher( )->addEventListenerWithSceneGraphPriority( listener, this );





    return true;
}



// called when the touch first begins
bool SinglePlayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("Touch began dude");
    return true; // true if the function wants to swallow the touch
}


// called when the user moves their finger
void SinglePlayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("Touch is moving dude");
}


// called when the user lifts their finger
void SinglePlayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

        CCLOG("Lets See dude");
        if(gameState != STATE_WON)
        {
            CheckAndPlacePiece(touch);
        }

}



// called when the device goes to another application such as a phone call
void SinglePlayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void SinglePlayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}





void SinglePlayer::InitGridRects( )
{
    float width = gridSprite->getBoundingBox().size.width / 3;
    float height = gridSprite->getBoundingBox().size.height / 3;
    float xOrigin = gridSprite->getBoundingBox( ).getMinX( );
    float yOrigin = gridSprite->getBoundingBox( ).getMinY( );

    for(int i=0;i<3;i++){
        for (int j = 0; j < 3; j++) {
            float x= xOrigin + (width * i) + width / 2;
            float y = yOrigin + (height * j) + height / 2;
            midPointOfGridSpaces[i][j] = {x,y};
            //CCLOG("midPointOfGridSpaces: {%d}{%d}  === { %f }  { %f }", i, j,
            //midPointOfGridSpaces[i][j].x, midPointOfGridSpaces[i][j].y);
            //CCLOG("Ameer origin: { %f } , { %f }", xOrigin + (width * i) + width / 2,yOrigin + (height * j) + height / 2);
            //CCLOG("x y origin: { %f } , { %f }", x,y);


        }
    }

    gridSpaces[0][0] = Rect(gridSprite->getBoundingBox( ).getMinX( ),
                            gridSprite->getBoundingBox( ).getMinY( ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[1][0] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox( ).getMinY( ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[2][0] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                            gridSprite->getBoundingBox( ).getMinY( ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[0][1] = Rect(gridSprite->getBoundingBox( ).getMinX( ),
                            gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[1][1] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[2][1] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                            gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[0][2] = Rect(gridSprite->getBoundingBox( ).getMinX( ),
                            gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[1][2] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );

    gridSpaces[2][2] = Rect(gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                            gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3
                            );


                            CCLOG("Individual Rectangle Grid is all initialized");
}


void SinglePlayer::InitGridPieces( )
{
    //int k=1;
    cocos2d::Color4F color = Color4F::RED;

    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            gridPieces[x][y] = Sprite::create(XPIECE);
            gridPieces[x][y]->setPosition( Vec2( gridSprite->getPositionX( ) + ( gridPieces[x][y]->getContentSize( ).width * ( x - 1 ) ), gridSprite->getPositionY( ) + ( gridPieces[x][y]->getContentSize( ).height * ( y - 1 ) ) ) );
            gridPieces[x][y]->setVisible( false );
            gridPieces[x][y]->setOpacity( 100 );
            this->addChild( gridPieces[x][y] );
            CCLOG("Grid Pieces also initialized");
        }
    }
}


void SinglePlayer::CheckAndPlacePiece( cocos2d::Touch *touch )
{
    gameState = STATE_PLACING_PIECE;
    Rect rect1 = gridSprite->getBoundingBox( );
    Point touchPoint = touch->getLocation( );
    for ( int x = 0; x < 3; x++ )
        {
            for ( int y = 0; y < 3; y++ )
            {

                if ( gridSpaces[x][y].containsPoint( touchPoint ) )
                {
                        if ( gridArray[x][y] == EMPTY_PIECE ){
                             CCLOG("Check and place piece method workin");


                                gridArray[x][y] = turn;
                              if ( X_PIECE == turn ){
                                  playOSound();
                                  gridPieces[x][y]->setTexture(XPIECE);
                                  //turn = O_PIECE;
                              }
                              else{
                                  playXSound();
                                  gridPieces[x][y]->setTexture(OPIECE);
                                  //turn = X_PIECE;
                              }
                             gridPieces[x][y]->setVisible( true );
                             gridPieces[x][y]->setOpacity( 100 );
                             gridPieces[x][y]->runAction( Sequence::create( FadeIn::create( PIECE_FADE_IN_TIME ), CallFunc::create( std::bind( &SinglePlayer::CheckWin, this, x, y ) ), NULL ) );
                        }

                }

            }

         }



}

void SinglePlayer::CheckWin( int x, int y )
{
    Check3PiecesForMatch( 0, 0, 1, 0, 2, 0);
    Check3PiecesForMatch( 0, 1, 1, 1, 2, 1);
    Check3PiecesForMatch( 0, 2, 1, 2, 2, 2);
    Check3PiecesForMatch( 0, 0, 0, 1, 0, 2);
    Check3PiecesForMatch( 1, 0, 1, 1, 1, 2);
    Check3PiecesForMatch( 2, 0, 2, 1, 2, 2);
    Check3PiecesForMatch( 0, 0, 1, 1, 2, 2);
    Check3PiecesForMatch( 0, 2, 1, 1, 2, 0);

    // check who's current turn it is and switch
    if ( X_PIECE == turn )
    {
        turn = O_PIECE;
    }
    else
    {
        turn = X_PIECE;
    }

    if ( STATE_PLACING_PIECE == gameState )
    {
        gameState = STATE_PLAYING;
    }
}


void SinglePlayer::placeStrike(int i, int j, int typeOfStrike){

    __String winningPieceStr;

    if(typeOfStrike==LEFT_DIAGONAL_WIN){
        winningPieceStr = LEFT_DIAGONAL_WIN_STRIKE;
    } else if(typeOfStrike==RIGHT_DIAGONAL_WIN){
        winningPieceStr = RIGHT_DIAGONAL_WIN_STRIKE;
    } else if(typeOfStrike==HORIZONTAL_WIN){
        winningPieceStr = HORIZONTAL_WIN_STRIKE;
    } else if(typeOfStrike==VERTICLE_WIN){
        winningPieceStr = VERTICLE_WIN_STRIKE;
    }
    //CCLOG("origin: { %d } , { %d } , {%d} , {%s}", i,j,typeOfStrike,winningPieceStr.getCString());
    auto gridSprite1 = Sprite::create(winningPieceStr.getCString());
    gridSprite1->setPosition(Vec2(midPointOfGridSpaces[i][j].x, midPointOfGridSpaces[i][j].y));
    this->addChild(gridSprite1);
    playWinSound();
    gameOver();



}



void SinglePlayer::Check3PiecesForMatch( int x1, int y1, int x2, int y2, int x3, int y3 ) {
    if (turn == gridArray[x1][y1] && turn == gridArray[x2][y2] && turn == gridArray[x3][y3]) {
        gameState = STATE_WON;

        int slope = 0;
        if (x1 != x2 && y1 != y2) {
            slope = (y2 - y1) / (x2 - x1);
            if (slope == 1) {
                slope = RIGHT_DIAGONAL_WIN;
            } else if (slope == -1) {
                slope = LEFT_DIAGONAL_WIN;
            }
        }
        if (y1 == y2) {
            //horizontal
            slope = HORIZONTAL_WIN;
        }
        if (x1 == x2) {
            slope = VERTICLE_WIN;
            //verticle
        }
        placeStrike(x2, y2, slope);
        //notify ui thread to not

    }

}


/*        __String winningPieceStr;

        if ( O_PIECE == turn )
        {
            winningPieceStr = O_WINNING_PIECE_FILEPATH;
        }
        else
        {
            winningPieceStr = X_WINNING_PIECE_FILEPATH;
        }

        Sprite *winningPieces[3];

        winningPieces[0] = Sprite::create( winningPieceStr.getCString() );
        winningPieces[0]->setPosition( gridPieces[x1][y1]->getPosition( ) );
        winningPieces[0]->setOpacity( 0 );
        this->addChild( winningPieces[0] );
        winningPieces[1] = Sprite::create( winningPieceStr.getCString( ) );
        winningPieces[1]->setPosition( gridPieces[x2][y2]->getPosition( ) );
        winningPieces[1]->setOpacity( 0 );
        this->addChild( winningPieces[1] );
        winningPieces[2] = Sprite::create( winningPieceStr.getCString( ) );
        winningPieces[2]->setPosition( gridPieces[x3][y3]->getPosition( ) );
        winningPieces[2]->setOpacity( 0 );
        this->addChild( winningPieces[2] );

        winningPieces[0]->runAction( FadeIn::create( PIECE_FADE_IN_TIME ) );
        winningPieces[1]->runAction( Sequence::create( DelayTime::create( PIECE_FADE_IN_TIME * 0.5 ), FadeIn::create( PIECE_FADE_IN_TIME ), NULL ) );
        winningPieces[2]->runAction( Sequence::create( DelayTime::create( PIECE_FADE_IN_TIME * 1.5 ), FadeIn::create( PIECE_FADE_IN_TIME ), NULL ) );*/


//}


void SinglePlayer::backButton(){
    auto scene = MainMenu::createScene();
    CCLOG("origin: { %d }", 12);
    Director::getInstance()->pushScene(scene);
}

void SinglePlayer::reloadGame(){
    auto scene = SinglePlayer::createScene();
    CCLOG("origin: { %d }", 12);
    Director::getInstance()->pushScene(scene);
}


void SinglePlayer::gameOver(){


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    ui::Button* HomeButton = ui::Button::create(HOME);
    HomeButton->setPosition(Vec2(visibleSize.width * 0.38 + origin.x, visibleSize.height * 0.22+ origin.y ));
    HomeButton->addTouchEventListener( CC_CALLBACK_0(SinglePlayer::backButton, this) );
    this->addChild(HomeButton);


    ui::Button* reloadButton = ui::Button::create(RELOAD);
    reloadButton->setPosition(Vec2(visibleSize.width * 0.68 + origin.x, visibleSize.height * 0.22+ origin.y ));
    reloadButton->addTouchEventListener( CC_CALLBACK_0(SinglePlayer::reloadGame, this) );
    this->addChild(reloadButton);

}

void SinglePlayer::playXSound(){

    auto audio = SimpleAudioEngine::getInstance();
    audio->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("soundsVolume", 1.0f));
    // set the background music and play it just once.
    audio->playEffect(X_PIECE_SOUND, false, 1.0f, 1.0f, 1.0f);
}

void SinglePlayer::playOSound(){
    auto audio = SimpleAudioEngine::getInstance();
    audio->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("soundsVolume", 1.0f));
    // set the background music and play it just once.
    audio->playEffect(Y_PIECE_SOUND, false, 1.0f, 1.0f, 1.0f);
}

void SinglePlayer::playWinSound(){
    auto audio = SimpleAudioEngine::getInstance();
    audio->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("soundsVolume", 1.0f));
    // set the background music and play it just once.
    audio->playEffect(WIN_SOUND, false, 1.0f, 1.0f, 1.0f);
}