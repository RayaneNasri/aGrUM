
	LICENCE :
	
SkoobEditor utilise la licence GPL.
Lisez le fichier LICENCE.txt pour plus de renseignement, ou visitez le site
http://www.gnu.org/licenses/gpl.html

	DEPENDANCES :

SkoobEditor d�pend de 
 - Qt 4.7+
 - QScintilla 2.4.6+
 - aGrUM 0.8+

Pour Windows, la dll d'aGrUM est fournie, mais Qt et QScintilla (pour Qt4) doivent pr�alablement �tre install�s.

Pour Unix, Qt, QScintilla et aGrUM doivent pr�alablement �tre install�s.

Qt et QScintilla doivent se trouver dans les d�p�ts de votre distribution.
Sinon, vous pouvez les r�cup�rer sur
http://qt.nokia.com/downloads/
http://www.riverbankcomputing.co.uk/software/qscintilla/ 

Le dep�t SVN de aGrUM est disponible � l'adresse suivante :
https://alm.lip6.fr/svn/agrum/trunk
Vous trouverez les instructions d'installation dans la documentation du projet :
http://agrum.lip6.fr
 
	INSTRUCTIONS :

1) Entrer la commande (*) : make
L'ex�cutable sera dans le dossier bin.

(*) sous windows, make peut soit �tre nmake ou mingw32-make en fonction de la version de Qt install�e.
V�rifiez que la variable d'environnement PATH contient bien %QTDIR%\qt\bin; avec %QTDIR% le 
r�pertoire ou vous avez install� Qt, pour que l'ex�cutable trouve les dll n�cessaires.

	INSTALLATEUR 1 :
	
Sous windows, il est possible de cr�er un installateur pour installer l'ex�cutable et ses d�pendances correctement
dans le dossier Program Files, de m�me que de cr�er les raccourcis dans le menu d�marrer.

Pour cela, NSIS doit �tre install� (dans le r�pertoire par d�faut, � savoir C:\Program Files\NSIS).

Entrer la commande (*) : make installer.
L'installateur sera dans le dossier bin.

	INSTALLATEUR 2 :
  
Sur les syst�mes Linux/Debian ou Linux/Ubuntu (ou tout autre syst�me bas� les paquets deb),
il est possible de cr�er un paquet pour installer facilement SkoobEditor.

Entrer la commande : make dpkg
Le paquet sera dans le dossier apps.

NB : Qt, aGrUM et QScintilla doivent quand m�me �tre pr�alablement install�s.
	
	BUGS / AMELIORATIONS :
	
Si vous trouvez des bugs ou souhaitez des am�liorations ou de nouvelles fonctionnalit�s,
consultez le fichier BUGS.txt.
