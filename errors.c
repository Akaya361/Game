#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "gamelife.h"

void showLostSence(gpointer data)
{
  GtkWidget *window = data;
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Игра завершена. Все последующие поколения будут одинаковыми.");
  gtk_window_set_title(GTK_WINDOW(dialog), "Игра завершена");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

}
