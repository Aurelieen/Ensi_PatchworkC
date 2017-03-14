# TPL Patchwork
Ensimag 1A – Construction de motifs par des arbres syntaxiques abstraits en C99.

## Installation
```sh
# Générer les exécutables
make

# Ne garder que les codes sources
make clean
```

## Usage
```sh
# Lancer l'aide du programme
./testpatch --help
./testpatch --usage

# Exemples d'utilisation
./testpatch
./testpatch -o mon_patchwork.ppm
./testpatch -f entree
./testpatch -s 64

# Générer depuis le fichier "entree" vers le résultat "mon_patchwork.ppm" avec des primitifs de taille 15
./testpatch -f entree -o mon_patchwork.ppm -s 15
```

---
Aurélien PEPIN
