#ifndef SFML_H
#define SFML_H
#include <SFML/Graphics.hpp>
#include "Chess.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

struct Sprites {
  sf::Sprite WhitePawn, BlackPawn, WhiteRook, BlackRook, WhiteKnight,
      BlackKnight, WhiteBishop, BlackBishop, WhiteQueen, BlackQueen, WhiteKing,
      BlackKing, BlacksTurn, WhitesTurn;
};

struct Textures {
  sf::Texture WhitePawn, BlackPawn, WhiteRook, BlackRook, WhiteKnight,
      BlackKnight, WhiteBishop, BlackBishop, WhiteQueen, BlackQueen, WhiteKing,
      BlackKing, BlacksTurn, WhitesTurn;
};

class icons {
public:
  Textures textures;
  Sprites sprites;
  Piece piece;
  sf::Sprite PlayerTurn = WhoMove() ? sprites.WhitePawn : sprites.BlackPawn;
  std::map<Piece, sf::Sprite> PieceToSprite;
  icons() {
    LoadTexToSprite(textures.WhitePawn, sprites.WhitePawn,
                    "/home/liam/Programming/C++/build/png/WP.png");
    LoadTexToSprite(textures.WhiteRook, sprites.WhiteRook,
                    "/home/liam/Programming/C++/build/png/WR.png");
    LoadTexToSprite(textures.WhiteKnight, sprites.WhiteKnight,
                    "/home/liam/Programming/C++/build/png/Wk.png");
    LoadTexToSprite(textures.WhiteBishop, sprites.WhiteBishop,
                    "/home/liam/Programming/C++/build/png/WB.png");
    LoadTexToSprite(textures.WhiteQueen, sprites.WhiteQueen,
                    "/home/liam/Programming/C++/build/png/WQ.png");
    LoadTexToSprite(textures.WhiteKing, sprites.WhiteKing,
                    "/home/liam/Programming/C++/build/png/WK.png");
    LoadTexToSprite(textures.BlackPawn, sprites.BlackPawn,
                    "/home/liam/Programming/C++/build/png/BP.png");
    LoadTexToSprite(textures.BlackRook, sprites.BlackRook,
                    "/home/liam/Programming/C++/build/png/BR.png");
    LoadTexToSprite(textures.BlackKnight, sprites.BlackKnight,
                    "/home/liam/Programming/C++/build/png/Bk.png");
    LoadTexToSprite(textures.BlackBishop, sprites.BlackBishop,
                    "/home/liam/Programming/C++/build/png/BB.png");
    LoadTexToSprite(textures.BlackQueen, sprites.BlackQueen,
                    "/home/liam/Programming/C++/build/png/BQ.png");
    LoadTexToSprite(textures.BlackKing, sprites.BlackKing,
                    "/home/liam/Programming/C++/build/png/BK.png");
    LoadTexToSprite(textures.BlacksTurn, sprites.BlacksTurn,"/home/liam/Programming/C++/build/png/BP.png");
    LoadTexToSprite(textures.WhitesTurn, sprites.WhitesTurn,"/home/liam/Programming/C++/build/png/WP.png");
    PopulateMap();
  }

private:
  bool LoadTexToSprite(sf::Texture &texture, sf::Sprite &sprite,
                       const std::string &filePath) {
    if (!texture.loadFromFile(filePath)) {
      std::cout << "Failed To Load Texture from: " << filePath << "\n";
      return false;
    }
    sprite.setTexture(texture);
    return true;
  }

