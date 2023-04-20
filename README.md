# VehicleSirenLights - Mobile

[English version](https://github.com/Danilo1301/GTASA_libGiroflexVSL/blob/main/README_EN.md)

<br>

## Necessário
* Apk com FLA 6.5 ou superior (com suporte a AML)

Libs:<br>

* CLEO - (libCLEOMod.so) (vem dentro do .zip)
* AML - (libAML.so) (vem dentro do .zip)
* SA Utils - (libSAUtils.so) (vem dentro do .zip)

( vem dentro do .zip, em 'libs necessárias', basta apenas jogar os arquivos .so dentro da sua pasta /mods )<br>
( exemplo: para /Android/data/ro.samp_mobile.game/mods )<br>

## Como instalar
Vídeo: [Como instalar](https://www.youtube.com/watch?v=QWzTS0r7288)

1. Baixe o .zip em [Releases](https://github.com/Danilo1301/GTASA_libGiroflex/releases)
2. Arraste o que estiver dentro da "/dentro da pasta do jogo" para a pasta do jogo
3. Jogue os arquivos .csa e .fxt em "/dentro da CLEO" para sua pasta CLEO

## Como usar
* Pressione os números imaginários 5 e 6 ([Exemplo](https://imgur.com/gA8Lfvt)) por 1s para abrir o menu e 0.5s para ligar a sirene
* Painel: Pressione os números imaginários 5 e 9 ([Exemplo](https://imgur.com/LLddMfg)) por 0.5s

## Config
Cada veículo tem um arquivo .ini separado para salvar as configurações<br>
Os arquivos ficam em: PASTA_DO_JOGO/configs/giroflex

## Crash
Caso haja algum crash, há um log que é gerado na sua pasta /configs/giroflex<br>
( exemplo: em /Android/data/ro.samp_mobile.game/configs/giroflex/giroflex.log )<br>

Se tiver algum problema você pode criar uma [Issue aqui](https://github.com/Danilo1301/GTASA_libGiroflex/issues) e me mandar o .log, ou deixar um comentário no yt

## Problemas

### Mods cleo sumiram do menu

Bastar ir no jogo, em Opções > Game e mudar a opção do CLEO LOCATION<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi
