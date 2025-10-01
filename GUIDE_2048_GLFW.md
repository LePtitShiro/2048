# Guide complet: réaliser un jeu 2048 avec interface graphique (GLFW)

Ce guide, en français, vous accompagne pas à pas pour concevoir un jeu 2048 en C++ avec une interface graphique basée sur GLFW et OpenGL. Il couvre la mise en place de l’environnement, l’architecture du code, la logique du jeu, le rendu, les entrées clavier, l’animation, la persistance du score, ainsi que des conseils de débogage et de portabilité.

Sommaire
- Objectif et aperçu
- Prérequis
- Mise en place du projet CMake
  - Intégrer GLFW (vcpkg, Conan, ou build manuel)
  - Dépendances utilitaires (stb_easy_font pour du texte)
- Architecture recommandée
- Logique du 2048 (déplacement/fusion)
- Boucle de jeu et rendu
- Entrées clavier et état du jeu
- Animations (apparition/fusion, interpolation)
- Score et persistances (fichier best score)
- Tests manuels et vérifications
- Performance et qualité
- Portabilité (Windows/Linux/macOS)
- Dépannage
- Ressources


## 1) Objectif et aperçu

2048 est un puzzle sur une grille 4x4. À chaque coup, toutes les tuiles glissent dans une direction. Les tuiles de même valeur qui se rencontrent fusionnent et doublent. À chaque tour, une nouvelle tuile (2 ou 4) apparaît. Le jeu se termine quand aucun mouvement n’est possible.

Nous allons :
- Implémenter la logique du plateau et des mouvements.
- Afficher les tuiles avec GLFW + OpenGL (rendu 2D simple).
- Gérer le clavier (flèches) et le score.
- Ajouter des animations légères.


## 2) Prérequis

- Outils
  - Un compilateur C++17/20 (MSVC, Clang, GCC)
  - CMake (≥ 3.20 recommandé)
  - Git (facultatif mais conseillé)
- Bibliothèques
  - GLFW (fenêtre + entrées)
  - OpenGL (fourni par votre pilote, utilisé via GLFW)
  - Optionnel: GLEW/GLAD (si vous souhaitez charger des fonctions modernes d’OpenGL; pour du 2D basique, vous pouvez rester sur des appels core simples). 
  - stb_easy_font (fichier d’en-tête unique) pour du texte simple sans pipeline complexe.

Sous Windows, vcpkg ou Conan simplifie l’intégration de GLFW.


## 3) Mise en place du projet CMake

Structure minimale conseillée:

- CMakeLists.txt
- src/
  - main.cpp
  - App.cpp/.hpp
  - Game.cpp/.hpp
  - Board.cpp/.hpp
  - Renderer.cpp/.hpp
  - Input.cpp/.hpp
- extern/
  - stb_easy_font.h

Vous pouvez commencer minimal et itérer.

### 3.1 Intégrer GLFW

Option A — vcpkg (Windows/others)
1. Installer vcpkg: https://vcpkg.io
2. Intégrer vcpkg à CMake (triplet x64-windows par ex.).
3. Installer GLFW: vcpkg install glfw3
4. CMakeLists.txt (exemple simplifié):

cmake_minimum_required(VERSION 3.20)
project(2048_glfw CXX)
set(CMAKE_CXX_STANDARD 20)

# vcpkg toolchain si nécessaire: -DCMAKE_TOOLCHAIN_FILE=... lors de l’invocation de CMake
find_package(glfw3 CONFIG REQUIRED)

add_executable(2048_glfw
  src/main.cpp
  src/App.cpp src/App.hpp
  src/Game.cpp src/Game.hpp
  src/Board.cpp src/Board.hpp
  src/Renderer.cpp src/Renderer.hpp
  src/Input.cpp src/Input.hpp
)

target_link_libraries(2048_glfw PRIVATE glfw)

# Sur certaines plateformes, il faut aussi lier OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(2048_glfw PRIVATE OpenGL::GL)

Option B — Conan
- profile conan, conanfile.txt:

[requires]
glfw/3.4

[generators]
CMakeDeps
CMakeToolchain

- Puis find_package(glfw3 CONFIG REQUIRED) via les fichiers générés.

