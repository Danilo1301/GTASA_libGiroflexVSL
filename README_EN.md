# Giroflex VSL (VehicleSirenLights)

<h1>Install</h1>

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
5. Optional: Coloque as texturas do giroflex no seu 'txd.txt' usando o 'TXD Tool'. [Tutorial](#how-to-add-textures)

<h1>How to use</h1>

* Press the 'imaginary buttons' 5 and 6 ([example](https://imgur.com/gA8Lfvt)) for 0.5s to turn lights on/off, and for 1s to open menu
* Panel: press the 'imaginary buttons' 5 and 9 ([example](https://imgur.com/LLddMfg)) for 0.5s

<h1>Tutorials</h1>

* Installing GTA SA 2.00 with original data: https://youtu.be/wT4N_NAYFOI
* Installing the giroflex mod: https://youtu.be/7Hqv3ZqYmv8
* Installing the textures of the audio panel: https://youtu.be/ghzIKrxtvx4
* Installing the retangular textures of the giroflex: https://youtu.be/A9JWQilIvYI

<h1>Config</h1>

Each vehicle has a .ini file where it stores its config<br>
Those files are located at: GAME_FOLDER/configs/giroflex

<h1>Issues</h1>

<h3>Crash with the libBASSMod.so</h3>

Some users have reported crashes with the new libBASSMod (which is for playing audio). If your game is crashing, try removing 'libBASSMod.so' from /mods folder

<h3>Mods cleo not showing on menu</h3>

You just need to go in the game, then "Options" > "Game" and change the "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi

(Don't forget to restart the game to apply the changes)

<h3>Menu too much to the left</h3>

To change the menu's position, you need to enable the edit mode in: "Options" > "Mods Settings" > "Giroflex VSL - Edit mode"

<h3>Discord</h3>

* https://discord.gg/mkCDRf4zJA

<h1 id="how-to-add-textures">How to add the textures</h1>

1. Open TXD Tool
2. Click on the folder button (in rose)
3. Click OK
4. Search for your "txd.txt". It's located in "Android/data/com.rockstargames.gtasa/files/textdb/txd"
* If you get an error when trying to open the folder /data/, temporarily move the folder '/txd' from out of the '/data' using the ZArchive (or another file explorer app)
5. Click on + (in rose)
6. Click "From images"
7. Select "Uncompressed 32bpp"
8. Select the mod textures
9. Click on the "ok" symbol in the upper right corner
10. Select "Overwrite" > Click "OK"
11. IMPORTANT: Don't forget to close the CACHE: click on ":" then "Close cache"
* Vídeo (tutorial by: NEYgamer): https://youtu.be/85NgNUoSkEY?t=629

<h1>Lib versions</h1>

* SAUtils: 1.6
* SAUtils for SAMP: 1.3.1
* AML: 1.0.3.1
* CLEO Mod: 2.0.1.3
* BASS Mod: 1.0.0.0