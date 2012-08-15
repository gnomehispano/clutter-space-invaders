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
#include "starship.h"

G_DEFINE_TYPE (Starship, starship, CLUTTER_TYPE_TEXTURE)

struct _StarshipPrivate {
  ClutterActor *stage;
};

static void
starship_finalize (GObject *object)
{
  Starship *ship = STARSHIP (object);

  if (ship->priv != NULL)
    {
      if (ship->priv->stage != NULL)
	{
	  g_object_unref (ship->priv->stage);
	  ship->priv->stage = NULL;
	}
    }

  G_OBJECT_CLASS (starship_parent_class)->finalize (object);
}

static void
starship_class_init (StarshipClass *klass)
{
}

static gboolean
on_key_press_event (ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
  gfloat width, height, x, y;
  Starship *ship = STARSHIP (actor);

  clutter_actor_get_size (ship->priv->stage, &width, &height);
  clutter_actor_get_position (actor, &x, &y);

  switch (clutter_event_get_key_symbol (event))
    {
    case CLUTTER_KEY_Right:
    case CLUTTER_KEY_KP_Right:
      if (x < (width - 68))
	{
	  clutter_actor_set_position (actor, x + 20, y);
	}
      break;
    case CLUTTER_KEY_KP_Left:
    case CLUTTER_KEY_Left:
      if (x > 20)
	{
	  clutter_actor_set_position (actor, x - 20, y);
	}
      break;
    case CLUTTER_KEY_space:
    case CLUTTER_KEY_KP_Space:
      new_laser (CLUTTER_STAGE (ship->priv->stage), CLUTTER_ACTOR (ship), DIRECTION_UP);
      break;
    default:
      return FALSE; /* Didn't process the event */
    }

  return TRUE; /* Stop further handling of this event. */
}

static void
starship_init (Starship *ship)
{
  GError *error = NULL;

  ship->priv = g_new0 (StarshipPrivate, 1);

  if (!clutter_texture_set_from_file (CLUTTER_TEXTURE (ship), "starship.png", &error))
    {
      g_warning ("Could not load starship.png file: %s\n", error->message);
      g_error_free (error);
      return;
    }

  /* Connect to key events */
  clutter_grab_keyboard (CLUTTER_ACTOR (ship));
  clutter_actor_set_reactive (CLUTTER_ACTOR (ship), TRUE);

  g_signal_connect (ship, "key-press-event", G_CALLBACK (on_key_press_event), NULL);
}

ClutterActor *
starship_new (ClutterActor *stage)
{
  gfloat width, height;
  Starship *ship = STARSHIP (g_object_new (TYPE_STARSHIP, NULL));

  ship->priv->stage = g_object_ref (stage);

  /* Position the starship */
  clutter_actor_get_size (ship->priv->stage, &width, &height);
  clutter_actor_set_position (CLUTTER_ACTOR (ship), width / 2, height);

  return CLUTTER_ACTOR (ship);
}