Option C — Build manuel
- Cloner GLFW (https://github.com/glfw/glfw), générer la solution avec CMake, compiler, puis lier vos includes/libs dans CMakeLists.

### 3.2 Texte: stb_easy_font

Téléchargez stb_easy_font.h (https://github.com/nothings/stb/blob/master/stb_easy_font.h) dans extern/. Ajoutez le répertoire aux include dirs:

target_include_directories(2048_glfw PRIVATE extern)

stb_easy_font permet d’afficher du texte bitmap sans shader complexe (parfait pour score, titres, etc.).


## 4) Architecture recommandée

- Board: contient l’état de la grille, la génération de nouvelles tuiles, la détection de mouvements possibles, l’application des déplacements/fusions et le calcul de score des fusions.
- Game: orchestre les tours, gère le score total, l’état (en cours, gagné, perdu), déclenche l’apparition de nouvelles tuiles après un mouvement valide.
- Renderer: convertit l’état du Board en primitives 2D (rectangles colorés) et texte (valeurs des tuiles, score). 
- Input: mappe les touches (flèches/ZQSD) vers des actions (MoveUp/Down/Left/Right, Reset, Quit).
- App: boucle principale GLFW, minuterie, animations, délègue à Game/Renderer.


## 5) Logique du 2048 (déplacement/fusion)

Représenter la grille comme un tableau 4x4 d’entiers (0 = vide, puissances de 2 pour les tuiles). Exemple de déplacement vers la gauche:

- Pour chaque ligne:
  1) Compacter: déplacer toutes les tuiles non nulles vers la gauche sans fusionner.
  2) Fusionner: si tile[i] == tile[i+1] et non fusionnée ce tour, faire tile[i] *= 2, tile[i+1] = 0, ajouter au score.
  3) Re-compacter pour éliminer les trous laissés par les fusions.

Pseudo-code (une ligne):

function slide_and_merge_left(row):
  compact = [x for x in row if x != 0]
  result = []
  i = 0
  score_gain = 0
  while i < len(compact):
    if i+1 < len(compact) and compact[i] == compact[i+1]:
      merged = compact[i]*2
      result.append(merged)
      score_gain += merged
      i += 2
    else:
      result.append(compact[i])
      i += 1
  while len(result) < 4: result.append(0)
  return result, score_gain

Les déplacements vers droite/haut/bas se ramènent à gauche via inversion/transposition.

Après un mouvement valide (l’état a changé), ajouter une tuile aléatoire (90% 2, 10% 4) dans une case vide aléatoire. Fin de partie si aucun mouvement n’est possible dans les 4 directions.


## 6) Boucle de jeu et rendu

Boucle GLFW classique:

- Init GLFW, créer une fenêtre (ex: 600x700), contexte OpenGL courant.
- Configurer le viewport et une projection orthographique 2D (ex: 0..width, 0..height).
- Tant que la fenêtre n’est pas fermée:
  - Poll événements (glfwPollEvents)
  - Mettre à jour l’animation (deltaTime)
  - Effacer l’écran
  - Dessiner le plateau et l’UI
  - Échanger les buffers (glfwSwapBuffers)

Pour le rendu 2D simple, vous pouvez dessiner des rectangles avec glBegin/glEnd (compatibilité) ou configurer un VBO + shader basique. Pour aller vite, sur OpenGL 2.x/compat vous pouvez faire:

- glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, W, H, 0, -1, 1);
- glMatrixMode(GL_MODELVIEW); glLoadIdentity();
- glBegin(GL_QUADS); ... glEnd();

Note: Pour un projet moderne, préférez un pipeline shader. Ce guide se concentre sur la simplicité pédagogique.


## 7) Entrées clavier et état du jeu

- glfwSetKeyCallback(window, key_callback)
- Dans le callback, traduire GLFW_KEY_UP/DOWN/LEFT/RIGHT en commandes.
- Empêcher les répétitions rapides non désirées: 
  - soit traiter les événements au relâchement, 
  - soit bloquer l’entrée durant une animation (200 ms).

États possibles: Playing, Won (si 2048 atteint), GameOver. 
- En Won, on peut autoriser de continuer (optionnel).
- En GameOver, afficher un overlay et proposer R pour reset.


## 8) Animations (apparition/fusion, interpolation)

- Apparition: nouvelle tuile scale de 0.0 à 1.0 en ~150 ms.
- Fusion: tuile résultante scale 1.0 -> 1.1 -> 1.0 (rebond léger) en ~120 ms.
- Glissement: interpoler la position de chaque tuile entre sa case source et destination pendant ~120–180 ms.

Technique:
- Conserver, pour chaque tuile animée, sa position logique (cell) et sa position/échelle visuelle (float) + temps de début + durée.
- À chaque frame, calculer t = clamp((now - t0)/durée, 0..1) et interpoler (lerp) pour position/scale.


## 9) Score et persistance

- Maintenir score courant (somme des fusions) dans Game.
- Best score: charger/sauver un fichier texte (ex: best_score.txt) dans le répertoire de travail.
- Sauvegarder au moment opportun (nouveau record) et à la fermeture.


## 10) Tests manuels et vérifications

