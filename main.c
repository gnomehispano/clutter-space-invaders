/*
 * Copyright (C) 2012 Rodrigo Moya
 *
 * Authors: Rodrigo Moya <rodrigo@gnome.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

#include "invader.h"
#include "starship.h"

int
main (int argc, char *argv[])
{
  GtkWidget *window, *vbox, *clutter_widget;
  ClutterActor *stage, *starship;

  if (gtk_clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    {
      g_warning ("Could not initialize clutter\n");
      return -1;
    }

  /* Create main window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
  g_signal_connect (window, "hide",
                    G_CALLBACK (gtk_main_quit), NULL);

  vbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  /* Create the Clutter widget */
  clutter_widget = gtk_clutter_embed_new ();
  gtk_box_pack_start (GTK_BOX (vbox), clutter_widget, TRUE, TRUE, 0);
  gtk_widget_show (clutter_widget);

  gtk_widget_set_size_request (clutter_widget, 800, 600);

  stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter_widget));
  clutter_stage_set_color (CLUTTER_STAGE (stage), CLUTTER_COLOR_Black);

  clutter_actor_show (stage);

  /* Create the user's starship */
  starship = starship_new (stage);
  clutter_actor_add_child (stage, starship);
  clutter_actor_show (starship);

  /* ...and the invader army */
  create_invader_army (stage);

  /* Show the window */
  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
