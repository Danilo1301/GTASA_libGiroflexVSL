# Giroflex VSL (VehicleSirenLights)

[English version](https://github.com/Danilo1301/GTASA_libGiroflexVSL/blob/main/README_EN.md)

<br>

## Instalação
* Apk necessário: GTA SA com FLA 6.5 ou superior (com suporte a AML)
* Libs necessárias:
  * AML (https://github.com/RusJJ/AndroidModLoader)
  * SAUtils (https://github.com/AndroidModLoader/SAUtils)
  * CLEO Mod (https://github.com/AndroidModLoader/GTA_CLEOMod)

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

### Mods cleo sumiram do menu

Bastar ir no jogo, em Opções > Game e mudar a opção do CLEO LOCATION<br>

* OLD CLEO - com.rockstargames.gtasa / mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa / cleo / mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa / files  / cleo / mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo / mod.csi

### Menu muito pra esquerda

Para mudar a posição do menu você precisa ativar o modo edição em: Options > Mods Settings > "Giroflex VSL - Edit mode"
