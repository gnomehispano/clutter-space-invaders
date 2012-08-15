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

#include "common.h"

static ClutterTimeline *laser_timeline = NULL;
static GList *lasers = NULL;

static void
on_timeline_new_frame (ClutterTimeline *timeline, gint msecs, gpointer user_data)
{
  GList *l;

  for (l = lasers; l != NULL; l = l->next)
    {
      ClutterActor *laser = CLUTTER_ACTOR (l->data), *ship, *our_ship;
      gfloat y, next_y, next_inc;
      gint direction = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (laser), "Starship:direction"));

      y = clutter_actor_get_y (laser);
      our_ship = g_object_get_data (G_OBJECT (laser), "Starship:ship");

      switch (direction)
	{
	case DIRECTION_UP:
	  next_y = y - 3;
	  next_inc = -3;
	  if (next_y <= 0)
	    {
	      g_print ("Laser didn't find anyone!\n");
	      lasers = g_list_remove (lasers, laser);
	      clutter_actor_remove_child (clutter_actor_get_stage (laser), laser);

	      return;
	    }
	  break;
	case DIRECTION_DOWN:
	  next_y = y + 3;
	  next_inc = 3;
	  if (next_y >= clutter_actor_get_height (clutter_actor_get_stage (laser)))
	    {
	      g_print ("Laser didn't find anyone!\n");
	      lasers = g_list_remove (lasers, laser);
	      clutter_actor_remove_child (clutter_actor_get_stage (laser), laser);

	      return;
	    }
	  break;
	}

      ship = clutter_stage_get_actor_at_pos (CLUTTER_STAGE (clutter_actor_get_stage (laser)),
					     CLUTTER_PICK_NONE,
					     clutter_actor_get_x (laser), next_y);
      if (ship && ship != our_ship && ship != laser && ship != clutter_actor_get_stage (laser))
	{
	  /* We are about to hit a ship, so make it explode */
	  g_print ("Ship hit, exploding!\n");
	  clutter_timeline_stop (timeline);
	  clutter_actor_remove_child (clutter_actor_get_stage (laser), laser);

	  return;
	}

      clutter_actor_move_by (laser, 0.0, next_inc);
    }
}

void
new_laser (ClutterStage *stage, ClutterActor *ship, gint direction)
{
  ClutterActor *laser;
  gfloat x, y, width, height;
  GError *error = NULL;

  g_print ("Creating laser...\n");
  laser = clutter_texture_new_from_file ("laser.png", &error);
  if (!laser)
    {
      g_warning ("Could not load laser.png file: %s\n", error->message);
      g_error_free (error);
      return;
    }

  /* Determine position */
  clutter_actor_get_size (ship, &width, &height);
  clutter_actor_get_position (ship, &x, &y);

  switch (direction)
    {
    case DIRECTION_UP:
      clutter_actor_set_position (laser, x + (width / 2), y - 24);
      break;
    case DIRECTION_DOWN:
      clutter_actor_set_position (laser, x + (width / 2), y + height);
      break;
    }

  g_object_set_data (G_OBJECT (laser), "Starship:direction", GINT_TO_POINTER (direction));
  g_object_set_data (G_OBJECT (laser), "Starship:ship", ship);
  clutter_actor_add_child (CLUTTER_ACTOR (stage), laser);
  clutter_actor_show (laser);

  /* Start the timeline for the laser movement */
  if (laser_timeline == NULL)
    {
      laser_timeline = clutter_timeline_new (10000);
      g_signal_connect (laser_timeline, "new-frame", G_CALLBACK (on_timeline_new_frame), NULL);
      clutter_timeline_set_loop (laser_timeline, TRUE);
      clutter_timeline_start (laser_timeline);
    }

  lasers = g_list_append (lasers, laser);
}
