# Giroflex VSL (VehicleSirenLights)

<h1>Tested APKs</h1>

* GTASA 2.00 AML 1.2.1 FLA 7.2
* SAMP Mobile Alyn v2
  * NOTE: you must use SAUtils 1.3.1
* SAMP Mobile Alyn v11.6
  * NOTE: you must use SAUtils 1.3.1
* SAMP Mobile Alyn v13.1
  * NOTE: it's not compatible with SAUtils (only version 1.1)

<h1>Install</h1>

* Required Libs:
  * AML (https://github.com/RusJJ/AndroidModLoader)
  * SAUtils (https://github.com/AndroidModLoader/SAUtils)
  * CLEO Mod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  * BASS Mod (https://github.com/AndroidModLoader/BASSMod)

( those libs are all inside the .zip file, in '/required libs', you just have to copy the .so files inside your /mods folder )<br>
( example: inside /Android/data/ro.samp_mobile.game/mods )<br>

1. Download the .zip file in [Releases](https://github.com/Danilo1301/GTASA_libGiroflex/releases)
2. Open the folder "/inside CLEO", copy the 'giroflex.csa' and 'giroflex.fxt' files and paste it where you load your cleo mods (csa, csi, fxt)
3. Open the folder "inside game folder", copy the folders "configs" and "mods" and paste it on the game folder (ex: /Android/data/ro.samp_mobile.game)
4. If you haven't done yet, move the libs from "/required libs" inside your '/mods' folder
5. Optional: Install the giroflex mod textures inside the 'txd.txt' using 'TXD Tool'. Tutorials below.

<h1>How to use</h1>

* Press the 'imaginary buttons' 5 and 6 ([example](https://imgur.com/gA8Lfvt)) for 0.5s to turn lights on/off
* Press the 'imaginary buttons' 5 and 6 ([example](https://imgur.com/gA8Lfvt)) for 1s to open the editing menu
* Panel: press the 'imaginary buttons' 5 and 9 ([example](https://imgur.com/LLddMfg)) for 0.5s

<h1>Tutorials</h1>

* Installing the giroflex mod: https://youtu.be/4mq7lfXUUfY
* Installing GTA SA 2.00 with original data: https://youtu.be/wT4N_NAYFOI
* Installing the textures of the audio panel: https://youtu.be/ghzIKrxtvx4
* Installing the retangular lights textures: https://youtu.be/A9JWQilIvYI
* How to create a flash pattern: https://youtu.be/KZkaiiXhtLU
* How to convert .INI files to .JSON: https://youtu.be/p_R810KUDb4

<h1>Config</h1>

Each vehicle has a .ini file where it stores its config<br>
Those files are located at: GAME_FOLDER/configs/giroflex

<h1>Issues</h1>

<h3>Crash with the libBASSMod.so</h3>

Some users have reported crashes with the new libBASSMod (which is for playing audio). If your game is crashing, try removing 'libBASSMod.so' from /mods folder

<h3>Message 'Remove the old giroflex.csa and .fxt'</h3>

If you see this message it means that there is a file called 'giroflex.csa' is one of the following folders:

* in the CLEO folder, in: /storage/emulated/0/cleo/sa/giroflex.csa
* or in the game folder, in: data/com.rockstargames.gtasa/giroflex.csa

You only need to delete this file (giroflex.csa) and the giroflex.fxt

<h3>Mods cleo not showing on menu</h3>

You just need to go in the game, then "Options" > "Game" and change the "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa/ mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa/cleo/ mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa/files/cleo/ mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo/sa/ mod.csi

(Don't forget to restart the game to apply the changes)

<h3>Menu too much to the left</h3>

To change the menu's position, you need to enable the edit mode in: "Options" > "Mods Settings" > "Giroflex VSL - Edit mode"

<h3>Bent lights / Lights rotated to the side by a little</h3>

This happens when you use libJPatch.so with 'PS2CoronaRotation' enabled. To disable you have to go to your configs, in the .ini file, go the where this option is located and change it to 0 (PS2CoronaRotation=0)

<h1>Discord</h1>

* https://discord.gg/mkCDRf4zJA

<h1>Lib versions</h1>

For all versions:
* BASS Mod: 1.0.0.0
* CLEO Mod: 2.0.1.3
* AML: 1.2.1
* AML Headers: 1.1

For SAMP:
* SAUtils: 1.3.1

For GTA SA 2.00
* SAUtils: 1.6

For GTA SA 2.10
* Not tested yet

<h1>Credits</h1>

* Mod made using AML (https://github.com/RusJJ/AndroidModLoader)

* Project base used from CLEOMod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  
* Siren icon from: https://www.vecteezy.com/vector-art/5728560-alarm-light-siren-solid-icon-vector-illustration-logo-template-suitable-for-many-purposes

* Config icon from: https://www.flaticon.com/free-icon/config_8053573