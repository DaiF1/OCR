/*
 * interface.h in include/
 * 
 * Made by julie.fiadino
 *
 * Classes and functions for GUI
 *
 * Started on  27/10 julie.fiadino
 * Last Update 27/10 julie.fiadino
*/

#pragma once
#include <gtk/gtk.h>

typedef struct UI
{
    GtkWindow               *window;
    GtkImage                *s_image;
    GtkModelButton    *file_chooser;
    GtkModelButton          *save_button;
    GtkModelButton          *solve_button;
    GtkModelButton          *step_button;
    GtkModelButton          *train_button;
    GtkModelButton          *load_button;
    GtkModelButton          *rotate_button;
    GtkModelButton          *autorot_button;
} UI;

typedef struct Progress
{
    GtkDialog       *dialog;
    GtkProgressBar  *bar;
} Progress;

/* file_set(button, user_data): called when pressing the open file button
 */
void file_set(GtkFileChooserButton *button, gpointer user_data);

/* on_solve(button, user_data): called when pressing the solve button
 */
void on_solve(GtkModelButton *button, gpointer user_data);

/* on_step(button, user_data): called when pressing the step button
 */
void on_step(GtkModelButton *button, gpointer user_data);

/* on_train(button, user_data): called when pressing the train button
 */
void on_train(GtkModelButton *button, gpointer user_data);

/* on_load(button, user_data): called when pressing the load button
 */
void on_load(GtkModelButton *button, gpointer user_data);

/* on_save(button, user_data): called when pressing the save button
 */
void on_save(GtkModelButton *button, gpointer user_data);

/* on_rotate(button, user_data): called when pressig the rotate button
 */
void on_rotate(GtkModelButton *button, gpointer user_data);

/* on_autorot(button, user_data): called when pressing the auto rotation button
 */
void on_autorot(GtkModelButton *button, gpointer user_data);
