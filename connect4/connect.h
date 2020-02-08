#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <gtkmm.h>
#include <gdkmm/rgba.h>
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using namespace std;

class Player {
    string name;
    int wins;
    public:
    Player(string in_string);
    string get_name();
    int get_wins();
    void add_wins();
};


class buttonRowCol
{
  Gtk::Button* checker;
  int row_num;
  int col_num;
  bool isSpotFilled;

public:
  buttonRowCol(int r, int c) {
    this->checker = new Gtk::Button();
    this->row_num = r;
    this->col_num = c;
    this->isSpotFilled = false;
    this->checker->add_label("NOT ME");
    this->checker->set_sensitive(false);
    this->checker->set_size_request(125,125);
  }
  int getRow_num() { return row_num; }

  int getCol_num() { return col_num; }
  Gtk::Button *getButton() { return checker; }
  bool getIsSpotFilled() { return this->isSpotFilled; }
  void setIsSpotFilled() {
      this->isSpotFilled = true;

  }
};
class connectFour : public Gtk::Window{
public:
  connectFour(Player player1, Player player2);
  virtual ~connectFour();
  bool isRedTurn;
  enum placed {BLANK, RED, BLACK};
  placed spots[6][6]=
  { {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
    {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK}};
    //Added for interface
  int PlayTies = 0;
  string Winner;
  vector<Player> GamePlayers;
protected:
  //int turn, numOff, winner;
  void change();
  void this_is_new();
  void placedChecker(buttonRowCol &button);

  bool checkHorizontal(bool isRedTurn, int row, int col);
  bool checkVertical(bool isRedTurn, int row, int col);
  bool checkDiagonal(bool isRedTurn, int row, int col);
  //Added for interface
  void winCountUp(string Winner);
  void borderChange(bool isRedTurn);
  //void GetNames();
  //void show_slot_
  vector<vector<buttonRowCol>> allButtons;
  Gtk::Grid grid;
  Gtk::Box box;
  Gtk::Button quit;
  //Added player info elements
  Gtk::Box left_border, right_border, top_border, all_content, top_content, left_content, mid_content, right_content;
  Gtk::Label player1_name, player2_name, player1_wins, player2_wins, player_turn, total_ties,input_name_text;
  Gtk::Dialog PlayerWin;
  Gtk::Button NewGame;
  Gtk::Dialog NameDialog,WinDialog;

  Gtk::HBox cols;
  Gtk::VBox rows;
  // void checkerPlaced(Gtk::Button*);
  // vector <Gtk::HBox* > rows;
  // vector <Gtk::VBox* > cols;


  Gtk::Button button3;
  Gtk::Button button4;
  public:
  void GetNames();
  void GetPlayers(Player first, Player second);

};

class nameWindow : public Gtk::Window {
    public:
    nameWindow();
    virtual ~nameWindow();
    void submitNames();
    string ent_red_in,ent_black_in;
    protected:
    Gtk::Box NameBox;
    Gtk::Label ent_name_pls;
    Gtk::Entry ent_red,ent_black;
    Gtk::Button send_ent;
};

class Homewindow : public Gtk::Window
{
  public:

  Homewindow();
  virtual ~Homewindow();

  protected:

  void opengame();
  void computerbot();
  void exit_game();

  Gtk::Button computer,close,pvp;
  Gtk::Entry player;
  Gtk::Box box;
  Gtk::VBox sort;

};

#endif
