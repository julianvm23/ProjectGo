Pushen en pullen naar git:  
=======

De eerste keer:  
git clone LINK_NAAR_REPO  

Pullen van git:  
(Doe dit aan het begin van elke sessie)  
git pull  

Pushen naar git:  
   git add .  
   git commit -m "tekst"  
   git push  

Git Merge conflict(Als je in een conflict zit en er niet uit kan komen gebruik volgend command)  
git reset --hard HEAD

Als je elke keer moet inloggen kan je gebruik maken van een SSH-key  
Je kopieert hem via  
clip < ~/.ssh/id_rsa.pub  
Voeg de gekopieerde key toe aan:  
https://github.com/settings/keys  