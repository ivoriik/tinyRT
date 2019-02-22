/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 16:33:57 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/23 16:34:28 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			trace(t_ray *ray, t_env *env, t_obj *objs, int pix)
{
	double		t;
	int			intersection;

	intersection = 0;
	while ((objs))
	{
		if (objs->intersect(ray->dir, ray->ori, *objs, &t))
		{
			intersection = 1;
			if (t < ray->t)
			{
				ray->t = t;
				if (pix >= 0 && pix < SCR_WID * SCR_HEI)
					env->pix_obj[pix] = objs;
				ray->obj = objs;
			}
		}
		objs = objs->next;
	}
	return (intersection);
}

t_vector	cast_ray(t_ray *ray, t_env *env, int pix, unsigned int depth)
{
	t_vector	li;
	t_light		*light;
	t_obj		*objs;
	t_vector	hit_c = {0.0f, 0.0f, 0.0f};
	t_ray		rldir;
	t_ray		rrdir;

	ray->t = INFINITY;
	objs = env->obj;
	if (!trace(ray, env, objs, pix))
		return (t_vector){BG_R, BG_G, BG_B};
	if (depth > R_DEPTH)
		return (t_vector){BG_R, BG_G, BG_B};
	ray->hit_n = get_normal(ray, ray->obj);
	if (dot_product(ray->hit_n, ray->dir) > 0)
		ray->hit_n = -ray->hit_n;
	ray->hit_p = ray->ori + vec_scalar_mult(ray->dir, ray->t);
	// printf("depth %d, dir %f,%f,%f ori %f,%f,%f hit  %f,%f,%f\n", depth, ray->dir[0], ray->dir[1], ray->dir[2], \
		ray->ori[0], ray->ori[1], ray->ori[2], ray->hit_p[0], ray->hit_p[1], ray->hit_p[2]);
	if (!ray->obj->reflect)
	{
		light = env->light;
		li = get_light(env, ray, ray->obj, light);
		hit_c[0] = L_X(ray->obj->col[0] * li[0], 255);
		hit_c[1] = L_X(ray->obj->col[1] * li[1], 255);
		hit_c[2] = L_X(ray->obj->col[2] * li[2], 255);
	}
	if (ray->obj->reflect)
	{
		rldir.dir = get_refl_dir(ray->dir, ray->hit_n);
		rldir.ori = ray->hit_p + vec_scalar_mult(ray->hit_n, 0.0001f);
		hit_c += vec_scalar_mult(cast_ray(&rldir, env, -1, depth + 1), 0.8);
	}
	if (ray->obj->refract)
	{
		float kr;

		kr = fresnel(ray->dir, ray->hit_n, 1.0f, ray->obj->refract);
		// printf("kr %f\n", kr);
		if (kr < 1.0)
		{
			rrdir.dir = get_refr_dir(ray->dir, ray->hit_n, 1.0f, ray->obj->refract);
			rrdir.ori = dot_product(ray->hit_n, ray->dir) < 0.0f ? ray->hit_p - vec_scalar_mult(ray->hit_n, 0.0001f) :
				ray->hit_p + vec_scalar_mult(ray->hit_n, 0.0001f);
			rldir.dir = get_refl_dir(ray->dir, ray->hit_n);
			rldir.ori = ray->hit_p + vec_scalar_mult(ray->hit_n, 0.0001f);
			hit_c += vec_scalar_mult(cast_ray(&rldir, env, -1, depth + 1), 0.8 * kr) \
				+ vec_scalar_mult(cast_ray(&rrdir, env, -1, depth + 1), 0.9 * (1.0f - kr));
		// printf("depth %d, refr dir %f,%f,%f hit_p %f,%f,%f\n", depth, rrdir.dir[0], rrdir.dir[1], rrdir.dir[2],\
			rrdir.hit_p[0], rrdir.hit_p[1], rrdir.hit_p[2]);
		}
		else
		{
			rldir.dir = get_refl_dir(ray->dir, ray->hit_n);
			rldir.ori = ray->hit_p + vec_scalar_mult(ray->hit_n, 0.0001f);
			hit_c += vec_scalar_mult(cast_ray(&rldir, env, -1, depth + 1), 0.8);
		}
	}

	hit_c = (t_vector){L_X(hit_c[0], 255), L_X(hit_c[1], 255), L_X(hit_c[2], 255)};
	return (hit_c);
}

t_vector	ray_generate(const t_env *env, int i, int j)
{
	t_scene		*scene;
	t_vector	dir;

	scene = env->scene;
	dir[0] = ((2.0f * (i + 0.5f) / SCR_WID - 1.0f) * env->asp_rat * \
		tan(scene->fov / 2.0f));
	dir[1] = ((1.0f - (2 * (j + 0.5f) / SCR_HEI)) * tan(scene->fov / 2.0f));
	dir[2] = -1;
	vec_multipl(&(scene->wto_cam), &dir);
	dir -= scene->r_ori;
	normilize_vec(&dir);
	return (dir);
}

void		render(t_env *env, t_scene *sc)
{
	t_obj		*ob;
	double		c_dis;

	// env_clear_window(env->env_ptr, env->win_ptr);
	ob = env->obj;
	while (ob)
	{
		if (ob->type == 3)
		{
			c_dis = fabs(ob->tr_pos[2] - (sc->cam_transl[2]));
			if (c_dis < 0.0f)
				c_dis = 1.0f;
			ob->tg2 = (ob->tr_siz / c_dis) * (ob->tr_siz / c_dis);
		}
		ob = ob->next;
	}
	printf("NEW RENDER\n");
	ft_bzero(env->pix_obj, sizeof(t_obj *) * SCR_HEI * SCR_WID);
	transform_mat(&(sc->wto_cam), sc->cam_transl, sc->cam_angles, 1.0f);
	ft_bzero(&(sc->r_ori), sizeof(t_vector));
	vec_multipl(&(sc->wto_cam), &(sc->r_ori));
	mult_threads(env);
}
