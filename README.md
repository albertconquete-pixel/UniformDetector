
# intrusion detector esp

Un firmware pour dispositif de dectection d'intrusion: envoie des notification a via une appli lorsque l'ia intégré détecte un intrus.


## Authors

- [@albertconquete-pixel(devmael)](https://github.com/albertconquete-pixel)


## Documentation

Concernant la documentation technique du code, vous pouvez le trouvez dans [Documentation](https://linktodocumentation)

concernant le projets, il s'agit du firmware d'un dispositif electronique dont le but est de détecter et signaler des intrusions. Ce dispositif est assimiler a une application encore en cour.

## version 

version 0.0.1 alpha encore instable

la fonctionalité de communication réseau avec l'appli est indisponible



## Setup and Prerequisites

Pour lancer ce projet, vous devriez au préalable ajouter 
`esp idf >= 5.5 `

* **Carte cible :** Seeed Studio **XIAO ESP32S3 Sense cam**


## Run Locally
Apres avoir cloner :

definir le type de carte 

```bash
  idf.py set-target esp32s3
```

compiler le projet

```bash
  idf.py build
```
les dependances du fichier .yml se telechargeront, soyez connecté.


flasher sur une vrai carte xiao s3 cam
```bash
  idf.py flash
```