  void PopulateMap() {
    PieceToSprite[Piece(PieceType::PAWN, PieceColour::WHITE)] =
        sprites.WhitePawn;
    PieceToSprite[Piece(PieceType::ROOK, PieceColour::WHITE)] =
        sprites.WhiteRook;
    PieceToSprite[Piece(PieceType::KNIGHT, PieceColour::WHITE)] =
        sprites.WhiteKnight;
    PieceToSprite[Piece(PieceType::BISHOP, PieceColour::WHITE)] =
        sprites.WhiteBishop;
    PieceToSprite[Piece(PieceType::QUEEN, PieceColour::WHITE)] =
        sprites.WhiteQueen;
    PieceToSprite[Piece(PieceType::KING, PieceColour::WHITE)] =
        sprites.WhiteKing;
    PieceToSprite[Piece(PieceType::PAWN, PieceColour::BLACK)] =
        sprites.BlackPawn;
    PieceToSprite[Piece(PieceType::ROOK, PieceColour::BLACK)] =
        sprites.BlackRook;
    PieceToSprite[Piece(PieceType::KNIGHT, PieceColour::BLACK)] =
        sprites.BlackKnight;
    PieceToSprite[Piece(PieceType::BISHOP, PieceColour::BLACK)] =
        sprites.BlackBishop;
    PieceToSprite[Piece(PieceType::QUEEN, PieceColour::BLACK)] =
        sprites.BlackQueen;
    PieceToSprite[Piece(PieceType::KING, PieceColour::BLACK)] =
        sprites.BlackKing;
  }
};


class Display {
public:
  icons Icons;
  ChessBoard chessBoard;
  int xPickUp;
  int yPickUp;
  int xPlace;
  int yPlace;
  sf::RenderWindow window;
  bool WaitingForFirstClick = true;
  
  Display() : window(sf::VideoMode(800, 900), "SFML Boilerplate Window") {
    xPickUp = yPickUp = xPlace = yPlace = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        sf::RectangleShape square(sf::Vector2f(100, 100));
        square.setPosition(j * 100, i * 100);
        auto fillclr = ((i + j) % 2 == 0) ? sf::Color::White : sf::Color::Blue;
        square.setFillColor(fillclr);
        Piece CheckSpace = chessBoard.getPieceAt(i, j);
        sf::Sprite PieceSprite(Icons.PieceToSprite[CheckSpace]);
        PieceSprite.setPosition(j * 100, i * 100);
        window.draw(square);
        window.draw(PieceSprite);
      }
    }
  }

  void DrawBackground() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        sf::RectangleShape square(sf::Vector2f(100, 100));
        square.setPosition(j * 100, i * 100);
        auto fillclr = ((i + j) % 2 == 0) ? sf::Color::White : sf::Color::Blue;
        square.setFillColor(fillclr);
        window.draw(square);
        sf::Sprite PlayerTurn = WhoMove() ? Icons.PieceToSprite[{PieceType::PAWN,PieceColour::WHITE}] : Icons.PieceToSprite[{PieceType::PAWN,PieceColour::BLACK}];
        PlayerTurn.setPosition(0,800);
        window.draw(PlayerTurn);
        
      }
    }
  }

  void drawboard() {
    window.clear(sf::Color::Green);
    DrawBackground();
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {

        Piece CheckSpace = chessBoard.getPieceAt(i, j);
        if (CheckSpace.type != PieceType::EMPTY) {
          sf::Sprite PieceSprite(Icons.PieceToSprite[CheckSpace]);
          PieceSprite.setPosition(j * 100, i * 100);
          window.draw(PieceSprite);
        }
      }
    }
    window.display();
  }
  void getPlayerClicks(sf::Event event) {

    if (WaitingForFirstClick && event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      xPickUp = event.mouseButton.y / 100;
      yPickUp = event.mouseButton.x / 100;
      WaitingForFirstClick = false;
    } else if (!WaitingForFirstClick &&
               event.type == sf::Event::MouseButtonPressed &&
               event.mouseButton.button == sf::Mouse::Left) {
      xPlace = event.mouseButton.y / 100;
      yPlace = event.mouseButton.x / 100;
      WaitingForFirstClick = true;
      std::cout << "Click detected at " << xPickUp << ","
                << yPickUp << " <><><> " << xPlace << ","
                << yPlace;
      doMove(chessBoard, xPickUp,xPlace,yPickUp,yPlace);
    }
  }
};



#endif