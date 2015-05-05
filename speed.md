This page covers methods of making the Starcraft application run faster. Note that the speed can also depend on the computer's processing power.



# Disable Sound #
![speed_tut_01.gif](speed_tut_01.gif)

Open the BWAPI config file in ` bwapi-data/bwapi.ini ` and set ` sound ` to ` OFF `.

_Muting the sound and music is not the same as disabling the entire sound engine. If the sound is muted, the functions to play sounds are still called and the sounds are still processed at Starcraft's level. When the sound engine is disabled, these are skipped entirely._

# Change Game Speed #
![speed_tut_03.gif](speed_tut_03.gif)

Typing ` /speed 0 ` in Starcraft will disable the delay between frames. You can perform the same action from an AI module by calling the function [Game::setLocalSpeed(0)](Game#setLocalSpeed.md).

# Disable the GUI #
![speed_tut_04.gif](speed_tut_04.gif)

Typing ` /nogui ` in Starcraft will toggle the display of graphics. Disabling the Graphical User Interface (GUI) will prevent Starcraft from calling its drawing routines and image processing functions. The same action can be performed from an AI module by calling the function [Game::setGUI(false)](Game#setGUI.md).

_Using this option will cause the game to remain inside of the game loop instead of returning after every game frame._

_Note that when this option is enabled, the CPU Throttling option in the game menu does not impact performance._
