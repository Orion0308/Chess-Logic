#include "Chess.h"
#include "SFML.h"

int main() {

  icons icons;
  ChessBoard chessBoard;
  Display display;
  display.drawboard();
  while (display.window.isOpen()) {
    sf::Event event;
    while (display.window.pollEvent(event)) {
      display.getPlayerClicks(event);
      
      if (event.type == sf::Event::Closed)
        display.window.close();
    }
    
    display.drawboard();
    std::string TurnMsg = WhoMove() ? "Whites Turn" : "Blacks Turn";
    std::cout << TurnMsg << "\n";
    ResetEnPassant(chessBoard);
    if (stalemate(chessBoard)) {
      std::cout << "Draw by stalemate\n";
      return false;
    }
    if (isCheckmate(chessBoard)) {
      std::cout << "CHECKMATE\n";
      return false;
    }
    
    
  }
  return 0;
}