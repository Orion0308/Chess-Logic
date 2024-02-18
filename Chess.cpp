#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <utility>
enum class PieceType {
  KING,
  QUEEN,
  PAWN,
  BISHOP,
  ROOK,
  KNIGHT,
  EMPTY,
};

enum class PieceColour {
  WHITE,
  BLACK,
  NONE,
};

struct Piece {
  PieceType type = PieceType::EMPTY;
  PieceColour colour = PieceColour::NONE;
};

int MoveCount = 0;

class ChessBoard {
private:
  Piece board[8][8];
  std::map<PieceType, std::string> PieceTypeToString = {
      {PieceType::KING, "K"},   {PieceType::QUEEN, "Q"},
      {PieceType::BISHOP, "B"}, {PieceType::KNIGHT, "k"},
      {PieceType::PAWN, "P"},   {PieceType::ROOK, "R"},
      {PieceType::EMPTY, "X"},
  };

  std::map<PieceColour, std::string> PieceColourToString = {
      {PieceColour::WHITE, "W"},
      {PieceColour::BLACK, "B"},
      {PieceColour::NONE, "X"},
  };

public:
  ChessBoard() {

    static constexpr Piece HomeRow[8] = {
        {PieceType::ROOK, PieceColour::WHITE},
        {PieceType::KNIGHT, PieceColour::WHITE},
        {PieceType::BISHOP, PieceColour::WHITE},
        {PieceType::QUEEN, PieceColour::WHITE},
        {PieceType::KING, PieceColour::WHITE},
        {PieceType::BISHOP, PieceColour::WHITE},
        {PieceType::KNIGHT, PieceColour::WHITE},
        {PieceType::ROOK, PieceColour::WHITE}};

    std::ranges::copy(HomeRow, board[0]);
    std::ranges::transform(HomeRow, board[7], [](Piece p) {
      p.colour = PieceColour::BLACK;
      return p;
    });
    std::ranges::fill(board[1], Piece{PieceType::PAWN, PieceColour::WHITE});
    std::ranges::fill(board[6], Piece{PieceType::PAWN, PieceColour::BLACK});
    return;
  }

  void printboard() {
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {

        std::cout << PieceTypeToString[board[x][y].type];
        std::cout << PieceColourToString[board[x][y].colour] << " ";
      }
      std::cout << std::endl;
    }
  }

  Piece getPieceAt(int x, int y) const { return board[x][y]; }

  void setPieceAt(int x, int y, const Piece &piece) { board[x][y] = piece; }

  void Promotion(int x, int y, PieceType PromoPiece) {
    board[x][y].type = PromoPiece;
  }

  bool HasWhiteKingMoved = false;
  bool HasBlackKingMoved = false;
  bool HasWhiteKingSideMoved = false;
  bool HasWhiteQueenSideMoved = false;
  bool HasBlackKingSideMoved = false;
  bool HasBlackQueenSideMoved = false;
};

bool WhoMove() {
  int BlacksTurn = MoveCount % 2;
  if (BlacksTurn == 0) {
    // if True its blacks turn
    return true;
  } else {
    // if false whites turn
    return false;
  }
}

std::pair<int, int> FindKingWhite(ChessBoard &chessBoard) {

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      Piece CheckSpace = chessBoard.getPieceAt(i, j);
      if (CheckSpace.type == PieceType::KING &&
          CheckSpace.colour == PieceColour::WHITE) {
        return std::make_pair(i, j);
      }
    }
  }
  return std::make_pair(-1, -1);
}

std::pair<int, int> FindKingBlack(ChessBoard &chessBoard) {

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      Piece CheckSpace = chessBoard.getPieceAt(i, j);
      if (CheckSpace.type == PieceType::KING &&
          CheckSpace.colour == PieceColour::BLACK) {
        return std::make_pair(i, j);
      }
    }
  }
  return std::make_pair(-1, -1);
}

bool CheckDiagonals(int x1, int x2, int y1, int y2,
                    const ChessBoard &chessBoard) {

  const int xSteps = abs(x2 - x1);
  const int ySteps = abs(y2 - y1);

  if (ySteps != xSteps) {

    return false;
  }

  const auto xDirection = (x2 - x1 > 0) ? 1 : -1;
  const auto yDirection = (y2 - y1 > 0) ? 1 : -1;
  const auto source = chessBoard.getPieceAt(x1, y1);
  for (int i = 1; i < xSteps; i++) {

    if (ySteps == 1) {
      break;
    }

    int x = x1 + i * xDirection;
    int y = y1 + i * yDirection;
    auto CheckSpace = chessBoard.getPieceAt(x, y);

    if (i <= xSteps - 1 && CheckSpace.type != PieceType::EMPTY) {

      return false;
    }
  }
  const Piece destination = chessBoard.getPieceAt(x2, y2);
  if (destination.type != PieceType::EMPTY &&
      destination.colour == source.colour) {

    return false;
  }

  return true;
}

