#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>


//#include <game/game.h>


int universeSize = 50;
//int generations;

bool universe[102][102]; // map
bool next[102][102]; // map

static void createUniverse()
{	int k;
	int j;
   for(k=10; k <= universeSize-10; k++) {
       for(j=10; j <= universeSize-10; j++) {
	if((rand()%3) == 0)
           universe[k][j] = true;
       }
   }
};

static bool isAlive(int k, int j)
{
   int power = 0;
   power += universe[k+1][j];
   power += universe[k-1][j];
   power += universe[k][j+1];
   power += universe[k][j-1];
   power += universe[k+1][j+1];
   power += universe[k-1][j-1];
   power += universe[k-1][j+1];
   power += universe[k+1][j-1];
   if (((universe[k][j] == true) && (power == 2)) || (power == 3))
          return true;
   return false;
};

/*
static void setUniverseSize(const int s)
//установить размер игрового поля - количество ячеек
{
   universeSize = s;

  // update();
}
*/

static void do_drawing(cairo_t *, GtkWidget *); //объявление ф-ии

static gboolean on_expose_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{  
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  do_drawing(cr, widget);
  cairo_destroy(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr, GtkWidget *widget)
{
  GtkWidget *win = gtk_widget_get_toplevel(widget);
//начало отрисовки сетки
  cairo_set_line_width(cr, 0.5);  
  cairo_set_source_rgba(cr, 0, 0, 0, 0.5);

  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);

  int size = height;//size = min{width, height}
	if (width<height) size = width;
  int cellSize = size/universeSize;

  int i;
  for ( i = 0; i <= universeSize; i++) {
  	cairo_move_to(cr, cellSize*i, 0);
  	cairo_line_to(cr, cellSize*i, cellSize*universeSize); 
};

  for ( i = 0; i <= universeSize; i++) {
  	cairo_move_to(cr, 0, cellSize*i);
  	cairo_line_to(cr, cellSize*universeSize, cellSize*i);
};
cairo_stroke_preserve(cr); 
//конец отрисовки сетки

//начало отрисовки рандомного поля
 cairo_set_source_rgb(cr, 0, 0, 0);
	int k;
	int j;
   for(k=0; k <= universeSize; k++) {
       for(j=0; j <= universeSize; j++) {
           if(universe[k][j] == true) {
               cairo_rectangle(cr, cellSize*k, cellSize*j, cellSize, cellSize);
               cairo_fill(cr);}

	}
   }
//конец отрисовки рандомного поля

//начало отрисовки игрового поля
cairo_set_source_rgb(cr, 0, 0, 0);
	//int k;
	//int j;
   for(k=0; k <= universeSize; k++) {
       for(j=0; j <= universeSize; j++) {
           if(isAlive(k,j)) {
               cairo_rectangle(cr, cellSize*k, cellSize*j, cellSize, cellSize);
               cairo_fill(cr);}

	}
   }
//конец отрисовки игрового поля
};

int main( int argc, char *argv[])
{

  GtkWidget *window;
  GtkWidget *vbox;
  
  GtkWidget *toolbar;
  GtkToolItem *new;
  GtkToolItem *open;
  GtkToolItem *save;
  GtkToolItem *sep;
  GtkToolItem *exit;

  GtkWidget *darea;

  gtk_init(&argc, &argv);

  createUniverse(); // создаёт рандомные живые клетки - начальное поле

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
  gtk_window_set_title(GTK_WINDOW(window), "Game of Life");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  darea = gtk_drawing_area_new();


  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

  new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, -1);

  open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);

  save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);

  sep = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 

  exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), darea, TRUE, TRUE, 0); 

  g_signal_connect(G_OBJECT(new), "clicked",
	G_CALLBACK(createUniverse), NULL);	
  
  g_signal_connect(G_OBJECT(new), "destroy",
	G_CALLBACK(createUniverse), NULL); 

  g_signal_connect(G_OBJECT(exit), "clicked", 
        G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(darea), "expose-event", 
      G_CALLBACK(on_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

 

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
