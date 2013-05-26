#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>


static void createUniverse();
static bool isAlive(int k, int j);

static gboolean newGeneration(gpointer data);

static void startGame(gpointer data);
static void pauseGame(gpointer data);

static void setUniverseSize(const int s);

static void setInterval(int msec);

static void do_drawing(cairo_t *, GtkWidget *);
static gboolean on_expose_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data);