bool CheckVerticals(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  const int xSteps = abs(x2 - x1);

  if (y1 != y2) {

    return false;
  }

  const auto xDirection = (x2 - x1 > 0) ? 1 : -1;
  const auto source = chessBoard.getPieceAt(x1, y1);
  for (int i = 1; i < xSteps; i++) {

    int x = x1 + i * xDirection;
    int y = y1;
    auto CheckSpace = chessBoard.getPieceAt(x, y);

    if (i <= xSteps - 1 && CheckSpace.type != PieceType::EMPTY) {

      return false;
    }
  }
  const Piece destination = chessBoard.getPieceAt(x2, y2);
  if (destination.type != PieceType::EMPTY &&
      destination.colour == source.colour) {

    return false;
  }

  return true;
}

bool CheckHorizontals(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  const int ySteps = abs(y2 - y1);

  if (x1 != x2) {

    return false;
  }

  const auto yDirection = (y2 - y1 > 0) ? 1 : -1;
  const auto source = chessBoard.getPieceAt(x1, y1);
  for (int i = 1; i < ySteps; i++) {

    int x = x1;
    int y = y1 + i * yDirection;
    auto CheckSpace = chessBoard.getPieceAt(x, y);

    if (i <= ySteps - 1 && CheckSpace.type != PieceType::EMPTY) {

      return false;
    }
  }

  const Piece destination = chessBoard.getPieceAt(x2, y2);
  if (destination.type != PieceType::EMPTY &&
      destination.colour == source.colour) {

    return false;
  }

  return true;
}

bool RookMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  if (x1 == x2) {

    return CheckHorizontals(x1, x2, y1, y2, chessBoard);
  } else if (y1 == y2) {

    return CheckVerticals(x1, x2, y1, y2, chessBoard);
  } else {

    return false;
  }
}

bool BishopMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  int xSteps = (x2 - x1);
  int ySteps = (y2 - y1);
  if (abs(xSteps) != abs(ySteps)) {
    return false;
  } else {
    return CheckDiagonals(x1, x2, y1, y2, chessBoard);
  }
}

bool KnightMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {

  int xSteps = abs(x2 - x1);
  int ySteps = abs(y2 - y1);
  if ((xSteps == 2 && ySteps == 1) || (xSteps == 1 && ySteps == 2)) {

    return true;
  } else {

    return false;
  }
}

bool QueenMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  int xSteps = (x2 - x1);
  int ySteps = (y2 - y1);
  if (x1 == x2) {
    return CheckHorizontals(x1, x2, y1, y2, chessBoard);
  } else if (y1 == y2) {
    return CheckVerticals(x1, x2, y1, y2, chessBoard);
  } else if (abs(xSteps) == abs(ySteps)) {
    return CheckDiagonals(x1, x2, y1, y2, chessBoard);
  } else {
    return false;
  }
}

bool KingMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  const int xSteps = x2 - x1;
  const int ySteps = y2 - y1;

  Piece destination = chessBoard.getPieceAt(x2, y2);
  Piece source = chessBoard.getPieceAt(x1, y1);
  if (abs(xSteps) > 1 || abs(ySteps) > 1) {
    return false;
  }
  if (destination.colour == source.colour) {
    return false;
  } else {
    return true;
  }
}