- Vérifier qu’un mouvement qui n’entraîne aucun changement n’ajoute pas de tuile et ne compte pas pour un tour.
- Tester les 4 directions sur des configurations simples (ex: [2,2,0,0] -> [4,0,0,0]).
- Vérifier la double fusion dans le bon ordre (ex: [2,2,2,0] -> gauche -> [4,2,0,0], pas [2,4,0,0]).
- Tester la détection de fin de partie.


## 11) Performance et qualité

- Éviter d’allouer à chaque frame; réutiliser des buffers.
- Déconnecter la logique de la fréquence de rendu (utiliser deltaTime fixé, ex: 60 FPS logique).
- Afficher les statistiques (ms/frame) en debug.


## 12) Portabilité

- Windows: vcpkg est pratique; veillez à lier OpenGL::GL si nécessaire.
- Linux: paquets GLFW via le gestionnaire de paquets, ou vcpkg/conan.
- macOS: GLFW supporte Cocoa; utilisez un contexte Core Profile; certaines fonctions legacy (glBegin) ne sont pas disponibles — privilégiez un pipeline moderne si vous ciblez macOS.


## 13) Dépannage

- Fenêtre noire: vérifier le contexte courant et le viewport; activer glClearColor.
- Texte invisible: vérifier la couleur, la projection orthographique et la profondeur (désactiver depth test si inutile).
- Crash au lancement: vérifier que GLFW s’initialise (glfwInit) et que la fenêtre est créée.
- Aucune entrée: assurer glfwPollEvents est appelé et le focus fenêtre.


## 14) Exemple d’implémentation (extraits)

Avertissement: extraits simplifiés et non exhaustifs pour démarrer.

// Board.hpp
#pragma once
#include <array>
#include <random>

struct MoveResult { bool moved{false}; int scoreGain{0}; };

class Board {
public:
  static constexpr int N = 4;
  using Grid = std::array<std::array<int, N>, N>;

  Board();
  void reset();
  const Grid& grid() const { return cells; }
  MoveResult moveLeft();
  MoveResult moveRight();
  MoveResult moveUp();
  MoveResult moveDown();
  bool canMoveAny() const;
  bool has2048() const;

private:
  Grid cells{};
  mutable std::mt19937 rng;
  void addRandomTile();
  static std::pair<std::array<int, N>, int> slideMergeLeft(const std::array<int, N>& row);
};

// Board.cpp (idées clés)
#include "Board.hpp"
#include <algorithm>
#include <chrono>

Board::Board() {
  rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  reset();
}

void Board::reset(){
  for (auto& r: cells) r.fill(0);
  addRandomTile();
  addRandomTile();
}

std::pair<std::array<int,4>,int> Board::slideMergeLeft(const std::array<int,4>& row){
  std::array<int,4> compact{0,0,0,0}; int k=0;
  for(int x: row) if(x) compact[k++]=x;
  std::array<int,4> result{0,0,0,0}; int i=0, j=0; int gain=0;
  while(i<4){
    if(i+1<4 && compact[i]==compact[i+1] && compact[i]!=0){
      result[j++]=compact[i]*2; gain+=compact[i]*2; i+=2;
    } else {
      result[j++]=compact[i]; i+=1;
    }
  }
  return {result, gain};
}

MoveResult Board::moveLeft(){
  MoveResult mr; Grid before = cells; int gain=0;
  for(int r=0;r<4;++r){
    auto [line, g] = slideMergeLeft(cells[r]);
    cells[r]=line; gain+=g;
  }
  mr.moved = (cells!=before); mr.scoreGain=gain;
  if(mr.moved) addRandomTile();
  return mr;
}

// Les autres directions: via inversion/transposition.


// Renderer.hpp (dessin simple)
#pragma once
#include "Board.hpp"
struct RendererConfig { int W=600, H=700; };
class Renderer{
public:
  explicit Renderer(RendererConfig cfg): cfg(cfg) {}
  void draw(const Board& board, int score, int bestScore);
private:
  RendererConfig cfg;
  void drawRect(float x,float y,float w,float h, float r,float g,float b);
  void drawText(float x,float y, const char* txt, float r, float g, float b, float scale=1.0f);
};

// Renderer.cpp (idées clés)
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

void Renderer::drawRect(float x,float y,float w,float h, float r,float g,float b){
  glColor3f(r,g,b);
  glBegin(GL_QUADS);
  glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h);
  glEnd();
}

