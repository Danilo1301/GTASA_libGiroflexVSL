# Giroflex VSL (VehicleSirenLights)

## Install
* Required Apk: GTA SA 2.00 with FLA 6.5 or superior (with AML support)
* Required Libs:
  * AML (https://github.com/RusJJ/AndroidModLoader)
  * SAUtils (https://github.com/AndroidModLoader/SAUtils)
  * CLEO Mod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  * BASS Mod (https://github.com/AndroidModLoader/BASSMod)

( those libs are all inside the .zip file, in '/required libs', you just have to copy the .so files inside your /mods folder )<br>
( example: inside /Android/data/ro.samp_mobile.game/mods )<br>

1. Download the .zip file in [Releases](https://github.com/Danilo1301/GTASA_libGiroflex/releases)
2. Move the 'libGiroflex.so' from "/inside game folder" to your '/mods' folder
3. Move the 'giroflex.csa' and 'giroflex.fxt' files from "/inside CLEO" to where you load your cleo mods
4. If you haven't done yet, move the libs from "/required libs" inside your '/mods' folder

## How to use
* Press the 'imaginary buttons' 5 and 6 ([example](https://imgur.com/gA8Lfvt)) for 0.5s to turn lights on/off, and for 1s to open menu
* Panel: press the 'imaginary buttons' 5 and 9 ([example](https://imgur.com/LLddMfg)) for 0.5s

## Config
Each vehicle has a .ini file where it stores its config<br>
Those files are located at: GAME_FOLDER/configs/giroflex

## Problems

### Crash with the libBASSMod.so

Some users have reported crashes with the new libBASSMod (which is for playing audio). If your game is crashing, try removing 'libBASSMod.so' from /mods folder

### Mods cleo sumiram do menu

You just need to go in the game, then "Options" > "Game" and change the "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi

### Menu too much to the left

To change the menu's position, you need to enable the edit mode in: "Options" > "Mods Settings" > "Giroflex VSL - Edit mode"

## How to add the textures

* 1º Open TXD Tool
* 2º Click on the folder button (in rose)
* 3º Click OK
* 4º Search for your "txd.txt". It's located in "Android/data/com.rockstargames.gtasa/files/textdb/txd"
* If you get an error when trying to open the folder /data/, temporarily move the folder '/txd' from out of the '/data' using the ZArchive (or another file explorer app)
* 5º Click on + (in rose)
* 6º Click "From images"
* 7º Select "Uncompressed 32bpp"
* 8º Select the mod textures
* 9º Click on the "ok" symbol in the upper right corner
* 10º Select "Overwrite" > Click "OK"
* 11º IMPORTANT: Don't forget to close the CACHE: click on ":" "then Close cache"
* Vídeo (tutorial by: NEYgamer): https://youtu.be/85NgNUoSkEY?t=629