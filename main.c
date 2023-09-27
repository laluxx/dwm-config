#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_REL_PATH "/xos/suckless/dwm/config.h"

typedef struct {
    char modifier[100];
    char key[100];
    char function[100];
    char argument[100];
} KeyBinding;

GList* keybindings = NULL;

void parse_config_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        KeyBinding kb;
        if (sscanf(line, " { %99[^,], %99[^,], %99[^,], {%99[^}] }", kb.modifier, kb.key, kb.function, kb.argument) == 4) {
            keybindings = g_list_append(keybindings, g_memdup(&kb, sizeof(KeyBinding)));
        }
    }
    fclose(file);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "DWM Config");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Main vertical box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10); // padding
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Adding grid for headers
    GtkWidget *header_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(header_grid), 10);
    gtk_box_pack_start(GTK_BOX(vbox), header_grid, FALSE, FALSE, 0);

    // Style the headers to make them bigger and bold
    const char *header_style = "<span font_weight='bold' font_size='large'>%s</span>";

    char styled_text[256];
    sprintf(styled_text, header_style, "Modifier                          ");
    GtkWidget *mod_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(mod_label), styled_text);

    sprintf(styled_text, header_style, "Key                                  ");
    GtkWidget *key_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(key_label), styled_text);

    sprintf(styled_text, header_style, "Function                        ");
    GtkWidget *func_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(func_label), styled_text);

    sprintf(styled_text, header_style, "Argument                          ");
    GtkWidget *arg_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(arg_label), styled_text);

    gtk_grid_attach(GTK_GRID(header_grid), mod_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), key_label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), func_label, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), arg_label, 3, 0, 1, 1);

    // Scrolled window for keybindings
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(scrolled), grid);

    int row = 0;
    for (GList* li = keybindings; li != NULL; li = li->next, row++) {
        KeyBinding* kb = (KeyBinding*) li->data;

        GtkWidget *mod_entry = gtk_entry_new();
        GtkWidget *key_entry = gtk_entry_new();
        GtkWidget *func_entry = gtk_entry_new();
        GtkWidget *arg_entry = gtk_entry_new();

        gtk_entry_set_text(GTK_ENTRY(mod_entry), kb->modifier);
        gtk_entry_set_text(GTK_ENTRY(key_entry), kb->key);
        gtk_entry_set_text(GTK_ENTRY(func_entry), kb->function);
        gtk_entry_set_text(GTK_ENTRY(arg_entry), kb->argument);

        gtk_grid_attach(GTK_GRID(grid), mod_entry, 0, row, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), key_entry, 1, row, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), func_entry, 2, row, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), arg_entry, 3, row, 1, 1);
    }

    gtk_widget_show_all(window);
}




int main(int argc, char** argv) {
    char config_path[512];
    snprintf(config_path, sizeof(config_path), "%s%s", getenv("HOME"), CONFIG_REL_PATH);
    parse_config_file(config_path);

    GtkApplication *app = gtk_application_new("com.example.DWMConfigApp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // Clean up the keybindings list
    g_list_free_full(keybindings, g_free);

    return status;
}







// status stage
/* #include <gtk/gtk.h> */
/* #include <glib.h> */
/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */

/* #define CONFIG_REL_PATH "/xos/suckless/dwm/config.h" */

/* typedef struct { */
/*     char modifier[100]; */
/*     char key[100]; */
/*     char function[100]; */
/*     char argument[100]; */
/* } KeyBinding; */

/* GList* keybindings = NULL; */

/* void parse_config_file(const char* path) { */
/*     FILE* file = fopen(path, "r"); */
/*     if (!file) return; */

/*     char line[512]; */
/*     while (fgets(line, sizeof(line), file)) { */
/*         KeyBinding kb; */
/*         if (sscanf(line, " { %99[^,], %99[^,], %99[^,], {%99[^}] }", kb.modifier, kb.key, kb.function, kb.argument) == 4) { */
/*             keybindings = g_list_append(keybindings, g_memdup(&kb, sizeof(KeyBinding))); */
/*         } */
/*     } */
/*     fclose(file); */
/* } */

/* static void activate(GtkApplication* app, gpointer user_data) { */
/*     GtkWidget *window = gtk_application_window_new(app); */
/*     gtk_window_set_title(GTK_WINDOW(window), "DWM Config"); */
/*     gtk_window_set_default_size(GTK_WINDOW(window), 500, 400); */

/*     GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL); */
/*     gtk_container_add(GTK_CONTAINER(window), scrolled); */

/*     GtkWidget *listbox = gtk_list_box_new(); */
/*     gtk_container_add(GTK_CONTAINER(scrolled), listbox); */

/*     for (GList* li = keybindings; li != NULL; li = li->next) { */
/*         KeyBinding* kb = (KeyBinding*) li->data; */

/*         GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); */
/*         GtkWidget *mod_label = gtk_label_new(kb->modifier); */
/*         GtkWidget *key_label = gtk_label_new(kb->key); */
/*         GtkWidget *func_label = gtk_label_new(kb->function); */
/*         GtkWidget *arg_label = gtk_label_new(kb->argument); */

/*         gtk_box_pack_start(GTK_BOX(row), mod_label, TRUE, TRUE, 0); */
/*         gtk_box_pack_start(GTK_BOX(row), key_label, TRUE, TRUE, 0); */
/*         gtk_box_pack_start(GTK_BOX(row), func_label, TRUE, TRUE, 0); */
/*         gtk_box_pack_start(GTK_BOX(row), arg_label, TRUE, TRUE, 0); */

/*         gtk_container_add(GTK_CONTAINER(listbox), row); */
/*     } */

/*     gtk_widget_show_all(window); */
/* } */

/* int main(int argc, char** argv) { */
/*     char config_path[512]; */
/*     snprintf(config_path, sizeof(config_path), "%s%s", getenv("HOME"), CONFIG_REL_PATH); */
/*     parse_config_file(config_path); */

/*     GtkApplication *app = gtk_application_new("com.example.DWMConfigApp", G_APPLICATION_FLAGS_NONE); */
/*     g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); */
/*     int status = g_application_run(G_APPLICATION(app), argc, argv); */
/*     g_object_unref(app); */

/*     // Clean up the keybindings list */
/*     g_list_free_full(keybindings, g_free); */

/*     return status; */
/* } */






// works but ugly stage
/* #include <gtk/gtk.h> */
/* #include <glib.h> */
/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */

/* #define CONFIG_REL_PATH "/xos/suckless/dwm/config.h" */

/* typedef struct { */
/*     char modifier[100]; */
/*     char key[100]; */
/*     char function[100]; */
/*     char argument[100]; */
/* } KeyBinding; */

/* GList* keybindings = NULL; */

/* void parse_config_file(const char* path) { */
/*     FILE* file = fopen(path, "r"); */
/*     if (!file) return; */

/*     char line[512]; */
/*     while (fgets(line, sizeof(line), file)) { */
/*         KeyBinding kb; */
/*         if (sscanf(line, " { %99[^,], %99[^,], %99[^,], {%99[^}] }", kb.modifier, kb.key, kb.function, kb.argument) == 4) { */
/*             keybindings = g_list_append(keybindings, g_memdup(&kb, sizeof(KeyBinding))); */
/*         } */
/*     } */
/*     fclose(file); */
/* } */

/* static void activate(GtkApplication* app, gpointer user_data) { */
/*     GtkWidget *window = gtk_application_window_new(app); */
/*     gtk_window_set_title(GTK_WINDOW(window), "DWM Config"); */
/*     gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); */

/*     GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL); */
/*     gtk_container_add(GTK_CONTAINER(window), scrolled); */

/*     GtkWidget *grid = gtk_grid_new(); */
/*     gtk_grid_set_column_spacing(GTK_GRID(grid), 10); */
/*     gtk_grid_set_row_spacing(GTK_GRID(grid), 5); */
/*     gtk_container_add(GTK_CONTAINER(scrolled), grid); */

/*     int row = 0; */
/*     for (GList* li = keybindings; li != NULL; li = li->next, row++) { */
/*         KeyBinding* kb = (KeyBinding*) li->data; */

/*         GtkWidget *mod_entry = gtk_entry_new(); */
/*         GtkWidget *key_entry = gtk_entry_new(); */
/*         GtkWidget *func_entry = gtk_entry_new(); */
/*         GtkWidget *arg_entry = gtk_entry_new(); */

/*         gtk_entry_set_text(GTK_ENTRY(mod_entry), kb->modifier); */
/*         gtk_entry_set_text(GTK_ENTRY(key_entry), kb->key); */
/*         gtk_entry_set_text(GTK_ENTRY(func_entry), kb->function); */
/*         gtk_entry_set_text(GTK_ENTRY(arg_entry), kb->argument); */

/*         gtk_grid_attach(GTK_GRID(grid), mod_entry, 0, row, 1, 1); */
/*         gtk_grid_attach(GTK_GRID(grid), key_entry, 1, row, 1, 1); */
/*         gtk_grid_attach(GTK_GRID(grid), func_entry, 2, row, 1, 1); */
/*         gtk_grid_attach(GTK_GRID(grid), arg_entry, 3, row, 1, 1); */
/*     } */


/*     gtk_widget_show_all(window); */
/* } */


/* int main(int argc, char** argv) { */
/*     char config_path[512]; */
/*     snprintf(config_path, sizeof(config_path), "%s%s", getenv("HOME"), CONFIG_REL_PATH); */
/*     parse_config_file(config_path); */

/*     GtkApplication *app = gtk_application_new("com.example.DWMConfigApp", G_APPLICATION_FLAGS_NONE); */
/*     g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); */
/*     int status = g_application_run(G_APPLICATION(app), argc, argv); */
/*     g_object_unref(app); */

/*     // Clean up the keybindings list */
/*     g_list_free_full(keybindings, g_free); */

/*     return status; */
/* } */


