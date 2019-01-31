/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 13:26:29 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/22 13:26:33 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_light	*init_slight(t_light **light, unsigned int *nb_li)
{
	t_light *ptr;

	(*nb_li)++;
	if (!(*light))
	{
		if (!(*light = (t_light *)malloc(sizeof(t_light))))
		{
			perror("Malloc: ");
			return (NULL);
		}
		ft_bzero((void *)(*light), sizeof(t_light));
		(*light)->next = NULL;
		return (*light);
	}
	ptr = *light;
	while (ptr->next)
		ptr = ptr->next;
	if (!(ptr->next = (t_light *)malloc(sizeof(t_light))))
	{
		perror("Malloc: ");
		return (NULL);
	}
	ft_bzero((void *)(ptr->next), sizeof(t_light));
	ptr->next->next = NULL;
	return (ptr->next);
}

t_obj	*init_sobj(t_obj **obj, unsigned int *nb_ob)
{
	t_obj *ptr;

	(*nb_ob)++;
	if (!(*obj))
	{
		if (!(*obj = (t_obj *)malloc(sizeof(t_obj))))
		{
			perror("Malloc: ");
			return (NULL);
		}
		ft_bzero((void *)(*obj), sizeof(t_obj));
		(*obj)->next = NULL;
		return (*obj);
	}
	ptr = *obj;
	while (ptr->next)
		ptr = ptr->next;
	if (!(ptr->next = (t_obj *)malloc(sizeof(t_obj))))
	{
		perror("Malloc: ");
		return (NULL);
	}
	ft_bzero((void *)(ptr->next), sizeof(t_obj));
	ptr->next->next = NULL;
	return (ptr->next);
}

int		to_struct(t_scene *scene, char *s, int type)
{
	char	**prop;
	int		nb;

	if (!(prop = ft_strsplit(s, '\t')))
		return (0);
	nb = 0;
	while (prop[nb++])
		;
	if (type == 4)
		if (nb != 4 || !set_lights(scene, prop))
			return (clear_mem(&prop, nb));
	if (type == 5)
		if (nb != 3 || !set_cam(scene, prop))
			return (clear_mem(&prop, nb));
	if (!type || type == 1)
		if (nb != 5 || !set_obj1(scene, prop, type))
			return (clear_mem(&prop, nb));
	if (type == 2 || type == 3)
		if (nb != 6 || !set_obj2(scene, prop, type))
			return (clear_mem(&prop, nb));
	clear_mem(&prop, nb);
	return (1);
}
