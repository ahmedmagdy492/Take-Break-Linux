#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "file_util.h"
#include "parser.h"

#define DEFAULT_TRIGGER_SECONDS 1200
#define WINDOW_DISAPPEAR_AFTER 20*1000

static long long timer_seconds = 0;
static char* config_content;
static pid_t pid = 1;

static int create_and_show_window();

int main(int argc, char** argv) {
  // reading config file
  int result;
  long long trigger_seconds = 0;

  result = read_file(&config_content);
  if(result != 0) {
    exit(1);
  }

  result = parse_seconds(config_content, &trigger_seconds);
  if(result == -1) {
    fprintf(stderr, "Did not find any valid configuration\n");
    exit(1);
  }

  if(trigger_seconds <= 0 || trigger_seconds <= 300)
    trigger_seconds = DEFAULT_TRIGGER_SECONDS;

  fprintf(stdout, "Read %d seconds from config\n", trigger_seconds);

  int isrunning = 1;

  while(isrunning) {
    sleep(1);
    timer_seconds++;
    if(timer_seconds >= trigger_seconds) {
      timer_seconds = 0;
      pid = fork();
      if(pid == 0) {
	create_and_show_window(argc, argv);
      }
    }
  }
}

gboolean destroy_window(gpointer data) {
  GtkWidget* window = data;
  gtk_window_close(GTK_WINDOW(window));
  return G_SOURCE_REMOVE;
}

static void on_activate(GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *label;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Take Break");
  gtk_window_fullscreen(GTK_WINDOW(window));

  label = gtk_label_new ("Please Take Break for 20 seconds and come back");
  gtk_window_set_child (GTK_WINDOW (window), label);

  gtk_window_present (GTK_WINDOW (window));

  g_timeout_add(WINDOW_DISAPPEAR_AFTER, destroy_window, window);
}

static int create_and_show_window(int argc, char** argv) {
  GtkApplication* app;
  int status;

  app = gtk_application_new("io.github.ahmedmagdy492", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);
  return status;
}
