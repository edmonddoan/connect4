#include "connect.h"
#include <iostream>
#include <glib.h>

using namespace std;
Homewindow::Homewindow(): box(Gtk::ORIENTATION_VERTICAL)
{
  set_title("Connect Four Start");
  set_size_request(500,600);
  pvp.add_label("Player vs Player");
  pvp.signal_pressed().connect(sigc::mem_fun(*this,&Homewindow::opengame));
  pvp.remove();
  pvp.add_pixlabel("pvp.jpg", "");
  sort.pack_start(pvp);
  computer.remove();
  computer.add_pixlabel("computer.jpg", "");
  computer.signal_pressed().connect(sigc::mem_fun(*this,&Homewindow::opengame));
  sort.pack_start(computer);
  close.remove();
  close.add_pixlabel("exit.jpg", "");
  close.signal_pressed().connect(sigc::mem_fun(*this,&Homewindow::exit_game));
  sort.pack_start(close);
  sort.show_all();
  add(sort);

}
Homewindow::~Homewindow(){}

void Homewindow::exit_game()
{
  hide();
  exit(0);
}

void Homewindow::opengame()
{
  string ent_name1, ent_name2;
  nameWindow w1;
  Gtk::Main::run(w1);
  Player player1(w1.ent_red_in);
  Player player2(w1.ent_black_in);
  connectFour w2(player1,player2);
  Gtk::Main::run(w2);

}
connectFour::connectFour(Player player1, Player player2)
: box(Gtk::ORIENTATION_VERTICAL),
  //Added player info elements
  all_content(Gtk::ORIENTATION_HORIZONTAL),
  top_content(Gtk::ORIENTATION_VERTICAL),
  left_content(Gtk::ORIENTATION_VERTICAL),
  left_border(Gtk::ORIENTATION_VERTICAL),
  mid_content(Gtk::ORIENTATION_VERTICAL),
  right_content(Gtk::ORIENTATION_VERTICAL)
{
  GamePlayers.push_back(player1);
  GamePlayers.push_back(player2);
  set_title("Team 1337: Connect Four");
  set_size_request(850, 825);
  //Added player info elements
  mid_content.set_size_request(750,825);
  left_content.set_size_request(150,825);
  left_border.set_size_request(150,825);
  right_border.set_size_request(150,825);
  left_border.override_background_color(Gdk::RGBA("red"),Gtk::STATE_FLAG_NORMAL);
  left_content.override_background_color(Gdk::RGBA("white"),Gtk::STATE_FLAG_NORMAL);
  right_border.override_background_color(Gdk::RGBA("black"),Gtk::STATE_FLAG_NORMAL);
  right_content.override_background_color(Gdk::RGBA("white"),Gtk::STATE_FLAG_NORMAL);
  top_border.override_background_color(Gdk::RGBA("red"),Gtk::STATE_FLAG_NORMAL);
  top_content.override_background_color(Gdk::RGBA("white"),Gtk::STATE_FLAG_NORMAL);
  right_content.set_size_request(150,825);
  top_border.set_size_request(750,75);
  top_content.set_size_request(750,75);
  top_content.set_border_width(10);
  left_content.set_border_width(10);
  right_content.set_border_width(10);

  player1_name.set_size_request(100,100);
  player1_wins.set_size_request(100,100);
  player2_name.set_size_request(100,100);
  player2_wins.set_size_request(100,100);
  player1_name.set_text(GamePlayers[0].get_name());
  player2_name.set_text(GamePlayers[1].get_name());
  player1_wins.set_text("Wins: "+to_string(GamePlayers[0].get_wins()));
  player2_wins.set_text("Wins: "+to_string(GamePlayers[1].get_wins()));
  player_turn.set_text(GamePlayers[0].get_name()+"'s turn");
  total_ties.set_text("Ties: #");



  for(int i = 0; i< 6; i++)
  {
    vector<buttonRowCol> temp;
    for(int j= 0; j< 6;j++)
    {
      buttonRowCol b(i,j);
      temp.push_back(b);
    }
    allButtons.push_back(temp);
  }

  for(int i =0; i < allButtons.size(); i++)
  {
    for(int j = 0; j< allButtons[i].size();j++)
    {
      // grid.attach(allButtons[i].at(j),i,j,10,10);
    // allButtons[i][j].getButton()->set_size_request(100,100);

      grid.attach(*(allButtons[i][j].getButton()),(i*125)+150,j*125,125,125);
    }
  }
  // intiate the first row on the bottom
  for(int c =0; c <allButtons[5].size(); c++)
  {
    allButtons[c][5].getButton()->set_sensitive(true);
    allButtons[c][5].getButton()->set_label("CHOOSE ME");
    allButtons[c][5].getButton()->signal_clicked().connect( sigc::bind<buttonRowCol>( sigc::mem_fun(*this, &connectFour::placedChecker),allButtons[c][5] ));

  }
  grid.show_all();

  all_content.pack_start(left_border);
  all_content.pack_start(mid_content);
  all_content.pack_start(right_border);
  left_border.pack_start(left_content);
  right_border.pack_start(right_content);
  top_border.pack_start(top_content);
  mid_content.pack_start(top_border);
  mid_content.pack_start(grid);
  left_content.pack_start(player1_name);
  left_content.pack_start(player1_wins);
  top_content.pack_start(player_turn);
  top_content.pack_start(total_ties); //should it show if there are no ties so far?
  right_content.pack_start(player2_name);
  right_content.pack_start(player2_wins);
  add(all_content);

  show_all_children();
  this->isRedTurn = true;


}
connectFour::~connectFour()
{}
  // connectFour starts from bottom to top
  // Everytime a spot is filled, the one above has to become available to place next checkered.
  // also not sure why rows and col indexs are inverted
  // NOTE: getRow_num shows the COLUMN AND VICE VERSA
  // NOTE: getCol_num shows the ROW number I don't know why it is that way
  // Since I am changing button status I can't use a copy.
  // I need the original
