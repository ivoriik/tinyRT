/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 17:26:37 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/02 17:26:39 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	translate(Uint32 key, t_vector *pos, int cam)
{
	double	i;

	i = 10;
	if (key == SDLK_w)
		(*pos)[1] += i;
	if (key == SDLK_d)
		(*pos)[0] += i;
	if (key == SDLK_a)
		(*pos)[0] -= i;
	if (key == SDLK_s)
		(*pos)[1] -= i;
	i = cam ? -10 : 10;
	if (key == SDLK_LSHIFT)
		(*pos)[2] += i;
	if (key == SDLK_LCTRL)
		(*pos)[2] -= i;
}

void	rotate(Uint32 key, t_vector *dir)
{
	double		k;
	t_matrix	m;

	k = DEG_TO_RAD(10);
	if (key == SDLK_UP)
		x_rotate(&m, k);
	if (key == SDLK_DOWN)
		x_rotate(&m, -k);
	if (key == SDLK_LEFT)
		y_rotate(&m, k);
	if (key == SDLK_RIGHT)
		y_rotate(&m, -k);
	if (key == SDLK_PAGEUP)
		z_rotate(&m, k);
	if (key == SDLK_PAGEDOWN)
		z_rotate(&m, -k);
	dir_multipl(&m, dir);
	normilize_vec(dir);
}

void	cam_rotate(Uint32 key, t_vector *angles)
{
	double k;

	k = DEG_TO_RAD(1);
	if (key == SDLK_UP)
		(*angles)[0] += k;
	if (key == SDLK_DOWN)
		(*angles)[0] -= k;
	if (key == SDLK_LEFT)
		(*angles)[1] -= k;
	if (key == SDLK_RIGHT)
		(*angles)[1] += k;
	if (key == SDLK_PAGEUP)
		(*angles)[2] -= k;
	if (key == SDLK_PAGEDOWN)
		(*angles)[2] += k;
}

void	scale(Uint32 key, double *siz, int cam)
{
	double	i;

	i = cam ? 0.1 : -1.0f;
	if (key == SDLK_KP_PLUS)
		*siz = ((*siz - i) < DEG_TO_RAD(0) && cam) ? DEG_TO_RAD(0) : *siz - i;
	if (key == SDLK_KP_MINUS)
	{
		if (((*siz + i) > 0 && !cam) || ((*siz + i) < DEG_TO_RAD(160) && cam))
			*siz += i;
		else
		{
			if (cam)
				*siz = DEG_TO_RAD(160);
			else
				*siz = 0;
		}
	}
}

void	reset(t_env *e)
{
	t_obj	*objs;
	t_scene	*scene;

	scene = e->scene;
	scene->cam_transl = (t_vector){0.0f, 0.0f, 0.0f};
	scene->cam_angles = (t_vector){0.0f, 0.0f, 0.0f};
	scene->fov = FOV;
	e->selected = NULL;
	objs = e->obj;
	while (objs)
	{
		objs->tr_pos = objs->pos;
		objs->tr_rot = objs->rot;
		objs->tr_siz = objs->size;
		objs = objs->next;
	}
}
