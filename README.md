# Giroflex VSL (VehicleSirenLights)

[English version](https://github.com/Danilo1301/GTASA_libGiroflexVSL/blob/main/README_EN.md)

<br>

<h1>Instalação</h1>

* Apk necessário: GTA SA 2.00 com FLA 6.5 ou superior (com suporte a AML)
* Libs necessárias:
  * AML (https://github.com/RusJJ/AndroidModLoader)
  * SAUtils (https://github.com/AndroidModLoader/SAUtils)
  * CLEO Mod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  * BASS Mod (https://github.com/AndroidModLoader/BASSMod)

( as libs já vem dentro do .zip, em 'libs necessárias', basta apenas jogar os arquivos .so dentro da sua pasta /mods )<br>
( exemplo: para /Android/data/ro.samp_mobile.game/mods )<br>

1. Baixe o .zip em [Releases](https://github.com/Danilo1301/GTASA_libGiroflex/releases)
2. Arraste o arquivo 'libGiroflex.so' da "/dentro da pasta do jogo" para sua pasta '/mods'
3. Jogue os arquivos 'giroflex.csa' e 'giroflex.fxt' da "/dentro da CLEO" para onde você carrega seus mods cleo (csa, csi, fxt)
4. Caso ainda não tenha feito, coloque as libs da "/libs necessárias" dentro da pasta '/mods'
5. Opcional: Coloque as texturas do giroflex no seu 'txd.txt' usando o 'TXD Tool'. [Tutorial](#how-to-add-textures)

<h1>Como usar</h1>

* Pressione os números imaginários 5 e 6 ([exemplo](https://imgur.com/gA8Lfvt)) por 1s para abrir o menu e 0.5s para ligar a sirene
* Painel: Pressione os números imaginários 5 e 9 ([exemplo](https://imgur.com/LLddMfg)) por 0.5s

<h1>Config</h1>

Cada veículo tem um arquivo .ini separado para salvar as configurações<br>
Os arquivos ficam em: PASTA_DO_JOGO/configs/giroflex

<h1>Problemas</h1>

<h3>Discord</h3>

* https://discord.gg/mkCDRf4zJA

<h3>Crash com a libBASSMod.so</h3>

Alguns usuários relataram crashes com a nova libBASSMod (que serve para tocar áudio). Se o seu jogo estiver crashando, experimente remover a 'libBASSMod.so' da pasta /mods

<h3>Mods cleo sumiram do menu</h3>

Bastar ir no jogo, em "Opções" > "Game" e mudar a opção do "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi

<h3>Menu muito pra esquerda</h3>

Para mudar a posição do menu você precisa ativar o modo edição em: "Opções" > "Mods Settings" > "Giroflex VSL - Edit mode"

<h1 id="how-to-add-textures">Como colocar as texturas</h1>

1. Abra o TXD Tool
2. Clique no símbolo de uma pasta em rosa
3. Aperte OK
4. Procure pelo "txd.txt". Ele está localizado na pasta "Android/data/com.rockstargames.gtasa/files/textdb/txd"
* Caso dê erro ao tentar abrir a pasta /data/, mova temporariamente a pasta '/txd' para fora da pasta '/data' usando o ZArchive (ou algum outro gerenciador de arquivos)
5. Clique no + em rosa
6. Clique em "From images"
7. Clique em "Uncompressed 32bpp"
8. Selecione as texturas do mod
9. Clique no símbolo de "ok" no canto superior direito
10. Selecione a opção "Overwrite" > Aperte "OK"
11. IMPORTANTE: Não se esqueca de fechar o CACHE: vá nos "tres pontinhos" e selecione 'Close'
* Vídeo (tutorial por: NEYgamer): https://youtu.be/85NgNUoSkEY?t=629