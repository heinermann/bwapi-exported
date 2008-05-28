/** @mainpage Welcome to Broodwar api documentaiton

@section intro Introduction

Broodwar API is library allowing simple access to broodwar relevant data and 
unit control.
The library is not inteded to make hacks or bw tools, but to make broodwar ai 
project possible, we are still considering how to avoid using this library to 
use some hack tools.

@section tutorial Tutorial

@section install Installation

The library target is Windows Xp + Visual studio 2005 only for now. 
To use the library:

-# Download latest sources.
-# Get Chaos launcher
-# Set Environment "ChaosDir" variable in windows (ControllPanels->System->Advanced->Envirenment variables) to your chaos launcher directory.
-# Compile (U must compile on "Release", you can also use debug later, but the ".bwl" has to be the release version)
-# move the content of the "starcraft-folder" into your starcraft folder (note that it is good to just move the bwapi.ini there and map the location of the bwapi-data to your project, co your data won't be doubled)
-# Run Chaos launcher and click to use the BWAPI addon

@section parametrization Parametrization of BW API

@section design Overall Design

*/

/** @page webpages Related webpages
  http://code.google.com/p/bwapi - home of the bwapi library.
  http://www.broodwarai.com/forums/index.php?showtopic=295 - Thread where the project started.
*/