bool PawnMove(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {
  Piece source = chessBoard.getPieceAt(x1, y1);
  Piece destination = chessBoard.getPieceAt(x2, y2);

  const int xSteps = (x2 - x1);
  const int ySteps = (y2 - y1);
  bool isHome = ((x1 == 6 && source.colour == PieceColour::BLACK) ||
                 (x1 == 1 && source.colour == PieceColour::WHITE))
                    ? true
                    : false;

  if ((abs(xSteps) == 2) && (isHome) && (ySteps == 0)) {
    return CheckVerticals(x1, x2, y1, y2, chessBoard);
  } else if ((abs(xSteps) == 1 && ySteps == 0) &&
             destination.type == PieceType::EMPTY) {
    return true;
  } else if ((abs(xSteps) == 1 && abs(ySteps) == 1) &&
             (destination.colour != source.colour &&
              destination.colour != PieceColour::NONE)) {
    return true;
  } else {
    return false;
  }
  return false;
}

bool MoveCheck(int x1, int y1, int x2, int y2, ChessBoard &chessBoard) {

  Piece source = chessBoard.getPieceAt(x1, y1);

  switch (source.type) {

  case PieceType::PAWN:
    return PawnMove(x1, x2, y1, y2, chessBoard);
    break;

  case PieceType::ROOK:
    return RookMove(x1, x2, y1, y2, chessBoard);
    break;

  case PieceType::BISHOP:
    return BishopMove(x1, x2, y1, y2, chessBoard);
    break;

  case PieceType::QUEEN:
    return QueenMove(x1, x2, y1, y2, chessBoard);
    break;

  case PieceType::KNIGHT:
    return KnightMove(x1, x2, y1, y2, chessBoard);
    break;

  case PieceType::KING:
    return KingMove(x1, x2, y1, y2, chessBoard);
    break;

  default:
    return false; // END OF MOVE CHECK FUNCTION!!!!!!!!!!!!!!!!
  }
}

void Promotion(int x1, int x2, int y1, int y2, ChessBoard &chessBoard) {

  int Choice = 0;
  Piece Colour, Empty;
  Colour.colour = WhoMove() ? PieceColour::WHITE : PieceColour::BLACK;
  Empty = {PieceType::EMPTY, PieceColour::NONE};

  std::cout << "To which piece do you want to promote your pawn: 1) Knight, 2) "
               "Queen, 3) Bishop, 4) Rook: ";
  std::cin >> Choice;
  std::cout << "\n";
  switch (Choice) {
  case 1:
    chessBoard.setPieceAt(x2, y2, Piece{PieceType::KNIGHT, Colour.colour});
    chessBoard.setPieceAt(x1, y1, Empty);
    break;
  case 2:
    chessBoard.setPieceAt(x2, y2, Piece{PieceType::QUEEN, Colour.colour});
    chessBoard.setPieceAt(x1, y1, Empty);
    break;
  case 3:
    chessBoard.setPieceAt(x2, y2, Piece{PieceType::BISHOP, Colour.colour});
    chessBoard.setPieceAt(x1, y1, Empty);
    break;
  case 4:
    chessBoard.setPieceAt(x2, y2, Piece{PieceType::ROOK, Colour.colour});
    chessBoard.setPieceAt(x1, y1, Empty);
    break;
  }
}

bool isSpaceUnderAttack(int x, int y, ChessBoard &chessBoard) {
  Piece Friendly;
  Friendly.colour = WhoMove() ? PieceColour::WHITE : PieceColour::BLACK;
  std::string colour = WhoMove() ? "WHITE" : "BLACK";

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      Piece Attacker = chessBoard.getPieceAt(i, j);

      if (MoveCheck(i, j, x, y, chessBoard) &&
          (Attacker.colour != Friendly.colour)) {

        return true; // XY is Under attack
      }
    }
  }
  return false;
}

bool isInCheck(ChessBoard &chessBoard) {
  auto [KingX, KingY] =
      WhoMove() ? FindKingWhite(chessBoard) : FindKingBlack(chessBoard);
  if (isSpaceUnderAttack(KingX, KingY, chessBoard)) {
    return true; // King is in check
  }
  return false; // King Isnt in Check
}

bool isCheckmate(ChessBoard &ChessBoard) {

  auto [KingX, KingY] =
      WhoMove() ? FindKingWhite(ChessBoard) : FindKingBlack(ChessBoard);
  Piece King = ChessBoard.getPieceAt(KingX, KingY);
  if (!isInCheck(ChessBoard)) {
    return false;
  }
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {

      if (i == 0 && j == 0) {
        continue;
      }

      int newX = KingX + i, newY = KingY + j;
      // Check board boundaries
      if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
        Piece CheckSpace = ChessBoard.getPieceAt(KingX + i, KingY + j);

        if (!isSpaceUnderAttack(KingX + i, KingY + j, ChessBoard) &&
            CheckSpace.colour != King.colour) {

          return false; // NOT IN CHECKMATE
        }
      }
    }
  }

  return true;
}

bool CastlingMoveLegality(int x1, int x2, int y1, int y2,
                          ChessBoard &chessBoard) {

  bool isWhiteTurn = WhoMove();
  bool KingSide = y1 < y2;

  if ((isWhiteTurn && chessBoard.HasWhiteKingMoved) ||
      (!isWhiteTurn && chessBoard.HasBlackKingMoved) || x1 != x2) {

    return false;
  }

  if ((isWhiteTurn && ((KingSide && chessBoard.HasWhiteKingSideMoved) ||
                       (!KingSide && chessBoard.HasWhiteQueenSideMoved))) ||
      (!isWhiteTurn && ((KingSide && chessBoard.HasBlackKingSideMoved) ||
                        (!KingSide && chessBoard.HasBlackQueenSideMoved)))) {
    return false;
  }

  int NumSpacesToCheck = KingSide ? 2 : 3;
  int Direction = KingSide ? 1 : -1;

  for (int i = 1; i <= NumSpacesToCheck; i++) {
    Piece CheckSpace = chessBoard.getPieceAt(x1, y1 + i * Direction);

    if (CheckSpace.type != PieceType::EMPTY ||
        isSpaceUnderAttack(x1, y1 + i * Direction, chessBoard)) {

      return false; // CANNOT CASTLE
    }
  }
  return true;
}

