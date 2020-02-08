#include "connect.h"
#include <gtkmm.h>

int main(int argc, char** argv)
{

Gtk::Main app(argc, argv);
Homewindow b;
Gtk::Main::run(b);
  // string ent_name1, ent_name2;
  // nameWindow w1;
  // Gtk::Main::run(w1);
  // Player player1(w1.ent_red_in);
  // Player player2(w1.ent_black_in);
  // connectFour w2(player1,player2);
  // Gtk::Main::run(w2);
  return 0;
}
