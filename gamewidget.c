#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

static void do_drawing(cairo_t *, GtkWidget *);

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
  cairo_set_line_width(cr, 0.5);  
  cairo_set_source_rgba(cr, 0, 0, 0, 0.5);

  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);
  int universeSize = 20;
  int size = height;//
	if (width<height) size = width;
  int cellSize = size/universeSize;

  int i;
  for ( i = 1; i <= universeSize; i++) {
  cairo_move_to(cr, cellSize*i, 0);
  cairo_line_to(cr, cellSize*i, cellSize*universeSize); 
};
for ( i = 1; i <= universeSize; i++) {
  cairo_move_to(cr, 0, cellSize*i);
  cairo_line_to(cr, cellSize*universeSize, cellSize*i);
};

 cairo_stroke_preserve(cr);    
}


int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "expose-event", 
      G_CALLBACK(on_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200); 
  gtk_window_set_title(GTK_WINDOW(window), "Game of Life");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
