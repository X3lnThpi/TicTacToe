#include "AIGameScene.h"
#include "Definitions.h"
#include "MainMenuScene.h"
#include <iostream>

USING_NS_CC;

Scene* AIGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = AIGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool AIGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    ui::Button* backButton = ui::Button::create(BACK_BUTTON);
    backButton->setPosition(Vec2(visibleSize.width * 0.18 + origin.x, visibleSize.height * 0.92+ origin.y ));
    backButton->addTouchEventListener( CC_CALLBACK_0(AIGame::backButton, this) );
    this->addChild(backButton);

    gridSprite = Sprite::create( GRID_BACKGROUND );
    gridSprite->setPosition( Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2) );
    this->addChild( gridSprite );

    InitGridRects( );
    InitGridPieces( );

    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            gridArray[x][y] = EMPTY_PIECE;
        }
    }

    turn = PLAYER_PIECE;

    ai = new AI( turn );

    gameState = STATE_PLAYING;

    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create( );
    listener->setSwallowTouches( true );

    listener->onTouchBegan = CC_CALLBACK_2( AIGame::onTouchBegan, this );
    listener->onTouchMoved = CC_CALLBACK_2( AIGame::onTouchMoved, this );
    listener->onTouchEnded = CC_CALLBACK_2( AIGame::onTouchEnded, this );
    listener->onTouchCancelled = CC_CALLBACK_2( AIGame::onTouchCancelled, this );

    Director::getInstance( )->getEventDispatcher( )->addEventListenerWithSceneGraphPriority( listener, this );

    return true;
}

// called when the touch first begins
bool AIGame::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true; // true if the function wants to swallow the touch
}

// called when the user moves their finger
void AIGame::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

// called when the user lifts their finger
void AIGame::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if ( STATE_PLAYING == gameState )
    {
        CheckAndPlacePiece( touch );
    }
}

