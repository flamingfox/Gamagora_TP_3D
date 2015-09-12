Pour windows sous codeblocks :

Le cbr est prêt ne pas le modifier.

Utilisation de Eigen, mais pas de soucis le cbr est configurer pour fonctionner avec la lib.
Il faut installer la lib sous windows, je vais expliquer comment l'installer.

1) Télécharger la lib sur le site en version 3.2.4 en .zip " http://eigen.tuxfamily.org/index.php?title=Main_Page "
2) décompresser le zip.
3) Dans le dossier décompresser, prendre le dossier "Eigen" et le déposé dans la racine c:/.
4) C'est prêt !

Le projet n'utilise pas opengl, du coup il faut passer par l'export .obj et un lecteur pour voir le résultat final.

Pour le lecteur sous windows il y a le lecteur GLC_Player. " http://www.glc-player.net/fr/download.php ".

Quand executer, File -> Open Models from path -> sélectionner le dossier où son stocker les .obj
(par défaut les .obj sont dans le dossier d'éxécution du programme, pour windows dans le dossier des .cpp et .h)

