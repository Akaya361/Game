#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "gamelife.h"
#include "errors.c"

int universeSize = 100; //не больше 200
guint interval = 200; //интервал таймера, msec

static gboolean startTimer = FALSE; // Запущен ли таймер
static gboolean continueTimer = FALSE; //Пауза/Возобновление

bool universe[202][202]; // карта
bool next[202][202]; // карта следующего поколения

static void clearUniverse() 
{	int k;
	int j;
   for(k=0; k <= 202; k++) {
       for(j=0; j <= 202; j++)
	   universe[k][j] = FALSE;
   }
};

//just for debug
static void setUniverse(GtkWidget *button, gpointer data)
{	
(void)button;
	int k;
	int j;
   for(k=1; k <= universeSize; k++) {
	for(j=1; j <= universeSize; j++) {
	if (k == j) {
	   universe[k][j] = TRUE;
	   universe[k+1][j] = TRUE;
 	   universe[k][j+1] = TRUE;
	}
	}
   }
   GtkWidget *window = data;
   gtk_widget_queue_draw(window); ///отрисовка
};

static void createUniverse(GtkWidget *button, gpointer data)
{	
(void)button;

	int k;
	int j;
   for(k=1; k <= universeSize; k++) {
       for(j=1; j <= universeSize; j++) {
	if((rand()%5) == 0)
           universe[k][j] = true;
       }
   }

   GtkWidget *window = data;
   gtk_widget_queue_draw(window); ///отрисовка
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


static gboolean newGeneration(gpointer data)
{
   GtkWidget *window = data;

   int notChanged=0;
	int k;
	int j;
   for(k=1; k <= universeSize; k++) {
       for(j=1; j <= universeSize; j++) {
           next[k][j] = isAlive(k, j);
           if(next[k][j] == universe[k][j])
               notChanged++;
       }
   }
   if(notChanged == universeSize*universeSize) {
       showLostSence(data);
       continueTimer = FALSE; //остановить таймер
       startTimer = FALSE; //сбросить таймер
       return continueTimer;
   }
   for(k=1; k <= universeSize; k++) {
       for(j=1; j <= universeSize; j++) {
           universe[k][j] = next[k][j];
       }
   }
  gtk_widget_queue_draw(window); // отрисовка обновлённого экрана
  return continueTimer;
}

static void startGame(GtkWidget *button, gpointer data)
{
(void)button;
    GtkWidget *window = data;
    if(!startTimer)
    {
        g_timeout_add(interval, newGeneration, window);
        startTimer = TRUE;
        continueTimer = TRUE;
    }
}

static void pauseGame(GtkWidget *button, gpointer data)
{
(void)button;
    GtkWidget *window = data;
    if(startTimer)
    {
        continueTimer = !continueTimer;
        if(continueTimer)
            g_timeout_add(interval, newGeneration, window);
    }
}

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
//начало отрисовки сетки paintGrid(...)
  cairo_set_line_width(cr, 0.5);  
  cairo_set_source_rgba(cr, 0, 0, 0, 0.5);

  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);

  int size = height;  //size = min{width, height}
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
   for(k=0; k <= (universeSize-1); k++) {
       for(j=0; j <= (universeSize-1); j++) {
           if(universe[k+1][j+1] == true) {
               cairo_rectangle(cr, cellSize*k, cellSize*j, cellSize, cellSize);
               cairo_fill(cr);}

	}
   }
//конец отрисовки рандомного поля

//начало отрисовки игрового поля
cairo_set_source_rgb(cr, 0, 0, 0);
	//int k;
	//int j;
   for(k=1; k <= (universeSize-1); k++) {
       for(j=1; j <= (universeSize-1); j++) {
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
  GtkToolItem *sep1;
  GtkToolItem *play;
  GtkToolItem *pause;
  GtkToolItem *exit;

  GtkWidget *darea;

  gtk_init(&argc, &argv);

  clearUniverse();

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 550);
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

  play = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), play, -1);

  pause = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), pause, -1);

  sep1 = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep1, -1); 

  exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), darea, TRUE, TRUE, 0); 

  g_signal_connect(G_OBJECT(new), "clicked", G_CALLBACK(createUniverse), window);	
  g_signal_connect(G_OBJECT(new), "destroy",
	G_CALLBACK(createUniverse), NULL); 

  g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(setUniverse), window);	
  g_signal_connect(G_OBJECT(open), "destroy",
	G_CALLBACK(setUniverse), NULL); 

  g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(startGame), window);
  g_signal_connect(G_OBJECT(play), "destroy",
	G_CALLBACK(startGame), NULL); 

  g_signal_connect(G_OBJECT(pause), "clicked", G_CALLBACK(pauseGame), window);
  g_signal_connect(G_OBJECT(pause), "destroy",
	G_CALLBACK(pauseGame), NULL); 

  g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(exit), "destroy",
	G_CALLBACK(gtk_main_quit), NULL); 

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(darea), "expose-event", 
      G_CALLBACK(on_expose_event), NULL);

  g_signal_connect(G_OBJECT(darea), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
