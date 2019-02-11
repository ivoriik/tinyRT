/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:46:17 by vbespalk          #+#    #+#             */
/*   Updated: 2018/09/21 20:46:19 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	*thread_funct(void *ptr)
{
	t_thread		*th;
	int				i;
	int				j;
	unsigned int	color;
	t_vector		rgb;
	t_ray			ray;

	th = (t_thread *)ptr;
	j = th->s_pix - 1;
	while (++j < th->e_pix)
	{
		i = -1;
		while (++i < SCR_WID)
		{
			ray.dir = ray_generate(th->env, i, j);
			ray.ori = th->env->scene->r_ori;
			rgb = cast_ray(&ray, th->env, j * SCR_WID + i, 0);
			color = get_rgb(th->env->sdl, rgb[0], rgb[1], rgb[2]);
			img_pixel_put(th->env, i, j, color);
		}
	}
	return (NULL);
}

int		mult_threads(t_env *env)
{
	int			count;
	pthread_t	threads_id[NB_THREADS];
	t_thread	th[NB_THREADS];

	count = -1;
	while (++count < NB_THREADS)
		th[count].env = env;
	count = -1;
	while (++count < NB_THREADS)
	{
		th[count].s_pix = count * SCR_HEI / NB_THREADS;
		th[count].e_pix = th[count].s_pix + SCR_HEI / NB_THREADS;
		if (pthread_create((&(threads_id[count])), NULL, \
			thread_funct, &th[count]))
			ft_error("error creating thread\n");
	}
	count = -1;
	while (++count < NB_THREADS)
		if (pthread_join(threads_id[count], NULL))
			ft_error("error joining thread\n");
	return (0);
}
