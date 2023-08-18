# Giroflex VSL (VehicleSirenLights)

[English version](https://github.com/Danilo1301/GTASA_libGiroflexVSL/blob/main/README_EN.md)

<br>

## Instalação
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
 
## Como usar
* Pressione os números imaginários 5 e 6 ([exemplo](https://imgur.com/gA8Lfvt)) por 1s para abrir o menu e 0.5s para ligar a sirene
* Painel: Pressione os números imaginários 5 e 9 ([exemplo](https://imgur.com/LLddMfg)) por 0.5s

## Config
Cada veículo tem um arquivo .ini separado para salvar as configurações<br>
Os arquivos ficam em: PASTA_DO_JOGO/configs/giroflex

## Problemas

### Crash com a libBASSMod.so

Alguns usuários relataram crashes com a nova libBASSMod (que serve para tocar áudio). Se o seu jogo estiver crashando, experimente remover a 'libBASSMod.so' da pasta /mods

### Mods cleo sumiram do menu

Bastar ir no jogo, em "Opções" > "Game" e mudar a opção do "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi

### Menu muito pra esquerda

Para mudar a posição do menu você precisa ativar o modo edição em: "Opções" > "Mods Settings" > "Giroflex VSL - Edit mode"

## Como colocar as texturas
* 1º Abra o TXD Tool
* 2º Clique no símbolo de uma pasta em rosa
* 3º Aperte OK
* 4º Procure pelo "txd.txt". Ele está localizado na pasta "Android/data/com.rockstargames.gtasa/files/textdb/txd"
* Caso dê erro ao tentar abrir a pasta /data/, mova temporariamente a pasta '/txd' para fora da pasta '/data' usando o ZArchive (ou algum outro gerenciador de arquivos)
* 5º Clique no + em rosa
* 6º CLique em "From images"
* 7º Clique em "Uncompressed 32bpp"
* 8º Selecione as texturas do mod
* 9º Clique no símbolo de "ok" no canto superior direito
* 10º Selecione a opção "Overwrite" > Aperte "OK"
* 11º IMPORTANTE: Não se esqueca de fechar o CACHE: vá nas "três setinhas" e selecione Close
* Vídeo (tutorial por: NEYgamer): https://youtu.be/85NgNUoSkEY?t=629