void Renderer::drawText(float x,float y,const char* txt,float r,float g,float b,float scale){
  char buffer[99999];
  int quads = stb_easy_font_print(x, y, (char*)txt, nullptr, buffer, sizeof(buffer));
  glColor3f(r,g,b); glScalef(scale, scale, 1);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 16, buffer);
  glDrawArrays(GL_QUADS, 0, quads*4);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void Renderer::draw(const Board& board, int score, int best){
  int W=cfg.W, H=cfg.H; glViewport(0,0,W,H);
  glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, W, H, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  glClearColor(0.95f,0.93f,0.88f,1); glClear(GL_COLOR_BUFFER_BIT);

  // UI simple
  drawText(20, 30, "2048", 0.2f,0.2f,0.2f, 1.5f);
  char s[64]; snprintf(s,64,"Score: %d  Best: %d", score, best); drawText(20, 60, s, 0.1f,0.1f,0.1f, 1.0f);

  // Grille
  const int N=Board::N; float pad=12.f; float boardW = W-2*pad; float boardTop=100.f; float cellPad=8.f;
  float cellW = (boardW - (N+1)*cellPad)/N; float x0=pad+cellPad; float y0=boardTop+cellPad;

  // Fond
  drawRect(pad, boardTop, boardW, boardW, 0.77f,0.75f,0.71f);

  for(int r=0;r<N;++r){
    for(int c=0;c<N;++c){
      float x = x0 + c*(cellW+cellPad); float y = y0 + r*(cellW+cellPad);
      int v = board.grid()[r][c];
      float cr=0.8f,cg=0.75f,cb=0.7f;
      if(v==0){ cr=0.83f; cg=0.80f; cb=0.76f; }
      else if(v==2){ cr=0.93f; cg=0.90f; cb=0.85f; }
      else if(v==4){ cr=0.93f; cg=0.89f; cb=0.78f; }
      else if(v==8){ cr=0.95f; cg=0.73f; cb=0.47f; }
      else if(v==16){ cr=0.96f; cg=0.68f; cb=0.39f; }
      else if(v==32){ cr=0.96f; cg=0.59f; cb=0.39f; }
      else if(v==64){ cr=0.96f; cg=0.49f; cb=0.38f; }
      else if(v>=128){ cr=0.93f; cg=0.81f; cb=0.44f; }
      drawRect(x,y, cellW, cellW, cr,cg,cb);
      if(v){ char t[8]; snprintf(t,8, "%d", v); drawText(x+cellW*0.35f, y+cellW*0.55f, t, 0.2f,0.2f,0.2f, 1.2f); }
    }
  }
}

// main.cpp (squelette)
#include <GLFW/glfw3.h>
#include "Board.hpp"
#include "Renderer.hpp"

int main(){
  if(!glfwInit()) return -1;
  int W=600, H=700;
  GLFWwindow* win = glfwCreateWindow(W,H, "2048 (GLFW)", nullptr, nullptr);
  if(!win){ glfwTerminate(); return -1; }
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1); // VSync

  Board board; int score=0, best=0;
  Renderer renderer({W,H});

  auto onKey = [&](GLFWwindow*, int key, int sc, int action, int mods){
    if(action != GLFW_PRESS) return;
    MoveResult mr;
    if(key==GLFW_KEY_LEFT) mr = board.moveLeft();
    else if(key==GLFW_KEY_RIGHT) mr = board.moveRight();
    else if(key==GLFW_KEY_UP) mr = board.moveUp();
    else if(key==GLFW_KEY_DOWN) mr = board.moveDown();
    else if(key==GLFW_KEY_R){ board.reset(); score=0; return; }
    else if(key==GLFW_KEY_ESCAPE){ glfwSetWindowShouldClose(win, 1); return; }
    score += mr.scoreGain;
  };
  glfwSetWindowUserPointer(win, &onKey);
  glfwSetKeyCallback(win, [](GLFWwindow* w,int key,int sc,int act,int mods){
    auto cb = (decltype(onKey)*)glfwGetWindowUserPointer(w); (*cb)(w,key,sc,act,mods);
  });

  while(!glfwWindowShouldClose(win)){
    glfwPollEvents();
    renderer.draw(board, score, best);
    glfwSwapBuffers(win);
  }
  glfwTerminate();
  return 0;
}


## 15) Commandes de build (exemples Windows)

Avec vcpkg intégré à CMake (toolchain):

- Générer: cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
- Compiler: cmake --build build --config Release --target 2048_glfw
- Exécuter: build/Release/2048_glfw.exe

Dans CLion, configurez le Toolchain vcpkg dans Settings > CMake profiles si nécessaire.


## 16) Améliorations possibles

- Transitions et easing avancés (easeOutBack sur les fusions)
- Thème sombre/clair, options UI
- Système de particules lors des fusions
- Historique/Undo (pile des états)
- Support tactile (swipe) via bibliothèques supplémentaires


## 17) Ressources

- GLFW: https://www.glfw.org/
- stb_easy_font: https://github.com/nothings/stb
- 2048 original: https://play2048.co/
- "Guide to 2D Rendering with OpenGL" (recherches web)

Bon développement et amusez-vous bien !