void connectFour::placedChecker(buttonRowCol &button) {
  // does not need to do anything if the row for that column is filled thus 0
  if (button.getCol_num() != 0) {
    // if button has not been revealed then work should be done to
    // reveal button by checking sensitive
    if (!allButtons[button.getRow_num()][button.getCol_num() - 1]
             .getButton()
             ->get_sensitive()) {
      if (button.getCol_num() != 0) {
        allButtons[button.getRow_num()][button.getCol_num() - 1]
            .getButton()
            ->set_sensitive(
                true); // make button above the one pressed available
        allButtons[button.getRow_num()][button.getCol_num() - 1]
            .getButton()
            ->set_label("CHOOSE ME");
        // attach function call for click
        allButtons[button.getRow_num()][button.getCol_num() - 1]
            .getButton()
            ->signal_clicked()
            .connect(sigc::bind<buttonRowCol>(
                sigc::mem_fun(*this, &connectFour::placedChecker),
                allButtons[button.getRow_num()][button.getCol_num() - 1]));
      }
    }
  }
  if( !button.getIsSpotFilled())
  {
    if (this->isRedTurn) {
      button.getButton()->remove();
      button.getButton()->add_pixlabel("red.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      connectFour::borderChange(isRedTurn);
      button.setIsSpotFilled();
      cout << "COL NUMBER: " << button.getRow_num() << " PRESSSED" << endl;
      cout << "ROW NUMBER: " << button.getCol_num() << " PRESSSED" << endl;

      spots[button.getCol_num()][button.getRow_num()] = RED;

      // if vertical not found
      checkVertical(isRedTurn,button.getCol_num(),button.getRow_num());
     cout <<  checkHorizontal(isRedTurn,button.getCol_num(),button.getRow_num()) << endl;
      checkDiagonal(isRedTurn,button.getCol_num(),button.getRow_num());
      this->isRedTurn = false;

      //**********************************
    } else {
      button.getButton()->remove();
      button.getButton()->add_pixlabel("black.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      connectFour::borderChange(isRedTurn);
      // record spot
      spots[button.getCol_num()][button.getRow_num()] = BLACK ;
      button.setIsSpotFilled();

      checkVertical(isRedTurn,button.getCol_num(),button.getRow_num());
      checkHorizontal(isRedTurn,button.getCol_num(),button.getRow_num());
      cout <<  checkHorizontal(isRedTurn,button.getCol_num(),button.getRow_num()) << endl;
      this->isRedTurn = true;
    }
  }
}

bool connectFour::checkVertical(bool isRedTurn, int row, int col) {
  placed color;
  if (isRedTurn) {
    color = RED;
  } else {
    color = BLACK;
  }

  if ((color == spots[row][col]) && (color == spots[row + 1][col]) &&
      (color == spots[row + 2][col]) && (color == spots[row + 3][col])) {
    if (color == RED) {
      allButtons[col][row].getButton()->remove();
      allButtons[col][row].getButton()->add_pixlabel(
          "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 1].getButton()->remove();
      allButtons[col][row + 1].getButton()->add_pixlabel(
          "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 2].getButton()->remove();
      allButtons[col][row + 2].getButton()->add_pixlabel(
          "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 3].getButton()->remove();
      allButtons[col][row + 3].getButton()->add_pixlabel(
          "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      cout << "Winner VERTICAL 2" << endl;

    } else {
      allButtons[col][row].getButton()->remove();
      allButtons[col][row].getButton()->add_pixlabel(
          "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 1].getButton()->remove();
      allButtons[col][row + 1].getButton()->add_pixlabel(
          "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 2].getButton()->remove();
      allButtons[col][row + 2].getButton()->add_pixlabel(
          "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      allButtons[col][row + 3].getButton()->remove();
      allButtons[col][row + 3].getButton()->add_pixlabel(
          "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    }
    return true;
  }
  return false;
}
bool connectFour::checkHorizontal(bool isRedTurn, int row, int col) {
  placed color;
  if (isRedTurn) {
    color = RED;
  } else {
    color = BLACK;
  }
  if (col == 0) {
    if ((color == spots[row][col]) && (color == spots[row][col + 1]) &&
        (color == spots[row][col + 2]) && (color == spots[row][col + 3])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 3][row].getButton()->remove();
        allButtons[col + 3][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 3][row].getButton()->remove();
        allButtons[col + 3][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 1" << endl;

      return true;
    }
  } else if (col == 1) {
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col + 1]) && (color == spots[row][col + 2])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 1" << endl;
      return true;
    }
  } else if (col == 2) {
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col - 2]) && (color == spots[row][col + 1])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 2" << endl;
      return true;
    }
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col + 2]) && (color == spots[row][col + 1])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 2.1 - 2/1/3/4" << endl;
      return true;
    }
    if ((color == spots[row][col]) && (color == spots[row][col + 1]) &&
        (color == spots[row][col + 2]) && (color == spots[row][col + 3])) {

      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 3][row].getButton()->remove();
        allButtons[col + 3][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 3][row].getButton()->remove();
        allButtons[col + 3][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 2.3 - 2/3/4/5" << endl;
      return true;
    }
  } else if (col == 3) {
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col - 2]) && (color == spots[row][col - 3])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 3 - 3/2/1/0" << endl;
      return true;
    }
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col - 2]) && (color == spots[row][col + 1])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 3.2 - 3/2/1/4" << endl;
      return true;
    }
    if ((color == spots[row][col]) && (color == spots[row][col + 1]) &&
        (color == spots[row][col + 2]) && (color == spots[row][col - 1])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 2][row].getButton()->remove();
        allButtons[col + 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 3.3- 3/4/5/2" << endl;
      return true;
    }
  }
  if (col == 4) {
    if ((color == spots[row][col]) && (color == spots[row][col + 1]) &&
        (color == spots[row][col - 1]) && (color == spots[row][col - 2])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col + 1][row].getButton()->remove();
        allButtons[col + 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 4- 4/5/3/2" << endl;
      return true;
    }
    if ((color == spots[row][col]) && (color == spots[row][col - 3]) &&
        (color == spots[row][col - 1]) && (color == spots[row][col - 2])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 4- 4/1/3/2" << endl;
      return true;
    }
  }

  if (col == 5) {
    if ((color == spots[row][col]) && (color == spots[row][col - 1]) &&
        (color == spots[row][col - 2]) && (color == spots[row][col - 3])) {
      if (color == RED) {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "red_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

      } else {
        allButtons[col][row].getButton()->remove();
        allButtons[col][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 1][row].getButton()->remove();
        allButtons[col - 1][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 2][row].getButton()->remove();
        allButtons[col - 2][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
        allButtons[col - 3][row].getButton()->remove();
        allButtons[col - 3][row].getButton()->add_pixlabel(
            "black_winner.png", "", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
      }
      cout << "Winner HORIZONTAL - MIDDLE @ COL 4- 5/4/3/2" << endl;
      return true;
    }
  }
  return false;
}

bool connectFour::checkDiagonal(bool isRedTurn, int row, int col) {
  placed color;
  if (isRedTurn) {
    color = RED;
  } else {
    color = BLACK;
  }
  if ((row == 5) || (row == 4) || (row == 3)) {
    if ((col == 0) || (col == 1) || (col == 2)) {
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
          (color == spots[row - 2][col + 2]) &&
          (color == spots[row - 3][col + 3])) {
        cout << "Winner DIAGONAL - MIDDLE @ ROW COL 5_012- 5.0/4.1/3.2/2.3"
             << endl;
             return true;
      }
    }
   if ((col == 3) || (col == 4) || (col == 5)) {
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
          (color == spots[row - 2][col - 2]) &&
          (color == spots[row - 3][col - 3])) {
        cout << "Winner DIAGONAL - MIDDLE @ ROW COL 5_012" << endl;
        return true;

      }
    }
  }
  if (row == 4)
  {
    if((col == 0) || (col ==1) || (col == 2)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row - 3][col + 3])) {
              cout << "Winner DIAGONAL - CASE 1" << endl;
              return true;
      }
    }
    if( (col== 3) || (col == 4) || (col == 5))
    {
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row - 3][col - 3])) {
              cout << "Winner DIAGONAL - CASE 2" << endl;
              return true;
      }
    }
    if((col ==1) || (col == 2)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if((col ==3) || (col == 4)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
  }
  if (row == 3)
  {
    if((col == 0) || (col ==1) || (col == 2)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row - 3][col + 3])) {
              cout << "Winner DIAGONAL - CASE 1" << endl;
              return true;
      }
    }
    if( (col== 3) || (col == 4) || (col == 5))
    {
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row - 3][col - 3])) {
              cout << "Winner DIAGONAL - CASE 2" << endl;
              return true;
      }
    }
    if((col == 1) || (col == 2)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if((col == 3) || (col == 4)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if((col == 2) ||(col ==3)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 5-" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row - 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row + 1][col - 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row - 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row - 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if(col == 3){
      if ((color == spots[row][col]) && (color == spots[row -1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row - 3][col - 3])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row -1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row -1][col - 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if(col == 4){
      if ((color == spots[row][col]) && (color == spots[row -1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row -1][col - 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }

  if (row == 2)
  {
    if( (col == 0) || (col == 1) || (col == 2) )
    {
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 3][col + 3])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if( (col == 1) || (col == 2) || (col == 3)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row + 1][col - 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row - 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if( (col == 2) || (col == 3) || (col == 4)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if( (col== 3) || (col == 4))
    {
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row - 2][col - 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 2" << endl;
              return true;
      }
    }
    if((col == 1) || (col == 2)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row - 2][col + 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 3" << endl;
              return true;
      }
    }
    if((col == 2) || (col == 3)){
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row - 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
  }
  if(row == 1){
    if( (col == 0) || (col == 1) || (col = 2)){
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 3][col + 3])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if( (col == 4) || (col == 5) || (col == 3) ){
      if ((color == spots[row][col]) && (color == spots[row + 1][col - 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row + 3][col - 3])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }

    }
    if( (col == 1) || (col ==2) || (col == 3)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col - 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 1][col + 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if( (col ==2) || (col == 3) || (col == 4)){
      if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row + 1][col - 1])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
  }
  if(row == 0){
    if( (col == 0) || (col == 1) || (col = 2)){
      if ((color == spots[row][col]) && (color == spots[row + 1][col + 1]) &&
              (color == spots[row + 2][col + 2]) &&
              (color == spots[row + 3][col + 3])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
    }
    if( (col == 4) || (col == 5) || (col == 3) ){
      if ((color == spots[row][col]) && (color == spots[row + 1][col - 1]) &&
              (color == spots[row + 2][col - 2]) &&
              (color == spots[row + 3][col - 3])) {
              cout << "Winner DIAGONAL - CASE 5" << endl;
              return true;
      }
  }
}
}

  //   ( (row == 4) || (row == 3) || (row == 2) ) && ((col == 1) || (col == 2) || (col == 3)) ) {
  //   if( ((row == 4) || (row == 3)) && (col != 3) )
  //   {
  //     if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
  //         (color == spots[row - 2][col + 2]) &&
  //         (color == spots[row - 3][col + 3])) {
  //         cout << "Winner DIAGONAL - CASE 1" << endl;
  //         return true;
  //
  //     }
  //  }
  //  if ((color == spots[row][col]) && (color == spots[row - 1][col + 1]) &&
  //       (color == spots[row - 2][col + 2]) &&
  //       (color == spots[row + 1][col - 1])) {
  //       cout << "Winner DIAGONAL - CASE 2" << endl;
  //       return true;
  //
  //   }
  // }

}



  void connectFour::winCountUp(string Winner) {
    if(Winner == "Red") {
      GamePlayers[0].add_wins();
      player1_wins.set_text("Wins: "+to_string(GamePlayers[0].get_wins()));
    }
    else if (Winner == "Black") {
      GamePlayers[1].add_wins();
      player2_wins.set_text("Wins: "+to_string(GamePlayers[1].get_wins()));
    }
    else if (Winner == "None") {
      PlayTies++;
      total_ties.set_text("Ties: "+to_string(PlayTies));
    }
  }
  void connectFour::borderChange(bool isRedTurn) {
    if(isRedTurn) {
      top_border.override_background_color(Gdk::RGBA("Red"),Gtk::STATE_FLAG_NORMAL);
      player_turn.set_text(GamePlayers[0].get_name()+"'s Turn");
    }
    else if(!isRedTurn) {
      top_border.override_background_color(Gdk::RGBA("Black"),Gtk::STATE_FLAG_NORMAL);
      player_turn.set_text(GamePlayers[1].get_name()+"'s Turn");
    }
  }
  void connectFour::GetNames() {
    input_name_text.set_text("Enter Player Names");
    NameDialog.set_size_request(100,100);
    NameDialog.get_vbox()->pack_start(input_name_text);
    NameDialog.show_all();
    NameDialog.run();
  }
  void connectFour::GetPlayers(Player first, Player second) {
    GamePlayers.push_back(first);
    GamePlayers.push_back(second);
  }
  Player::Player(string in_string) {
    name = in_string;
    wins = 0;
  }
  string Player::get_name() {
    return name;
  }
  int Player::get_wins() {
    return wins;
  }
  void Player::add_wins() {
    wins++;
  }
  nameWindow::nameWindow() :
  NameBox(Gtk::ORIENTATION_VERTICAL)
  {

  set_size_request(200,200);
  ent_name_pls.set_text("Please Enter Player Names");
  ent_red.set_max_length(16);
  ent_red.set_text("Player 1");
  ent_red.set_overwrite_mode(true);
  ent_red.set_size_request(200,25);
  ent_black.set_max_length(16);
  ent_black.set_text("Player 2");
  ent_black.set_overwrite_mode(true);
  send_ent.set_label("Submit");

  NameBox.pack_start(ent_name_pls);
  NameBox.pack_start(ent_red);
  NameBox.pack_start(ent_black);
  NameBox.pack_start(send_ent);
  send_ent.signal_clicked().connect(sigc::mem_fun(*this,&nameWindow::submitNames));
  add(NameBox);
  show_all_children();
  }
  nameWindow::~nameWindow() {}
  void nameWindow::submitNames() {
  ent_red_in = ent_red.get_text();
  ent_black_in = ent_black.get_text();
  hide();
  }