#include <limits> // For dealing with invalid input

#include <limits>

void getPlayerMove(int &x1, int &x2, int &y1, int &y2) {
    while (true) { 
        std::cout << "Enter Your Move (x1,y1),(x2,y2):";
        std::cin >> x1 >> y1 >> x2 >> y2;

        if (std::cin.fail() || x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 ||
        x2 > 7 || y2 < 0 || y2 > 7) {
            std::cout << "Invalid input. Please enter coordinates within the board (0-7)." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Added line
            break; 
        }
    }
}


bool doMove(ChessBoard &chessBoard) {
  int x1, x2, y1, y2;

  getPlayerMove(x1, x2, y1, y2);

  Piece source = chessBoard.getPieceAt(x1, y1);

  Piece destination = chessBoard.getPieceAt(x2, y2);

  Piece Empty;

  Empty.type = PieceType::EMPTY;
  Empty.colour = PieceColour::NONE;

  if (source.colour == destination.colour &&
      !CastlingMoveLegality(x1, x2, y1, y2, chessBoard)) {
    return false;
  }

  if (source.colour == destination.colour &&
      !CastlingMoveLegality(x1, x2, y1, y2, chessBoard)) {
    return false;
  }

  if (WhoMove() && source.colour != PieceColour::WHITE) {
    return false;
  } else if (!WhoMove() && source.colour != PieceColour::BLACK) {
    return false;
  }

  switch (source.type) {
  case PieceType::PAWN:

    if (WhoMove() && x2 == 7 && MoveCheck(x1, x2, y1, y2, chessBoard)) {
      Promotion(x1, x2, y1, y2, chessBoard);
      MoveCount++;
    } else if (!WhoMove() && x2 == 0 && MoveCheck(x1, x2, y1, y2, chessBoard)) {
      Promotion(x1, x2, y1, y2, chessBoard);
      MoveCount++;
    }

    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      MoveCount++;
    } else {
      return false;
    }
    break;

  case PieceType::BISHOP:
    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      MoveCount++;
    } else {
      return false;
    }
    break;

  case PieceType::QUEEN:
    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      MoveCount++;
    } else {
      return false;
    }
    break;

  case PieceType::ROOK:
    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      if (y1 == 0) {
        WhoMove() ? chessBoard.HasWhiteQueenSideMoved = true
                  : chessBoard.HasBlackQueenSideMoved = true;
      } else if (y1 == 7) {
        WhoMove() ? chessBoard.HasWhiteKingSideMoved = true
                  : chessBoard.HasBlackKingSideMoved = true;
      }

      MoveCount++;

    } else {
      return false;
    }
    break;

  case PieceType::KING:

    if ((y2 == 7 || y2 == 0) &&
        CastlingMoveLegality(x1, x2, y1, y2, chessBoard)) {
      chessBoard.setPieceAt(x1, y1, Empty);
      chessBoard.setPieceAt(x2, y2, Empty);
      int DirectionY = (y2 == 7) ? 1 : -1;
      chessBoard.setPieceAt(x1, y1 + 2 * DirectionY, source);
      chessBoard.setPieceAt(x1, y1 + 1 * DirectionY, destination);
    }

    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      source.colour == PieceColour::WHITE ? chessBoard.HasWhiteKingMoved = true
                                          : chessBoard.HasBlackKingMoved = true;

      MoveCount++;
    } else {
      return false;
    }
    break;

  case PieceType::KNIGHT:
    if (MoveCheck(x1, y1, x2, y2, chessBoard)) {
      chessBoard.setPieceAt(x2, y2, source);
      chessBoard.setPieceAt(x1, y1, Empty);
      MoveCount++;
    } else {
      return false;
    }
    break;

  default:
    return false;
  }

  return false;
}

int mainy() {

  ChessBoard chessBoard;
  chessBoard.printboard();
  while (true) {

    std::string TurnMsg = WhoMove() ? "Whites Turn" : "Blacks Turn";
    std::cout << TurnMsg << "\n";
    doMove(chessBoard);
    if (isCheckmate(chessBoard)) {
      std::cout << "CHECKMATE\n";
      return false;
    }
    chessBoard.printboard();
  }
}
