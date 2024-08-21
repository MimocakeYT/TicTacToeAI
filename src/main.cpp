#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>
#include <vector>

constexpr int BLANK = 0;
constexpr int CROSS = 1;
constexpr int ZERO = 2;

constexpr int CROSS_WON = 10;
constexpr int ZERO_WON = 11;
constexpr int TIE = 12;
constexpr int GAME_NOT_OVER = 13;

constexpr int BOARD_SIZE = 9;
constexpr int ROW_SIZE = 3;
constexpr int CELL_SIZE = 275;
constexpr int WINDOW_SIZE = 850;
constexpr int SPRITE_OFFSET = 25;

int game_situation(const std::vector<int> &board) {
  const int win_patterns[8][3] = {
      {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
      {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
      {0, 4, 8}, {2, 4, 6}             // Diagonals
  };

  for (const auto &pattern : win_patterns) {
    if (board[pattern[0]] == CROSS && board[pattern[1]] == CROSS &&
        board[pattern[2]] == CROSS)
      return CROSS_WON;
    if (board[pattern[0]] == ZERO && board[pattern[1]] == ZERO &&
        board[pattern[2]] == ZERO)
      return ZERO_WON;
  }

  for (const auto &cell : board) {
    if (cell == BLANK)
      return GAME_NOT_OVER;
  }

  return TIE;
}

int minimax(std::vector<int> &board, int alpha, int beta, bool is_zero) {
  int gs = game_situation(board);
  if (gs == CROSS_WON)
    return 1000;
  if (gs == ZERO_WON)
    return -1000;
  if (gs == TIE)
    return 0;

  if (is_zero) {
    int min_score = std::numeric_limits<int>::max();
    for (int i = 0; i < BOARD_SIZE; ++i) {
      if (board[i] != BLANK)
        continue;
      board[i] = ZERO;
      min_score = std::min(min_score, minimax(board, alpha, beta, false));
      board[i] = BLANK;
      beta = std::min(beta, min_score);
      if (beta <= alpha)
        break;
    }
    return min_score;
  } else {
    int max_score = std::numeric_limits<int>::min();
    for (int i = 0; i < BOARD_SIZE; ++i) {
      if (board[i] != BLANK)
        continue;
      board[i] = CROSS;
      max_score = std::max(max_score, minimax(board, alpha, beta, true));
      board[i] = BLANK;
      alpha = std::max(alpha, max_score);
      if (beta <= alpha)
        break;
    }
    return max_score;
  }
}

void reset_game(std::vector<int> &board, int &cnt, bool &game_over) {
  board.assign(BOARD_SIZE, BLANK);
  cnt = 0;
  game_over = false;
  std::system("cls");
}

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE),
                          "Tic Tac Toe");

  sf::Texture field_tex, cross_tex, zero_tex;
  field_tex.loadFromFile("../image/field.png");
  cross_tex.loadFromFile("../image/cross.png");
  zero_tex.loadFromFile("../image/zero.png");

  sf::Sprite field_spr(field_tex);

  std::vector<int> board(BOARD_SIZE, BLANK);
  int cnt = 0;
  bool game_over = false;

  while (window.isOpen()) {
    int gs = game_situation(board);
    if (gs != GAME_NOT_OVER && !game_over) {
      switch (gs) {
      case CROSS_WON:
        std::cout << "CROSS WON" << std::endl;
        break;
      case ZERO_WON:
        std::cout << "ZERO WON" << std::endl;
        break;
      case TIE:
        std::cout << "TIE" << std::endl;
        break;
      }
      game_over = true;
    }

    if (cnt % 2 == 1 && !game_over) {
      int best_move = -1, min_eval = std::numeric_limits<int>::max();
      for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == BLANK) {
          board[i] = ZERO;
          int cur = minimax(board, std::numeric_limits<int>::min(),
                            std::numeric_limits<int>::max(), false);
          if (cur < min_eval) {
            min_eval = cur;
            best_move = i;
          }
          board[i] = BLANK;
        }
      }
      if (best_move != -1) {
        board[best_move] = ZERO;
        ++cnt;
      }
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::R) {
        reset_game(board, cnt, game_over);
      } else if (event.type == sf::Event::MouseButtonPressed && cnt % 2 == 0 &&
                 !game_over) {
        sf::Vector2f ev(event.mouseButton.x < WINDOW_SIZE ? event.mouseButton.x
                                                          : WINDOW_SIZE,
                        event.mouseButton.y < WINDOW_SIZE ? event.mouseButton.y
                                                          : WINDOW_SIZE);
        int index = int(ev.y / CELL_SIZE) * ROW_SIZE + int(ev.x / CELL_SIZE);
        if (board[index] == BLANK) {
          board[index] = CROSS;
          ++cnt;
        }
      }
    }

    window.clear();
    window.draw(field_spr);

    for (int i = 0; i < BOARD_SIZE; ++i) {
      if (board[i] != BLANK) {
        sf::Sprite sprite(board[i] == CROSS ? cross_tex : zero_tex);
        sprite.setPosition(SPRITE_OFFSET + CELL_SIZE * (i % ROW_SIZE),
                           SPRITE_OFFSET + CELL_SIZE * (i / ROW_SIZE));
        window.draw(sprite);
      }
    }

    window.display();
  }

  return 0;
}