/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sscene.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 18:45:32 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/21 18:45:50 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		set_lights(t_scene *scene, char **prop)
{
	char		*pos;
	char		*col;
	char		*rad;
	t_light		*light;

	if (!(pos = ft_ptrtostr(prop[0], POSI)))
		return (0);
	if (!(col = ft_ptrtostr(prop[1], COLO)))
		return (0);
	if (!(rad = ft_ptrtostr(prop[2], RADI)))
		return (0);
	if (!(light = init_slight(&(scene->light), &(scene->nb_light))))
		return (0);
	if (!((get_vec(pos, &(light->pos))) && (get_col(col, &(light->col))) \
	&& (get_val(rad, &(light->rad)))))
		return (0);
	return (1);
}

int		set_cam(t_scene *scene, char **prop)
{
	char	*pos;
	char	*dir;

	if (!(pos = ft_ptrtostr(prop[0], POSI)))
		return (0);
	if (!(dir = ft_ptrtostr(prop[1], DIRE)))
		return (0);
	if (!(get_vec(pos, &(scene->cam_pos))) || \
		!(get_vec(dir, &(scene->cam_dir))))
		return (0);
	scene->cam_transl = (t_vector){scene->cam_pos[0], \
		scene->cam_pos[1], scene->cam_pos[2]};
	scene->fov = DEG_TO_RAD(90);
	return (1);
}

int		set_obj1(t_scene *scene, char **prop, int type)
{
	t_obj	*obj;
	int		(*intersect[4])();

	intersect[0] = p_intersect;
	intersect[1] = s_intersect;
	if (!(obj = init_sobj(&(scene->obj), &(scene->nb_obj))))
		return (0);
	obj->type = type;
	if (!type && !((get_vec(ft_ptrtostr(prop[0], POSI), &(obj->pos))) \
		&& (get_vec(ft_ptrtostr(prop[1], ROTI), &(obj->rot))) \
		&& (get_col(ft_ptrtostr(prop[2], COLO), &(obj->col))) \
		&& (get_val(ft_ptrtostr(prop[3], REFL), &(obj->reflect)))))
		return (0);
	if (type && !((get_vec(ft_ptrtostr(prop[0], POSI), &(obj->pos))) \
		&& (get_val(ft_ptrtostr(prop[1], SIZE), &(obj->size))) \
		&& (get_col(ft_ptrtostr(prop[2], COLO), &(obj->col))) \
		&& (get_val(ft_ptrtostr(prop[3], REFL), &(obj->reflect)))))
		return (0);
	obj->tr_pos = obj->pos;
	obj->tr_rot = *(normilize_vec(&obj->rot));
	obj->tr_siz = obj->size;
	obj->intersect = intersect[type];
	return (1);
}

int		set_obj2(t_scene *scene, char **prop, int type)
{
	t_obj	*obj;
	int		(*intersect[4])();

	intersect[2] = cy_intersect;
	intersect[3] = co_intersect;
	if (!(obj = init_sobj(&(scene->obj), &(scene->nb_obj))))
		return (0);
	obj->type = type;
	if (!((get_vec(ft_ptrtostr(prop[0], POSI), &(obj->pos))) \
		&& (get_vec(ft_ptrtostr(prop[1], ROTI), &(obj->rot))) \
		&& (get_val(ft_ptrtostr(prop[2], SIZE), &(obj->size))) \
		&& (get_col(ft_ptrtostr(prop[3], COLO), &(obj->col))) \
		&& (get_val(ft_ptrtostr(prop[4], REFL), &(obj->reflect)))))
		return (0);
	obj->tr_pos = obj->pos;
	obj->tr_rot = *(normilize_vec(&obj->rot));
	obj->tr_siz = obj->size;
	obj->intersect = intersect[type];
	return (1);
}