// called when the device goes to another application such as a phone call
void AIGame::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void AIGame::InitGridRects( )
{
    gridSpaces[0][0] = Rect(    gridSprite->getBoundingBox( ).getMinX( ),
                                gridSprite->getBoundingBox( ).getMinY( ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[1][0] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox( ).getMinY( ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[2][0] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                                gridSprite->getBoundingBox( ).getMinY( ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[0][1] = Rect(    gridSprite->getBoundingBox( ).getMinX( ),
                                gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[1][1] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[2][1] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                                gridSprite->getBoundingBox( ).getMinY( ) + gridSprite->getBoundingBox().size.height / 3,
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[0][2] = Rect(    gridSprite->getBoundingBox( ).getMinX( ),
                                gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[1][2] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );

    gridSpaces[2][2] = Rect(    gridSprite->getBoundingBox( ).getMinX( ) + ( ( gridSprite->getBoundingBox().size.width / 3 ) * 2 ),
                                gridSprite->getBoundingBox( ).getMinY( ) + ( ( gridSprite->getBoundingBox().size.height / 3 ) * 2 ),
                                gridSprite->getBoundingBox().size.width / 3,
                                gridSprite->getBoundingBox().size.height / 3
    );
}

void AIGame::InitGridPieces( )
{
    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            gridPieces[x][y] = Sprite::create( XPIECE );
            gridPieces[x][y]->setPosition( Vec2( gridSprite->getPositionX( ) + ( gridPieces[x][y]->getContentSize( ).width * ( x - 1 ) ), gridSprite->getPositionY( ) + ( gridPieces[x][y]->getContentSize( ).height * ( y - 1 ) ) ) );
            gridPieces[x][y]->setVisible( false );
            gridPieces[x][y]->setOpacity( 0 );
            this->addChild( gridPieces[x][y] );
        }
    }
}

void AIGame::CheckAndPlacePiece( cocos2d::Touch *touch )
{
    Rect rect1 = gridSprite->getBoundingBox( );
    Point touchPoint = touch->getLocation( );

    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            if ( gridSpaces[x][y].containsPoint( touchPoint ) )
            {
                if ( gridArray[x][y] == EMPTY_PIECE )
                {
                    gameState = STATE_PLACING_PIECE;

                    gridArray[x][y] = turn;

                    // check who's current turn it is and switch
                    if ( X_PIECE == turn )
                    {
                        gridPieces[x][y]->setTexture( XPIECE );
                    }
                    else
                    {
                        gridPieces[x][y]->setTexture( OPIECE );
                    }

                    gridPieces[x][y]->setVisible( true );

                    gridPieces[x][y]->runAction( Sequence::create( FadeIn::create( PIECE_FADE_IN_TIME ), CallFunc::create( std::bind( &AIGame::CheckWin, this, x, y ) ), NULL ) );
                }
            }
        }
    }
}

void AIGame::CheckWin( int x, int y )
{
    Check3PiecesForMatch( 0, 0, 1, 0, 2, 0, PLAYER_PIECE );
    Check3PiecesForMatch( 0, 1, 1, 1, 2, 1, PLAYER_PIECE );
    Check3PiecesForMatch( 0, 2, 1, 2, 2, 2, PLAYER_PIECE );
    Check3PiecesForMatch( 0, 0, 0, 1, 0, 2, PLAYER_PIECE );
    Check3PiecesForMatch( 1, 0, 1, 1, 1, 2, PLAYER_PIECE );
    Check3PiecesForMatch( 2, 0, 2, 1, 2, 2, PLAYER_PIECE );
    Check3PiecesForMatch( 0, 0, 1, 1, 2, 2, PLAYER_PIECE );
    Check3PiecesForMatch( 0, 2, 1, 1, 2, 0, PLAYER_PIECE );

    if ( STATE_WON != gameState )
    {
        gameState = STATE_AI_PLAYING;
        ai->PlacePiece( &gridArray, gridPieces, &gameState );

        Check3PiecesForMatch( 0, 0, 1, 0, 2, 0, AI_PIECE );
        Check3PiecesForMatch( 0, 1, 1, 1, 2, 1, AI_PIECE );
        Check3PiecesForMatch( 0, 2, 1, 2, 2, 2, AI_PIECE );
        Check3PiecesForMatch( 0, 0, 0, 1, 0, 2, AI_PIECE );
        Check3PiecesForMatch( 1, 0, 1, 1, 1, 2, AI_PIECE );
        Check3PiecesForMatch( 2, 0, 2, 1, 2, 2, AI_PIECE );
        Check3PiecesForMatch( 0, 0, 1, 1, 2, 2, AI_PIECE );
        Check3PiecesForMatch( 0, 2, 1, 1, 2, 0, AI_PIECE );
    }

    int emptyNum = 9;

    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            if ( EMPTY_PIECE != gridArray[x][y] )
            {
                emptyNum--;
            }
        }
    }

    // check if the game is a draw
    if ( 0 == emptyNum )
    {
        gameState = STATE_DRAW;
    }

    std::cout << gameState << std::endl;
}

void AIGame::Check3PiecesForMatch( int x1, int y1, int x2, int y2, int x3, int y3, int pieceToCheck )
{
    if ( pieceToCheck == gridArray[x1][y1] && pieceToCheck == gridArray[x2][y2] && pieceToCheck == gridArray[x3][y3] )
    {
        __String winningPieceStr;

        if ( O_PIECE == pieceToCheck )
        {
            winningPieceStr = O_WINNING_PIECE_FILEPATH;
        }
        else
        {
            winningPieceStr = X_WINNING_PIECE_FILEPATH;
        }

        Sprite *winningPieces[3];

        winningPieces[0] = Sprite::create( winningPieceStr.getCString( ) );
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
        winningPieces[2]->runAction( Sequence::create( DelayTime::create( PIECE_FADE_IN_TIME * 1.5 ), FadeIn::create( PIECE_FADE_IN_TIME ), NULL ) );

        if ( PLAYER_PIECE == pieceToCheck )
        {
            gameState = STATE_WON;
        }
        else
        {
            gameState = STATE_LOSE;
        }
    }
}

void AIGame::backButton(){
    auto scene = MainMenu::createScene();
    CCLOG("backButton");
    Director::getInstance()->pushScene(scene);
}