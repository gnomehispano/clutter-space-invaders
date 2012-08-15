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

#ifndef _STARSHIP_H_
#define _STARSHIP_H_

#include <clutter/clutter.h>

G_BEGIN_DECLS

#define TYPE_STARSHIP                (starship_get_type ())
#define STARSHIP(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_STARSHIP, Starship))
#define IS_STARSHIP(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_STARSHIP))
#define STARSHIP_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_STARSHIP, StarshipClass))
#define IS_STARSHIP_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_STARSHIP))
#define STARSHIP_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_STARSHIP, StarshipClass))

typedef struct _StarshipPrivate StarshipPrivate;

typedef struct {
  ClutterTexture parent;
  StarshipPrivate *priv;
} Starship;

typedef struct {
  ClutterTextureClass parent_class;
} StarshipClass;

GType         starship_get_type (void);

ClutterActor *starship_new (ClutterActor *stage);

G_END_DECLS

#endif
