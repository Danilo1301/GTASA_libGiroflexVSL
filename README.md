# Giroflex VSL (VehicleSirenLights)

[English version](https://github.com/Danilo1301/GTASA_libGiroflexVSL/blob/main/README_EN.md)

<br>

<h1>APKs testados</h1>

* GTASA 2.00 AML 1.2.1 FLA 7.2
* SAMP Mobile Alyn v2
* SAMP Mobile Alyn v11.6

<h1>Instalação</h1>

* Libs necessárias:
  * AML (https://github.com/RusJJ/AndroidModLoader)
  * SAUtils (https://github.com/AndroidModLoader/SAUtils)
  * CLEO Mod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  * BASS Mod (https://github.com/AndroidModLoader/BASSMod)

( as libs já vem dentro do .zip, em 'libs necessárias', basta apenas jogar os arquivos .so dentro da sua pasta /mods )<br>
( exemplo: para /Android/data/ro.samp_mobile.game/mods )<br>

1. Baixe o .zip em [Releases](https://github.com/Danilo1301/GTASA_libGiroflex/releases)
2. Abra a pasta "/dentro da CLEO", copie os arquivos 'giroflexVSL.csa' e 'giroflexVSL.fxt' e cole na pasta onde você carrega seus mods cleo (csa, csi, fxt)
3. Abra a pasta "/dentro da pasta do jogo", copie as pastas "mods" e "configs" e cole na pasta raiz do jogo (ex: em /Android/data/ro.samp_mobile.game)
4. Caso ainda não tenha feito, coloque as libs da "/libs necessárias" dentro da pasta '/mods'
5. Opcional: Coloque as texturas do giroflex no seu 'txd.txt' usando o 'TXD Tool'. Tutoriais abaixo.

<h1>Como usar</h1>

* Pressione os números imaginários 5 e 6 ([exemplo](https://imgur.com/gA8Lfvt)) por 0.5s para ligar a sirene
* Pressione os números imaginários 5 e 6 ([exemplo](https://imgur.com/gA8Lfvt)) por 1s para abrir o menu de edição
* Painel: Pressione os números imaginários 5 e 9 ([exemplo](https://imgur.com/LLddMfg)) por 0.5s

<h1>Tutoriais</h1>

* Instalando o mod giroflex: https://youtu.be/4mq7lfXUUfY
* Instalando GTA SA 2.00 com data original: https://youtu.be/wT4N_NAYFOI
* Instalando as texturas do painel de áudio: https://youtu.be/ghzIKrxtvx4
* Instalando as texturas das luzes retangulares: https://youtu.be/A9JWQilIvYI
* Como criar um padrão de piscar: https://youtu.be/KZkaiiXhtLU
* Como converter arquivos .INI para .JSON: https://youtu.be/p_R810KUDb4

<h1>Config</h1>

Cada veículo tem um arquivo .ini separado para salvar as configurações<br>
Os arquivos ficam em: PASTA_DO_JOGO/configs/giroflex

<h1>Problemas</h1>

<h3>Crash com a libBASSMod.so</h3>

Alguns usuários relataram crashes com a nova libBASSMod (que serve para tocar áudio). Se o seu jogo estiver crashando, experimente remover a 'libBASSMod.so' da pasta /mods

<h3>Mensagem 'Remova o antigo giroflex.csa e .fxt'</h3>

Se aparece essa mensagem isso significa que há um arquivo chamado 'giroflex.csa' em alguma das duas pastas:

* na pasta CLEO, em: /storage/emulated/0/cleo/sa/giroflex.csa
* ou na pasta do jogo, em: data/com.rockstargames.gtasa/giroflex.csa

Você precisa apenas deletar esse arquivo (giroflex.csa) e o giroflex.fxt

<h3>Mods cleo sumiram do menu</h3>

Bastar ir no jogo, em "Opções" > "Game" e mudar a opção do "CLEO LOCATION"<br>

* OLD CLEO - com.rockstargames.gtasa/ mod.csi
* OLD CLEO (+CLEO) - com.rockstargames.gtasa/cleo/ mod.csi
* ../FILES/CLEO - com.rockstargames.gtasa/files/cleo/ mod.csi
* CLEO 2.0.1 - storage/emulated/0/cleo/sa/ mod.csi

(Não se esqueça de reiniciar o jogo para aplicar as modificações)

<h3>Menu muito pra esquerda</h3>

Para mudar a posição do menu você precisa ativar o modo edição em: "Opções" > "Mods Settings" > "Giroflex VSL - Edit mode"

<h3>Discord</h3>

* https://discord.gg/mkCDRf4zJA

<h1>Lib versions</h1>

* SAUtils: 1.6
* SAUtils for SAMP: 1.3.1
* AML: 1.0.3.1
* CLEO Mod: 2.0.1.3
* BASS Mod: 1.0.0.0

<h1>Credits</h1>

* Mod made using AML (https://github.com/RusJJ/AndroidModLoader)

* Project base used from CLEOMod (https://github.com/AndroidModLoader/GTA_CLEOMod)
  
* Siren icon from: https://www.vecteezy.com/vector-art/5728560-alarm-light-siren-solid-icon-vector-illustration-logo-template-suitable-for-many-purposes

* Config icon from: https://www.flaticon.com/free-icon/config_8053573