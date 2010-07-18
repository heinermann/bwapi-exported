#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/BulletImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Flag.h>
#include <BWAPI.h>

#include <BW/Unit.h>
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>
#include <BW/Dialog.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"
#include "BWtoBWAPI.h"

namespace BWAPI
{
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int  GameImpl::mapWidth()
  {
    /* Get the width of the map */
    return Map::getWidth();
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int  GameImpl::mapHeight()
  {
    /* Get the height of the map */
    return Map::getHeight();
  }
  //---------------------------------------------- MAP FILE NAME ---------------------------------------------
  std::string  GameImpl::mapFileName()
  {
    /* Get the map file name */
    return Map::getFileName();
  }
  //---------------------------------------------- MAP PATH NAME ---------------------------------------------
  std::string  GameImpl::mapPathName()
  {
    /* Get the map file name */
    return Map::getPathName();
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string  GameImpl::mapName()
  {
    /* Get the name of the map */
    return Map::getName();
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  std::string GameImpl::mapHash()
  {
    /* Return a hash of the map file */
    return this->savedMapHash;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  int GameImpl::getFPS()
  {
    /* Retrieve the Frames Per Second */
    return fps;
  }
  //-------------------------------------------- GET Average FPS ---------------------------------------------
  double GameImpl::getAverageFPS()
  {
    /* Retrieve the moving average Frames Per Second */
    return averageFPS;
  }
  //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
  int  GameImpl::getGroundHeight(int x, int y)
  {
    /* Return the ground height */
    this->setLastError(Errors::None);
    return this->map.groundHeight(x, y);
  }
  //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
  int  GameImpl::getGroundHeight(TilePosition position)
  {
    /* Return the ground height */
    this->setLastError(Errors::None);
    return this->map.groundHeight(position.x(), position.y());
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool  GameImpl::isBuildable(int x, int y)
  {
    /* Check if the specified coordinates are buildable */
    this->setLastError(Errors::None);
    return this->map.buildable(x, y);
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool  GameImpl::isBuildable(TilePosition position)
  {
    /* Check if the specified coordinates are buildable */
    this->setLastError(Errors::None);
    return this->map.buildable(position.x(), position.y());
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool  GameImpl::isWalkable(int x, int y)
  {
    /* Check if the specified coordinates are walkable */
    this->setLastError(Errors::None);
    return this->map.walkable(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isVisible(int x, int y)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.visible(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isVisible(TilePosition position)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.visible(position.x(), position.y());
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isExplored(int x, int y)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.isExplored(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isExplored(TilePosition position)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.isExplored(position.x(), position.y());
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool  GameImpl::hasCreep(int x, int y)
  {
    /* Check if the specified coordinates have creep */
    this->setLastError(Errors::None);
    /* Deny this information if you don't have complete map information */
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->isVisible(x, y))
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    return this->map.hasCreep(x, y);
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool  GameImpl::hasCreep(TilePosition position)
  {
    return this->hasCreep(position.x(), position.y());
  }
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool  GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight)
  {
    /* Check if the specified coordinates have power */
    this->setLastError(Errors::None);
    if (!(tileWidth == 2 && tileHeight == 2) &&
        !(tileWidth == 3 && tileHeight == 2) &&
        !(tileWidth == 4 && tileHeight == 3))
    {
      return false;
    }
    if (tileWidth == 4)
    {
      x++;
    }
    /* Loop through all pylons for the current player */
    foreach (UnitImpl* i, myPylons)
    {
      int px = i->getTilePosition().x();
      int py = i->getTilePosition().y();
      int bx = x - px + 7;
      int by = y - py + 4;
      /* Deal with special cases, pylon offset cutoff */
      if (bx >= 0 && by >= 0 && bx <= 14 && by <= 8)
      {
        switch(by)
        {
          case 0:
            if (bx >= 1 && bx <= 12) return true;
          break;
          case 1:
            if (bx <= 13) return true;
          break;
          case 2:
          case 3:
          case 4:
          case 5:
            return true;
          break;
          case 6:
            if (bx <= 13) return true;
          case 7:
            if (bx >= 1 && bx <= 12) return true;
          case 8:
            if (bx >= 4 && bx <= 9) return true;
          break;
        }
      }
    }
    return false;
  }
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool  GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight)
  {
    return this->hasPower(position.x(),position.y(),tileWidth,tileHeight);
  }
  //---------------------------------------------- CAN BUILD HERE --------------------------------------------
  bool  GameImpl::canBuildHere(Unit* builder, TilePosition position, UnitType type)
  {
    this->setLastError(Errors::Unbuildable_Location);
    if (position.x() < 0)
      return false;
    if (position.y() < 0)
      return false;
    int width  = type.tileWidth();
    int height = type.tileHeight();
    if (position.x() + width > this->mapWidth())
      return false;
    if (position.y() + height >= this->mapHeight())
      return false;
    if (type.isRefinery())
    {
      foreach (Unit* g, getGeysers())
      {
        if (g->getTilePosition() == position)
        {
          if (g->isVisible() && g->getType()!=UnitTypes::Resource_Vespene_Geyser)
            return false;
          this->setLastError(Errors::None);
          return true;
        }
      }
      return false;
    }
    for(int x = position.x(); x < position.x() + width; x++)
    {
      for(int y = position.y(); y < position.y() + height; y++)
      {
        std::set<Unit*> groundUnits;
        foreach (Unit* i, unitsOnTile(x,y))
          if (!i->getType().isFlyer() && !i->isLifted())
            groundUnits.insert(i);

        if (!this->isBuildable(x,y) || groundUnits.size() > 1)
          return false;

        if (!groundUnits.empty())
        {
          Unit* blocking = *(groundUnits.begin());
          if (blocking != builder)
            return false;
        }
      }
    }
    if (type.getRace() == BWAPI::Races::Zerg)
    {
      if (!type.isResourceDepot())
        for(int x = position.x(); x < position.x() + width; x++)
          for(int y = position.y(); y < position.y() + height; y++)
            if (!BWAPI::Broodwar->hasCreep(x, y))
              return false;
    }
    else
    {
      for(int x = position.x(); x < position.x() + width; x++)
        for(int y = position.y(); y < position.y() + height; y++)
          if (BWAPI::Broodwar->hasCreep(x, y))
            return false;
    }

    if (type.requiresPsi())
    {
      if (this->hasPower(position.x(), position.y(), width, height))
      {
        this->setLastError(Errors::None);
        return true;
      }
      return false;
    }

    if (type.isResourceDepot())
    {
      foreach (BWAPI::Unit* m, getStaticMinerals())
      {
        if (isVisible(m->getInitialTilePosition()) ||
            isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
          if (!m->isVisible())
            continue; // tile position is visible, but mineral is not => mineral does not exist
        if (m->getInitialTilePosition().x() > position.x() - 5 &&
            m->getInitialTilePosition().y() > position.y() - 4 &&
            m->getInitialTilePosition().x() < position.x() + 7 &&
            m->getInitialTilePosition().y() < position.y() + 6)
        {
          return false;
        }
      }
      foreach (BWAPI::Unit* g, getStaticGeysers())
      {
        if (g->getInitialTilePosition().x() > position.x() - 7 &&
            g->getInitialTilePosition().y() > position.y() - 5 &&
            g->getInitialTilePosition().x() < position.x() + 7 &&
            g->getInitialTilePosition().y() < position.y() + 6)
        {
          return false;
        }
      }
    }
    this->setLastError(Errors::None);
    return true;
  }
  //------------------------------------------------- CAN MAKE -----------------------------------------------
  bool  GameImpl::canMake(Unit* builder, UnitType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (builder != NULL)
    {
      /* Check if the owner of the unit is you */
      if (builder->getPlayer() != self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      /* Check if this unit can actually build the unit type */
      if (builder->getType() != type.whatBuilds().first)
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }

      /* Carrier space */
      if (builder->getType() == UnitTypes::Protoss_Carrier)
      {
        int max_amt = 4;
        if (self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity)>0)
          max_amt += 4;
        if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
        {
          this->setLastError(Errors::Insufficient_Space);
          return false;
        }
      }

      /* Reaver Space */
      if (builder->getType() == UnitTypes::Protoss_Reaver)
      {
        int max_amt = 5;
        if (self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
          max_amt += 5;
        if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
        {
          this->setLastError(Errors::Insufficient_Space);
          return false;
        }
      }
    }

    /* Check if player has enough minerals */
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }

    /* Check if player has enough gas */
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    
    /* Check if player has enough supplies */
    if (type.supplyRequired() > 0)
      if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first.supplyRequired())
      {
        this->setLastError(Errors::Insufficient_Supply);
        return false;
      }

    UnitType addon = UnitTypes::None;
    for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
      if (i->first.isAddon())
        addon=i->first;

    for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
    {
      bool pass = false;
      if (self()->completedUnitCount(i->first) >= i->second)
        pass = true;
      if (i->first == UnitTypes::Zerg_Hatchery)
      {
        if (self()->completedUnitCount(UnitTypes::Zerg_Lair) >= i->second)
          pass = true;
        if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
      }
      if (i->first == UnitTypes::Zerg_Lair)
        if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
      if (pass == false)
      {
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }
    }

    if (type.requiredTech() != TechTypes::None)
      if (!self()->hasResearched(type.requiredTech()))
      {
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }

    if (builder != NULL)
      if (addon != UnitTypes::None && addon.whatBuilds().first==type.whatBuilds().first)
        if (builder->getAddon() == NULL || builder->getAddon()->getType() != addon)
        {
          this->setLastError(Errors::Insufficient_Tech);
          return false;
        }
    return true;
  }
  //----------------------------------------------- CAN RESEARCH ---------------------------------------------
  bool  GameImpl::canResearch(Unit* unit, TechType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatResearches())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->isResearching(type))
    {
      this->setLastError(Errors::Currently_Researching);
      return false;
    }
    if (self()->hasResearched(type))
    {
      this->setLastError(Errors::Already_Researched);
      return false;
    }
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //----------------------------------------------- CAN UPGRADE ----------------------------------------------
  bool  GameImpl::canUpgrade(Unit* unit, UpgradeType type)
  {
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatUpgrades())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->isUpgrading(type))
    {
      this->setLastError(Errors::Currently_Upgrading);
      return false;
    }
    if (self()->getUpgradeLevel(type)>=type.maxRepeats())
    {
      this->setLastError(Errors::Fully_Upgraded);
      return false;
    }
    if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  std::set< TilePosition >& GameImpl::getStartLocations()
  {
    /* Return the set of Start Locations */
    this->setLastError(Errors::None);
    return this->startLocations;
  }
  //----------------------------------------------- GET FORCES -----------------------------------------------
  std::set< Force* >& GameImpl::getForces()
  {
    /* Return a set of forces */
    this->setLastError(Errors::None);
    return this->forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  std::set< Player* >& GameImpl::getPlayers()
  {
    this->setLastError(Errors::None);
    return this->playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  std::set< Unit* >& GameImpl::getAllUnits()
  {
    this->setLastError(Errors::None);
    return this->allUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getMinerals()
  {
    this->setLastError(Errors::None);
    return this->minerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getGeysers()
  {
    this->setLastError(Errors::None);
    return this->geysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getNeutralUnits()
  {
    this->setLastError(Errors::None);
    return this->neutralUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticMinerals()
  {
    this->setLastError(Errors::None);
    return this->staticMinerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticGeysers()
  {
    this->setLastError(Errors::None);
    return this->staticGeysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getStaticNeutralUnits()
  {
    this->setLastError(Errors::None);
    return this->staticNeutralUnits;
  }
  //---------------------------------------------- GET BULLETS -----------------------------------------------
  std::set< Bullet* >& GameImpl::getBullets()
  {
    this->setLastError(Errors::None);
    return this->bullets;
  }

  //---------------------------------------------- GET EVENTS ------------------------------------------------
  std::list< Event >& GameImpl::getEvents()
  {
    this->setLastError(Errors::None);
    return this->events;
  }

  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::isMultiplayer()
  {
    this->setLastError(Errors::None);
    return (*BW::BWDATA_IsMultiplayer != 0);
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet()
  {
    this->setLastError(Errors::None);
    return (*BW::BWDATA_IsBattleNet != 0);
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::isInGame()
  {
    this->setLastError(Errors::None);
    return this->inGame;
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_IsNotPaused == 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::isReplay()
  {
    this->setLastError(Errors::None);
    return *(BW::BWDATA_InReplay) != 0;
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void  GameImpl::printf(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    if (_isReplay() || _isInGame())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (!_isInGame())
      printEx(8, "%s", buffer);
  }

  void  GameImpl::sendText(const char *text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    sendTextEx(0, 0, "%s", buffer);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void  GameImpl::changeRace(BWAPI::Race race)
  {
    this->setLastError(Errors::None);
    this->_changeRace(this->BWAPIPlayer->getIndex(),race);
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool GameImpl::onSendText(const char* text)
  {
    /* prep onSendText */
    if (this->parseText(text) || !this->isFlagEnabled(BWAPI::Flag::UserInput))
      return true;
    else
    {
      if (this->client != NULL)
      {
        events.push_back(Event::SendText(std::string(text)));
        return !this->client->onSendText(std::string(text));
      }
    }
    return false;
  }
  //---------------------------------------------- ON RECV TEXT ----------------------------------------------
  void GameImpl::onReceiveText(int playerId, std::string text)
  {
    /* Do onReceiveText */
    int realId = stormIdToPlayerId(playerId);
    if ( this->client != NULL && realId != -1 && realId != this->BWAPIPlayer->getIndex())
    {
      events.push_back(Event::ReceiveText(this->players[realId], text));
      this->client->onReceiveText(this->players[realId], text);
    }
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    /* Starts the game as a lobby host */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    /* Pauses the game */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    /* Resumes the game */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    /* Leaves the current game. Moves directly to the post-game score screen */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    /* Restarts the current match 
       Does not work on Battle.net */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 5;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::getMouseX()
  {
    /* Retrieves the mouse's X coordinate */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_Mouse->x;
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::getMouseY()
  {
    /* Retrieves the mouse's Y coordinate */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_Mouse->y;
  }
  //------------------------------------------- GET MOUSE POSITION -------------------------------------------
  BWAPI::Position GameImpl::getMousePosition()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return BWAPI::Positions::Unknown;
    }
    return BWAPI::Position(BW::BWDATA_Mouse->x, BW::BWDATA_Mouse->y);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button)
  {
    return getMouseState((int)button);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(int button)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    if (button<0 || button>=3) return false;
    SHORT ButtonDown = 0;
    switch (button)
    {
      case BWAPI::M_LEFT:
        ButtonDown = GetKeyState(VK_LBUTTON);
        break;
      case BWAPI::M_MIDDLE:
        ButtonDown = GetKeyState(VK_MBUTTON);
        break;
      case BWAPI::M_RIGHT:
        ButtonDown = GetKeyState(VK_RBUTTON);
        break;
      default:
        break;
    }
    bool pressed = (ButtonDown & 128) > 0;
    return pressed;
    
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key)
  {
    return getKeyState((int)key);
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(int key)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    if (key < 0 || key >= 255)
      return false;

    return (GetKeyState(key) & 128) > 0;
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::getScreenX()
  {
    /* Retrieves the screen's X coordinate in relation to the map */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::getScreenY()
  {
    /* Retrieves the screen's Y coordinate in relation to the map */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenY);
  }
  //------------------------------------------- GET SCREEN POSITION ------------------------------------------
  BWAPI::Position GameImpl::getScreenPosition()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return BWAPI::Positions::Unknown;
    }
    return BWAPI::Position(*(BW::BWDATA_ScreenX),*(BW::BWDATA_ScreenY));
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    /* Sets the screen's position relative to the map */
    if (x<0) x=0;
    if (y<0) y=0;
    if (x>Map::getWidth()*32-640) x=Map::getWidth()*32-640;
    if (y>Map::getHeight()*32-480) y=Map::getHeight()*32-480;

    this->setLastError(Errors::None);
    x &= 0xFFFFFFF8;
    y &= 0xFFFFFFF8;
    *BW::BWDATA_MoveToX = x;
    BW::BWDATA_MoveToTile->x = (u16)(x >> 5);
    *BW::BWDATA_MoveToY = y;
    BW::BWDATA_MoveToTile->y = (u16)(y >> 5);
    BW::BWFXN_UpdateScreenPosition();
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI::Position p)
  {
    setScreenPosition(p.x(),p.y());
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)x,(u16)y)), sizeof(BW::Orders::MinimapPing));
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)p.x(),(u16)p.y())), sizeof(BW::Orders::MinimapPing));
  }
  //----------------------------------------------------------------------------------------------------------
  Force* GameImpl::getForce(int forceID)
  {
    return server.getForce(forceID);
  }
  //----------------------------------------------------------------------------------------------------------
  Player* GameImpl::getPlayer(int playerID)
  {
    return server.getPlayer(playerID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* GameImpl::getUnit(int unitID)
  {
    return server.getUnit(unitID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* GameImpl::indexToUnit(int unitIndex)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(Flag::CompleteMapInformation) == false)
      return NULL;
    int i = (unitIndex & 0x7FF);
    if ( i < 1700 && this->unitArray[i]->canAccess() )
      return this->unitArray[i];
    return NULL;
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  std::set<BWAPI::Unit*>& GameImpl::getSelectedUnits()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return emptySet;
    }
    return selectedUnitSet;
  }
  //--------------------------------------------- GET GAME TYPE ----------------------------------------------
  GameType GameImpl::getGameType()
  {
    this->setLastError(Errors::None);
    if (isReplay())
      return GameTypes::None;
    return GameType(*BW::BWDATA_gameType);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency()
  {
    /* Returns the real latency values */

    /* Error checking */
    this->setLastError(Errors::None);
    if (_isSinglePlayer())
      return BWAPI::Latency::SinglePlayer;

    if (_isBattleNet())
    {
      switch(*BW::BWDATA_Latency)
      {
        case 0:
          return BWAPI::Latency::BattlenetLow;
        case 1:
          return BWAPI::Latency::BattlenetMedium;
        case 2:
          return BWAPI::Latency::BattlenetHigh;
        default:
          return BWAPI::Latency::BattlenetLow;
      }
    }
    else
    {
      switch(*BW::BWDATA_Latency)
      {
        case 0:
          return BWAPI::Latency::LanLow;
        case 1:
          return BWAPI::Latency::LanMedium;
        case 2:
          return BWAPI::Latency::LanHigh;
        default:
          return BWAPI::Latency::LanLow;
      }
    }
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int  GameImpl::getFrameCount()
  {
    /* Retrieves the number of frames since game start */
    this->setLastError(Errors::None);
    return this->frameCount;
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::set<Unit*>& GameImpl::unitsOnTile(int x, int y)
  {
    /* Retrieves a set of units that are on the specified tile */
    this->setLastError(Errors::None);
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptySet;

    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
    {
      this->setLastError(Errors::Access_Denied);
      return this->emptySet;
    }
    return unitsOnTileData[x][y];
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error  GameImpl::getLastError() const
  {
    /* returns the last error encountered in BWAPI */
    return this->lastError;
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool  GameImpl::isFlagEnabled(int flag)
  {
    /* checks if a BWAPI flag is enabled */
    this->setLastError(Errors::None);
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void  GameImpl::enableFlag(int flag)
  {
    /* Enable the specified flag */

    /* Error checking */
    this->setLastError(Errors::None);
    if (this->frameCount > 0)
    {
      this->sendText("Flags can only be enabled at the start of a game.");
      return;
    }

    if (flag >= BWAPI::FLAG_COUNT)
    {
      this->sendText("Invalid flag (%d).", flag);
      return;
    }

    /* Modify flag state */
    this->flags[flag] = true;
    switch(flag)
    {
    case BWAPI::Flag::CompleteMapInformation:
      this->sendText("Enabled Flag CompleteMapInformation");
      break;
    case BWAPI::Flag::UserInput:
      this->sendText("Enabled Flag UserInput");
      break;
    }
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player*  GameImpl::self()
  {
    /* Retrieves the class for the current player */
    this->setLastError(Errors::None);
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player*  GameImpl::enemy()
  {
    /* Retrieves the class for the first opponent player */
    this->setLastError(Errors::None);
    return (Player*)this->opponent;
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void  GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box */
    if (!inScreen(ctype,left,top,right,bottom)) return;
    addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    addShape(new ShapeDot(ctype, x, y, color.getID()));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color.getID()));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void  GameImpl::setTextSize(int size)
  {
    if ( size < 1 || size > 3 )
      size = 1;
    textSize = size;
  }
  void  GameImpl::drawText(int ctype, int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(ctype,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMap(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMouse(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextScreen(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Screen,x,y,std::string(buffer),(char)textSize));
  }
  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void  GameImpl::setLocalSpeed(int speed)
  {
    /* Sets the frame rate of the client */
    if (speed < 0)
    {
      /* Reset the speed if it is negative */
      for ( int i = 0; i < 7; i++ )
      {
        BW::BWDATA_GameSpeedModifiers[i]    = BW::OriginalSpeedModifiers[i];
        BW::BWDATA_GameSpeedModifiers[i+7]  = BW::OriginalSpeedModifiers[i] * 3;
      }
    }
    else
    {
      /* Set all speeds if it is positive */
      for (int i = 0; i < 7; i++)
      {
        BW::BWDATA_GameSpeedModifiers[i]    = speed;
        BW::BWDATA_GameSpeedModifiers[i+7]  = speed * 3;
      }
    }
  }
};
