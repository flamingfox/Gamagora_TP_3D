Pour windows sous codeblocks :

Le cbr est pr�t ne pas le modifier.

Utilisation de Eigen, mais pas de soucis le cbr est configurer pour fonctionner avec la lib.
Il faut installer la lib sous windows, je vais expliquer comment l'installer.

1) T�l�charger la lib sur le site en version 3.2.4 en .zip " http://eigen.tuxfamily.org/index.php?title=Main_Page "
2) d�compresser le zip.
3) Dans le dossier d�compresser, prendre le dossier "Eigen" et le d�pos� dans la racine c:/.
4) C'est pr�t !

Le projet n'utilise pas opengl, du coup il faut passer par l'export .obj et un lecteur pour voir le r�sultat final.

Pour le lecteur sous windows il y a le lecteur GLC_Player. " http://www.glc-player.net/fr/download.php ".

Quand executer, File -> Open Models from path -> s�lectionner le dossier o� son stocker les .obj
(par d�faut les .obj sont dans le dossier d'�x�cution du programme, pour windows dans le dossier des .cpp et .h)

