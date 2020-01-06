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

#include "cocos2d.h"


USING_NS_CC;



Scene* SinglePlayer::createScene()
{
    return SinglePlayer::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
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


    //The Rectangular 3X3 grid
    gridSprite = Sprite::create("Grid.png");
    gridSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(gridSprite);

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
    CCLOG("Touch is working dude");
    return true; // true if the function wants to swallow the touch
}


// called when the user moves their finger
void SinglePlayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}


// called when the user lifts their finger
void SinglePlayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

        CCLOG("Lets See");
        CheckAndPlacePiece(touch);

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
    for ( int x = 0; x < 3; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            gridPieces[x][y] = Sprite::create("HelloWorld.png");
            gridPieces[x][y]->setPosition( Vec2( gridSprite->getPositionX( ) + ( gridPieces[x][y]->getContentSize( ).width * ( x - 1 ) ), gridSprite->getPositionY( ) + ( gridPieces[x][y]->getContentSize( ).height * ( y - 1 ) ) ) );

            gridPieces[x][y]->setVisible( false );
            gridPieces[x][y]->setOpacity( 0 );
            this->addChild( gridPieces[x][y] );
            CCLOG("Grid Pieces also initialized");
        }
    }
}


void SinglePlayer::CheckAndPlacePiece( cocos2d::Touch *touch )
{
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
                             gridPieces[x][y]->setTexture("X.png");
                             gridPieces[x][y]->setVisible( true );
                             gridPieces[x][y]->setOpacity( 100 );
                        }


                }






            }

         }



}
